#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>

// geometry_msgs/Twist.h includes the twist msg so that we can publish twist commands to the turtle
// sensor_msgs/Joy.h includes the joystick msg so that we can listen to the joy topic 

// Here we create the TeleopTurtle class and define the joyCallback function that will take a joy msg. We also create a node handle, publisher, and subscriber for later use. 


class TeleopTurtle
{
public:
    TeleopTurtle();
  
private:
    void joyCallback(const sensor_msgs::Joy::ConstPtr& joy);
 
   ros::NodeHandle nh_;
 
   int linear_, angular_;
   double l_scale_, a_scale_;
   ros::Publisher vel_pub_;
   ros::Subscriber joy_sub_;
 
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
}
 

// Here we take the data from the joystick and manipulate it by scaling it and using independent axes to control the linear and angular velocities of the turtle. Finally we publish the prepared message. 

void TeleopTurtle::joyCallback(const sensor_msgs::Joy::ConstPtr& joy)
{
  geometry_msgs::Twist twist;
  twist.angular.z = a_scale_*joy->axes[angular_];
  twist.linear.x = l_scale_*joy->axes[linear_];
  vel_pub_.publish(twist);
}


// Lastly we initialize our ROS node, create a teleop_turtle, and spin our node until Ctrl-C is pressed. 

int main(int argc, char** argv)
{
  ros::init(argc, argv, "teleop_turtle");
  TeleopTurtle teleop_turtle;
 
  ros::spin();
}