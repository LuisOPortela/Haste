cmake_minimum_required(VERSION 3.8)
project(haste_manager)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(my_library REQUIRED)


add_executable(node_manager src/node_manager.cpp)
ament_target_dependencies(node_manager rclcpp std_msgs my_library)

install(TARGETS 
node_manager 
DESTINATION lib/${PROJECT_NAME})

ament_package()
