cmake_minimum_required(VERSION 3.8)
project(haste_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # comment the line when a copyright and license is added to all source files
  set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # comment the line when this package is in a git repo and when
  # a copyright and license is added to all source files
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()


add_executable(simple_motor_control src/simple_motor_control.cpp)
ament_target_dependencies(simple_motor_control rclcpp std_msgs)

add_executable(omni_control src/omni_control.cpp)
ament_target_dependencies(omni_control rclcpp std_msgs)

add_executable(line_follow src/line_follow.cpp)
ament_target_dependencies(line_follow rclcpp std_msgs)

install(TARGETS
  simple_motor_control
  omni_control
  line_follow
  DESTINATION lib/${PROJECT_NAME})


ament_package()
