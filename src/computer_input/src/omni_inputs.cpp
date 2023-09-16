#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16.hpp"
//#include "std_msgs/msg/string.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <functional>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

using std::placeholders::_1;


class Omni_inputs : public rclcpp::Node
{
  public:
    Omni_inputs(const std::string &name)       //constructor variable
    : Node(name)                              //initiate node name with the variable name
    {

    
      publisher_ = this->create_publisher<std_msgs::msg::Int16>("topic_omni_control", 10);

      message_ = std::make_shared<std_msgs::msg::Int16>(); 

      pubkeyinput();
    }

    void pubkeyinput()
    { 
      while(1) {
     
        system("stty raw");
        char key = std::getchar();
        
        message_->data  = key;
        
        publisher_->publish(*message_);
        
        if(int(key)==3){
          system("stty cooked");
          break;
        }
      } 
    }    

  private:
      rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher_;
      std_msgs::msg::Int16::SharedPtr message_;

   
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Boas");
  rclcpp::spin(std::make_shared<Omni_inputs>("Omni_inputs"));
  rclcpp::shutdown();
  return 0;
}