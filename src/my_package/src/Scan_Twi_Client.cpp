
#include "rclcpp/rclcpp.hpp"
#include "custom_interfaces/srv/scan_twi.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>

using namespace std::chrono_literals;

//DELETE THIS
int i;

int main(int argc, char **argv)
{
  
  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("Scan_Twi_client");
  rclcpp::Client<custom_interfaces::srv::ScanTwi>::SharedPtr client =
    node->create_client<custom_interfaces::srv::ScanTwi>("Scan_Twi");

  auto request = std::make_shared<custom_interfaces::srv::ScanTwi::Request>();
  //request->a = atoll(argv[1]);
 
  std::string input;
  while (rclcpp::ok()) {

    std::cout << "Enter a Command (or 'q' to quit):";
    std::getline(std::cin, input);

    if(input == "q")
      break;

    request->command = input;
   
    while (!client->wait_for_service(1s)) { //process request
      if (!rclcpp::ok()) {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Interrupted while waiting for the service. Exiting.");
        return 0;
      
      }
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "service not available, waiting again...");
    }

    auto result = client->async_send_request(request);
    // Wait for the result.
    if (rclcpp::spin_until_future_complete(node, result) ==
      rclcpp::FutureReturnCode::SUCCESS)
    {
      RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Console answer: \n %s", result.get()->output.c_str() );
    } else {
      RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "Failed to call service add_two_ints");
    }
  }

  rclcpp::shutdown();
  return 0;
}
