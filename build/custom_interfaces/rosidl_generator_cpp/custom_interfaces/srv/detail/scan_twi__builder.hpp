// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from custom_interfaces:srv/ScanTwi.idl
// generated code does not contain a copyright notice

#ifndef CUSTOM_INTERFACES__SRV__DETAIL__SCAN_TWI__BUILDER_HPP_
#define CUSTOM_INTERFACES__SRV__DETAIL__SCAN_TWI__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "custom_interfaces/srv/detail/scan_twi__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace custom_interfaces
{

namespace srv
{

namespace builder
{

class Init_ScanTwi_Request_command
{
public:
  Init_ScanTwi_Request_command()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::custom_interfaces::srv::ScanTwi_Request command(::custom_interfaces::srv::ScanTwi_Request::_command_type arg)
  {
    msg_.command = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::srv::ScanTwi_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::srv::ScanTwi_Request>()
{
  return custom_interfaces::srv::builder::Init_ScanTwi_Request_command();
}

}  // namespace custom_interfaces


namespace custom_interfaces
{

namespace srv
{

namespace builder
{

class Init_ScanTwi_Response_output
{
public:
  Init_ScanTwi_Response_output()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::custom_interfaces::srv::ScanTwi_Response output(::custom_interfaces::srv::ScanTwi_Response::_output_type arg)
  {
    msg_.output = std::move(arg);
    return std::move(msg_);
  }

private:
  ::custom_interfaces::srv::ScanTwi_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::custom_interfaces::srv::ScanTwi_Response>()
{
  return custom_interfaces::srv::builder::Init_ScanTwi_Response_output();
}

}  // namespace custom_interfaces

#endif  // CUSTOM_INTERFACES__SRV__DETAIL__SCAN_TWI__BUILDER_HPP_
