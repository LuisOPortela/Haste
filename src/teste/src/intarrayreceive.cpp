#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <stdio.h>
#include <stdlib.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16_multi_array.hpp"

using std::placeholders::_1;


/* This example creates a subclass of Node and uses std::bind() to register a
* member function as a callback from the timer. */

class MinimalPublisher : public rclcpp::Node
{
  public:
    MinimalPublisher()
    : Node("minimal_sub"){

        subscription_ = this->create_subscription<std_msgs::msg::Int16MultiArray>(
        "Teste", 10,std::bind(&MinimalPublisher::topic_callback,this, _1));
    }

  private:
    void topic_callback(const std_msgs::msg::Int16MultiArray & msg)
    {

      RCLCPP_INFO(this->get_logger(), "Receiving");

      for(auto i=0; i<msg.data.size();i++){
        RCLCPP_INFO(this->get_logger(), "Number  %d: '%d'", i,msg.data[i]);
      }

    }
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<std_msgs::msg::Int16MultiArray>::SharedPtr subscription_ ;
    size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<MinimalPublisher>());
  rclcpp::shutdown();
  return 0;
}
