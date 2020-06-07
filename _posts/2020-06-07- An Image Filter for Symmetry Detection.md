---
layout: post
title: 基于图像梯度积变换的对称元素检测算法
date: 2020-06-07
categories: blog
tags: [论文研读]
---


###  算法概述
输入：灰度图、算法参数
输出：对称元素的所在的ROI矩形区域。对称中心
![pipeline.PNG](https://i.loli.net/2020/06/07/lX5dPO3q6CbYMrL.png)



### 原理概述
核心是梯度积运算，利用梯度图像进行运算，运算后每个像素都将得到一个**用于评价对称性的正相关性打分（对称中心将得到最高分）**。

具体内容还阐述了计算时一些参数取值的设计思路，以及在遇到不同对称类型(旋转对称、轴对称)时如何区分。

### 应用
-  对称元素检测
![app;ication2.PNG](https://i.loli.net/2020/06/07/8B7FIwrsyShnuvZ.png)
-  医疗影像识别提取。如眼底图、血管造影影像（利用血管形状的局部对称性）
![app;ication1.PNG](https://i.loli.net/2020/06/07/rHPw3xKCuztM8AS.png)
### 局限性

1. 透视比较强烈的object无法识别。
如下case。当obejct（钟表）视角变化后，无法再检测出
![distadvantage2.PNG](https://i.loli.net/2020/06/07/1GV7ULrXvQa24ye.png)

2. 图像中充斥着pattern无法识别
如下图中教堂墙面的墙砖，均为平铺性的pattern排布，检测并不理想
![distadvantage.PNG](https://i.loli.net/2020/06/07/BVXg2WdQR3UlMq8.png)
3. 部分参数不太合理的情况下效率会比较低



 
