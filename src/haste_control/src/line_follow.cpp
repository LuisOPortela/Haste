#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16_multi_array.hpp"
#include "std_msgs/msg/int16.hpp"

//#include "std_msgs/msg/string.hpp"

#include <chrono>
#include <cstdlib>
#include <memory>
#include <functional>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <fcntl.h>

using std::placeholders::_1;


class Line_follow_control : public rclcpp::Node
{
  public:
    Line_follow_control(const std::string &name)       //constructor variable
    : Node(name)                              //initiate node name with the variable name
    {

      subscription_ = this->create_subscription<std_msgs::msg::Int16MultiArray>(
        "topic_Tracker_Sensor", 10,std::bind(&Line_follow_control::sub_callback,this, _1));
    
      pub_motor1_ = this->create_publisher<std_msgs::msg::Int16>("topic_Motor_Left", 10);
      pub_motor2_ = this->create_publisher<std_msgs::msg::Int16>("topic_Motor_Right", 10);
    
      declare_parameter("kp",3.0);
      declare_parameter("kd",0.0);


      get_parameter("kp",kp_);
      get_parameter("kd",kd_);

    
    
    
    
    
    
    
    }




  private:

    int last_proportional = 0;
    int integral = 0;
    int spd = 75; 
    int c = 0;
    float distance;

    //PID
    int i; 
    int power_difference = 0;
    double kp_, Ki=0, kd_;
    unsigned int position;
    int derivative, proportional;

    int left_motor;
    int right_motor;

    void sub_callback(const std_msgs::msg::Int16MultiArray & msg)
    {

      //PROCESS DATA
      //RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "msg1 %d msg2 %d msg3 %d msg4 %d msg5 %d",msg.data[0],msg.data[1],msg.data[2],msg.data[3],msg.data[4]);
      int position=( 4000*msg.data[0]+ 3000*msg.data[1] + 2000*msg.data[2] + 1000*msg.data[3] + 0*msg.data[4])/
      (msg.data[0] + msg.data[1] + msg.data[2] + msg.data[3] + msg.data[4]);

      RCLCPP_INFO(this->get_logger(), "position  %d", position);
      
      proportional = (position - 2000);
    
      derivative = proportional - last_proportional;
      integral = integral+proportional;

      last_proportional = proportional;
    // use the tutorial to set initial values of Kp, Ki, and Kd
      
      power_difference = proportional*kp_ + integral*Ki + derivative*kd_;
      const int max = spd/2 + 30;
      if(power_difference > max)
       power_difference = max;
      if(power_difference < -max)
      power_difference = (-1*max);

      if(power_difference < 0){ //left
        left_motor=max+power_difference;
        right_motor=max;
      } 

      else{  //right 
        left_motor=max;
        right_motor=max-power_difference;
      } 
      
      //Send Left Motor command
      auto message = std_msgs::msg::Int16();

      message.data=left_motor;
      
      pub_motor1_->publish(message);
      
      //Send Right Motor command
      message.data=right_motor;
      
      pub_motor2_->publish(message);




    }
    
    /*
    void node_manager_callback(const std_msgs::msg::Int16 & msg)
    {
      //when a new motor is connected a new publisher is created
  
    }
    */
    rclcpp::Subscription<std_msgs::msg::Int16MultiArray>::SharedPtr subscription_;
    rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr pub_motor1_;
    rclcpp::Publisher<std_msgs::msg::Int16>::SharedPtr pub_motor2_;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Line_follow_control>("Simple_motor_control"));
  rclcpp::shutdown();
  return 0;
}