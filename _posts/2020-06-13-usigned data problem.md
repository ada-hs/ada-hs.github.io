---
layout: post
title: 2020-06-13 无符号溢出问题
date: 2020-06-13
categories: blog
tags: [learn form work]
---

这周工作遇到了无符号数加减溢出 问题，记录一下。

再现一下
usigned int 相减如果出现负数会出现很大值

```
#include<iostream>
#include<climits>
using namespace std;

int main() {

    unsigned int  a = 10;
    unsigned int  b = 11;

    cout << "结果： " << a-b << endl;
    return 0;
}

```
最后结果是 4294967295

分析可以看这个类似的
[无符号类型的数据类型做减法](hhttps://blog.csdn.net/zhangkai19890929/article/details/85071253?utm_medium=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase&depth_1-utm_source=distribute.pc_relevant.none-task-blog-BlogCommendFromMachineLearnPai2-2.nonecase)

