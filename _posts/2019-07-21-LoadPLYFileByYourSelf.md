---
layout: post
title:  "PCL读取点云慢？自己写一个！"
categories: PCL与点云万象
tags:  PCL与点云万象
---

为了方便运算有时候我们只需要点云文件里的坐标信息。尝试在PCL中我们加载点云感觉实在太慢了。索性自己鼓捣了一个读写方案，只取自己需要的信息存在自己定义的数组或者容器里，还方便存取。

这篇文章适合有以下需求的：

1. 需要自己读取ply格式点云坐标并存储预做后续处理的
2. 无论是Ascii 还是二进制码流都支持。

我就结合功能+代码，提炼我用到的一些方法。

### ply文件剖析
有时候不同的软件所生成的ply数据，在标识头部分都有很多信息：
比如：

```
ply
format binary_little_endian 1.0
comment VCGLIB generated
element vertex 262144
property float x
property float y
property float z
element face 0
property list uchar int vertex_indices
end_header
```

还有PCL自己生成的(特别长)：

```
ply
format ascii 1.0
comment PCL generated
element vertex 179490
property float x
property float y
property float z
element camera 1
property float view_px
property float view_py
property float view_pz
property float x_axisx
property float x_axisy
property float x_axisz
property float y_axisx
property float y_axisy
property float y_axisz
property float z_axisx
property float z_axisy
property float z_axisz
property float focal
property float scalex
property float scaley
property float centerx
property float centery
property int viewportx
property int viewporty
property float k1
property float k2
end_header
```


ply文件头部字段说明的信息大概说起来有以下：

