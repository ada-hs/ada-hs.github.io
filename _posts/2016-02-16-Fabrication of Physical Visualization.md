---
layout: post
title:  "Fabrication of Physical Visualization"
categories: 科研之路
tags: 论文研读
---

po一篇最近研读的论文之一，在此作一个总结概括。为保护版权，将以内留的PPT图片为内容。
這篇文章的有趣之处在于对可视化的concern，更在于其交互性。

值得考量的问题是物理模型受限于分辨率，细节描述未必是可满足的。


我们可以看到MakerVis所制造出的无力可视化模型，所描述的数据有散点图（a）,柱状（c）等等，在其参数选择界面种可以调节。
![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%871.PNG)

传统的物理可视化在制作上最大的缺陷无疑是消耗人与物力。比如讲美国一个周的热度图可视化制作出来，需要计算切割，喷漆。组装。
![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%872.PNG)

而盘桓在物理可视化面前的瓶颈一在于传统的数据可视化软件太依托于PC端，现有的可视化工具，都是依托于计算机呈现。随着数字化制造工艺的兴起，如3D打印，数控切削等。这无疑是有益于物理可视化制造的，然而问题在于在没有数据驱动时生成3维模型的机制下，也是十分尴尬的，比如普通用户想记录宝宝身高随着年龄变化的过程。
即使Ta可以预定数字制造服务，Ta自己却无法生成该可视化的3维模型，这需要专业人士建模。

![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%873.PNG)

![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%874.PNG)

MakerVis的开发打破了以上瓶颈（截至文章发布前尚未看到Web的Application）。按照paper的论述，该应用所以提供的自动性（选择常用的数据，如csv.和呈现类型即可自动生成三位模型并提供预览），交互性(用户可选择不同属性个性化定制)，先导性(提供切割后的元件组装建议)

![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%875.PNG)

![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%876.PNG)

![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%877.PNG)

![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%878.PNG)

![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%879.PNG)

paper的作者们结合user study所发现的缺陷，于作者个人而言，尚未见到這款应用的发布，评价和体验仍有需要修正的空间。
![image](http://7xq62e.com1.z0.glb.clouddn.com/%E5%B9%BB%E7%81%AF%E7%89%8710.PNG)

如需转载，请联系本人。
