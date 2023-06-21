#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16.hpp"

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

struct type1_params{
std::string name={};
int measure_frequency=0;
int number_of_values=0;
std::vector<int> max_value={};
};


class Type1_Node : public rclcpp::Node
{
  public:
    Type1_Node(const std::string &name)
    : Node(name), count_(0)
    {
      declare_parameter("name","type1_default");
      declare_parameter("measure_frequency","500");
      declare_parameter("number_of_values","1");

      get_parameter("name",name_);
      get_parameter("measure_frequency",measure_frequency_);
      get_parameter("number_of_values",number_of_values_);
      
      std::string aux_topic = "topic_";
      
      auto aux_timer =std::chrono::milliseconds(measure_frequency_);
      publisher_ = this->create_publisher<std_msgs::msg::Int16>(aux_topic+name_, 10);
      timer_ = this->create_wall_timer(
      std::chrono::milliseconds(measure_frequency_), std::bind(&Type1_Node::timer_callback, this));
    
      char *bus = "/dev/i2c-1";
      if((file = open(bus, O_RDWR)) < 0)
	    {
	      printf("Failed to open the bus. \n");
		  
	    } 
    
    
    
    
    }

  private:
    //Params list
    std::string name_;
    int measure_frequency_=500;
    //assuming number of values is always 1
    int number_of_values_=1;
    
    std::vector<int> max_value_={};
    

    void timer_callback()
    {
      auto message = std_msgs::msg::Int16();
  

      RCLCPP_INFO(this->get_logger(), "Publishing: '%d'", message.data);



      ioctl(file, I2C_SLAVE, 0x20);

    
      uint8_t reg[2] = {0x04,0};
      write(file, reg, 1);
      read (file, reg, 2);
      message.data=reg[0]*256+reg[1];
      
      RCLCPP_INFO(this->get_logger(), "Publishing: '%d'", message.data);
      publisher_->publish(message);

    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr publisher_;
    size_t count_;
    int file;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  
  rclcpp::spin(std::make_shared<Type1_Node>("Type"));
  rclcpp::shutdown();
  return 0;
}