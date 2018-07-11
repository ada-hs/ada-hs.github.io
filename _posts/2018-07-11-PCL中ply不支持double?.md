---
layout: post
title:  "PCL中ply不支持double"
categories: PCL与点云万象
tags: PCL与点云万象
---
准备用pcl中的io::loadPLYFile读取数据来着，断点调试以后点云数据全变成0了有木有
查了半天，最后在github上的issue中找到了蛛丝马迹
[pcl_viewer not displaying points when used with pcd file with size 8](https://github.com/PointCloudLibrary/pcl/issues/1953)

用notepad看了一下ply文件的前几行字段，正常的都是float型，我的是float64（float64的来历很可能是typedef double float64 所以跟double是一样的。）

靠谱的解释是：

![](http://7xq62e.com1.z0.glb.clouddn.com/pic/7.11.png)

以及给出的解决方案是：

    #define PCL_NO_PRECOMPILE
    #include <pcl/point_types.h>
    #include <pcl/io/pcd_io.h>
    
    namespace pcl
    {
      #define PCL_ADD_UNION_POINT4D_DOUBLE \
        union EIGEN_ALIGN16 { \
          double data[4]; \
          struct { \
            double x; \
            double y; \
            double z; \
          }; \
        };
    
      struct _PointXYZDouble
      {
        PCL_ADD_UNION_POINT4D_DOUBLE; // This adds the members x,y,z which can also be accessed using the point (which is float[4])
    
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      };
    
      struct EIGEN_ALIGN16 PointXYZDouble : public _PointXYZDouble
      {
        inline PointXYZDouble (const _PointXYZDouble &p)
        {
          x = p.x; y = p.y; z = p.z; data[3] = 1.0;
        }
    
        inline PointXYZDouble ()
        {
          x = y = z = 0.0;
          data[3] = 1.0;
        }
    
        inline PointXYZDouble (double _x, double _y, double _z)
        {
          x = _x; y = _y; z = _z;
          data[3] = 1.0;
        }
    
        EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      };
    }
    
      POINT_CLOUD_REGISTER_POINT_STRUCT (pcl::_PointXYZDouble,
        (double, x, x)
        (double, y, y)
        (double, z, z)
      )
      POINT_CLOUD_REGISTER_POINT_WRAPPER(pcl::PointXYZDouble, pcl::_PointXYZDouble)
    
    int
    main ()
    {
      pcl::PointCloud<pcl::PointXYZ> cloud;
      if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/tmp/test_2.pcd", cloud))
      {
        PCL_ERROR ("Couldn't read PCD file \n");
        exit(1);
      }
      std::cout << "Cloud1 size: " << cloud.size () << std::endl;
      for (size_t i = 0; i < cloud.size (); ++i)
        std::cout << "x: " << cloud[i].x
                  << " y: " << cloud[i].y
                  << " z: " << cloud[i].z
                  << std::endl;
    
      pcl::PointCloud<pcl::PointXYZDouble> cloud2;
      if (pcl::io::loadPCDFile<pcl::PointXYZDouble> ("/tmp/test_2.pcd", cloud2))
      {
        PCL_ERROR ("Couldn't read PCD file \n");
        exit(1);
      }
      std::cout << "Cloud2 size: " << cloud2.size () << std::endl;
      for (size_t i = 0; i < cloud2.size (); ++i)
        std::cout << "x: " << cloud2[i].x
                  << " y: " << cloud2[i].y
                  << " z: " << cloud2[i].z
                  << std::endl;
    
    	return 0;
    }
    
也就是自己指定了一个PointXYZDouble类型，来引入点.

但后续通过io::savePLYFile()存储文件作为中间文件时，也会输出为double的，如果我后面操作又要用到这个中间文件为输入，有时候会有问题。
于是我重新声明了一个pcl中原本支持的数据类型，然后一一赋值出去

    pcl::PointCloud<pcl::PointXYZ> final_cloud;
    final_cloud.points.resize(combined_clouds.size());
    for (size_t i = 0; i < final_cloud.points.size(); i++) {
        final_cloud.points[i].x = combined_clouds.points[i].x;
        final_cloud.points[i].y = combined_clouds.points[i].y;
        final_cloud.points[i].z = combined_clouds.points[i].z;
    }
    pcl::io::savePLYFileBinary(outfile, final_cloud);
    
之后再就没什么问题啦。如果以后有小伙伴遇见这个坑可以借鉴看看。
