#!/usr/bin/env python

import cv2

# do not forget to do this : 
# cd catkin_ws/src/mai_sim/src
# chmod +x video_cam.py

#  VideoCapture  has the device index or the name of a video file
# 0 : first or primary camera
# 1 : second camera, etc. 
# capture the video frame by frame
cap = cv2.VideoCapture(0) 

# Returns true if video capturing has been initialized already.
# print(cap.isOpened())

if not cap.isOpened():
    print("Cannot open camera")

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()
    # print(ret) boolean
    # print(frame) picture
    
    if ret: #true
        # Display the resulting frame
        cv2.imshow('frame', frame)
        
        # Press q on keyboard to  exit
        if cv2.waitKey(1) == ord('q'):
            break
    
    # break the loop
    else:
        break

# When everything done, release the video capture object
cap.release()
# Closes all the frames
cv2.destroyAllWindows()

