# mai_sim
mai_sim package is used for 3D SLAM in dynamic environment using deep learning. Deep learning detects dynamic obstacle such as human. It will filter dynamic obstacle after detecting. Image without dynamic obstacle will be an input for building 3D map. It use RTAB-Map package to make 3D SLAM.      

---
<a name="table"></a>
## Table of Contents
* [Installation](#setup)
   * [RTAB-Map Installation](#RTAB_in) 
   * [Realsense (D435i) Installation](#RS_in) 
   * [IMU Filter Madwick Installation](#IMU_in)
   * [Mask R-CNN Installation](#Mask_in)
   * [Kobuki Installation](#kobuki_in)
   * [Turtlesim Installation](#turtle_in)
   * [Joystick Installation](#joy_in)
   * [mai_sim Installation](#mai_in)
* [Launch Files](#launch)
  * [RTAB-Map with 1 Realsense](#RTAB_L)
     * [Launching](#RTAB_L_L)
     * [RTAB-Map Mapping Mode](#RTAB_L_M)
     * [RTAB-Map Localization Mode](#RTAB_L_Lo)
  * [Kobuki](#kobuki_L)
  * [Kobuki with Multi-Realsense](#kobuki_M_L)
  * [Multi-Realsense](#M_L)
    * [2 Realsense](#M_L_2)
    * [4 Realsense](#M_L_4)
  * [RTAB-Map with Multi-Realsense](#RTAB_M_L)
  * [Turtlesim Trajectory Control Using Joystick](#T_J_L)
  * [Kobuki Trajectory Control Using Joystick](#kobuki_J_L)  
* [Object Detection and Segmentation](#technology)
  * [Mask R-CNN](#Mask) 
    * [Demo Mask-RCNN](#DMR)
    * [Segmentation Only Person](#SOP)
    * [Average Computation Time for Sub - Processes](#AVE_M)
    * [All Example Code](#EX_M)
* [My Learning](#learning)

---
<a name="setup"></a>
## Installation ⚙️ 
<a name="RTAB_in"></a>
### RTAB-Map Installation 
1. Install RTAB-Map Binaries : [Link](https://github.com/introlab/rtabmap_ros)
~~~
sudo apt-get install ros-$ROS_DISTRO-rtabmap-ros
sudo apt install ros-$ROS_DISTRO-rtabmap ros-$ROS_DISTRO-rtabmap-ros
~~~
2. Install RTAB-Map standalone libraries 
~~~
cd ~
git clone https://github.com/introlab/rtabmap.git rtabmap
cd rtabmap/build
cmake .. [<---double dots included]
make -j6
sudo make install
~~~
3. Install RTAB-Map ros-pkg 
```diff
! If you install rtabmap-ros which is not support multi-camera, you need to remove it  
! Also delete rtabmap-ros in catkin_ws/src, build folder and devel folder too 
! If you do not use multi-camera, you can skip this process
```
~~~
sudo apt remove ros-$ROS_DISTRO-rtabmap-ros
~~~
```diff
@@ After remove rtabmap-ros or install first time @@
```
~~~
cd ~/catkin_ws/src
git clone -b $ROS_DISTRO-devel https://github.com/introlab/rtabmap_ros.git
cd ~/catkin_ws
catkin_make -DRTABMAP_SYNC_MULTI_RGBD=ON 
~~~
```diff
- RTAB-Map Multi-Camera Not Working : https://github.com/introlab/rtabmap_ros/issues/459 
- RTAB-Map Melodic Error Installation : https://enormous-bulb-826.notion.site/RTABMAP-a01c090bc07e49ceae4fc2187dc44f9c 
```
<a name="RS_in"></a>
### Realsense (D435i) Installation 
1. Install Realsense (D435i) : [Link](https://github.com/IntelRealSense/realsense-ros)
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
<a name="IMU_in"></a>
### IMU Filter Madwick Installation 
1. Install Madwick : [Link](https://github.com/CCNYRoboticsLab/imu_tools)
~~~
sudo apt-get install ros-$ROS_DISTRO-imu-tools
cd ~/catkin_ws/src/
git clone -b $ROS_DISTRO https://github.com/CCNYRoboticsLab/imu_tools.git
rosdep install imu_tools
cd ~/catkin_ws
catkin_make
~~~
<a name="Mask_in"></a>
### Mask R-CNN Installation
1. Requirement package : [Link](https://github.com/BupyeongHealer/Mask_RCNN_tf_2.x/blob/master/requirements.txt)
```diff 
- numpy
- scipy
- Pillow
- cython
- matplotlib
- scikit-image
- tensorflow>=1.3.0
- keras>=2.0.8
- opencv-python
- h5py
- imgaug
- IPython[all]
```
2. My Using Version
```diff
@@ Python         3.8.10    @@
@@ numpy          1.19.5    @@
@@ Pillow         9.2.0     @@
@@ Cython         0.29.14   @@
@@ matplotlib     3.1.2     @@
@@ scikit-image   0.19.3    @@
@@ tensorflow     2.4.3     @@
@@ Keras          2.4.0     @@
@@ opencv-python  4.6.0.66  @@
@@ h5py           2.10.0    @@
@@ imgaug         0.4.0     @@
@@ ipython        7.34.0    @@
@@ pycocotools    2.0.4     @@
@@ Ubuntu         20.04 LTS @@
```
3. Change Python Version 
    - Checking version
    ~~~
    python3 --version
    ~~~
    - Changing version :  [Link](https://www.itsupportwale.com/blog/how-to-upgrade-to-python-3-7-on-ubuntu-18-10/)
    ~~~
    # install desire version
    sudo apt-get install python3.x

    # set priority python version
    sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1
    sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.x 2

    # change version by selecting number which is /usr/bin/python3.x
    sudo update-alternatives --config python3
    ~~~

4. Change Numpy Version 
    - Checking version
    ~~~
    pip3 show numpy
    ~~~
    - Changing version : [Link](https://github.com/tensorflow/models/issues/9200)
    ~~~
    pip3 uninstall numpy

    # choose desire version
    pip3 install numpy==1.19.5
    ~~~
5. Change Tensorflow Version 
    - Checking version
    ~~~
    pip3 show tensorflow
    ~~~
    - Changing version : [Link](https://stackoverflow.com/questions/67905185/module-keras-engine-has-no-attribute-layer)
    ~~~
    pip3 uninstall tensorflow

    # choose desire version
    pip3 install tensorflow==2.4.3
    ~~~
6. Change Keras Version 
    - Checking version
    ~~~
    pip3 show keras
    ~~~
    - Changing version 
    ~~~
    pip3 uninstall keras

    # choose desire version
    pip3 install keras==2.4.0
    ~~~
7. Change h5py Version 
    - Checking version
    ~~~
    pip3 show h5py
    ~~~
    - Changing version 
    ~~~
    pip3 uninstall h5py

    # choose desire version
    pip3 install h5py==2.10.0
    ~~~
8. Change Pillow Version 
    - Checking version
    ~~~
    pip3 show pillow
    ~~~
    - Changing version : [Link](https://pillow.readthedocs.io/en/stable/installation.html)
    ~~~
    python3 -m pip install --upgrade pip
    python3 -m pip install --upgrade Pillow
    ~~~
9. Install pycocotools
~~~
pip3 install pycocotools
~~~
10. Install Mask R-CNN : [Link](https://github.com/BupyeongHealer/Mask_RCNN_tf_2.x)
~~~
cd catkin_ws/src
git clone https://github.com/BupyeongHealer/Mask_RCNN_tf_2.x.git
cd Mask_RCNN/
pip3 install -r requirements.txt
python3 setup.py install
~~~
```diff
- if need permission, try this command below 
```
~~~
sudo python3 setup.py install
~~~

<a name="kobuki_in"></a>
### Kobuki Installation
```diff
! Please follow the youtube tutorial
```
1. Create Kobuki Workspace : [Link](https://www.youtube.com/watch?v=edNsh7bHkhQ)
~~~
mkdir kobuki_ws
cd kobuki_ws
mkdir src
catkin_make
~~~
2. Install Kobuki : [Link](https://github.com/yujinrobot/kobuki)
~~~
cd src
git clone https://github.com/yujinrobot/kobuki
~~~
3. Install Yujin Open Control System (yocs) : [Link](https://github.com/yujinrobot/yujin_ocs)
~~~
git clone https://github.com/yujinrobot/yujin_ocs
~~~
4. For Noetic Version : [Link](https://github.com/yujinrobot/kobuki/issues/427#issuecomment-779439686)
```diff
- Delete everything except 'yocs_cmd_vel_mux', 'yocs_controllers', and 'yocs_velocity_smoother' 
```
5. Install liborocos-kdl-dev
~~~
cd 
sudo apt install liborocos-kdl-dev
~~~
6. Add more dependency
~~~
cd kobuki_ws
rosdep install --from-paths src --ignore-src -r -y
catkin_make
source devel/setup.bash
~~~

<a name="turtle_in"></a>
### Turtlesim Installation
1. Install Turtlesim : [Link](http://wiki.ros.org/turtlesim)
~~~
sudo apt-get install ros-$ROS_DISTRO-turtlesim
~~~

<a name="joy_in"></a>
### Joystick Installation
1. Install Joystick : [Link](http://wiki.ros.org/joy/Tutorials/ConfiguringALinuxJoystick)
~~~
sudo apt-get install ros-$ROS_DISTRO-joy
~~~
2. Configuring the Joystick
~~~
ls /dev/input/

# X is joystick's number
sudo jstest /dev/input/jsX
ls -l /dev/input/jsX
sudo chmod a+rw /dev/input/jsX
~~~

<a name="mai_in"></a>
### mai_sim Installation 
1. Install mai_sim : [Link](https://github.com/Natanan-Tirasukvongsa/mai_sim.git)
~~~
cd catkin_ws/src
git clone https://github.com/Natanan-Tirasukvongsa/mai_sim.git
cd ..
catkin_make
~~~

---
<a name="launch"></a>
## Launch Files 📁
<a name="RTAB_L"></a>
### RTAB-Map with 1 Realsense
<a name="RTAB_L_L"></a>
#### Launching
- Launch Realsense : [Link](http://wiki.ros.org/rtabmap_ros/Tutorials/HandHeldMapping)
~~~
roslaunch realsense2_camera rs_camera.launch \
align_depth:=true \
unite_imu_method:="linear_interpolation" \
enable_gyro:=true \
enable_accel:=true
~~~
- Launch Madgick
~~~
rosrun imu_filter_madgwick imu_filter_node \
_use_mag:=false \
_publish_tf:=false \
_world_frame:="enu" \
/imu/data_raw:=/camera/imu \
/imu/data:=/rtabmap/imu
~~~

<a name="RTAB_L_M"></a>
#### RTAB-Map Mapping Mode 
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

<a name="RTAB_L_Lo"></a>
#### RTAB-Map Localization Mode 
~~~
#  resetting the odometry
rosservice call /rtabmap/reset_odom

roslaunch rtabmap_ros rtabmap.launch localization:=true
~~~

![localize](https://user-images.githubusercontent.com/78638430/184532139-8f7d73d5-ef50-4d4c-b8be-3da68bce1a21.png)
|:--:| 
| *Localization with RTAB-Map* |

<a name="kobuki_L"></a>
### Kobuki 
~~~
roslaunch mai_sim kobuki.launch
~~~

![kobuki](https://user-images.githubusercontent.com/78638430/184531755-f6873998-30b4-415a-9be7-1c134c69d12a.png)
|:--:| 
| *Kobuki (Turtlebot2)* |

<a name="kobuki_M_L"></a>
### Kobuki with Multi-Realsense
~~~
roslaunch mai_sim kobuki_rs.launch
~~~

![kobuki_rs](https://user-images.githubusercontent.com/78638430/184531813-eeaa8244-d36d-4c56-9354-51f273794b8c.png)
|:--:| 
| *Kobuki with Multi-Realsense* |

<a name="M_L"></a>
### Multi-Realsense

<a name="M_L_2"></a>
#### 2 Realsense
~~~
roslaunch mai_sim test_rs.launch
~~~

![rs](https://user-images.githubusercontent.com/78638430/184531874-32c3b962-d3ab-4b69-ad7c-23df0176c201.png)
|:--:| 
| *2-Realsense* |

<a name="M_L_4"></a>
#### 4 Realsense
~~~
roslaunch mai_sim 4_rs.launch
~~~

![4_rs](https://user-images.githubusercontent.com/78638430/191685994-1529579c-3330-486f-8e00-934f636068ea.png)
|:--:|
| *4-Realsense* |

<a name="RTAB_M_L"></a>
### RTAB-Map with Multi-Realsense
~~~
roslaunch mai_sim test_rs.launch

# open another terminal 
roslaunch mai_sim rtabmap_ros8.launch rtabmap_args:="--delete_db_on_start --Optimizer/GravitySigma 0.3"
~~~

![Screenshot from 2022-08-16 17-42-51](https://user-images.githubusercontent.com/78638430/184860954-a89cd103-c6ba-4256-aafd-1fd73dea6a64.png)
|:--:| 
| *RTAB-Map with Multi- Realsense* |

<a name="T_J_L"></a>
### Turtlesim Trajectory Control Using Joystick

#### Launching
~~~
roscore
rosrun joy joy_node
rosrun turtlesim turtlesim_node 
rosrun mai_sim joy_trajectory3
~~~
```diff
@@ another way to launch @@
```
~~~
roslaunch mai_sim joy_trajectory.launch 
~~~

#### Joystick
```diff
! Brand : Logitech Gamepad F710
! xbox mode
! Mode Button : off
! Left Analog Stick : Control linear-x and angular-z velocity
! Right Analog Stick : Control angular-z velocity
```

![Screenshot from 2022-08-19 20-55-32](https://user-images.githubusercontent.com/78638430/185634686-37945c1b-ebfe-42e6-8c9b-2012c0aae415.png)
|:--:| 
| *Turtlesim Velocity Control* |

<a name="kobuki_J_L"></a>
### Kobuki Trajectory Control Using Joystick

#### Launching
~~~
roslaunch mai_sim joy_kobuki.launch 
~~~

#### Joystick
```diff
! Brand : Logitech Gamepad F710
! xbox mode
! Mode Button : off
! Left Analog Stick : Control linear-x and angular-z velocity
! Right Analog Stick : Control angular-z velocity
```
---
<a name="technology"></a>
## Object Detection and Segmentation 🧠
<a name="Mask"></a>
### Mask R-CNN
<a name="DMR"></a>
#### Demo Mask-RCNN
You can run all the code too see result
~~~
cd catkin_ws/src/Mask_RCNN_tf_2.x/samples
code demo.ipynb 
~~~

![mrcnn_output](https://user-images.githubusercontent.com/78638430/191957189-3c44845e-3e73-4fd5-aba1-7e3a0b8de96a.png)
|:--:| 
| *Mask R-CNN Output* |

<a name="SOP"></a>
#### Segmentation Only Person
You can add this [code](https://github.com/Natanan-Tirasukvongsa/mai_sim/blob/main/src/segment_only_human_car.py) below last command in demo.ipynb 
![human_seg](https://user-images.githubusercontent.com/78638430/184534020-36ac0a5b-3160-476e-a702-6f93bbab0341.png)
|:--:| 
| *Human Segmentation* |

<a name="AVE_M"></a>
#### Average Computation Time for Sub - Processes
You can add this [code](https://github.com/Natanan-Tirasukvongsa/mai_sim/blob/main/src/ave_time.py) below Segmentation Only Person function

<a name="EX_M"></a>
#### All Example Code
You can see all code [here](https://github.com/Natanan-Tirasukvongsa/mai_sim/blob/main/src/demo.ipynb)

---
<a name="learning"></a>
## My Learning 📕
- Learning Diary : 
  - https://www.notion.so/Learning-SCRUM-c5d8c57dbaba445d9ce6e99bdd0f157d

- Create 3D Map with RTAB-Map :
  - http://official-rtab-map-forum.206.s1.nabble.com/How-to-process-RGBD-SLAM-datasets-with-RTAB-Map-td939.html#a647
  - http://official-rtab-map-forum.206.s1.nabble.com/RtabMap-reads-video-images-from-file-td1290.html#a1291
  - http://official-rtab-map-forum.206.s1.nabble.com/RGBD-source-td2869.html#a8086
  - http://official-rtab-map-forum.206.s1.nabble.com/RtabMap-reads-video-images-from-file-td1290.html#a1693

- RGB-D SLAM Dataset : 
  - https://vision.in.tum.de/data/datasets/rgbd-dataset/download?fbclid=IwAR2UmqP0Cqyt3t_zzH-llFTA7--ZPEv5y4wq1kZ6i3BqDxu8GXX-KT0zFaI

- RTAB-Map Program : 
  - https://github.com/introlab/rtabmap/wiki/Kinect-mapping#yourfirstmap
  
- RTAB-Map with Multi - Camera :
  - http://official-rtab-map-forum.206.s1.nabble.com/RTABMAP-to-be-integrated-for-two-ZED2i-cameras-td8976.html 
  - https://www.heypasteit.com/clip/0ISEH9
  - https://github.com/introlab/rtabmap_ros/blob/master/launch/demo/demo_two_kinects.launch

- RTAB-Map Trajectory : 
  - http://official-rtab-map-forum.206.s1.nabble.com/DataBaseViewer-export-trajectory-td9123.html#a9161

- RTAB-Map Error : 
  - https://answers.ros.org/question/232015/problem-with-rtabmap_ros-and-nonfree-opencv/ 
  - https://github.com/introlab/rtabmap_ros/issues/228

- RTAB-Map Calibration : 
  - http://official-rtab-map-forum.206.s1.nabble.com/Calibration-tool-td2738.html#a2800   

- Control using Joystick : 
  - http://wiki.ros.org/joy/Tutorials

- Control Turtlesim : 
  - https://automaticaddison.com/how-to-move-the-turtlesim-robot-to-goal-locations-ros/
  - https://linklab-uva.github.io/autonomousracing/assets/files/ros-turtlesim.pdf
  - https://subscription.packtpub.com/book/hardware-and-creative/9781782175193/1/ch01lvl1sec14/turtlesim-the-first-ros-robot-simulation

- C++ :
  - https://www.geeksforgeeks.org/c-classes-and-objects/
  - https://www.youtube.com/watch?v=vLnPwxZdW4Y

- Creating Readme :
  - https://community.atlassian.com/t5/Bitbucket-questions/How-to-write-a-table-of-contents-in-a-Readme-md/qaq-p/673363
  - https://mozilla.github.io/open-leadership-training-series/articles/get-your-project-online/the-github-interface-and-markdown/
  - https://docs.github.com/en/enterprise-server@3.3/get-started/writing-on-github/getting-started-with-writing-and-formatting-on-github/basic-writing-and-formatting-syntax
  - https://github.com/fefong/markdown_readme

- Fine Tuning : 
  - https://www.tensorflow.org/guide/keras/functional
  - https://www.youtube.com/watch?v=3ou0KYtDlOI
  - https://colab.research.google.com/github/Hvass-Labs/TensorFlow-Tutorials/blob/master/10_Fine-Tuning.ipynb#scrollTo=1BdM6cmNq_yc
  - https://github.com/Hvass-Labs/TensorFlow-Tutorials
  - https://www.youtube.com/watch?v=H8sXcAXrGR4
  - https://stackoverflow.com/questions/41668813/how-to-add-and-remove-new-layers-in-keras-after-loading-weights?fbclid=IwAR3Crf0suf3AKG5BvdQusUk3gThKkhRbCriX3WgvRR8QirY7nsCNxyGVnvs

- Quote : 
> You cannot change the past but you can still fuck up your future.
