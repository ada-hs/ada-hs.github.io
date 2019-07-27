---
layout: post
title:  "Gazebo(1) : Gazebo基础介绍"
categories: SLAM与机器人
tags:  SLAM与机器人
---

### Gazebo介绍

-  Gazebo是一个基于物理仿真的3D机器人模拟器软件。Gazebo能够在复杂的室内和室外环境中精确模拟一个或多个机器人的运动。对机器人设计，场景测试十分有用。虽然Gazebo不依赖平台，可以运行在Windows、Mac和Linux上，但它主要与运行在Linux系统上的机器人操作系统（ROS）结合使用。

### Gazebo 特性

- **Dynamics Simulation:** 动态模拟仿真
- **Advanced 3D Graphics**：高质量的图形渲染 ，支持 灯光、阴影，纹理、
- **Sensors**:支持给机器人人添加传感器 ，并生成数据，模拟噪声
- **Plugins:** 可以编写插件。
- **Model Database:** 内置模型库，也可以自己构造
- **Socket-Based Communication:** 支持基于socket方式和远程服务器上的Gazebo交互通信交互
- **Cloud Simulation:** 可在服务器上运行并 通过 浏览器交互
- **Command Line Tools:**支持命令行控制

目前我只体验过一部分，如果 有兴趣的可以访问[Gazebo 官网](http://gazebosim.org/)

### Gazebo图形界面及交互

![gazebo_gui_all.png](https://i.loli.net/2019/06/30/5d185081a6b9234869.png)

![gazebo_gui_1.png](https://i.loli.net/2019/06/30/5d185081a736586999.png)

### Gazebo组件

主要有以下组件：

1. Gazebo Server （服务器）
2. Gazebo Client（客户端）
3. World Files （世界描述 文件）
4. Model Files （模型 描述文件）
5. Environment Variables（环境变量）
6. Plugins （插件 ）

分别解释一下。

1. **Gazebo Server：**Gazebo服务器。也叫做**gzserver**。负责解析场景描述文件。然后使用物理和传感器引擎模世界。在Linux上可以用命令行 启动

   ```
   $ gzserve
   ```

2. **Gazebo Client:** 通过连接服务器，做渲染可视化用。

   ```
   $ gzclient
   ```

通常 是先启动**gzserver** ，然后在调用**gzclient**。因为场景、场景内的目标都需要初始化工作，不过也可以 直接同时启动

```
$ gazebo
```

3. **World Files :**世界描述文件

   对三维图形学略知一二的人都知道，在三维场景中描述世界，需要一些元素，如场景、光照（灯光 ），在机器人仿真里还有机器人，传感器等等 。整个的世界描述信息都可以用世界描述文件串联起来。文件后缀名就是`.world`. Gazebo可以直接打开并访问

   ```
   $ gazebo <yourworld>.world
   ```

   关于 世界描述文件，它是一种SDF文件（ **Simulation Description Format** ）

   举个例子：

   ```SDF
   <?xml version="1.0" ?>
   <sdf version="1.5">
     <world name="default"> //世界名称
       <physics type="ode">
         ...
       </physics>
   
       <scene>
         ...
       </scene>
   
       <model name="box"> //场景中的元素1
         ...
       </model>
   
       <model name="sphere">//场景中的元素2
         ...
       </model>
   
       <light name="spotlight">//场景中灯光描述
         ...
       </light>
   
     </world>
   </sdf>
   ```

4. 模型描述文件 

   我们使用与世界文件完全相同的格式创建机器人的单独SDF文件。一个模型文件应该只代表一个模型（机器人），且可以被世界文件导入。而之所以把模型描述文件和世界文件区分，是为了模型或者世界的复用。如果想在世界文件 中添加某一机器人模型文件。只需要包含进去，像这样

   ```
   <include>
     <uri>model://model_file_name</uri>
   </include>
   ```

5. 环境变量 ：

   Gazebo使用了许多环境变量，基本用于定位文件（world、model，…）和在gzserver和gzclient之间建立通信。

6. 插件

   为了 和Gazebo中的世界（场景）、模型，传感器交互。可以 自行写一些 插件脚本 。他们可以命令行的方式载入，也可以直接嵌套在SDF文件中。

请注入我更新的动力！
![adaAlipay.jpg](https://i.loli.net/2019/07/27/5d3bc2958327410625.jpg)
