---
layout: post
title:  "深入了解机器学习-Descending into ML"
categories: MachineLearning
tags: MachineLearning
---

说明：這是个人学习课程的笔记文档，原笔记在[课程主页](https://developers.google.com/machine-learning/crash-course/)上有

**线性回归**

通常我们這么表示线性回归
	
<math xmlns="http://www.w3.org/1998/Math/MathML" display="block">
  <mi>y</mi>
  <mo>=</mo>
  <mi>m</mi>
  <mi>x</mi>
  <mo>+</mo>
  <mi>b</mi>
</math>

其中

- y指的是试图预测的值
- m为直线斜率
- x为输入特征的值
- b为y轴截距。

而在机器学习里
<math xmlns="http://www.w3.org/1998/Math/MathML" display="block">
  <msup>
    <mi>y</mi>
    <mo>&#x2032;</mo>
  </msup>
  <mo>=</mo>
  <mi>b</mi>
  <mo>+</mo>
  <msub>
    <mi>w</mi>
    <mn>1</mn>
  </msub>
  <msub>
    <mi>x</mi>
    <mn>1</mn>
  </msub>
</math>

其中

- <math xmlns="http://www.w3.org/1998/Math/MathML">
  <msup>
    <mi>y</mi>
    <mo>&#x2032;</mo>
  </msup>
</math>指的是预测标签（理想输出值）
- <math xmlns="http://www.w3.org/1998/Math/MathML">
  <msub>
    <mi>w</mi>
    <mn>1</mn>
  </msub>
</math>指的是特征 1 的权重。权重与上文中用  表示的“斜率”的概念相同。
- <math xmlns="http://www.w3.org/1998/Math/MathML">
  <msub>
    <mi>x</mi>
    <mn>1</mn>
  </msub>指的是特征（已知输入项）。
</math>为输入特征的值
- <math xmlns="http://www.w3.org/1998/Math/MathML">
  <mi>b</mi>
</math>指的是偏差（y 轴截距）。而在一些机器学习文档中，它称为<math xmlns="http://www.w3.org/1998/Math/MathML">
  <msub>
    <mi>w</mi>
    <mn>0</mn>
  </msub>
</math> 。

-----------------------------

**训练与损失**

<math xmlns="http://www.w3.org/1998/Math/MathML" display="block">
  <mi>M</mi>
  <mi>S</mi>
  <mi>E</mi>
  <mo>=</mo>
  <mfrac>
    <mn>1</mn>
    <mi>N</mi>
  </mfrac>
  <munder>
    <mo>&#x2211;<!-- ∑ --></mo>
    <mrow class="MJX-TeXAtom-ORD">
      <mo stretchy="false">(</mo>
      <mi>x</mi>
      <mo>,</mo>
      <mi>y</mi>
      <mo stretchy="false">)</mo>
      <mo>&#x2208;<!-- ∈ --></mo>
      <mi>D</mi>
    </mrow>
  </munder>
  <mo stretchy="false">(</mo>
  <mi>y</mi>
  <mo>&#x2212;<!-- − --></mo>
  <mi>p</mi>
  <mi>r</mi>
  <mi>e</mi>
  <mi>d</mi>
  <mi>i</mi>
  <mi>c</mi>
  <mi>t</mi>
  <mi>i</mi>
  <mi>o</mi>
  <mi>n</mi>
  <mo stretchy="false">(</mo>
  <mi>x</mi>
  <mo stretchy="false">)</mo>
  <msup>
    <mo stretchy="false">)</mo>
    <mn>2</mn>
  </msup>
</math>