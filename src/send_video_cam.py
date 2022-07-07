#!/usr/bin/env python

from numpy import rate
import rospy
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import cv2

cap = cv2.VideoCapture(0)
# print(cap.isOpened())
bridge = CvBridge()

# can launch : rosrun mai_sim send_video_cam.py
# or launch : roslaunch mai_sim realsense.launch 

def talker():
    # topic named /realsense
    pub = rospy.Publisher('/webcam', Image, queue_size=1)
    #node named image
    rospy.init_node('realsense_talker', anonymous=False)
    # sleep time
    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        ret, frame = cap.read()
        if not ret:
            break
        
        # convert number matrix to message 
        msg = bridge.cv2_to_imgmsg(frame, "bgr8")
        
        # publish image
        pub.publish(msg)
        
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
        
        if rospy.is_shutdown():
            cap.release()
    

if __name__=='__main__':
    try:
        talker()
    except rospy.ROSInternalException:
        pass