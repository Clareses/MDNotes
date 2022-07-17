# **Flutter**笔记

## 关于StatelessWidget与StatefulWidget

- 他们都继承自Widget

- 但是实现细节有变化

- StatefulWidget中有一个setState函数，调用该函数会刷新整个StatefulWidget

	```dart
		//setState函数原型
	void setState(VoidCallback fn);
	//传入一个函数参数，会执行这里面的内容
	//但是实际上,调用时写在里面写在外面是差不多的...
	
	//这样
	void onClick() {
	    count++;
	    setState(() {});
	}
	//和这样
	void onClick() {
	    setState(() {
	        count++;
	    });
	}
	//其实是没差的
	//想想肯定没差...setState会存在这样一个参数估计只是为了
	//有些时候不用多写一个onClick这样的函数，而可以直接把setState作为一个参数
	//丢到控件的响应属性里
	```

- 至于StatelessWidget...那就简单咯~

- 此外，一个小坑...似乎好多控件都需要一个 "MediaQuery Widget" 作为父布局...这里的MediaQuery Widget可以用materialApp作为父布局~ 贴一下报错

	![image-20220316154022535](F:\MDNotes\_Images\image-20220316154022535.png)

## 关于布局

### 布局的原理

- 依赖于控件
- 控件分为多child与单child
- 单child一般不提供很丰富的布局方式，但是提供很丰富的装饰功能
- 多child控件会提供很丰富的布局功能，但是装饰功能很单一

#### 约束的传递

不同控件对子控件会传递不同效果的约束。

有一些是紧约束，即将子控件控制成和自己一样大

还有一部分松约束，会给子控件提供一个范围，允许子控件在范围内自行调整

| Widget                            | 说明             | 用途                                                         |
| --------------------------------- | ---------------- | ------------------------------------------------------------ |
| **Leaf**RenderObjectWidget        | 非容器类组件基类 | Widget树的叶子节点，用于没有子节点的widget，通常基础组件都属于这一类，如Image。 |
| **SingleChild**RenderObjectWidget | 单子组件基类     | 包含一个子Widget，如：ConstrainedBox、DecoratedBox等         |
| **MultiChild**RenderObjectWidget  | 多子组件基类     | 包含多个子Widget，一般都有一个children参数，接受一个Widget数组。如Row、Column、Stack等 |

约束会从父布局传递下来，再递归地返回到根节点绘图

### 尺寸约束类容器 

#### 盒式约束

- 这种约束以BoxConstraints对象作为载体传递对子控件的约束信息

- ### **BoxConstraints** 

	是盒模型布局过程中父渲染对象传递给子渲染对象的**约束信息**，包含最大宽高信息，子组件大小需要在约束的范围内，BoxConstraints 默认的构造函数如下：

	```dart
	const BoxConstraints({
	  this.minWidth = 0.0, //最小宽度
	  this.maxWidth = double.infinity, //最大宽度
	  this.minHeight = 0.0, //最小高度
	  this.maxHeight = double.infinity //最大高度
	})
	```

	它包含 4 个属性，BoxConstraints还定义了一些便捷的构造函数，用于快速生成特定限制规则的BoxConstraints，如`BoxConstraints.tight(Size size)`，它可以生成固定宽高的限制；`BoxConstraints.expand()`可以生成一个尽可能大的用以填充另一个容器的BoxConstraints。除此之外还有一些其它的便捷函数，读者可以查看类定义。另外我们会在后面深入介绍布局原理时还会讨论 Constraints，在这里，读者只需知道父级组件是通过 BoxConstraints 来描述对子组件可用的空间范围即可。

	> 约定：为了描述方便，如果我们说一个组件不约束其子组件或者取消对子组件约束时是指对子组件约束的最大宽高为无限大，而最小宽高为0，相当于子组件完全可以自己根据需要的空间来确定自己的大小。

	

- ### **ConstrainedBox**

	`ConstrainedBox`用于对子组件添加额外的约束。例如，如果你想让子组件的最小高度是80像素，你可以使用`const BoxConstraints(minHeight: 80.0)`作为子组件的约束。



- ### **SizedBox**

	`SizedBox`是ConstraintedBox的特殊定制，相当于tight约束的约束盒子



- ### **UnconstraintedBox**

	可以去除父布局的约束



- ### **其它约束类容器**

	除了上面介绍的这些常用的尺寸限制类容器外，还有一些其他的尺寸限制类容器，比如`AspectRatio`，它可以指定子组件的长宽比、`LimitedBox` 用于指定最大宽高、`FractionallySizedBox` 可以根据父容器宽高的百分比来设置子组件宽高等，由于这些容器使用起来都比较简单，我们便不再赘述，读者可以自行了解。

### 线性布局

#### Row

### Row

Row可以沿水平方向排列其子widget。定义如下：

```dart
Row({
  ...  
  TextDirection textDirection,    
  MainAxisSize mainAxisSize = MainAxisSize.max,    
  MainAxisAlignment mainAxisAlignment = MainAxisAlignment.start,
  VerticalDirection verticalDirection = VerticalDirection.down,  
  CrossAxisAlignment crossAxisAlignment = CrossAxisAlignment.center,
  List<Widget> children = const <Widget>[],
})
```

- `textDirection`：表示水平方向子组件的布局顺序(是从左往右还是从右往左)，默认为系统当前Locale环境的文本方向(如中文、英语都是从左往右，而阿拉伯语是从右往左)。
- `mainAxisSize`：表示`Row`在主轴(水平)方向占用的空间，默认是`MainAxisSize.max`，表示尽可能多的占用水平方向的空间，此时无论子 widgets 实际占用多少水平空间，`Row`的宽度始终等于水平方向的最大宽度；而`MainAxisSize.min`表示尽可能少的占用水平空间，当子组件没有占满水平剩余空间，则`Row`的实际宽度等于所有子组件占用的的水平空间；
- `mainAxisAlignment`：表示子组件在`Row`所占用的水平空间内对齐方式，如果`mainAxisSize`值为`MainAxisSize.min`，则此属性无意义，因为子组件的宽度等于`Row`的宽度。只有当`mainAxisSize`的值为`MainAxisSize.max`时，此属性才有意义，`MainAxisAlignment.start`表示沿`textDirection`的初始方向对齐，如`textDirection`取值为`TextDirection.ltr`时，则`MainAxisAlignment.start`表示左对齐，`textDirection`取值为`TextDirection.rtl`时表示从右对齐。而`MainAxisAlignment.end`和`MainAxisAlignment.start`正好相反；`MainAxisAlignment.center`表示居中对齐。读者可以这么理解：`textDirection`是`mainAxisAlignment`的参考系。
- `verticalDirection`：表示`Row`纵轴（垂直）的对齐方向，默认是`VerticalDirection.down`，表示从上到下。
- `crossAxisAlignment`：表示子组件在纵轴方向的对齐方式，`Row`的高度等于子组件中最高的子元素高度，它的取值和`MainAxisAlignment`一样(包含`start`、`end`、 `center`三个值)，不同的是`crossAxisAlignment`的参考系是`verticalDirection`，即`verticalDirection`值为`VerticalDirection.down`时`crossAxisAlignment.start`指顶部对齐，`verticalDirection`值为`VerticalDirection.up`时，`crossAxisAlignment.start`指底部对齐；而`crossAxisAlignment.end`和`crossAxisAlignment.start`正好相反；
- `children` ：子组件数组。

### Column

`Column`可以在垂直方向排列其子组件。参数和`Row`一样，不同的是布局方向为垂直，主轴纵轴正好相反，读者可类比`Row`来理解