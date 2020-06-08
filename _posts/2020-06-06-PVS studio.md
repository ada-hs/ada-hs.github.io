---
layout: post
title: 2020-06-06 代码静态检测工具
date: 2020-06-06
categories: blog
tags: [learn form work]
---
### PVS studio
####  正常下载安装（可能需要注册码）
[PVS studio](https://www.viva64.com/en/pvs-studio/)
#### 建议取消自动更新
![image](https://i.loli.net/2020/06/08/KcgvMzUiZrWaxp3.png)
#### 根据需要开启或关闭检测
![image](https://i.loli.net/2020/06/08/4nTS9kADd8Hrbut.png)
#### 设置过滤文件：

常用于使用Qt进行开发时，可以对qt本身的头文件和一些自动生成的文件进行过滤，否则会产品大量我们无法修改的错误，同样的方法，如果开发中引用了第三方的头文件，我们使用相同的方法来屏蔽静态检测，具体方法如下：
##### 1. 屏蔽目录
![image.png](https://i.loli.net/2020/06/08/E4WOcJPpCrGXfoM.png)

##### 2.屏蔽某一类文件
![image.png](https://i.loli.net/2020/06/08/m2HrKMZDkiWjcnx.png)

#### 检测
如果项目比较大，检测时间可能比较长
![image.png](https://i.loli.net/2020/06/08/hfjXpNwlUmaOc27.png)

#### 导出检测报告
最终检测的结果如下，可以根据条件过滤查找，警告级别分别是High，Medium，Low，General

![image.png](https://i.loli.net/2020/06/08/T3pLxNaF1WEjry7.png)
之后进入open/save中save Analysis Report进行导出即可
