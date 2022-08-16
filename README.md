# mai_sim

## Installation ⚙️

- RTAB-Map Installation : https://github.com/introlab/rtabmap_ros
~~~
sudo apt-get install ros-$ROS_DISTRO-rtabmap-ros
sudo apt install ros-$ROS_DISTRO-rtabmap ros-$ROS_DISTRO-rtabmap-ros

# Install RTAB-Map standalone libraries
cd ~
git clone https://github.com/introlab/rtabmap.git rtabmap
cd rtabmap/build
cmake .. [<---double dots included]
make -j6
sudo make install

# Install RTAB-Map ros-pkg
# If you install rtabmap-ros which is not support multi-camera, you need to remove it  
sudo apt remove ros-$ROS_DISTRO-rtabmap-ros

# After remove or install first time
cd ~/catkin_ws/src
git clone -b $ROS_DISTRO-devel https://github.com/introlab/rtabmap_ros.git
cd ~/catkin_ws
catkin_make -DRTABMAP_SYNC_MULTI_RGBD=ON 
~~~
```diff
- RTAB-Map Multi-Camera Not Working : https://github.com/introlab/rtabmap_ros/issues/459
- RTAB-Map Melodic Error Installation : https://enormous-bulb-826.notion.site/RTABMAP-a01c090bc07e49ceae4fc2187dc44f9c -
```

- Realsense (D435i) Installation : https://github.com/IntelRealSense/realsense-ros
~~~
sudo apt-get install ros-$ROS_DISTRO-realsense2-camera
sudo apt-get install ros-$ROS_DISTRO-realsense2-description
cd ~/catkin_ws/src/
git clone https://github.com/IntelRealSense/realsense-ros.git
cd realsense-ros/
git checkout `git tag | sort -V | grep -P "^2.\d+\.\d+" | tail -1`
cd ..
catkin_init_workspace
cd ..
catkin_make clean
catkin_make -DCATKIN_ENABLE_TESTING=False -DCMAKE_BUILD_TYPE=Release
catkin_make install
~~~

- IMU Filter Madwick Installation : https://github.com/CCNYRoboticsLab/imu_tools
~~~
sudo apt-get install ros-$ROS_DISTRO-imu-tools
cd ~/catkin_ws/src/
git clone -b $ROS_DISTRO https://github.com/CCNYRoboticsLab/imu_tools.git
rosdep install imu_tools
cd ~/catkin_ws
catkin_make
~~~

- Mask R-CNN Installation : https://github.com/matterport/Mask_RCNN
~~~
cd ~/catkin_ws/src
pip3 install -r requirements.txt
python3 setup.py install
~~~
```diff
- ROS Melodic needs to upgrade python3 (3.7.x or more) -
```

- Kobuki Installation : https://github.com/yujinrobot/kobuki
~~~
cd catkin_ws/src
git clone https://github.com/yujinrobot/kobuki
cd ..
catkin_make
~~~
 
- mai_sim Installation : https://github.com/Natanan-Tirasukvongsa/mai_sim.git
~~~
cd catkin_ws/src
git clone https://github.com/Natanan-Tirasukvongsa/mai_sim.git
cd ..
catkin_make
~~~

## Launch Files 📁
### RTAB-Map with 1 Realsense

- Launch RTAB-Map : http://wiki.ros.org/rtabmap_ros/Tutorials/HandHeldMapping
~~~
roslaunch realsense2_camera rs_camera.launch \
align_depth:=true \
unite_imu_method:="linear_interpolation" \
enable_gyro:=true \
enable_accel:=true

rosrun imu_filter_madgwick imu_filter_node \
_use_mag:=false \
_publish_tf:=false \
_world_frame:="enu" \
/imu/data_raw:=/camera/imu \
/imu/data:=/rtabmap/imu
~~~

- Mapping Mode : http://wiki.ros.org/rtabmap_ros/Tutorials/HandHeldMapping
~~~~
# --delete_db_on_start is used to start mapping from a clean database

