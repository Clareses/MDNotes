

# HTML5标签总结

![img](https:////upload-images.jianshu.io/upload_images/7166236-56a3d9aabca88961.png?imageMogr2/auto-orient/strip|imageView2/2/w/564/format/webp)

HTML元素 (1).png

- 日常安利工具，不想去买/破解`xmind`可以使用百度出的[百度脑图](https://link.jianshu.com?t=http%3A%2F%2Fnaotu.baidu.com)来画思维导图。

### 按上面的分类来分别介绍

#### HTML文档标签

1. `<!DOCTYPE>`：定义文档类型。
2. `<html>`： 定义HTML文档。
3. `<head>`：定义文档的头部。
	1. `<meta>`：定义元素可提供的有关页面的元信息，比如针对搜索引擎和更新频度的描述和关键字。
	2. `<base>`：定义页面上的所有链接规定默认地址或默认目标。
	3. `<title>`：定义文档标题。
	4. `<link>`：定义文档与外部资源的关系。
	5. `<style>`：定义HTML文档样式信息。
4. `<body>`：定义文档的主体。（脚本在非必需情况时在<body>的最后）
	1. `<script>`：定义客户端脚本，比如`javascript`。
	2. `<noscript>`：定义脚本未被执行时的替代内容。（如文本）

#### 按闭合特征分类

##### 闭合标签：

- 闭合标签是指由开始标签和结束标签组成的一对标签，这种标签允许嵌套和承载内容，例如`<html></html>、<p></p>`等。

##### 空标签：

- 空标签是指没有内容的标签，在开始标签中自动闭合。常见的空标签有：`<br>、<hr>、 <img>、 <input>、 <link> <meta>`。

#### 按是否换行特征分类

##### 块级元素：

- 块级元素是指本身的属性为`display:block`的元素。因为他自身的特点，我们通常使用块级元素来进行大布局（大结构）的搭建。
- 块级元素的特点：
	1. 每个块级元素独占一行，每个块级元素都会从新的一行开始，从上到下排布。
	2. 块级元素可以直接控制宽度、高度以及盒子模型的相关css属性。
	3. 在不设置宽度的情况下，块级元素的宽度是他父级元素内容的宽度。
	4. 在不设置高度的情况下，块级元素的高度是他本身内容的高度。
- 常见**块级元素**：

| 标签       | 描述                                    |
| ---------- | --------------------------------------- |
| div        | 常用块级容器，也是css和layout的主要标签 |
| h1、h2、h3 | 一、二、三级标题                        |
| h4、h5、h6 | 四、五、六级标题                        |
| hr         | 水平分隔线                              |
| menu       | 菜单列表                                |
| ol、ul、li | 有序列表、无序列表、列表项              |
| dl、dt、dd | 定义列表、定义术语、定义描述            |
| table      | 表格                                    |
| p          | 段落                                    |
| form       | 交互表单                                |

##### 内联元素

- 内联元素是指本身属性为`display:inline`的元素，其宽度随元素的内容而变化。因为他自身的特点，我们通常使用内联元素来进行文字、小图片（小结构）的搭建。
- 内联元素的特点：
	1. 内联元素会和其他元素从左到右显示在一行。
	2. **内联元素不能直接控制宽度、高度以及盒子模型的相关css属性，但是可以设置内外边距的水平方向的值。也就是说对于内联元素的`margin`和`padding`，只有`margin-left/margin-right`和`padding-left/padding-right`是有效的，但是竖直方向的`margin`和`padding`无效果。**
	3. 内联元素的宽高是由内容本身的大小决定的（文字、图片等）。
	4. 内联元素只能容纳文本或者其他内联元素（不要在内联元素中嵌套块级元素）。
- 常见的内联元素：

| 标签   | 描述                           |
| ------ | ------------------------------ |
| a      | 锚点                           |
| b      | 加粗                           |
| span   | 常用的内联容器，定义文本内区块 |
| img    | 引入图片                       |
| input  | 输入框                         |
| select | 下拉列表                       |
| strong | 加粗强调                       |
| label  | Input元素定义标注（标记）      |

#### H5新标签

##### 祭出脑图：

![img](https:////upload-images.jianshu.io/upload_images/7166236-7416a45973370997.png?imageMogr2/auto-orient/strip|imageView2/2/w/910/format/webp)

##### 然而。。。

- h5新特性感觉面试必问，但是记住所有是不可能的，也不切实际。把几个主要的新标签记住，讲清楚应该就差不多了吧。

| 标签       | 描述                                                         |
| ---------- | ------------------------------------------------------------ |
| <time>     | 定义日期或时间                                               |
| <progress> | 状态标签（任务过程：安装、加载）                             |
| <canvas>   | 定义图形，比如图标和其他图像。，canvas作容器，用脚本绘制图形。 |
| <dialog>   | 定义对话框或窗口。                                           |
| <video>    | 定义视频 ，像电影片段或其他视频流而不用再使用第三方插件。现在主要支持 Ogg 和 MPEG 两种视屏格式。 |
| <audio>    | 定义音频，如音乐或其他音频流。                               |
| <keygen>   | 定义表单里一个生产的键值，加密信息传送。                     |

#### 常见问题

##### 1. 内联元素可以使用padding和margin吗?（别人的面试题）

- 答：对于内联元素的`margin`和`padding`，只有`margin-left/margin-right`和`padding-left/padding-right`是有效的，但是竖直方向的`margin`和`padding`无效果。但是内联元素父级设置的`padding`对内联元素是有作用的。举个小例子：



```html
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>Title</title>
</head>
<body>
<div style="height: 300px;width: 300px">
    <!--此处的margin、padding只有水平方向有效-->
    <span style="padding: 50px; margin: 50px">helloworld</span>
    <!--在父级节点设置的margin，对于嵌套的内联元素，也只是水平方向有效果-->
    <div style="width: 200px;height: 200px;background-color: cyan;margin: 50px">
        <span>helloworld</span>
    </div>
</div>
<div style="height: 300px;width: 300px">
    <!--在父级节点设置的padding，对于嵌套的内联元素，水平和垂直方向都有效果-->
    <div style="width: 200px;height: 200px;background-color: cyan;padding: 50px">
        <span>helloworld</span>
    </div>
</div>
</body>
</html>
```

- 效果如下：

![img](https:////upload-images.jianshu.io/upload_images/7166236-d76924394a71f427.png?imageMogr2/auto-orient/strip|imageView2/2/w/517/format/webp)

image.png

##### 2. 如何让2个元素在同一行？

这是个有趣的问题，应该有不少人搜过，我也搜过。出现这个问题主要还是滥用块级元素，或者是不会使用一些小技巧。举个例子，有时候我们想实现如下表单效果：



```html
<label for="name2">姓名：</label>
<input id="name2" type="text" placeholder="请输入姓名">
```

![img](https:////upload-images.jianshu.io/upload_images/7166236-de50875980b4f781.png?imageMogr2/auto-orient/strip|imageView2/2/w/239/format/webp)

image.png

但有时候写着写着会变成下面的样子：

![img](https:////upload-images.jianshu.io/upload_images/7166236-0b436315bb10614c.png?imageMogr2/auto-orient/strip|imageView2/2/w/282/format/webp)

image.png

按我从以前遇到过情况来说，出现上面的情况的原因，可能有2个：

**第一，就是可能在某个地方<label>被设置块级元素包住了**

```html
<div>
<label for="name1">姓名：</label>
</div>
<input id="name1" type="text" placeholder="请输入姓名">
```

**解决方法：**在外部样式使用`display: inline-block`

```html
<div style="display: inline-block">
<label for="name1">姓名：</label>
</div>
<input id="name1" type="text" placeholder="请输入姓名">
```

**第二，就是<label>的样式使用了 `display: block`**

```html
<label for="name2" style="display: block">姓名：</label>
<input id="name2" type="text" placeholder="请输入姓名">
```

所以如果以后在写表单的时候，出现这个问题，可以从上面2个方向去查找原因，以及找出相应的解决方法。

##### `<!Doctype>` 确定浏览器使用何种模式渲染

（1）、<!DOCTYPE>声明位于位于HTML文档中的第一行，处于 <html> 标签之前。告知浏览器的解析器用什么文档标准解析这个文档。DOCTYPE不存在或格式不正确会导致文档以兼容模式呈现。使用`<!Doctype>`会让文档中的所有盒子模型以w3c标准盒子模型呈现。

（2）、标准模式的排版 和JS运作模式都是以该浏览器支持的最高标准运行。在兼容模式中，页面以宽松的向后兼容的方式显示,模拟老式浏览器的行为以防止站点无法工作。

##### 标准模式和怪异模式的来由

在HTML与CSS的标准化未完成之前，各个浏览器对于HTML和CSS的解析有各自不同的实现，而有很多旧的网页都是按照这些非标准的实现去设计的。在HTML与CSS标准确定之后，浏览器一方面要按照标准去实现对HTML与CSS的支持，另一方面又要保证对非标准的旧网页设计的后向兼容性。因此，现代的浏览器一般都有两种渲染模式：**标准模式**和**怪异模式**。在**标准模式**下，浏览器按照HTML与CSS标准对文档进行解析和渲染；而在**怪异模式**下，浏览器则按照旧有的非标准的实现方式对文档进行解析和渲染。这样的话，对于旧有的网页，浏览器启动怪异模式，就能够使得旧网页正常显示；对于新的网页，则可以启动标准模式，使得新网页能够使用HTML与CSS的标准特性。