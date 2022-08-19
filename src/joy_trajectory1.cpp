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

float velo_angular_start = 0; //start angular velocity
float velo_angular_stop = 0; //stop angular velocity
float velo_linear_start = 0; //start linear velocity
float velo_linear_stop = 0; //stop linear velocity

float save_velo_angular_stop = 0; //stop angular velocity
float save_velo_linear_stop = 0; //stop linear velocity

float tau_set = 3.0; // second

float velo_angular = 0.0; //desire angular velocity output
float velo_linear = 0.0; //desire linear velocity output

double time_start = 0.0; //save initial time

double tau = 0.0; //range of chaning time

int start_calulate = 1; //start calulate coeficient

class Joy
{
public:
    void Trajectory();
  
private:
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
    void poseCallback(const turtlesim::Pose::ConstPtr& msg);
 
   ros::NodeHandle nh_;
   ros::Publisher vel_pub_;
   ros::Subscriber joy_sub_;
   ros::Subscriber vel_sub_;
 
};

// Here we initialize some parameters: the linear_ and angular_ variables are used to define which axes of the joystick will control our turtle. We also check the parameter server for new scalar values for driving the turtle. 


void Joy::Trajectory()
{
    
  if (start_calulate)
  {
    c_0_angular = velo_angular_stop;
    c_3_angular = 10*(velo_angular_stop-velo_angular_start)/powf(tau_set,3);
    c_4_angular = 15*(velo_angular_start-velo_angular_stop)/powf(tau_set,4);
    c_5_angular = 6*(velo_angular_stop-velo_angular_start)/powf(tau_set,5);

    c_0_linear = velo_linear_stop;
    c_3_linear = 10*(velo_linear_stop-velo_linear_start)/powf(tau_set,3);
    c_4_linear = 15*(velo_linear_start-velo_linear_stop)/powf(tau_set,4);
    c_5_linear = 6*(velo_linear_stop-velo_linear_start)/powf(tau_set,5);

    start_calulate = 0;

    time_start = ros::Time::now().toSec();

    save_velo_angular_stop = velo_angular_stop;
    save_velo_linear_stop = velo_linear_stop;  
  }

  if (ros::Time::now().toSec()-time_start <= tau_set)
  {
    if (save_velo_angular_stop != velo_angular_stop || save_velo_linear_stop != velo_linear_stop)
    {
      start_calulate = 1; 
    }
    
    if (start_calulate == 0)
    {
      tau = ros::Time::now().toSec() - time_start;
      velo_angular = c_0_angular+c_1_angular*powf(tau,1)+c_2_angular*powf(tau,2)+c_3_angular*powf(tau,3)+c_4_angular*powf(tau,4)+c_5_angular*powf(tau,5);
      velo_linear = c_0_linear+c_1_linear*powf(tau,1)+c_2_linear*powf(tau,2)+c_3_linear*powf(tau,3)+c_4_linear*powf(tau,4)+c_5_linear*powf(tau,5);
    }
  }
  else
  {
    start_calulate = 1;
  }
  
    
  // Here we create a publisher that will advertise on the command_velocity topic of the turtle. 
    
  vel_pub_ = nh_.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 1);
  
  //   Here we subscribe to the joystick topic for the input to drive the turtle. If our node is slow in processing incoming messages on the joystick topic, up to 10 messages will be buffered before any are lost.
  
  joy_sub_ = nh_.subscribe<sensor_msgs::Joy>("joy", 10, &Joy::joyCallback, this);

  //rostopic echo /turtle1/pose
  //rostopic info /turtle1/pose
  //rosmsg show /turtle1/Pose

  vel_sub_ = nh_.subscribe<turtlesim::Pose>("turtle1/pose", 10, &Joy::poseCallback, this);
  // /*sub_name*/ = nh.subscribe</*msg_type*/>("/*topic_name*/", 10, /*subscribe_callback_name*/);
  
}
 

// Here we take the data from the joystick and manipulate it by scaling it and using independent axes to control the linear and angular velocities of the turtle. Finally we publish the prepared message. 

void Joy::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  geometry_msgs::Twist twist;
  twist.angular.z = joy->axes[0];
  twist.linear.x = joy->axes[1];
  velo_angular_stop = joy->axes[0];
  velo_linear_stop = joy->axes[1]; 
  vel_pub_.publish(twist);
}

void Joy::poseCallback(const turtlesim::Pose::ConstPtr& msg)
{
  velo_angular_start = msg->angular_velocity;
  velo_linear_start = msg->linear_velocity;
}


// Lastly we initialize our ROS node, create a teleop_turtle, and spin our node until Ctrl-C is pressed. 

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_turtle"); 
  Joy joy;
  ros::spin();
}