---
layout: post
title:  "深入了解机器学习-Descending into ML"
categories: MachineLearning
tags: MachineLearning
---

说明：這是个人学习课程的笔记文档，原笔记在[课程主页](https://developers.google.com/machine-learning/crash-course/)上有



通常我们這么表示线性回归
	
<a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;y=mx&plus;b" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;y=mx&plus;b" title="y=mx+b" /></a>

其中

- *y* 指的是试图预测的值
- *m* 为直线斜率
- *x* 为输入特征的值
- *b* 为y轴截距。

而在机器学习里

<a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;y'=&space;b&plus;w_1x_1" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;y'=&space;b&plus;w_1x_1" title="y'= b+w_1x_1" /></a>

其中

- *y'* 指的是预测标签（理想输出值）
- *w1* 指的是特征 1 的权重。权重与上文中用  表示的“斜率”的概念相同。
- *x1* 指的是特征（已知输入项）.为输入特征的值
- *b* 指的是偏差（y 轴截距）。而在一些机器学习文档中，它称为 $w_0$

多个特征的可以表示为

<a href="https://www.codecogs.com/eqnedit.php?latex=\inline&space;y'&space;=&space;b&space;&plus;&space;w_1x_1&space;&plus;&space;w_2x_2&space;&plus;&space;w_3x_3&plus;&space;.....w_nx_n" target="_blank"><img src="https://latex.codecogs.com/gif.latex?\inline&space;y'&space;=&space;b&space;&plus;&space;w_1x_1&space;&plus;&space;w_2x_2&space;&plus;&space;w_3x_3&plus;&space;.....w_nx_n" title="y' = b + w_1x_1 + w_2x_2 + w_3x_3+ .....w_nx_n" /></a>

-----------------------------

**训练与损失**

线性回归模型通常使用的是一种称为平方损失（又称为 L2 损失）的损失函数。单个样本的平方损失如下：

	  = the square of the difference between the label and the prediction
	  = (observation - prediction(x))2
	  = (y - y')2

均方误差 (MSE) 指的是每个样本的平均平方损失。要计算 MSE，请求出各个样本的所有平方损失之和，然后除以样本数量：

![](http://chart.googleapis.com/chart?cht=tx&chl=\Large MSE = \frac{1}{N} \sum_{(x,y)\in D} (y - prediction(x))^2)

 - $(x, y)$ 指的是样本，其中
 	- $x$ 指的是模型进行预测时使用的特征集（例如，温度、年龄和交配成功率）。
 	- $y$ 指的是样本的标签（例如，每分钟的鸣叫次数）。
 - $prediction(x)$ 指的是权重和偏差与特征集  结合的函数。
 - $D$ 指的是包含多个有标签样本（即 ）的数据集。
 - $N$ 指的是  中的样本数量。
