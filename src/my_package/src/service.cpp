
#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/scan_twi.hpp"

#include <memory>
#include <cstdio>
#include <iostream>
#include <array>


void Scan(const std::shared_ptr<custom_interfaces::srv::ScanTwi::Request> request,
          std::shared_ptr<custom_interfaces::srv::ScanTwi::Response>      response)
{
  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Incoming request");
  
  std::string text = std::string("popo");
  //response->output = text;
  response->output= request->command;


  //std::string executeCommand(const std::string& command) {
  std::array<char, 128> buffer;
  std::string result;

  FILE* pipe=popen(request->command.c_str(), "r");
  if (!pipe){
    throw std::runtime_error("Failed to execute command.");

  }

  while (fgets(buffer.data(),buffer.size(),pipe)!= nullptr) {
    result += buffer.data();
  }

  pclose(pipe);

  response->output= result;
}


int main(int argc, char **argv)
{
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("Scan_Twi_server");

  rclcpp::Service<custom_interfaces::srv::ScanTwi>::SharedPtr service =
    node->create_service<custom_interfaces::srv::ScanTwi>("Scan_Twi", &Scan);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Ready to responde");

  rclcpp::spin(node);
  rclcpp::shutdown();
}
