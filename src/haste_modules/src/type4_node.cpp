#include <linux/i2c-dev.h>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int16.hpp"

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

struct type4_params{
std::string name={};
int         wheel_type={};
int         radius={};

};



class Type4_Node : public rclcpp::Node
{
  public:
    Type4_Node(const std::string &name)       //constructor variable
    : Node(name)                              //initiate node name with the variable name
    {
      //declaring parameters sent from outside
      
      declare_parameter("name","Type4_default");
      declare_parameter("wheel_type",1);
      declare_parameter("radius",100);
      declare_parameter("i2c_adress",50);

      get_parameter("name",name_);
      get_parameter("wheel_type",wheel_type_);
      get_parameter("radius",radius_);
      get_parameter("i2c_adress",i2c_adress_);
      
      //create subscriber
      std::string aux_topic = "topic_";
      
      subscription_ = this->create_subscription<std_msgs::msg::Int16>(
        aux_topic+name_, 10,std::bind(&Type4_Node::topic_callback,this, _1));
    
      
      //start i2cbus 
      char *bus = "/dev/i2c-1";
      if((file = open(bus, O_RDWR)) < 0)
	    {
	      printf("Failed to open the bus. \n");
	    } 
    
    }

  private:
    //Params list
    std::string name_;
    int wheel_type_;
    int radius_;
    int i2c_adress_;
    //process received data
    void topic_callback(const std_msgs::msg::Int16 & msg)
    {
      uint8_t M[2];

      if(wheel_type_==1) //LEFT mmust revert

        if  (msg.data>=0){
            M[0]=1;
            M[1]=msg.data;
          }

          else{
            M[0]=0;
            M[1]=abs(msg.data);
          }
        
      else{             //RIGHT

        if  (msg.data>=0){
            M[0]=0;
            M[1]=msg.data;
          }

          else{
            M[0]=1;
            M[1]=abs(msg.data);
          }

      }


      ioctl(file, I2C_SLAVE, i2c_adress_);
      write(file, M, 2);

      RCLCPP_INFO(this->get_logger(), "Received motor control of:%d",msg.data);

    }

    rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr subscription_;
    int file;

};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Type4_Node>("Type4_Default"));
  rclcpp::shutdown();
  return 0;
}