#include <cstdlib> // Use for the absolute value method abs()
#include <iostream> // Enables command line input and output
 
#include "ros/ros.h" // Necessary header files for ROS
#include "geometry_msgs/Twist.h" // Twist messages (linear & angular velocity)
#include "geometry_msgs/Pose2D.h" // x, y position and theta orientation
#include "turtlesim/Pose.h" // x, y, theta, linear & angular velocity
#include <sensor_msgs/Joy.h>
#include "nav_msgs/Odometry.h"
 
// Remove the need to use std:: prefix
using namespace std;
 
// Key variable declarations 
geometry_msgs::Twist velCommand; // Linear and angular velocity in m/s 
geometry_msgs::Pose2D current; // Current x, y, and theta 

geometry_msgs::Twist velStop; // Stop angula velocity and linear velocity

// turtlesim::Pose velStart; // Start Linear and angular velocity in m/s 
geometry_msgs::Twist velStart;

int start = 0; //start calculate trajectory

float timer = 0.0; //time chaning @trajectory
float beginTime; //start trajectory
float durationTime; //time offset
float endTime; //stop trajectory
float tau; // trajectory time
float tau_max = 1.5; // maximum time

float tau_max_angular = 0.0; // trajectory in second
float tau_max_linear = 0.0; // trajectory in second

float velo_max_angular = 0.2; // trajectory in second (max @ pi rad/s)
float velo_max_linear = 0.15; // trajectory in second (max @ 0.7 m/s)

float acc_max_angular = 0.2; // maximum angular acceleration
float jerk_max_angular = 0.1; //maimum angular jerk 
float acc_max_linear = 0.1; // maximum linear acceleration
float jerk_max_linear = 0.05; //maimum linear jerk 

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

 
// Initialized variables and take care of other setup tasks
void setup() {

  // Initialize the Twist message.
  // Initial linear and angular velocities are 0 m/s and rad/s, respectively.
  velCommand.linear.x = 0.0;
  velCommand.linear.y = 0.0;
  velCommand.linear.z = 0.0;
  velCommand.angular.x = 0.0;
  velCommand.angular.y = 0.0;
  velCommand.angular.z = 0.0;

  velStop.angular.z = 0.0;
  velStop.linear.x = 0.0; 

//   velStart.angular_velocity = 0.0;
//   velStart.linear_velocity = 0.0; 
  velStart.angular.z = 0.0;
  velStart.linear.x = 0.0;
}

// This callback function updates the current position and 
// orientation of the robot. 
void callbackPose(const turtlesim::PoseConstPtr &currentPose) {
  current.x = currentPose->x;
  current.y = currentPose->y;
  current.theta = currentPose->theta;
}

// void callbackVel(const geometry_msgs::Twist &currentVel)
// {
// //   velStart.angular_velocity = currentVel->angular_velocity;
// //   velStart.linear_velocity = currentVel->linear_velocity; 

//     //recieve velocity from robot
//     velStart.angular.z = currentVel.angular.z;
//     velStart.linear.x = currentVel.linear.x;

//     // velStart.angular.z = velStop.angular.z;
//     // velStart.linear.x = velStop.linear.x;
// }

void callbackVelKobuki(const nav_msgs::Odometry::ConstPtr& odom)
{

    //recieve velocity from robot
    velStart.angular.z = odom->twist.twist.angular.z;
    velStart.linear.x = odom->twist.twist.linear.x;
}

void callbackJoy(const sensor_msgs::JoyConstPtr &joy) {
    //recieve value from joystick
    if (joy->axes[0] == 0.0 && joy->axes[1]==0.0)
    {
      velStop.angular.z = velo_max_angular*joy->axes[3];
      velStop.linear.x = 0; 
    }
    else
    {
      velStop.angular.z = velo_max_angular*joy->axes[0];
      velStop.linear.x = -velo_max_linear*joy->axes[1];  //-1 to inverse kobuki velocity
    }
    start = 1;
}

