---
layout: post
title: 判断点是否在多边形内部(2D)
date: 2020-06-13
categories: blog
tags: [微分几何]
---


这个问题太经典， 资料一搜一大堆，核心就是射线法。
凹凸polygon都适用。

从目标点出发引一条射线，看这条射线和多边形所有边的交点数目。如果有奇数个交点，则说明在内部，如果有偶数个交点（成对出现进点和出点），则说明在外部。
这里仅做一下 代码记录。
因为我想用图像绘制出来，所以中间采用了OpenCV进行了实验



```
#include <iostream> 
#include <cmath> 
#include <minmax.h>
#include <omp.h>

#include<ctime>
#include "opencv2/opencv.hpp"

using namespace std;

const double eps = 1e-6;

#define FILL_COLOR 255; //指定填充颜色

struct Point{
	double x, y;
	Point(double x = 0, double y = 0) :x(x), y(y){}

	//向量+
	Point operator +(const Point &b)const
	{
		return Point(x + b.x, y + b.y);
	}
	//向量-
	Point operator -(const Point &b)const
	{
		return Point(x - b.x, y - b.y);
	}
	//点积
	double operator *(const Point &b)const
	{
		return x*b.x + y*b.y;
	}
	//叉积
	//P^Q>0,P在Q的顺时针方向；<0，P在Q的逆时针方向；=0，P，Q共线，可能同向或反向
	double operator ^(const Point &b)const
	{
		return x*b.y - b.x*y;
	}
};


//三态函数，判断两个double在eps精度下的大小关系
int dcmp(double x)
{
	if (fabs(x) < eps) return 0;
	else
		return x < 0 ? -1 : 1;
}

//判断点Q是否在P1和P2的线段上
bool onSegment(Point P1, Point P2, Point Q)
{
	return dcmp((P1 - Q) ^ (P2 - Q)) == 0 && dcmp((P1 - Q)*(P2 - Q)) <= 0;
}

//判断点P在多边形内-射线法
bool inPolygon(Point polygon[], int n, Point P)
{
	bool isInPolygon = false;
	Point P1, P2; //多边形一条边的两个顶点
	int idx = 0;
	do
	{
		int next = (idx + 1) % n;
		P1 = polygon[idx];
		P2 = polygon[next];
		if (onSegment(P1, P2, P)) return true; //点在多边形一条边上
		if ((dcmp(P1.y - P.y)>0 != dcmp(P2.y - P.y) > 0) && 
			 dcmp(P.x - (P.y - P1.y)*(P1.x - P2.x) / (P1.y - P2.y) - P1.x) < 0)
			isInPolygon = !isInPolygon;
		idx = next;
	} while (idx != 0);
	return isInPolygon;
}

//确定多边形的矩形包围区域
void defineRange(Point polygon[], int n, Point& minPt, Point& maxPt)
{
	minPt.x = INT_MAX;
	minPt.y = INT_MAX;
	maxPt.x = INT_MIN;
	maxPt.y = INT_MIN;

	for (int i = 0; i < n; ++i)
	{
		minPt.x = min(minPt.x, polygon[i].x);
		minPt.y = min(minPt.y, polygon[i].y);
		maxPt.x = max(maxPt.x, polygon[i].x);
		maxPt.y = max(maxPt.y, polygon[i].y);
	}

}

void updateImgVal(cv::Mat& image, Point polygon[], int n)
{
	Point minPt, maxPt;
	defineRange(polygon, n, minPt, maxPt);

	//minPt.x = 0;
	//minPt.y = 0;
	//maxPt.x = image.cols;
	//maxPt.y = image.rows;
#pragma omp parallel num_threads(8)  
	for (int rowIdx = minPt.y; rowIdx <= maxPt.y; rowIdx++)
	{
		for (int colIdx = minPt.x; colIdx <= maxPt.x; colIdx++)
		{
			Point tmpPixel(colIdx,rowIdx );

			if (inPolygon(polygon, n, tmpPixel))
			{
				image.at<uchar>(rowIdx, colIdx) = FILL_COLOR;
			}
		}
	}
}



int orientation(Point p, Point q, Point r)
{
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0) return 0;  // colinear 

	return (val > 0) ? 1 : 2; // clock or counterclock wise 
}


bool doIntersect(Point p1, Point q1, Point p2, Point q2)
{
	// Find the four orientations needed for general and 
	// special cases 
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case 
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases 
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1 
	if (o1 == 0 && onSegment(p1, q1, p2)) return true;

	// p1, q1 and q2 are colinear and q2 lies on segment p1q1 
	if (o2 == 0 && onSegment(p1, q1, q2)) return true;

	// p2, q2 and p1 are colinear and p1 lies on segment p2q2 
	if (o3 == 0 && onSegment(p2, q2, p1)) return true;

	// p2, q2 and q1 are colinear and q1 lies on segment p2q2 
	if (o4 == 0 && onSegment(p2, q2, q1)) return true;

	return false; // Doesn't fall in any of the above cases 
}

bool whetherNewSegIntersect(Point p, Point polygon[], int n)
{
	bool isIntersect = false;
	Point q(polygon[n - 1].x, polygon[n - 1].y);	
	
	int ptIndex = n-1;
	do
	{
		int lastPtIdx = ptIndex-1;
		Point p2 = polygon[ptIndex];
		Point q2 = polygon[lastPtIdx];

		if (doIntersect(p, q, p2, q2))
		{
			isIntersect = true;
		}
		ptIndex--;
	} while (ptIndex > 0);
	return isIntersect;
}


int main()
{

	Point polygon[] = { (0, 0), (0, 10), (10, 10), (5, 5) };
	Point newPt(-5,5);
	int n1 = sizeof(polygon) / sizeof(polygon[0]);
	whetherNewSegIntersect(newPt, polygon, n1) ? cout << "Yes\n" : cout << "No\n";
	cv::Mat img = cv::imread("test.png"); //582*487
	//声明Mat对象，作二值化用
	cv::Mat gray, bin;
	//灰度变换
	cv::cvtColor(img, gray, CV_BGR2GRAY);
	//2値化
	threshold(gray, bin, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//cv::imshow("原图", img);
	//凸多边形
	Point polygon1[] = { { 168, 195 }, { 218, 139 }, { 312, 157 }, { 357, 224 }, { 337, 289 }, { 274, 345 }, { 191, 294 }, { 147, 242 } };
	//凹多边形
	//Point polygon1[] = { { 377, 228 }, { 519, 150 }, { 474, 240 }, { 514, 325 }};	
	int n = sizeof(polygon1) / sizeof(polygon1[0]);

	Point minPt, maxPt;
	defineRange(polygon1, n, minPt, maxPt);
	int width = maxPt.x - minPt.x;
	int height = maxPt.y - minPt.y;
	
	clock_t startTime = clock();
	updateImgVal(bin, polygon1, n);
	clock_t endTime = clock();
	cout << "刷新" << width << "*" << height<<" 大小图像用时" << double(endTime - startTime)  << "ms" << endl;

	//cv::imshow("更新后", bin);
	//cv::waitKey();
	system("pause");
	return 0;
}
```

