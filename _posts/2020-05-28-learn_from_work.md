---
layout: post
title: 2020-05-28 一周一点小技巧 宏、VA、VS自动生成
date: 2020-05-28
categories: blog
tags: [learn form work]
---
## 宏的各种使用
####  1. 将简单的函数封装成宏

```
#define SWAP(a, b, t) \
{ t=a; a=b; b=t; }
```
- 宏函数没有入栈出栈时间上的开销，效率更高
- 一行写不下可以用反斜杠符 \\ , 表示本行与下一行连接

#### 2. 用宏切换模块

开发过程中遇到了需要模块A，但模块A还没有进行开发的情况（比如数据库并未设计好）。

但为了不影响后续的开发，可以先写一个伪数据生成模块B。注意B是作为A的临时版本，为了不影响后续开发或测试用的，因此A、B里的接口、函数名称建议要一致。

```
#pragma once
#ifdef B
------------
// 类定义，接口等等
------------
#endif // B

```
接着，在需要调用A模块功能的地方定义下。
```
#ifdef TEST
using DataSource = B;
#else
using DataSource = A;
#endif // TEST
```
但注意的是，需要在属性中的c++项中的预处理器添加宏TEST


也就是说，工程属性中如果添加了TEST宏定义，会直接调用B中的接口函数，如果A设计好了，移除宏定义就能自如切换了。

---

## VA工具的使用（2017版本）
从来都是把va番茄助手当成代码提示器用，这周发现有些地方还是很使用的

#### 1.自动添加实现
比如头文件中写好了接口定义。
鼠标移到接口定义上，右键
Quick Actions and Refactorings(VA)-->Creat Implemention
#### 2. 自动添加申明
适用于喜欢先写实现后写声明的情境
#### 3. 重命名
rename
常用于函数名称更改用，这个重命名应该是全局的
#### 4. surround With
主动填充好常见的热点代码段形式。for 循环，if else等


---
## vs中的生成事件

比如你工程编译出的文件(dll,pdb,lib)等在path1下，但协同开发的人真正要使用的文件所在目录在path2下，为了避免手动拷贝，可以在属性里面添加简单的文件拷贝脚本，一旦工程编译成功，指定的拷贝文件就会自动拷贝到指定目录下

```
copy 
"$(TargetDir)$(ProjectName)d.dll" //source path
"$(SolutionDir)..\..\..\..\..\dist\1.0.alpha\bin\win64_v141_debug" //target path
```
![E9BDFA0B7B4F45F29CAD1608C890A7B6.png](https://i.loli.net/2020/05/31/y3rzh7oQ2eKkvOI.png)
## 其它
- Alt+G 头文件跳转




