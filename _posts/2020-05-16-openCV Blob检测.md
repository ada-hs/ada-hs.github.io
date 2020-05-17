---
layout: post
title:  "openCV Blob检测"
categories: openCV
tags: openCV
---


Blob是图像中一组相互连接的像素，它们具有一些共同的属性(比如灰度值)。在上图中，暗连通区域是blobs，斑点检测的目的是识别和标记这些区域。

参考文章：https://www.learnopencv.com/blob-detection-using-opencv-python-c/

SimpleBlobDetector，是一个检测算子，检测到的blob性质可以用以下参数约束：
1. 阈值化:通过对源图像进行阈值化，从最小阈值开始，将源图像转换为多个二值图像。这些阈值通过阈值步长递增直到最大阈值。第一个阈值是最小阈值，第二个是最小阈值+阈值步长，第三个是最小阈值+ 2 x阈值步长，以此类推。
2. 分组:在每个二值图像中，将连通的白色像素分组在一起。我们称这些为二元团。
3. 合并:计算二值图像中二值块的中心，并合并比minDistBetweenBlobs更近的二值块。
4. 中心和半径计算:计算并返回新合并的blob的中心和半径。
## 通过属性筛选

![difparam.png](https://wx2.sbimg.cn/2020/05/17/difparam.png)

- by color

需要将参数进行设置，即filterByColor = 1，如果设置blobColor = 0 则将挑选出更深色的blob区域，设置blobColor = 255 则相反，将筛选出更为明亮的区域
- by size

该筛选方法通过filterByArea= 1 激活，故名思意筛选的条件是blob的面积大小，可以设置minArea 和Max Area
-  by shape

这里需要涉及到形状评价方案了。
- Circularity（圆度）：

可能说是圆度不正确，我的理解就是用来评价blob和圆形的相似性。通过参数filterByCircularity= 1 激活，可以设置minCircularity和maxCircularity. Circularity度量公式为：

不看公式的话，就是面积占比，以正方形为例，想象一个半径为1的圆被边长为2的正方形包裹，那么圆面积/正方形面积 = 3.14/4 = 0.785。

- Convexity （凸度）

凸度嘛，就是看形状趋近于凸包(convex)的程度，设置filterByConvexity =1进行激活，minConvexity 和maxConvexity 的参考设置范围是0~1.

什么是凸包（凸多边形）：对于边界上的某一条线段，图形中的所有点集都在其同侧.如图 右所示
![concave.png](https://wx2.sbimg.cn/2020/05/17/concave.png)

- Inertia Ratio （惯性比）

惯性比是直译，我理解的是扁度，对于圆形，扁度为1，椭圆为0到1，直线就是0。通过设置filterByInertia = 1进行激活。

贴个图说明下：

![interiaRation.png](https://wx2.sbimg.cn/2020/05/17/interiaRation.png)

示范代码如下：
```
#include "opencv2/opencv.hpp"

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
	// 读图，图像前景黑，背景白为佳
	Mat im = imread( "blob.jpg", IMREAD_GRAYSCALE );

	// 设定blob筛选算子
	SimpleBlobDetector::Params params;

	// 灰度筛选
	params.minThreshold = 10;
	params.maxThreshold = 200;

	// 面积筛选
	params.filterByArea = true;
	params.minArea = 1500;

	// 圆度筛选
	params.filterByCircularity = true;
	params.minCircularity = 0.1;

	// 凸度筛选
	params.filterByConvexity = true;
	params.minConvexity = 0.87;

	// 扁度筛选
	params.filterByInertia = true;
	params.minInertiaRatio = 0.01;

	// 存储特征点的容器
	vector<KeyPoint> keypoints;
	//开始检测
	Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);   
	// 以红圈的方式标志检测出的Blob
	// DrawMatchesFlags::DRAW_RICH_KEYPOINTS flag ensures
	// 圈半径大小和blob 的半径大小一致

	Mat im_with_keypoints;
	drawKeypoints( im, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );

	imshow("keypoints", im_with_keypoints );
	waitKey(0);
```




