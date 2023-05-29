cmake_minimum_required(VERSION 3.8)
project(cpp_pubsub)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(my_custom_msgs REQUIRED)
find_package(tutorial_interfaces REQUIRED)

add_executable(talker src/publisher_member_function.cpp)
ament_target_dependencies(talker rclcpp tutorial_interfaces)

add_executable(listener src/subscriber_member_function.cpp)
ament_target_dependencies(listener rclcpp tutorial_interfaces)

add_executable(command_pub src/command_pub_function.cpp)
ament_target_dependencies(command_pub rclcpp std_msgs my_custom_msgs)

install(TARGETS
  command_pub
  talker
  listener
  DESTINATION lib/${PROJECT_NAME})
  
ament_package()
