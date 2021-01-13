#An algorithm of Extracting Line Feature of Laser Radar Combined with SVM

#Authur：Xumin Gao (Institute of Robotics and Intelligent Systems, Wuhan University of Science and Technology,China)

#Paper: 

X. Gao, L. Jiang, H. Wang, et al. Algorithm of Extracting Line Feature of Laser Radar Combined with SVM[J]. Computer Engineering and Design, 2019, 40(08): 2384-2388.【http://en.cnki.com.cn/Article_en/CJFDTotal-SJSJ201908047.htm】


#E-mail: comin15071460998@gmail.com



# Requirements

- Opencv 2.4.9


- Sick LMS111 lidar (Be used to access the data)


# Introduction

###cvs

-The file which is used to store data from lidar

###Serial.h and URG.h

-The serial port which is used to access the lidar, it used to obtain point cloud data of lidar

###WeightedFit.cpp and WeightedFit.h

-The weighted method of least squares

###Coordinate.cpp and Coordinate.h

-Calculate the transformation parameters between the robot coordinate and the lidar coordinate.

###OpenRadar.cpp and OpenRadar.h

-Segment the point cloud;fit the line segments; merge the line fragments using the SVM classifier based on salient features of line segments; visualization;

###Radar.cpp

-Main function


#Abstract

Aiming at the phenomenon that the fixed segmentation threshold leads to under segmentation and over
segmentation in extracting line feature of laser radar data by the IEPF algorithm, a separate and merged extraction algorithm of line feature combined with Support Vector Machine is proposed. In separate stage, we use the IEPF algorithm to segment the data preliminarily. In the merged stage, first of all,the threshold value is adjusted to eliminate the line segments of under segmentation as far as possible, and then the three features from over segmented line segments and normal line segments which include the adjacent degree, the collinear degree and the overlap degree are extracted respectively as feature vectors to train the SVM model. Finally, the SVM model is applied to the actual test, and the classification results which is over segmented line segments are executed merged. Experimental results show that the algorithm effectively eliminates most of the over segmented line segments and under segmented line segments produced by line segment extraction using IEPF algorithm.

Then, according to the geometric relationship between merged line segments, the robot can recognize the indoor structured environments, for instance walls, corridors, corners and passageway (this part of work is included in my master's thesis [https://drive.google.com/drive/folders/1_Bk6lemu2a-_1MYub1tQvBuKXg1RNfAo]).


![Image text](https://github.com/XuminGaoGithub/An-algorithm-of-extracting-line-feature-of-laser-radar-combined-with-SVM/blob/main/1.png) 

![Image text](https://github.com/XuminGaoGithub/An-algorithm-of-extracting-line-feature-of-laser-radar-combined-with-SVM/blob/main/2.png) 

![Image text](https://github.com/XuminGaoGithub/An-algorithm-of-extracting-line-feature-of-laser-radar-combined-with-SVM/blob/main/3.png) 

![Image text](https://github.com/XuminGaoGithub/An-algorithm-of-extracting-line-feature-of-laser-radar-combined-with-SVM/blob/main/4.png) 




