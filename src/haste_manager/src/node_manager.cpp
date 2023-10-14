#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/u_int8_multi_array.hpp"
#include "my_library/my_library.hpp"

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


/*###### Node Manager Parameters######*/

std::string Node_name = "Topic_Node_name";
std::chrono::milliseconds Frequency = 3000ms;
uint8_t first_adress= 10, last_adress= 50;

/*###############################*/

class Node_manager : public rclcpp::Node
{
  public:
    Node_manager()
    : Node("node_manager")
    {
      
      publisher_ = this->create_publisher<std_msgs::msg::UInt8MultiArray>(Node_name, 10);
      timer_ = this->create_wall_timer(Frequency, std::bind(&Node_manager::timer_callback, this));
    
      char *bus = "/dev/i2c-1";                  //Open I2C Bus
      if((file = open(bus, O_RDWR)) < 0)
	    {
	      printf("Failed to open the bus. \n");
		  
	    } 
    }

  private:
    
    //Timer_callback is the Main loop of the node
    void timer_callback()
    {
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Timer_Callback");
      std::vector<uint8_t> adresses=module_scanner(); //Get adresses of connected modules
      node_creator(adresses);                         //Verify if new nodes need to be created
    }


    //Scans the connected modules in the I2C bus
    std::vector<uint8_t> module_scanner()
    {
      
      int i,j;
      uint8_t data[1] = {0};
      
      std::vector<uint8_t> adresses;
      //auto message = std_msgs::msg::UInt8MultiArray();
      
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Scanning for connected modules");
    	
      for (i = first_adress; i < last_adress; i++)
    	{
    		
    		ioctl(file, I2C_SLAVE, i);
    		if(read(file, data, 1) != 1) //not found
    		{
        	continue;
    		}			
        
        adresses.push_back(i);
        //message.data.push_back(i);
    		j++;
      
      }
      
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Number of modules detected: %i",adresses.size());
      //publisher_->publish(message);
      return adresses;
    }

    //Verify if new nodes need to be created and create them
    void node_creator(std::vector<uint8_t> adresses)
    {
      
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node Creator Cycle");
      std::vector<uint8_t> nodes_to_create={};
      
      //CHECK FOR UNCREATED NODES
      bool aux_created;
      for(uint8_t adress : adresses){
        aux_created=0;
        for(const auto& module :created_nodes)
        {
          
          if (adress==module.adress)
            aux_created=1;
        }  
        if (aux_created==0)
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
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Creating Type 1 Node");
              
              char M_1[T1_MESSAGE_1_SIZE]={};
              type1_params new_node_1;

              M[0]=M_T1_PARAMS_1;
              
              write(file, M, 1); 
              read (file, M_1, T1_MESSAGE_1_SIZE);

              std::string aux_name(M_1,NODE_NAME_SIZE);

              new_node_1.name = aux_name;
              new_node_1.measure_frequency = (int)M_1[15]*256 + (int)M_1[16];  
              new_node_1.number_of_values  = (int)M_1[17];
              new_node_1.max_value         = (int)M_1[18]*256 + (int)M_1[19];

               //STORE IN CREATED_NODES
              module_name_adress aux_module;
              aux_module.name=new_node_1.name;
              aux_module.adress=adress;
              created_nodes.push_back(aux_module);
              
              new_node_1.name.pop_back(); //line needed to cut null signal at the end of string

              //CREATE NODE
              std::string command="ros2 run haste_modules type1_node --ros-args --remap __node:=" +new_node_1.name+ 
              " -p measure_frequency:=" +std::to_string(new_node_1.measure_frequency)+ + 
              " -p max_value:=" +std::to_string(new_node_1.max_value)+  " -p name:=" +new_node_1.name+ " &";

              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Commando %s",command.c_str());
              std::string result;
              std::array<char, 128> buffer;

              FILE* pipe=popen(command.c_str(), "w"); //change to r to see console output
              if (!pipe){
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "FAILED TO OPEN PIPE");
              } 
              
              while (fgets(buffer.data(),buffer.size(),pipe)!= nullptr) {
                result += buffer.data();
              } 

              pclose(pipe);


              //create node with this params
              break;
            }
            
            
            case 2:       //TYPE 2 MODULE CONTROL SENSOR
            {
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Creating Type 2 Node");
              
              char M_1[T2_MESSAGE_SIZE]={};
              type2_params new_node_2;

              M[0]=M_T2_PARAMS;
              
              write(file, M, 1); 
              read (file, M_1, T2_MESSAGE_SIZE);

              std::string aux_name(M_1,NODE_NAME_SIZE);

              new_node_2.name = aux_name;
              new_node_2.measure_frequency = (int)M_1[15]*256 + (int)M_1[16];  
              new_node_2.number_of_values  = (int)M_1[17];
              new_node_2.max_value         = (int)M_1[18]*256 + (int)M_1[19];

              module_name_adress aux_module;
              aux_module.name=new_node_2.name;
              aux_module.adress=adress;
              created_nodes.push_back(aux_module);
              
              new_node_2.name.pop_back(); //line needed to cut null signal at the end of string

              //CREATE NODE
              std::string command="ros2 run haste_modules type2_node --ros-args --remap __node:=" +new_node_2.name+ 
              " -p measure_frequency:=" +std::to_string(new_node_2.measure_frequency)+ + " -p max_value:=" +std::to_string(new_node_2.max_value)+  
              " -p name:=" +new_node_2.name+" -p i2c_adress:=" +std::to_string(adress)+ " &";

              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Commando %s",command.c_str());
              std::string result;
              std::array<char, 128> buffer;

              FILE* pipe=popen(command.c_str(), "w"); //change to r to see console output
              if (!pipe){
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "FAILED TO OPEN PIPE");
              } 
              
