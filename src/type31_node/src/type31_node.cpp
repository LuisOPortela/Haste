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

using std::placeholders::_1;

struct type31_params{
std::string name={};
std::string sensor_name={};
int on_percentage=0;   //set max to 100
};



class Type31_Node : public rclcpp::Node
{
  public:
    Type31_Node(const std::string &name)
    : Node(name)
    {
      declare_parameter("name","type31_default");
      declare_parameter("sensor_name","type1_default");
      declare_parameter("on_percentage",50);

      get_parameter("name",name_);
      get_parameter("sensor_name",sensor_name_);
      get_parameter("on_percentage",on_percentage_);
      
      std::string aux_topic = "topic_";
      
      subscription_ = this->create_subscription<std_msgs::msg::Int16>(
        aux_topic+sensor_name_, 10,std::bind(&Type31_Node::topic_callback,this, _1));
    
      char *bus = "/dev/i2c-1";
      if((file = open(bus, O_RDWR)) < 0)
	    {
	      printf("Failed to open the bus. \n");
		  
	    } 
    
    }

  private:
    //Params list
    std::string name_;
    std::string sensor_name_;
    int on_percentage_;

    void topic_callback(const std_msgs::msg::Int16 & msg)
    {
      //ADRESSO DEVE SER RECEBIDO TBM
      //Max Value deve ser recebido do sensor
      int max_value=103;
      float percentage= (float)msg.data/max_value;


      //DELETE THIS, ON PERCENTAGE BEING FORCED BEACAUSE CANT CORRECT BUG IN MICRO
      on_percentage_=70;
      //FORCOING IT TO 70

      int8_t state[1] = {};
      if(percentage>=on_percentage_) // ON
        state[0]=11;

      
      else if(percentage<on_percentage_) // ON
        state[0]=10;
      
      ioctl(file, I2C_SLAVE, 0x14);
      write(file, state, 1);

      RCLCPP_INFO(this->get_logger(), "GOT:%d ,Max value: %d Percentage:%f Sending state: '%d'",msg.data,max_value, percentage, state[0]);

    }

    rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr subscription_;
    int file;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Type31_Node>("Type"));
  rclcpp::shutdown();
  return 0;
}