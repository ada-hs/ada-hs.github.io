---
layout: post
title:  "PCL中采用OpenMP加速法线估计"
categories: PCL与点云万象
tags: PCL与点云万象
---


1、引入头文件

    #include <pcl/features/normal_3d.h>
    #include <pcl/features/normal_3d_omp.h>//含有omp模块



2 、VS中开启OpenMP支持

![1533191638815](http://7xq62e.com1.z0.glb.clouddn.com/pic/OpenMP_estimation_normal.png)



3、代码变更为：

    pcl::NormalEstimationOMP<pcl::PointXYZ, pcl::Normal> ne

速度没有变快，甚至相对变慢了，不知道是不是因为数据量不够大。


