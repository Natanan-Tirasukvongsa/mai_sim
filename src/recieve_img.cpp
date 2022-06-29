#include <ros/ros.h>
#include <image_transport/image_transport.h> // image_transport/image_transport.h includes everything we need to publish and subscribe to images. 
#include <opencv2/highgui/highgui.hpp> //These headers will allow us to display images using OpenCV's simple GUI capabilities. 
#include <cv_bridge/cv_bridge.h> //These headers will allow us to display images using OpenCV's simple GUI capabilities. 

//don't forget to run this : source devel/setup.bash
//after that run : rosrun mai_sim recieve_img


// This is the callback function that will get called when a new image has arrived on the "camera/image" topic. Although the image may have been sent in some arbitrary transport-specific message type, notice that the callback need only handle the normal sensor_msgs/Image type. All image encoding/decoding is handled automagically for you. 
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  //The body of the callback. We convert the ROS image message to an OpenCV image with BGR pixel encoding, then show it in a display window. See this tutorial for more on ROS-OpenCV image conversion. 
  try
  {
    cv::imshow("view", cv_bridge::toCvShare(msg, "bgr8")->image);
    cv::waitKey(10);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");

  // Create an OpenCV display window. 
  ros::NodeHandle nh;
  cv::namedWindow("view");

  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
  
  // Dispose of our display window. 
  ros::spin();
  cv::destroyWindow("view");
}
