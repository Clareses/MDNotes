# HTML & CSS

## HTML结构

```html
<!DOCTYPE html>
<html lang="en">

<!-- head部分包含了大部分参数，包括样式表的绑定等 -->
<head>
    <!-- meta是元标签，用于记录一些参数供浏览器解析 -->
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <!-- title是浏览器标签页上显示的页面 -->
    <title>HELLO</title>
</head>

<!-- body部分记录html的结构，注意一个原则：html和css应该各负责各自的内容部分（w3c制定的标准使得html和css都具有一定的控制样式的能力...尽量分开写） -->
<body>
    <a href="http://www.baidu.com">转到百度</a>
</body>

</html>
```

## HTML元素

### 全局attribute

| attribute | mean                           | option value |
| --------- | ------------------------------ | ------------ |
| id        | 唯一标识当前文档中的元素       | any string   |
| title     | 当鼠标移动到元素上时的显示内容 | any string   |
|           |                                |              |



### 格式

```html
<!-- HTML元素的一般格式 -->
<elementName>content</elementName>

<!-- HTML空元素 -->
<elementName /> 或 <elementName>
<!-- 如 -->
<img src alt/>
<meta attribute/>
    
<!-- HTML属性的书写方式 -->
<elementName attribute="attribute value">content</elementName>
<!-- 如<a>标签 -->
<a href="https://www.baidu.com">跳转到百度</a>


```

### 文本元素

```html
<h1>1级标题</h1>
<h2>2级标题</h2>
<h3>3级标题</h3>
<h4>4级标题</h4>
<h5>5级标题</h5>
<h6>6级标题</h6>
<!-- 快捷输入, h$*6>{$级标题} -->

<p>段落元素</p>
<!-- 可以用lorem生成乱序假文进行测试 -->

<span>文本切片</span>
<!-- 用来给文本分段，方便样式设置 -->

<pre>预格式化文本元素</pre>
<!-- 不会发生空白折叠 -->

<!-- 实体符号 -->
要在文本中显示一些可能被浏览器解析的内容，需要使用实体符号
如：<p>等
实体符号的表示：&+单词;
   	< —— lt(less than)
    > —— gt(great than)
等
```

### 超链接

```html
<a> content </a>

关键attribute: href(hyper reference?)
- href会让浏览器访问对应地址的文件（浏览器的地址栏变为href）
- href也可以用于访问锚链接

访问锚链接的方式如下：
<a href="#chapter"></a>
在#后加上对应元素的id即可
    
关键attribute: target
- 选择当前页面的打开方式
- 可选值： _self, _blank
- 会分别在当前窗口、新窗口打开超链接
```

### map与area元素

```html
<map name="name">
	<area shape="" coords="" href="" target="">
</map>

map------name 用于标识地图，值为字符串
area----shape 用于标记有效区域的形状（circle等）
area---coords 用于标识有效区域的关键点坐标数据（circle的格式为x,y,r(半径)）
area-----href 同a标签
area---target 同a标签
```

### 图片元素

```html
<img>

关键attribute: src
- src会访问对应的超链接拉取资源

关键attribute: alt
- 当src失效时，使用alt中的文字替代

关键attribute: usemap
- 值为对应#+map的name属性
```

#### 与超链接共用

```html
<a href="">
	<img src="">
</a>
```

#### 与map共用

修改usemap属性的值即可

### 多媒体元素

```html
<video>content</video>
<audio>content</audio>

关键attribute：
- src：写入资源路径
- controls：可选值只能为"controls"，用于显示进度条
- autoplay：可选值只能为"autoplay"，表示自动播放
- muted：可选值为"muted"，表示默认静音播放
- loop：可选值为"loop"，表示循环播放
```

### 列表

```html
有序列表
<ol>
	<li>item</li>
    <li>item</li>
    <li>item</li>
</ol>

无序列表
<ul>
	<li>item</li>
	<li>item</li>
    <li>item</li>
</ul>
```

### 容器元素

```html
<div> </div>
<header> </header>
<footer> </footer>
<article> </article>
```

### link

```html
<link>

关键attribute：
- rel：指定链接的类型，可选值有stylesheet（样式表）
- href：对应文件的绝对路径或相对路径
```



## CSS

### 结构

```css
/*选择器，对选中的属性应用声明块中的属性*/
selector {
    /*声明块，包含了css属性*/
    declare block;
}
```

```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Document</title>
    <style>
        /* 会选中所有的h1标签 */
        h1 {
            color: aqua;
            font-size: 3em;
        }
        /* 会选中id为myHeader的标签 */
        #myHeader {
            color: blue;
        }
        /* 会选中red类中的所有元素 */
        .red {
            color: red;
        }
    </style>
</head>
<body>
    
</body>
</html>
```

### CSS属性

#### 颜色相关

| attribute        | description    | optional |
| ---------------- | -------------- | -------- |
| color            | 元素内部的颜色 | /        |
| background-color | 元素的背景颜色 | /        |

#### 字体

| attribute   | description | optional          |
| ----------- | ----------- | ----------------- |
| font-size   | 字体大小    | px、em、预设值    |
| font-family | 字体族      | 字体名            |
| font-weight | 字体粗细    | 100 ~ 900、预设值 |
| font-style  | 字体风格    | normal、itali     |

#### 