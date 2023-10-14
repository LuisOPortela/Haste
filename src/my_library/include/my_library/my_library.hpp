#ifndef MY_LIBRARY_H
#define MY_LIBRARY_H

#include <string>
#include <vector>

struct type1_params{
std::string name={};
int measure_frequency=0;
int number_of_values=0;
int max_value=0;
};

struct type2_params{
std::string name={};
std::string control_node={};
int measure_frequency=0;
int number_of_values=0;
int max_value=0;
};

struct type31_params{
std::string name={};
std::string sensor_name={};
int on_percentage=0;   //set max to 100
};

struct type4_params{
std::string name={};
std::string control_node={};
int wheel_type=0; 
int radius=0;   //set max to 100
};

struct module_name_adress{
std::string name;
uint8_t adress;
};

//Type1 params
#define NODE_NAME_SIZE 15
#define MEASURE_FREQUENCY_SIZE 2
#define NUMBER_OF_VALUE_SIZE 1
#define MAX_VALUE_SIZE 2

//Type2 params
//#define NODE_NAME_SIZE 15
//#define MEASURE_FREQUENCY_SIZE 2
//#define NUMBER_OF_VALUE_SIZE 1
//#define MAX_VALUE_SIZE 2
#define CONTROL_NODE_SIZE 15

//Type3.1 params
//#define NODE_NAME_SIZE 15
#define SENSOR_NAME_SIZE 15
#define ACTIVATION_PERCENTAGE_SIZE 1  


//Type4 params
//#define NODE_NAME_SIZE 15
#define WHEEL_TYPE_SIZE 1
#define RADIUS_SIZE 1


//Ms Message definition
#define M_ASK_FOR_TYPE 1
#define M_T1_PARAMS_1 2
#define M_T1_PARAMS_2 3

#define M_T2_PARAMS 2
#define M_T31_PARAMS 2
#define M_T4_PARAMS 2

#define T1_MESSAGE_1_SIZE NODE_NAME_SIZE+MEASURE_FREQUENCY_SIZE+NUMBER_OF_VALUE_SIZE+MAX_VALUE_SIZE
#define T2_MESSAGE_SIZE   NODE_NAME_SIZE+MEASURE_FREQUENCY_SIZE+NUMBER_OF_VALUE_SIZE+MAX_VALUE_SIZE+CONTROL_NODE_SIZE
#define T31_MESSAGE_SIZE  NODE_NAME_SIZE+SENSOR_NAME_SIZE+ACTIVATION_PERCENTAGE_SIZE
#define T4_MESSAGE_SIZE   NODE_NAME_SIZE+WHEEL_TYPE_SIZE+RADIUS_SIZE

#endif