- ply 格式说明
- 码流说明（二进制还是Ascii）
- 来源说明  (PCL还是VCGlib,也有可能是别的库或软件）
- 坐标数值类型(建议float,PCL目前支持float)
- 如果带法向，则有法线数值类型(同上)
- 颜色或强度
- 曲率
- ..........

 
 如果是非点云的网格类型ply，后面还有三角面片索引，我就不展开了。

 这里我仅针对仅有坐标的ply点云文件进行读写。以下代码如果理解了，可以自行修改，获取各种你想要的东西，以下代码仅供学习用。


###  FileIO.h

```
#pragma once
#include<iostream>
#include<string>
#include<fstream>
#include<istream>
#include<ostream>
#include<sstream>
#include<vector>
#include<cstring>

#include <stdio.h>
#include <stdlib.h>

using namespace std;
class FileIO
{
public:
	FileIO();
	~FileIO();

	//加载点云。判断待读取的ply点云为二进制还是asc码流
	void LoadPts(char *fileName);

	//加载二进制ply点云码流
	void loadAscPts(char *fileName);

	//加载asc ply点云码流
	void loadBinPts(char *fileName);

	//以二进制方式保存点云文件

	void saveBinPts(std::vector<float> &Pts,char *fileName);

	//以ASCII方式保存点云文件
	void saveAscPts(std::vector<float> &Pts,char *fileName);
	std::vector<float> Pts;
	int numPts; 

};


```

这里没什么好说的，如果已经知道是什么码流，可以分别调用loadAscPts或者loadBinPts。如果不知道。就调用LoadPts即可，在LoadPts内部会跳转。
接下来我们看看怎么实现。

### FileIO.cpp

代码有点长，可以先跳过，先看以下解释部分

```
#include "FileIO.h"

FileIO::FileIO(){}

FileIO::~FileIO(){}

void FileIO::LoadPts(char * filename)
{
	ifstream in;
	in.open(filename);
	string line;

	string format_flag_bin = "binary";
	string format_flag_asc = "ascii";
	
	while (!in.eof())
	{
		getline(in, line);
		if (line.find(format_flag_bin) != string::npos)
		{
			cout << "binary format point cloud loaded! " << endl;
			loadBinPts(filename);
			in.close();
			break;
		}
		else if (line.find(format_flag_asc) != string::npos)
		{
			cout << "ascii format point cloud loaded! " << endl;
			loadAscPts(filename);
			in.close();
			break;
		}
		else
			continue;
	}
}

void FileIO::loadBinPts(char * filename)
{

	FILE *fp;

	fp = fopen(filename, "rb");

	char strLine[1024];

	char end_flag[] = "end_header ";
	char num_flag[] = "element vertex ";
	char *p;
	char num[100];


	if (fp == NULL)
	{
		printf("Error:Open input.c file fail!\n");
		return;
	}

	while (!feof(fp))									//循环读取每一行，直到文件尾
	{
		fgets(strLine, 1024, fp);

		if (strlen(strLine) == (strlen(end_flag)))
		{
			break;
		}

		if ((p = strstr(strLine, num_flag)) != NULL)
		{
			int start = strlen(num_flag);
			int sub_len = strlen(strLine) - strlen(num_flag);

			for (int i = 0; i < sub_len; i++)
			{
				num[i] = strLine[start + i];    //从第start+i个元素开始向数组内赋值
			}
			numPts = atoi(num);
		}
	}

	float *pts = (float*)malloc(numPts * 3 * sizeof(float));

	float cnt = numPts * 3;

	fread(pts, sizeof(float), cnt, fp);

	fclose(fp);

	for (int i = 0; i<numPts; i++)
	{
		//注意点的访问方式
		Pts.push_back(pts[3 * i + 0]);
		Pts.push_back(pts[3 * i + 1]);
		Pts.push_back(pts[3 * i + 2]);	
	}

	// 打印验证，可以自行取消注释打印出来看读入是否正确
	
	//ofstream out("bin_pts_check.txt");
	//for (int pt_idx = 0; pt_idx < numPts; ++pt_idx)
	//{
	//	for (int j = 0; j < 3; ++j)
	//	{
	//		out << pts[pt_idx * 3 + j] << " "; //注意点的访问方式
	//	}
	//	out << endl;
	//}
}

void FileIO::loadAscPts(char *filename)
{
	ifstream in;
	in.open(filename);

	if (!in)
	{
		std::cerr << "File " << filename << " could not be opened" << std::endl;
	}

	string line;

	string num_flag = "element vertex ";

	int pos = num_flag.size();


	//提取标识字段中所包含的点的数目信息
	while (!in.eof())
	{
		getline(in, line);

		if (line.find(num_flag) != string::npos)
		{

			string num = line.substr(pos);
			numPts = atoi(num.c_str());

			cout << "There are " << numPts << " Points" << endl << endl;
		}

		if (line == "end_header")
		{
			break;
		}
	}

	float x, y, z;

	ofstream out("asc_pts_check.txt");

	while (!in.eof())
	{
		in >> x >> y >> z;
		getline(in, line);
		if (in.fail())
			break;
		//out << x << " " << y << " " << z << endl;
		Pts.push_back(x);
		Pts.push_back(y);
		Pts.push_back(z);
	}
	in.close();
}

void FileIO::saveBinPts(std::vector<float> &Pts, char *fileName)
{
	int elementNum = Pts.size() / 3;
	FILE *fp = fopen(fileName, "wb");
	fprintf(fp, "ply\n");
	fprintf(fp, "format binary_little_endian 1.0\n");
	fprintf(fp, "comment File generated\n");
	fprintf(fp, "element vertex %d\n", elementNum);
	fprintf(fp, "property float x\n");
	fprintf(fp, "property float y\n");
	fprintf(fp, "property float z\n");
	fprintf(fp, "end_header\n");

	for (int i = 0; i < elementNum; i++)
	{
		float tmp2[3] = { Pts[3 * i + 0], Pts[3 * i + 1], Pts[3 * i + 2] };
		fwrite(tmp2, sizeof(float), 3, fp);
	}
}

void FileIO::saveAscPts(std::vector<float> &Pts, char *fileName)
{
	int elementNum = Pts.size() / 3;
	std::ofstream out(fileName);
	out << "ply" << std::endl;
	out << "format ascii 1.0" << std::endl;
	out << "comment File generated" << std::endl;
	out << "element vertex " << elementNum << std::endl;
	out << "property float x" << std::endl;
	out << "property float y" << std::endl;
	out << "property float z" << std::endl;
	out << "end_header" << std::endl;

	for (int i = 0; i < elementNum; i++)
	{
		out << Pts[3 * i + 0] << " " << Pts[3 * i + 1] << " " << Pts[3 * i + 2] << " " << std::endl;
	}
}

```
### 调用

```
#include"FileIO.h"
using namespace std;
int main()
{
	FileIO testObj;
	testObj.LoadPts("XXX.ply");
	cout << testObj.Pts.size() / 3;
	testObj.saveAscPts(testObj.Pts, "XXX.ply");
	return 0;
}
```


### 几个小技巧

**LoadPts**的主要功能就是逐行读取文件，读到标识码流信息的行就记录，并调用相应的方法（loadAscPts或者loadBinPts）.然后跳出。如果提前知道是什么码流，自行区别调用
**loadAscPts**或者**loadBinPts**
##### 小技巧1： find 和npos

判断读到的行line里有没有 “ binary” 还是“asc ”

```
if (line.find(format_flag_bin) != string::npos)
```
line 是 string . 所以这里需要的就是在一个字符串中找到子字符串。

npos 是一个常数，用来表示不存在的位置，类型一般是std::container_type::size_typ许多容器都提供这个东西。由于[find](http://www.cplusplus.com/reference/algorithm/find/)函数的返回值是整数(是下标啦)，假如字符串存在包含关系，其返回值必定不等于npos，但如果字符串不存在包含关系，那么返回值就一定是npos

##### 小技巧2： [strstr](http://www.cplusplus.com/reference/cstring/strstr/)

```
if ((p = strstr(strLine, num_flag)) != NULL)
```
strstr(str1,str2) 用来判断后者是不是前者的子字符串，如果是，则返回str2在str1中首次出现的地址；对比一下find，find返回主字符串中查找到的子字符串的索引。一个偏折C风格，一个偏C++风格，大家灵活使用

##### 小技巧3：f族的使用
相对来说，MSVC平台上C风格的读写比C++ 要快很多，不过我在其它帖子也看到过似乎不同的编译器和平台会有很大差异++。MinGW的话C++读写又很快。

同时，我在读取二进制文件的时候，采用了整块内存读入（fread）的方式，而asc文件时，采用了getLine的坑逐行读取，感兴趣的可以参见我另一篇文章：[c++从第n行开始读入数据](http://adastaybrave.com/%E7%AE%97%E6%B3%95%E4%B8%8E%E9%9D%A2%E8%AF%95/2018/07/09/c++%E4%BB%8E%E7%AC%ACn%E8%A1%8C%E5%BC%80%E5%A7%8B%E8%AF%BB%E5%85%A5%E6%95%B0%E6%8D%AE/)

当然还有其它多种多样的格式，不过，举一反三都搞得定哟。