void trajectory()
{
    if (start)
    {
        start = 0;

        // tau_max_angular = 15/8*(velStop.angular.z - velStart.angular.z)/acc_max_angular >= sqrtf(fabs(((10*powf(3+sqrtf(3),1))-(5*powf(3+sqrtf(3),2))+(5*powf(3+sqrtf(3),3)/9))*(velStop.angular.z-velStart.angular.z)/jerk_max_angular)) ? 15/8*(velStop.angular.z - velStart.angular.z)/acc_max_angular : sqrtf(fabs(((10*powf(3+sqrtf(3),1))-(5*powf(3+sqrtf(3),2))+(5*powf(3+sqrtf(3),3)/9))*(velStop.angular.z-velStart.angular.z)/jerk_max_angular));
        // tau_max_linear = 15/8*(velStop.linear.x - velStart.linear.x)/acc_max_linear >= sqrtf(fabs(((10*powf(3+sqrtf(3),1))-(5*powf(3+sqrtf(3),2))+(5*powf(3+sqrtf(3),3)/9))*(velStop.linear.x-velStart.linear.x)/jerk_max_linear)) ? 15/8*(velStop.linear.x - velStart.linear.x)/acc_max_linear : sqrtf(fabs(((10*powf(3+sqrtf(3),1))-(5*powf(3+sqrtf(3),2))+(5*powf(3+sqrtf(3),3)/9))*(velStop.linear.x-velStart.linear.x)/jerk_max_linear));
        // tau_max = tau_max_angular >= tau_max_linear ? tau_max_angular : tau_max_linear;

        c_0_angular =  velStart.angular.z;
        c_3_angular = 10*(velStop.angular.z-velStart.angular.z)/powf(tau_max,3);
        c_4_angular = 15*(velStart.angular.z- velStop.angular.z)/powf(tau_max,4);
        c_5_angular = 6*( velStop.angular.z-velStart.angular.z)/powf(tau_max,5);

        c_0_linear = velStart.linear.x;
        c_3_linear = 10*(velStop.linear.x-velStart.linear.x)/powf(tau_max,3);
        c_4_linear = 15*(velStart.linear.x-velStop.linear.x)/powf(tau_max,4);
        c_5_linear = 6*(velStop.linear.x-velStart.linear.x)/powf(tau_max,5);

        beginTime = timer;
        durationTime = tau_max;
        endTime = beginTime + durationTime;
    }
    else
    {
        if (timer <= endTime)
        {
            tau = timer - beginTime;
            velCommand.angular.z = c_0_angular+c_1_angular*powf(tau,1)+c_2_angular*powf(tau,2)+c_3_angular*powf(tau,3)+c_4_angular*powf(tau,4)+c_5_angular*powf(tau,5);
            velCommand.linear.x = c_0_linear+c_1_linear*powf(tau,1)+c_2_linear*powf(tau,2)+c_3_linear*powf(tau,3)+c_4_linear*powf(tau,4)+c_5_linear*powf(tau,5);
        }
        else
        {
            start = 1;
        }
    }
}

int main(int argc, char **argv) {
 
  setup();  
 
  // Initiate ROS
  ros::init(argc, argv, "trajectory");
     
  // Create the main access point to communicate with ROS
  ros::NodeHandle node;
 
  // Subscribe to the robot's pose
  // Hold no messages in the queue. Automatically throw away 
  // any messages that are received that are not able to be
  // processed quickly enough.
  // Every time a new pose is received, update the robot's pose.
  // ros::Subscriber currentPoseSub = node.subscribe("turtle1/pose", 0, callbackPose);

  // Subscribe to the Robot's velocity
                                                  
  // ros::Subscriber currentVelSub = node.subscribe("turtle1/cmd_vel", 0, callbackVel);

  ros::Subscriber kobukiVel = node.subscribe("odom", 0, callbackVelKobuki);

  // Subscribe to the Joy's axis
  ros::Subscriber joyAxisSub = node.subscribe("joy", 0, callbackJoy);
 
  // Publish velocity commands to a topic.
  // Hold no messages in the queue. Automatically throw away 
  // any messages that are received that are not able to be
  // processed quickly enough.
                                                                      // turtle1/cmd_vel
  ros::Publisher velocityPub = node.advertise<geometry_msgs::Twist>("mobile_base/commands/velocity", 0);
 
  // Specify a frequency that want the while loop below to loop at
  // In this case, we want to loop 10 cycles per second
  ros::Rate loop_rate(10); 
 
  // Keep running the while loop below as long as the ROS Master is active. 
  while (ros::ok()) {

    // Here is where we call the callbacks that need to be called.
    ros::spinOnce();
 
    // After we call the callback function to update the robot's pose, we 
    // set the velocity values for the robot.
    // setVelocity();

    trajectory();
 
    // Publish the velocity command to the ROS topic
    velocityPub.publish(velCommand);

    timer += 0.1;

    // Print the output to the console
    cout << "velStop.angular.z = " << velStop.angular.z << endl
         << "velStop.linear.x = " << velStop.linear.x << endl
         << "velCommand.angular.z= " << velCommand.angular.z << endl
         << "velCommand.linear.x = " << velCommand.linear.x << endl
         << "beginTime = " << beginTime << " s" << endl
         << "durationTime = " << durationTime << " s" << endl
         << "endTime = " << endTime << " s" << endl
         << "tau = " << tau << " s" << endl
         << endl;
 
    // Sleep as long as we need to to make sure that we have a frequency of
    // 10Hz
    loop_rate.sleep();
  }
 
  return 0;
}