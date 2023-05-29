// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <thread>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"


using std::placeholders::_1;
using namespace std::chrono_literals;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class CommandPublisher : public rclcpp::Node   //Classe derivada de Node
{
public:
  CommandPublisher()          
  : Node("command_publisher"), count_(0)
  {
    publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);
    
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10, std::bind(&CommandPublisher::sub_callback, this, _1));
    
  
  

  }

  void publish_message(const std::string input )  //function used in main to publish message
  {
    auto message = std_msgs::msg::String();
    message.data = input;
    RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", message.data.c_str());
    publisher_->publish(message);
  }
  

private:
  
  void sub_callback(const std_msgs::msg::String & msg){

      RCLCPP_INFO(this->get_logger(), "I heard: '%s'", msg.data.c_str());
  
  }
  
  
  
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;

  bool waiting_message_ =false;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  
  auto node = std::make_shared<CommandPublisher>();

  std::thread spin_thread([&node]() {
    rclcpp::spin(node);
  });
  
  std::string input;

  while (rclcpp::ok()) {

    std::cout << "Enter a Request (or 'q' to quit):";
    std::getline(std::cin, input);

    if(input == "q")
      break;

    node->publish_message(input);
   
  }

  
  rclcpp::shutdown();
  return 0;
}
