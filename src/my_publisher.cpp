#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>

int main(int argc, char** argv)
{
      /**
      * The ros::init() function needs to see argc and argv so that it can perform
      * any ROS arguments and name remapping that were provided at the command line.
      * For programmatic remappings you can use a different version of init() which takes
      * remappings directly, but for most command-line programs, passing argc and argv is
      * the easiest way to do it.  The third argument to init() is the name of the node.
      *
      * You must call one of the versions of ros::init() before using any other
      * part of the ROS system.
      */
  ros::init(argc, argv, "image_publisher");


      /**
      * NodeHandle is the main access point to communications with the ROS system.
      * The first NodeHandle constructed will fully initialize this node, and the last
      * NodeHandle destructed will close down the node.
      */
  ros::NodeHandle nh;
  image_transport::ImageTransport it(nh);

      /**
      * The advertise() function is how you tell ROS that you want to
      * publish on a given topic name. This invokes a call to the ROS
      * master node, which keeps a registry of who is publishing and who
      * is subscribing. After this advertise() call is made, the master
      * node will notify anyone who is trying to subscribe to this topic name,
      * and they will in turn negotiate a peer-to-peer connection with this
      * node.  advertise() returns a Publisher object which allows you to
      * publish messages on that topic through a call to publish().  Once
      * all copies of the returned Publisher object are destroyed, the topic
      * will be automatically unadvertised.
      *
      * The second parameter to advertise() is the size of the message queue
      * used for publishing messages.  If messages are published more quickly
      * than we can send them, the number here specifies how many messages to
      * buffer up before throwing some away.
      */
  image_transport::Publisher pub = it.advertise("camera/image", 1);

  cv::Mat image = cv::imread(argv[1], cv::IMREAD_COLOR);
  sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", image).toImageMsg();

      /**A ros::Rate object allows you to specify a frequency that you would like to loop at. It will keep track of how long it has been since the last call to Rate::sleep(), and sleep for the correct amount of time. */
  ros::Rate loop_rate(5); /** 5 Hz **/


  while (nh.ok()) {
    pub.publish(msg);

      /**Calling ros::spinOnce() here is not necessary for this simple program, because we are not receiving any callbacks. However, if you were to add a subscription into this application, and did not have ros::spinOnce() here, your callbacks would never get called. So, add it for good measure.*/ 
    ros::spinOnce();

      /**Now we use the ros::Rate object to sleep for the time remaining to let us hit our 10Hz publish rate. */
    loop_rate.sleep();
  }
}

