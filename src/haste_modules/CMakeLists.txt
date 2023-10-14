cmake_minimum_required(VERSION 3.5)
project(haste_modules)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(my_library REQUIRED)


add_executable(type1_node src/type1_node.cpp)
ament_target_dependencies(type1_node rclcpp std_msgs my_library)

add_executable(type2_node src/type2_node.cpp)
ament_target_dependencies(type2_node rclcpp std_msgs my_library)

add_executable(type31_node src/type31_node.cpp)
ament_target_dependencies(type31_node rclcpp std_msgs my_library)

add_executable(type4_node src/type4_node.cpp)
ament_target_dependencies(type4_node rclcpp std_msgs my_library)



install(TARGETS
  type1_node
  type31_node
  type4_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()