roslaunch rtabmap_ros rtabmap.launch \
rtabmap_args:="--delete_db_on_start --Optimizer/GravitySigma 0.3" \
depth_topic:=/camera/aligned_depth_to_color/image_raw \
rgb_topic:=/camera/color/image_raw \
camera_info_topic:=/camera/color/camera_info \
approx_sync:=false \
wait_imu_to_init:=true \
imu_topic:=/rtabmap/imu

# Press CTRL+C to save map, map will be saved in ~/.ros/rtabmap.db
~~~~

![map](https://user-images.githubusercontent.com/78638430/184532129-a68e18e9-53cb-4499-a6cd-ac20ba12bbc6.png)
|:--:| 
| *Mapping with RTAB-Map* |


- Localization Mode : http://wiki.ros.org/rtabmap_ros/Tutorials/HandHeldMapping
~~~
#  resetting the odometry
rosservice call /rtabmap/reset_odom

roslaunch rtabmap_ros rtabmap.launch localization:=true
~~~

![localize](https://user-images.githubusercontent.com/78638430/184532139-8f7d73d5-ef50-4d4c-b8be-3da68bce1a21.png)
|:--:| 
| *Localization with RTAB-Map* |

### Kobuki 
~~~
roslaunch mai_sim kobuki.launch
~~~

![kobuki](https://user-images.githubusercontent.com/78638430/184531755-f6873998-30b4-415a-9be7-1c134c69d12a.png)
|:--:| 
| *Kobuki (Turtlebot2)* |

### Kobuki with Multi-Realsense
~~~
roslaunch mai_sim kobuki_rs.launch
~~~

![kobuki_rs](https://user-images.githubusercontent.com/78638430/184531813-eeaa8244-d36d-4c56-9354-51f273794b8c.png)
|:--:| 
| *Kobuki with Multi-Realsense* |

### Multi-Realsense
~~~
roslaunch mai_sim test_rs.launch
~~~

![rs](https://user-images.githubusercontent.com/78638430/184531874-32c3b962-d3ab-4b69-ad7c-23df0176c201.png)
|:--:| 
| *Multi-Realsense* |


### RTAB-Map with Multi-Realsense (***Not Complete***)
~~~
roslaunch mai_sim rtabmap_ros6.launch rtabmap_args:="--delete_db_on_start" 
~~~

## Object Detection and Segmentation 🧠
### Mask R-CNN (***Coming Soon***) 

![human_seg](https://user-images.githubusercontent.com/78638430/184534020-36ac0a5b-3160-476e-a702-6f93bbab0341.png)
:--:| 
| *Human Segmentation* |


## My Learning 📕
- Learning Diary : https://www.notion.so/Learning-SCRUM-c5d8c57dbaba445d9ce6e99bdd0f157d

- Create 3D Map with RTAB-Map :
  - http://official-rtab-map-forum.206.s1.nabble.com/How-to-process-RGBD-SLAM-datasets-with-RTAB-Map-td939.html#a647
  - http://official-rtab-map-forum.206.s1.nabble.com/RtabMap-reads-video-images-from-file-td1290.html#a1291
  - http://official-rtab-map-forum.206.s1.nabble.com/RGBD-source-td2869.html#a8086

- RGB-D SLAM Dataset : https://vision.in.tum.de/data/datasets/rgbd-dataset/download?fbclid=IwAR2UmqP0Cqyt3t_zzH-llFTA7--ZPEv5y4wq1kZ6i3BqDxu8GXX-KT0zFaI

- RTAB-Map with Multi - Camera :
  - http://official-rtab-map-forum.206.s1.nabble.com/RTABMAP-to-be-integrated-for-two-ZED2i-cameras-td8976.html 
  - https://www.heypasteit.com/clip/0ISEH9

- Quote : 
> You cannot change the past but you can still fuck up your future.
