---
layout: post
title:  "在Qt中利用QProcess调用 exe"
categories: QT5.7.0学习
tags:  QT5.7.0学习
---


因为总调用一些exe, 因为总是需要在控制台手写参数。稍稍不甚就会输错，就借助Qt来传参调用吧。

### 1、QProcess 

直接上代码吧：

```
void test::slot_runPossoin()
{
    //获取参数的两个槽函数
    slot_depth();
    
    //创建一个进程
    QProcess * pocess = new QProcess(this);
    
    //调用exe
    QString program = "Poisson.exe";
    //把所有的参数存在一个 QStringList里
 
    QStringList argus;
    argus.append("--in");
    argus.append(in_file_name);
    argus.append("--out" );
    argus.append(out_file_name+"_recon.ply");
    argus.append("--depth");
    argus.append(QString::number(depth));
 
    //开始调用
    pocess->start(program, argus);
}
```

​	本例中其实在exe中的参数如下：也就是打在命令行里的。上述代码中的 **in_file_name** 就是下面的 **bunny.ply** 输出文件同理。

`Poisson.exe --in bunny.ply --out bunny.ply --depth 10 `

### 2、一些遇到的问题和处理技巧

​	因为自己也是才开始用QT的缘故，很多技巧都可以用，写在这里做一个记录：

#### 2.1 **int 转为QString**

比如depth是一个int 变量，由于参数表，也就是上文的argus存的是QString, 所以转换一下就好了。

` argus.append(QString::number(depth));`

#### 2.2 获取文件路径和文件名、后缀名 

```
QString file_full, file_name, file_path，file_suffix ;
QFileInfo fileinfo;
file_full = QFileDialog::getOpenFileName(this,.....);
fileinfo = QFileInfo(file_full);
//文件名
file_name = fileinfo.fileName(); 
//文件后缀
file_suffix = fileinfo.suffix()
//绝对路径
file_path = fileinfo.absolutePath();
```

#### 2.3 如何打包成可执行程序exe

​	当我在QT中写完且运行成功以后，如果想让别人也使用这个小工具。就得打包起来，打包除了把 vs 下生成的exe 拷走以外，还需要依赖平台的动态库。就是一些 dll 文件。

​	记住，有一个在QT中运行下的exe（最好用release版本的）, 一个是你总是重复调用但又不想手动在命令行打参数的exe。把两个放在一个文件下。

​       例如我的是PoissionRecon.exe。 cd 到所在目录下：

​       写入**windeployqt** PoissionRecon.exe。所依赖文件就自动生成了。

![](http://7xq62e.com1.z0.glb.clouddn.com/qtversion.png)

之后就可以打包用了，不用在新的机器上配置环境。