              while (fgets(buffer.data(),buffer.size(),pipe)!= nullptr) {
                result += buffer.data();
              } 

              pclose(pipe);


              //create node with this params
              break;
            }
            
            case 31:     //TYPE 31 MODULE SIMPLE SENSOR 
            {
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Creating Type 31 Node");
              
              char M_1[T31_MESSAGE_SIZE]={}; 
              type31_params new_node_31;

              M[0]=M_T31_PARAMS;
              
              write(file, M, 1); 
              read (file, M_1, T31_MESSAGE_SIZE);

              //GET PARAMETER NAME

              size_t aux_length = 0;
              while(M_1[aux_length] != '\0'){
                aux_length++;
              }

              std::string aux_name_1(M_1,aux_length);
              new_node_31.name = aux_name_1;

              //GET PARAMETER SENSOR NAME


              aux_length = NODE_NAME_SIZE;
              while(M_1[aux_length] != '\0'){
                aux_length++;
              }

              std::string aux_name_2(M_1 + NODE_NAME_SIZE,aux_length-NODE_NAME_SIZE);
              new_node_31.sensor_name = aux_name_2;

              //GET PARAMETER ON_PERCENTAGE

              new_node_31.on_percentage = (int)M_1[NODE_NAME_SIZE+SENSOR_NAME_SIZE];

              /*
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "M_1 29: %d ",(int) M_1[29]);
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "M_1 30: %d ",(int) M_1[30]);
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "M_1 31: %d ",(int) M_1[31]);
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node.name: %s ",new_node_31.name.c_str());
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node.sensor_name: %s",new_node_31.sensor_name.c_str());
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Node.on_percentage: %d",new_node_31.on_percentage);
              */

              //STORE IN CREATED_NODES
              
              module_name_adress aux_module;
              aux_module.name=new_node_31.name;   
              aux_module.adress=adress;
              created_nodes.push_back(aux_module);
              
              //CREATE NODE

              std::string command="ros2 run haste_modules type31_node --ros-args --remap __node:="+new_node_31.name+
              " -p sensor_name:=" + new_node_31.sensor_name+" -p on_percentage:=" + std::to_string(new_node_31.on_percentage)+ " &";

              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Commando %s",command.c_str());
              std::string result;
              std::array<char, 128> buffer;

              FILE* pipe=popen(command.c_str(), "r");
              if (!pipe){
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Failed to open pipe");
              } 
              
              while (fgets(buffer.data(),buffer.size(),pipe)!= nullptr) {
                result += buffer.data();
              }

              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "FEEDBACK DA CONSOLA: %s",result.data());

              pclose(pipe);


              break;
            
            
            }
            
            case 4:
            {  
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Creating Type 4 Node");
              
              char M_1[T4_MESSAGE_SIZE]={}; 
              type4_params new_node_4;

              M[0]=M_T4_PARAMS;
              
              write(file, M, 1); 
              read (file, M_1, T4_MESSAGE_SIZE);

              //GET 1st paramter 
              size_t aux_length = 0;
              while(M_1[aux_length] != '\0'){
                aux_length++;
              }

              std::string aux_name_1(M_1,aux_length);
              new_node_4.name = aux_name_1;
              
              //GET 2nd paramter
              new_node_4.wheel_type = (int)M_1[NODE_NAME_SIZE];
              //GET 3rd paramter
              new_node_4.radius = (int)M_1[NODE_NAME_SIZE+WHEEL_TYPE_SIZE];


              //STORE IN CREATED_NODES
              
              module_name_adress aux_module;
              aux_module.name=new_node_4.name;   
              aux_module.adress=adress;
              created_nodes.push_back(aux_module);
              

              //CREATE NODE

              std::string command="ros2 run haste_modules type4_node --ros-args --remap __node:=" +new_node_4.name+ 
              " -p name:=" +new_node_4.name+                      " -p wheel_type:=" +std::to_string(new_node_4.wheel_type)+
              " -p radius:=" + std::to_string(new_node_4.radius)+ " -p i2c_adress:=" +std::to_string(adress)+ " &";

              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Commando %s",command.c_str());
              std::string result;
              std::array<char, 128> buffer;

              FILE* pipe=popen(command.c_str(), "w");
              if (!pipe){
                RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "FAILED TO OPEN PIPE");
              } 
              
              while (fgets(buffer.data(),buffer.size(),pipe)!= nullptr) {
                result += buffer.data();
              }

              pclose(pipe);


              break;
            }
            default:
            {
             
              RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "CREATING ERROR:TYPE NOT FOUND");
              break;
            }
         
          }
          //execute node
        }


      }
    
    
    }

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::UInt8MultiArray>::SharedPtr publisher_;
    int file;
    
    std::vector<module_name_adress> created_nodes;


};

int main(int argc, char * argv[])
{
  // Create I2C bus


  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Node_manager>());
  rclcpp::shutdown();
  return 0;
}
