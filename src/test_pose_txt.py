import numpy as np
import matplotlib.pyplot as plt
x,y,z = [],[],[]

with open("/home/natanan/test_pose.txt","r") as txt_file:
    txt = txt_file.readlines()
    for pose in txt[2:len(txt)]:
        x.append(float(pose[2]))
        y.append(float(pose[4]))
        z.append(float(pose[6]))
    x.append(x[0])
    y.append(y[0])
    z.append(z[0])
    
plt.title("Top view trajectory graph")
plt.xlabel("X axis [m]")
plt.ylabel("Y axis [m]")
plt.plot(x, y, color ="blue")
plt.xlim([0,10])
plt.ylim([0,10])
plt.show()
        