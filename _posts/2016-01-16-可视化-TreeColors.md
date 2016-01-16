# 可视化之TreeColor

------
###1.前言
------
研一可视化课程的Final Project, 研读info-vis的论文并实现。我选择的這篇：
[Tree Colors: Color Schemes for Tree-Structured Data
](https://www.computer.org/csdl/trans/tg/preprint/06875961.pdf)

作者：[Martijn Tennekes](http://www.von-tijn.nl/tijn)&[Edwin de Jonge](https://github.com/edwindj)

這是一篇用于呈现树状层级结构数据的可视化图形的paper,树状结构在可视化中十分常见。比如常见的TreeMap，
一个有意思的网站[NewsMap](http://newsmap.jp/)就是利用了這样的图形。
![cmd-markdown-logo](http://7xq62e.com1.z0.glb.clouddn.com/fig9.png)
我们可以看到同一色系表现相同的性类。面积大小占篇幅。其它情形也可以理解为数据容量，数值大小。Treemap是应用较为广泛的信息可视化之一。

這篇paper的意义和用处在于如何利用环状的layout及复合人眼视觉可分辨的颜色为层级树状结构数据提供配色方案。

###2、算法
------
值得一提的是，idea采用了[HCL](https://en.wikipedia.org/wiki/CIELUV#Cylindrical_representation)颜色空间.因其更符合人眼的视觉感知。HCL即Hue（色相） , Chroma （浓度）, Luminance （明度）

算法核心思想如下：
```
$AssignHue(v, r, f , perm, rev) //v:结点 r:色域 perm:重排标记  rev:翻转标记  f:色域间隔
 1、Select the middle hue value in r as the hue value of node v, which is Hv //中间色相值为根结点颜色，灰色
 2、Let N be the number of child nodes of v. If N > 0 :
   i divide r in N equal parts ri with i = 1,...,N;
   ii if perm then permute the ri’s;
   iii if rev then reverse the even-numbered ri’s;
   iv reduce each ri by keeping its middle fraction f ;
   v for each child node vi DO AssignHue(vi, ri, f , perm, rev).
```
 不难看出递归调用。算法的精妙之处在于子节点及其branch的翻转，何为翻转，我们看这个对比图：
 ![cmd-markdown-logo](http://7xq62e.com1.z0.glb.clouddn.com/fig6.png)
其中左图是未翻转，右图为翻转之后的效果。B.1.a和A.4.c颜色是很接近的，不利于不同类别颜色的差异分辨，翻转后，变化较大。这种翻转是可以理解的，由于色域的划分使得相同结点及其分支的颜色某种程度上是具有连续性的，這样的翻转才能增加差异。

为了加大這样的差异，fraction的引入也十分重要。fraction可以理解为一个gap,拉开两个节点及其所属分支的范围，可以看到fraction越大，颜色差异就越明显。f=0.25时B枝和f=1的尤为明显。
![cmd-markdown-logo](http://7xq62e.com1.z0.glb.clouddn.com/fig4.png)

而明度，浓度的设定也是有影响的。
i表示结点深度，P1为默认参数，在减性色彩空间下，
P1=-10   
P2=-5 
L1=75  
C1=70
公式定义：[^LaTeX]
$$Li=(i-1)P1+L1$$
$$Ci=(i-1)P2+C1$$
![cmd-markdown-logo](http://7xq62e.com1.z0.glb.clouddn.com/FIG87.png)

![cmd-markdown-logo](http://7xq62e.com1.z0.glb.clouddn.com/fig2.png)
------


###3、实现
------
采用[d3.js](http://d3js.org/)来实现,d3支持HCL颜色空间的转换.

关于layout,参考[Radial Reingold–Tilford Tree](http://bl.ocks.org/mbostock/4063550)。同时也给出了具体的code,可以直接套用哦
[Python也可实现](http://www.jianshu.com/p/cd04ced4bdaf)

不得不提json文件格式对树形的逻辑结构定义之便利[^code]

`
var tree = {  
name: "root", 
 children: [{    
  name: "child1"},{    
  name: "child2"} ]};
`
我还用过igraph来画过，但是方法极为手动且low，有兴趣的邮箱探讨。
