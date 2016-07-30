---
layout: post
title:  "BeautifulSoup简介"
categories: Python爬虫
tags:  Python
---

决定开一个系统的基于python 3.5的爬虫教程，囊括对Html的解析，API的使用，数据的存储，文档的读取。数据清洗，语言处理。

之前的6篇文章里对爬虫时用到的BeautifulSoup篇幅太少，为了后续的教程，准备认真开一个写。

Beautiful库出自于《爱丽丝仙境》的诗歌，不得不说很多库的命名十分任性又透着诙谐。

# 安装BeautifulSoup
BeautifulSoup 库不是 Python 标准库，需要我们自己单独安装，這里我只写了windows下的安装方法。
常用的pip和install在3.5版本的Python中都封装好了。
在此基础上即可安装BeautifulSoup。

如果使用BeautifulSoup4时候出现了ImportError: cannot import name 'HTMLParseError'的错误

一种可能是eautifulSoup在4.4.0以前的版本不支持Python3.5，所以我们需要把我们的BeautifulSoup升级到4.4.0版本以上。

如果你看的是本教程，采用的是python3.5版本，需要升级beautifulsoup.
1、使用pip升级
在管理员权限下使用命令行输入命令(采用cd 法，在cmd模式下
cd \ 返回c盘 cd python3.5的文件夹下，输入以下命令，同时我的pip还需要升级，详见截图)
    
    pip install --upgrade beautifulsoup4

2、使用源码重新安装
在[http://www.crummy.com/software/BeautifulSoup/bs4/download/](http://note.youdao.com/)下找到你需要下载的版本下载=》解压文件=》找到根目录下的setup.py=》以管理员权限打开命令行输入
    
    python setup.py install
    
![image](http://7xq62e.com1.z0.glb.clouddn.com/bs_install.png)

# 运行 BeautifulSoup  
    from urllib.request import urlopen 
    from bs4 import BeautifulSoup 

    html = urlopen("http://adastaybrave.com") 

    bsObj = BeautifulSoup(html.read(),'html.parser')       
    print(bsObj.h1)
    
输出的结果应该是：
    
    <h1>挣脱不足与蒙昧</h1>
    
我们曾用urlopen，通过调用html.read()获取网页的html内容，将html.read（）获取到的内容传入给BeautifulSoup对象，可以得到以下结构
![image](http://7xq62e.com1.z0.glb.clouddn.com/structure.png)

我们从网页中提取的 <h1> 标签被嵌在 BeautifulSoup 对象 bsObj 结构的第二层
（**html → body → h1**）。但是，当我们从对象里提取 h1 标签的时候，可以直接调用它：

事实上，以下语句也有同样的效果：
    
    bsObj.html.body.h1
    bsObj.body.h1
    bsObj.html.h1
    
在之后的正则表达式中，与BeaytifulSoup结合后对网站信息的提取将更加容易。

# 可靠的网络连接
网络是复杂的，数据是混乱无组织的，服务器也会宕机，数据标签的丢失，这都是爬虫数据时可能遇到的种种窘境，预计可能出现的异常，高瞻远瞩，才是培养程序员缜密严谨思维的不二法门。
从第一句开始
    html = urlopen("http://adastaybrave.com")
    
這里可能会出现什么异常呢？
1. 网页不存在
2. 服务器不存在

第一种异常发生时，通常返回 “404 not found”
或者“500 Internal Server Error”,這时，urlopen函数都会抛出“HTTPError”异常。处理方式如下：

    try:
        html = urlopen("http://adastaybrave.com")
    except HTTPError as e:
        print(e)
        
如果程序返回 HTTP 错误代码，程序就会显示错误内容,如果服务器不存在,也就是url不存在，或许拼错了,urlopen 会返回一个 None 对象.我们
可以增加一个判断语句检测返回的 html 是不是 None

    if html is None:
        print("URL is not found")
        
然而，当我们能从服务器获取网页，也可能在获取内容上出现异常，调用BeautifulSoup时出现标签时，如果我们调用的标签不存在，BeautifulSoup就会返回None对象，再调用None对象下的子标签，会再次引发生 AttributeErrorde。

比如：

    print(bsObj.nonExistentTag)
    
假设nonExistentTag是虚拟的标签，如果我们此时不检查，当我们访问其子标签时,如

    print(bsObj.nonExistentTag.someTag)
 
此时就会返回异常：
    AttributeError: 'NoneType' object has no attribute 'someTag'
    
如何检查呢？

    try:
        badContent=bsObj.nonExistingTag.anotherTag
    except AttributeError as e:
        print("Tag was not found")
    else:
        if badContent == None:
            print ("Tag was not found")
        else:
            print(badContent) 
            
            
我们来看一段完整的代码：
    
    from urllib.request import urlopen
    from urllib.error import HTTPError
    from bs4 import BeautifulSoup
    import sys


    def getTitle(url):
        try:
            html = urlopen(url)
        except HTTPError as e:
            print(e)
            return None
        try:
            bsObj = BeautifulSoup(html.read(),'html.parser')
            title = bsObj.body.h1
        except AttributeError as e:
            return None
        return title
    
    title = getTitle("http://adastaybrave.com")
    if title == None:
        print("Title could not be found")
    else:
        print(title)
        
getTitle 函数，可以返回网页的标题，如果获取网页的时候遇到问题就返回一个 None 对象。就检查了HTTPError，然后把两行 BeautifulSoup 代码封装在一个 try 语句里面。这两行中的任何一
行有问题，AttributeError 都可能被抛出（如果服务器不存在，html 就是一个 None 对象，
html.read() 就会抛出 AttributeError）。事实上，我们可以在 try 语句里面放任意多行代码，
或者放一个在任意位置都可以抛出 AttributeError 的函数。
    
    
    
    
