#!/usr/bin/env python

from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import cv2
import rospy


# convert ros image message to OpenCV image
def image_callback(img_msg):
    try:
        # webcam
        # cv_image = CvBridge().imgmsg_to_cv2(img_msg, "passthrough")
        
        # realsense
        cv_image = CvBridge().imgmsg_to_cv2(img_msg, desired_encoding="bgr8")

        cv2.imshow('frame', cv_image)
        cv2.waitKey(3)
        
        
    except CvBridgeError:
        pass
    

def reciever():
    rospy.init_node('realsense_reciever', anonymous=False)
    
    # webcam
    # webcam_info = rospy.Subscriber('/webcam',Image ,image_callback)
    
    # realsense 
    realsense_info = rospy.Subscriber('/camera/color/image_raw',Image ,image_callback)
    
    
    while not rospy.is_shutdown():
        rospy.spin()
    

if __name__=='__main__':
    try:
        reciever()
    except rospy.ROSInternalException:
        pass