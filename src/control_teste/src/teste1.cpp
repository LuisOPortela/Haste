#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

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
using std::placeholders::_1;

class Teste1 : public rclcpp::Node
{
  public:
    Teste1(const std::string &name)
    : Node(name)
    {
      declare_parameter("name","type1_default");
      declare_parameter("measure_frequency",500);

      get_parameter("name",name_);
      get_parameter("measure_frequency",measure_frequency_);


      subscription_ = this->create_subscription<std_msgs::msg::String>(
        "receive", 10,std::bind(&Teste1::topic_callback,this, _1));
    
    }
    
    
    
    void topic_callback(const std_msgs::msg::String & msg)
    {
      if(msg.data[0]=='A')
        publisher1_ = this->create_publisher<std_msgs::msg::String>(msg.data, 10);


      if(msg.data[0]=='B')
        publisher2_ = this->create_publisher<std_msgs::msg::String>(msg.data, 10);


      if(msg.data[0]=='C')
        publisher3_ = this->create_publisher<std_msgs::msg::String>(msg.data, 10);
    
    }


  private:
    //Params list
    std::string name_;
    int measure_frequency_;




    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher1_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher2_;
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher3_;
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;


};



int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  
  rclcpp::spin(std::make_shared<Teste1>("Type"));
  rclcpp::shutdown();
  return 0;
}