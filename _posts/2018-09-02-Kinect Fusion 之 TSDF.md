---
layout: post
title:  "Kinect Fusion 之 TSDF"
categories: 论文研读
tags: 论文研读
---

## Kinect Fusion 之 TSDF

## Depth Map Conversion

利用相机的内参信息，和深度相机获取到的深度图像转换为点云。

利用相邻像素点求取每个顶点的法向。得到Normal map

## Camera Tracking

利用ICP算法比较两帧之间的位姿。因为是实时重建，两个相邻帧之间的位姿变化很小。

接下来就是融合与更新。

## Volumetric Representation

Volume就是一个体空间。

### SDF 与 TSDF

- TSDF( Truncated Signed Distance Functions ) ---  截断符号距离函数
- SDF （Signed Distance Functions）--- 符号距离函数

TSDF本质还是SDF的，但是关于距离定义不同。

首先是**SDF**

![sdf.png](https://i.loli.net/2018/11/15/5bed57a28eae3.png)

而**TSDF**的距离只考虑在surface 附近一定距离范围（paper中是相机到单元voxel的距离与深度距离的差）。其它截断。这样的方式使得重建的质量对数据敏感，如果噪声显著。质量不佳。

![tsdf.png](https://i.loli.net/2018/11/15/5bed57c2267aa.png)



将待重建的三维空间划分成voxel.每个voxel中存储数值代表离重建表面的距离（有效范围内）。从正到负的跨越点表示穿过重建的表面上的顶点。

也就是**zero-crossing**点

![TSDF_voxel.jpg](https://i.loli.net/2018/11/15/5bed57d79e9f0.jpg)

### 为什么TSDF快？

TSDF设定在GPU上运算，一个线程处理一个一个z维上的一堆voxels(蓝色部分)。可以实时对GPU读写。但是应该会占用比较大空间。

![TSDF_GPU.jpg](https://i.loli.net/2018/11/15/5bed5809a3ab6.jpg)

### TSDF

首先总结性地宏观说一下原理 Volume建好后。

> 每一次用 TSDF，都是在全局TSDF的基础上进行融合与更新。即在累计值上更新当前帧的TSDF值与权重



论文中的算法步骤：

1. 对于构造出的每个体素Voxel中的每一柱空间内（上图中蓝色部分）

2. 从前往后进行更新.

3. v_g  把体素g 转换到全局坐标系下（世界坐标系下）

4. 将每个voxel单元中的点从全局坐标系转到相机坐标系：![formula1.png](https://i.loli.net/2018/11/15/5bed584aef129.png)

5. 对v进行透视变换得到p。从相机坐标系投影到平面

6. 如果v在视窗内

7. 符号距离函数![formula2.png](https://i.loli.net/2018/11/15/5bed58685385e.png)

   - sdfi : 距离
   - ti：相机光心的全局坐标
   - vg:  体素单元点的全局坐标
   - Di(p)：(u,v)处的深度值

   其中：|ti-vg|:voxel点到光心的Z向距离.

8. 如果距离函数（sdfi>0）

9. 截断符号函数取 （1，sdfi / 最大截断值 ）中的最小值

10. 否则

11. 取（-1，sdfi / 最大截断值 ）中的最大值。

12. wi取（最大权重，wi-1 +1）的最小值。

13. 更新tsdf(表示网格中通过不断加权融合更新的值 )的值

14. 存储每个权重wi 和tsdf 的值。

![TSDF_STEP.jpg](https://i.loli.net/2018/11/15/5bed587b882d1.jpg)

每一个Volume中的点都存储着TSDF值。其中**zero-crossing** 点 所构成 的等值面只要提取出来。就能重建出surface。

最后简单说一下我的理解：
- 1、首先计算体素在全局坐标系下的坐标vg( x,y, z),结合ICP配准得到的变换矩阵把店从全局坐标系转换到相机坐标系，记为V（x, y, z）
- 2、利用相机的内参矩阵，将v 点转换到图像坐标系得到v_uv(u,v)
- 3、判断在第i帧下的深度图***I***中的像素点（u,v ）所在的深度值 d (u, v) 与体素在相机坐标系下的点 V（x, y, z）中 z的大小
- - d(u, v) >z: 体素离相机更近。反之则更远。


## Raycasting

根据上一帧的帧位姿，将当前帧的点云融合到网格模型中之后，再用光线投影算法计算在当前视角下可以看到的场景的表面，还能算法线，之后就可以用来渲染了。

最后吐槽一下Kinect分辨率和噪声挺大的。真正应用的效果不会有paper里的那么好。不过用来做**motion tracking**不错

## 跑一个demo看看

在git上找了一个[TSDF](https://github.com/andyzeng/tsdf-fusion) FUSION 的demo. 跑了一下，
我:
顺便记录一下遇到的问题。

### CUDA 9.2配置遇到的问题 

[CUDA9.0+VS2017+win10详细配置](https://blog.csdn.net/u013165921/article/details/77891913)

虽然我用的是2013，但是我觉得这个挺有用的，尤其是最后在cmd中利用deviceQuery.exe  和 bandwidthTest.exe 做cuda安装是否成功的法子非常有效。

### Opencv3.0 配置

[ 配置：win 10+opencv 3.0+vs 2013 ](https://blog.csdn.net/CYJ2014go/article/details/78400124)

跑demo之前，一看到人家的平台是在Ubantu上的就心里一咯噔，要知道window总是一言难尽的。但是只要细细的一步一步来，总还是可以run起来的。

不要着急直接贴人家的代码。我的建议是，先确定依赖的环境是否都安装正确。比如Cuda和OpenCV。可以分别测试。我先在OpenCV中测试通过以后。再建立一个Cuda/C++的项目。保留openCV的配置。再开始run代码。

一次就通了。

### 遇到堆栈溢出的问题怎么办？

虽然编译通过了，但是在debug/release下跑起来的时候。报了堆栈溢出的异常。

我的解决方法是：

解决方法：

将“项目属性-->链接器-->系统-->堆栈保留大小”设大一点，例如比如2000000.就好了

![1536200619752](http://7xq62e.com1.z0.glb.clouddn.com/pic\Stackoverflowl.png)

至此万事都OK了。

### 参考文献

[1].[Volumetric Range Image Integration](http://www.ifp.uni-stuttgart.de/lehre/diplomarbeiten/korcz/index.html)

[2].Izadi S, Kim D, Hilliges O, et al. KinectFusion: real-time 3D reconstruction and interaction using a moving depth camera[C]//Proceedings of the 24th annual ACM symposium on User interface software and technology. ACM, 2011: 559-568. 
