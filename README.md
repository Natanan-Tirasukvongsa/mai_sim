# mai_sim

## Installation

- RTAB-Map Installation : https://github.com/introlab/rtabmap_ros

- RTAB-Map Melodic Error Installation : https://enormous-bulb-826.notion.site/RTABMAP-a01c090bc07e49ceae4fc2187dc44f9c

- Realsense Installation : https://github.com/IntelRealSense/realsense-ros

- IMU Filter Madwick Installation : https://github.com/CCNYRoboticsLab/imu_tools

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

## Launch Files
### RTAB-Map with 1 Realsense

Launch RTAB-Map : http://wiki.ros.org/rtabmap_ros/Tutorials/HandHeldMapping
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

Mapping Mode : http://wiki.ros.org/rtabmap_ros/Tutorials/HandHeldMapping
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

Localization Mode : http://wiki.ros.org/rtabmap_ros/Tutorials/HandHeldMapping
~~~
#  resetting the odometry
rosservice call /rtabmap/reset_odom

roslaunch rtabmap_ros rtabmap.launch localization:=true
~~~

### Kobuki 
~~~
roslaunch mai_sim kobuki.launch
~~~

### Kobuki with Multi-Realsense
~~~
roslaunch mai_sim kobuki_rs.launch
~~~

### Multi-Realsense
~~~
roslaunch mai_sim test_rs.launch
~~~

### RTAB-Map with Multi-Realsense (- ***Not Complete***)
~~~
roslaunch mai_sim rtabmap_ros6.launch rtabmap_args:="--delete_db_on_start" 
~~~
