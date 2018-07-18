---
layout: page
title: "Tags"
description: "夕花朝拾"
header-img: "img/zhihu2.jpg"
---


## 本页使用方法

1. 选择你兴趣内容
2. 点击它
3. 相关的文章会「唰」地一声跳到页面顶端
4. Have a try

## 基因列表


<ul class="tags">
    {% for tag in site.tags %}
    <li>
        <a href="#{{ tag[0] }}">{{ tag[0] }}</a> <sup>{{ tag[1].size }}</sup>
    </li>
    {% endfor %}
</ul>

<ul class="listing">
    {% for tag in site.tags %}
    <li class="listing-seperator" id="{{ tag[0] }}">{{ tag[0] }}</li>
    {% for post in tag[1] %}
    <li class="listing-item">
        <time datetime="{{ post.date | date:"%Y-%m-%d" }}">{{ post.date | date:"%Y-%m-%d" }}</time>
        <a href="{{ post.url }}" title="{{ post.title }}">{{ post.title }}</a>
    </li>
    {% endfor %}
{% endfor %}
</ul>
