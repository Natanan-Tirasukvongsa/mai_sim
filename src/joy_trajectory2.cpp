#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <cmath>
#include <turtlesim/Pose.h>

// geometry_msgs/Twist.h includes the twist msg so that we can publish twist commands to the turtle
// sensor_msgs/Joy.h includes the joystick msg so that we can listen to the joy topic 

// Here we create the TeleopTurtle class and define the joyCallback function that will take a joy msg. We also create a node handle, publisher, and subscriber for later use. 
float c_0_angular = 0; //first coeficient for augular velocity 
float c_1_angular = 0; //second coeficient for augular velocity 
float c_2_angular = 0; //third coeficient for augular velocity 
float c_3_angular = 0; //fourth coeficient for augular velocity 
float c_4_angular = 0; //fifth coeficient for augular velocity 
float c_5_angular = 0; //sixth coeficient for augular velocity 

float c_0_linear = 0; //first coeficient for linear velocity 
float c_1_linear = 0; //second coeficient for linear velocity 
float c_2_linear = 0; //third coeficient for linear velocity 
float c_3_linear = 0; //fourth coeficient for linear velocity 
float c_4_linear = 0; //fifth coeficient for linear velocity 
float c_5_linear = 0; //sixth coeficient for linear velocity 

geometry_msgs::Twist velStart; // Start Linear and angular velocity in m/s 
geometry_msgs::Twist velCommand; // Output Linear and angular velocity in m/s 
geometry_msgs::Twist velStop; // Stop angula velocity and linear velocity

int start_calulate = 0; //start calulate coeficient

float tau_set = 3.0; // second

double beginTime;
double durationTime;
double endTime;
double tau;

ros::Publisher vel_pub_;

class TeleopTurtle
{
public:
    TeleopTurtle();
private:
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
    void twistCallback(const geometry_msgs::Twist::ConstPtr& msg);
 
   int linear_, angular_;
   double l_scale_, a_scale_;

   ros::NodeHandle nh_;
    ros::Publisher vel_pub_;
   ros::Subscriber joy_sub_;
   ros::Subscriber vel_sub_;
 
};

// Here we initialize some parameters: the linear_ and angular_ variables are used to define which axes of the joystick will control our turtle. We also check the parameter server for new scalar values for driving the turtle. 

TeleopTurtle::TeleopTurtle():
    linear_(1),
    angular_(2)
{    
   nh_.param("axis_linear", linear_, linear_);
   nh_.param("axis_angular", angular_, angular_);
   nh_.param("scale_angular", a_scale_, a_scale_);
   nh_.param("scale_linear", l_scale_, l_scale_);
  // Here we create a publisher that will advertise on the command_velocity topic of the turtle. 
    
  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);
  
  //   Here we subscribe to the joystick topic for the input to drive the turtle. If our node is slow in processing incoming messages on the joystick topic, up to 10 messages will be buffered before any are lost.
  
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &TeleopTurtle::joyCallback, this);

  //rostopic echo /turtle1/pose
  //rostopic info /turtle1/pose
  //rosmsg show /turtle1/Pose

  vel_sub_ = nh_.subscribe<geometry_msgs::Twist>("turtle1/cmd_vel", 10, &TeleopTurtle::twistCallback, this);
  // /*sub_name*/ = nh.subscribe</*msg_type*/>("/*topic_name*/", 10, /*subscribe_callback_name*/);
}
 

// Here we take the data from the joystick and manipulate it by scaling it and using independent axes to control the linear and angular velocities of the turtle. Finally we publish the prepared message. 

void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
    velStop.angular.z = a_scale_*joy->axes[angular_];
    velStop.linear.x = l_scale_*joy->axes[linear_]; 

    start_calulate = 1;
}

void TeleopTurtle::twistCallback(const geometry_msgs::Twist::ConstPtr& msg)
{
    velStart.angular.z = msg->angular.z ;
    velStart.linear.x = msg->linear.x;
}

// Lastly we initialize our ROS node, create a teleop_turtle, and spin our node until Ctrl-C is pressed. 

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_turtle"); 
  TeleopTurtle teleop_turtle;

  velCommand.angular.z = 0.5;
  velCommand.linear.x = 1;

  vel_pub_.publish(velCommand);

  ros::spin();
    // while(ros::ok())
    // {
    //     if (start_calulate)
    //     {
    //         c_0_angular =  velStop.angular.z;
    //         c_3_angular = 10*(velStop.angular.z-velStart.angular.z)/powf(tau_set,3);
    //         c_4_angular = 15*(velStart.angular.z- velStop.angular.z)/powf(tau_set,4);
    //         c_5_angular = 6*( velStop.angular.z-velStart.angular.z)/powf(tau_set,5);

    //         c_0_linear = velStop.linear.x;
    //         c_3_linear = 10*(velStop.linear.x-velStart.linear.x)/powf(tau_set,3);
    //         c_4_linear = 15*(velStart.linear.x-velStop.linear.x)/powf(tau_set,4);
    //         c_5_linear = 6*(velStop.linear.x-velStart.linear.x)/powf(tau_set,5);

    //         start_calulate = 0;

    //         beginTime = ros::Time::now().toSec();
    //         durationTime = ros::Duration(1).toSec();
    //         endTime = beginTime + durationTime;
    //     }

    //     if(ros::Time::now().toSec()<= endTime)
    //     {
    //         tau = ros::Time::now().toSec() - beginTime;
    //         velCommand.angular.z = 0.5;
    //         velCommand.linear.x = 1;
    //         // velCommand.angular.z = c_0_angular+c_1_angular*powf(tau,1)+c_2_angular*powf(tau,2)+c_3_angular*powf(tau,3)+c_4_angular*powf(tau,4)+c_5_angular*powf(tau,5);
    //         // velCommand.linear.x = c_0_linear+c_1_linear*powf(tau,1)+c_2_linear*powf(tau,2)+c_3_linear*powf(tau,3)+c_4_linear*powf(tau,4)+c_5_linear*powf(tau,5);
    //         vel_pub_.publish(velCommand);
    //         ros::Duration(1).sleep();
    //     }
    //     else
    //     {
    //         start_calulate = 1;
    //     }

    // }
}