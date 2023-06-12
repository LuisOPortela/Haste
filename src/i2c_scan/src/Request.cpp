#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int8_multi_array.hpp"

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

/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

//Parametros

std::string Node_name = "Topic_Node_name";
std::chrono::milliseconds Frequency = 2000ms;
uint8_t first_adress= 0x10, last_adress= 0x30;





class I2cScanner : public rclcpp::Node
{
  public:
    I2cScanner()
    : Node("i2c_scanner"), count_(0)
    {
      publisher_ = this->create_publisher<std_msgs::msg::Int8MultiArray>(Node_name, 10);
      timer_ = this->create_wall_timer(Frequency, std::bind(&I2cScanner::timer_callback, this));
    
      if((file = open(bus, O_RDWR)) < 0)
	    {
	      printf("Failed to open the bus. \n");
		  
	    } 
    
    }

  private:
    void timer_callback()
    {
      int i,j;
      
      uint8_t data[1] = {0};

    	for (i = first_adress; i < last_adress; i++)
    	{
    		

    		ioctl(file, I2C_SLAVE, i);
    	
    		if(read(file, data, 1) != 1) //not found
    		{
        		continue;
    		}		
    		
    		adresses[j]=i;
    		j++;
    	
        for(i=0;i<sizeof(adresses);i++)
        {
          if (adresses[i]==0)
          { 
            break;
          }	
          printf("%d: %x  |",i , adresses[i]);
        }
      
      
      
      }
      
      
      
      auto message = std_msgs::msg::Int8MultiArray();
      //message.data = "Hello, world! " + std::to_string(count_++);
      //RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
      publisher_->publish(message);
    
    }
    

    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int8MultiArray>::SharedPtr publisher_;
    size_t count_;

    char *bus = "/deb/i2c-1";
    int file;
    
    
    //MALLOC TAMNHO DE ULTIMO ADRESS MENOS PRIMEIRO ADRESS
    uint8_t adresses[30]={};


};

int main(int argc, char * argv[])
{
  // Create I2C bus

  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<I2cScanner>());
  rclcpp::shutdown();
  return 0;
}
