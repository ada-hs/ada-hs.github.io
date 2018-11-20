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

![OpenMP_estimation_normal.png](https://i.loli.net/2018/11/20/5bf3f8793f4e8.png)



3、代码变更为：

    pcl::NormalEstimationOMP<pcl::PointXYZ, pcl::Normal> ne

速度没有变快，甚至相对变慢了，不知道是不是因为数据量不够大。

在后续的测试中，有序点云的会比无序点云的计算效率高。毕竟有序点云的查询和访问会更加高效。

OpenMp似乎是基于CPU多核进行计算的，在三维空间的运算操作，还是GPU会快些。所以cuda相比好些。

