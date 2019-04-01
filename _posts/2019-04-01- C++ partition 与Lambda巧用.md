---
layout: post
title:  "C++ partition 与Lambda巧用"
categories: 算法与面试
tags:  算法与面试
---

### C++ partition 与Lambda巧用

当自己偷懒的时候，经常用C++ stl , 今天发现了一个冷门但很有用的算法函数**partition**

> 对[first, last)元素进行重排处理，使得满足p的元素移到[first, last)前部，不满足的移到后部，返回第一个不满足p元素所在的迭代器，如果都满足的话返回last。
>
> 注意：。**该算法不能保证元素的初始相对位置**,如果需要保证初始相对位置，应该使用stable_partition. 

刚好之前刷到一道leetcode的算法题目，给一个整数序列，重排以后偶数在前，奇数在后。我当时的做法是另开两个序列，分别存奇数和偶数，然后合并。现在想想，用partion完美解决：

```
vector<int> rearrange(vector<int>& A) {
    auto is_even = [] (auto e) { return e % 2 == 0; };
    partition (A.begin (), A.end (), is_even);
    return A;
}
```

### 涉及到的Lambda表达式

Lambda表达式在Python， c#特别常用。在c++里，Lambda表达式，可以方便的定义和创建匿名函数。

申明如下 

```
[capture list] (params list) mutable exception-> return type { function body }
```

1. capture list：捕获外部变量列表
2. params list：形参列表
3. mutable指示符：用来说用是否可以修改捕获的变量
4. exception：异常设定
5. return type：返回类型
6. function body：函数体

利用这个我就不用在单独写一个判断奇数偶数的函数了。在一些简单的场合，用lamda表达式相对简单多了

比如比较大小：

```
sort(vec.begin(), vec.end(), [](int a, int b) -> bool { return a < b; }); 
```

之后有用到或者延展再补充吧



