#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <functional>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

using namespace std::chrono_literals;
/*##IN MY__LIBRARY, DELETE LATER###*/

struct type1_params{
std::string name={};
int measure_frequency=0;
int number_of_values=0;
std::vector<int> max_value={};
};

struct type31_params{
std::string name;
std::string sensor_name;
int on_percentage;   //set max to 100
};

struct module_name_adress{
std::string name;
uint8_t adress;
};

//Type1 params
#define NODE_NAME_SIZE 15
#define MEASURE_FREQUENCY_SIZE 2
#define NUMBER_OF_VALUE_SIZE 1
#define SINGLE_MAX_VALUE_SIZE 2



//Type3.1 params
#define NODE_NAME_SIZE 15
#define SENSOR_NAME_SIZE 15
#define ACTIVATION_PERCENTAGE 1  


//Ms 
#define M_ASK_FOR_TYPE 1
#define M_T1_PARAMS_1 2
#define M_T1_PARAMS_2 3
#define M_T31_PARAMS_1 2

#define T1_MESSAGE_1_SIZE NODE_NAME_SIZE+MEASURE_FREQUENCY_SIZE+NUMBER_OF_VALUE_SIZE
#define T31_MESSAGE_1_SIZE  NODE_NAME_SIZE+SENSOR_NAME_SIZE+ACTIVATION_PERCENTAGE

/*###############################*/

/*######I2C_Scan Parameters######*/
std::string Node_name = "Topic_Node_name";
std::chrono::milliseconds Frequency = 2000ms;
uint8_t first_adress= 0x10, last_adress= 0x30;

/*###############################*/





class I2cScanner : public rclcpp::Node
{
  public:
    I2cScanner()
    : Node("i2c_scanner")
    {
      
      publisher_ = this->create_publisher<std_msgs::msg::UInt8MultiArray>(Node_name, 10);
      timer_ = this->create_wall_timer(Frequency, std::bind(&I2cScanner::timer_callback, this));
    
      char *bus = "/dev/i2c-1";
      if((file = open(bus, O_RDWR)) < 0)
	    {
	      printf("Failed to open the bus. \n");
		  
	    } 
    
    }

  private:
    
    //Timer_callback is the Main loop of the node
    void timer_callback()
    {
      std::vector<uint8_t> adresses=module_scanner();
      node_creator(adresses);
    }

    //Scans the connected modules in the I2C bus
    std::vector<uint8_t> module_scanner()
    {
      int i,j;
      uint8_t data[1] = {0};
      
      std::vector<uint8_t> adresses;
      auto message = std_msgs::msg::UInt8MultiArray();
      
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Timer_Callback");
    	for (i = first_adress; i < last_adress; i++)
    	{
    		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Scanning");
        
    		ioctl(file, I2C_SLAVE, i);
    	
    		if(read(file, data, 1) != 1) //not found
    		{
        	continue;
    		}		
    		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Found");
    		
        adresses.push_back(i);
        message.data.push_back(i);
    		j++;
      
      }
      
      //publisher_->publish(message);
    }

    void node_creator(std::vector<uint8_t> adresses)
    {
      
      std::vector<uint8_t> nodes_to_create;
      
      //CHECK FOR UNCREATED NODES
      for(uint8_t adress : adresses){
        for(const auto& module :created_nodes)
        {
          if (adress==module.adress)
            break;
        }  
        nodes_to_create.push_back(adress);
      }
      
      //CREATE UNCREATED NODES
      if(nodes_to_create.size()!=0)
      {
        for(uint8_t adress : nodes_to_create)
        {
          //get type
          uint8_t M[1]={};
          
          M[0]=M_ASK_FOR_TYPE;

          ioctl(file, I2C_SLAVE, adress);
          
          write(file, M, 1);
          read (file, M, 1);
          
          ///get params for specific module type
          switch (M[0])
          {
            case 1:       //TYPE 1 MODULE SIMPLE SENSOR
            {
              uint8_t M_1 [T1_MESSAGE_1_SIZE]={};;
              type1_params new_node;

              //ask for all parameeters except for max_value//
              M[0]=M_T1_PARAMS_1;
              
              write(file, M, 1); 
              read (file, M_1, T1_MESSAGE_1_SIZE);

              char* aux=(char*) M_1;
              std::string aux_name(aux,NODE_NAME_SIZE);

              new_node.name = aux_name;
              new_node.measure_frequency = M_1[15]*256 + M_1[16];    //NOT READY FOR CHANGE OF SIZE OF PARAMETERS
              new_node.number_of_values  = M_1[17];
              
              //ask for max_values//
              int M_2_size=new_node.number_of_values*SINGLE_MAX_VALUE_SIZE;
            
              std::vector<uint8_t> M_2(M_2_size);
              
              M[0]=M_T1_PARAMS_2;
              
              write(file, M, 1); 
              read (file, M_2.data(), M_2_size);

              int j=0;
              for(int i=0;i<M_2.size();i+=2)
              {
                
                new_node.max_value[j]=M_2[i]*256+M_2[i+1];
                j++;
              }  
              
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node.name: %s ",new_node.name);
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node.measure_frequency: %d",new_node.measure_frequency);
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node.number_of_values: %d",new_node.number_of_values);
              
              for(int i=0;i<new_node.max_value.size();i+=1)
              {
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node.max_value(%d): %d",i,new_node.max_value[i]);
              }

              
              module_name_adress aux_module;
              aux_module.name=new_node.name;
              aux_module.adress=adress;
              created_nodes.push_back(aux_module);
              
              //create node with this params
              break;
            /*
            case 31:       
              uint8_t M_1[T31_MESSAGE_1_SIZE]={} 
              type31_params new_node;

              //ask for all parameeters except for max_value//
              M[0]=M_T3_PARAMS_1;
              
              write(file, M, 1); 
              read (file, M_1, T31_MESSAGE_1_SIZE);

              std::string aux_name(reinterpret_cast<char*>(M_1),reinterpret_cast<char*>(M_1)+NODE_NAME_SIZE);
              new_node.name = aux_name
              
              aux_name(reinterpret_cast<char*>(&M_1[NODE_NAME_SIZE],SENSOR_NAME_SIZE);
              new_node.sensor_name = aux_name;

              new_node.on_percentage = M_1[30];
              

              break;
            
            */
            }
            case 4:
            {  /* code */
              break;
            }
            default:
            {
              break;
            }
          }
          //execute node
        }


      }
    
    
    }
/*  
    void type1_module_data_publisher()
    {

    }

*/
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr publisher_;
    int file;
    
    std::vector<module_name_adress> created_nodes;


};

int main(int argc, char * argv[])
{
  // Create I2C bus


  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<I2cScanner>());
  rclcpp::shutdown();
  return 0;
}
