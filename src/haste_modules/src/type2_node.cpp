#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16_multi_array.hpp"
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


class Type2_Node : public rclcpp::Node
{
  public:
    Type2_Node(const std::string &name)
    : Node(name)
    {
      declare_parameter("name","type2_default");
      declare_parameter("measure_frequency",500);
      declare_parameter("number_of_values",1);
      declare_parameter("max_value",100);
      declare_parameter("i2c_adress",22);

      get_parameter("name",name_);
      get_parameter("measure_frequency",measure_frequency_);
      get_parameter("number_of_values",number_of_values_);
      get_parameter("max_value",max_value_);
      get_parameter("i2c_adress",i2c_adress_);

      std::string aux_topic = "topic_";
      
      auto aux_timer =std::chrono::milliseconds(measure_frequency_); //get frequency of timer from parameter to initialize timer
      
      publisher_ = this->create_publisher<std_msgs::msg::Int16MultiArray>(aux_topic+name_, 10);
      timer_ = this->create_wall_timer(
      aux_timer, std::bind(&Type2_Node::timer_callback, this));
    
      char *bus = "/dev/i2c-1";
      if((file = open(bus, O_RDWR)) < 0)
	    {
	      printf("Failed to open the bus. \n");
		  
	    } 
    }

  private:
    //Params list
    std::string name_;
    int measure_frequency_;
    int number_of_values_;
    int max_value_;
    int i2c_adress_;
     // IT ASSUMES ONLY ONE VALUE

    void timer_callback()
    {
      int i;
      auto message = std_msgs::msg::Int16MultiArray();
  
      ioctl(file, I2C_SLAVE, i2c_adress_);

      uint8_t M[1] = {0x04};
      uint8_t data[100]={};     //SIZE JUST BE VARIABLE, BUT ISNT, CAPPED AT MAXIMUM 50 VALUES
      uint16_t aux;

      write(file, M, 1);
      read (file, data, number_of_values_*2);
      



      for(i=0;i<number_of_values_*2;i+=2){
     
        aux= (data[i]<<8)|data[i+1];
        message.data.push_back(aux);
        RCLCPP_INFO(this->get_logger(), "Received sensor %d data:%d and %d",i,data[i],data[i+1]);
      }
      publisher_->publish(message);

    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Publisher<std_msgs::msg::Int16MultiArray>::SharedPtr publisher_;
    int file;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  
  rclcpp::spin(std::make_shared<Type2_Node>("Type2_Default"));
  rclcpp::shutdown();
  return 0;
}