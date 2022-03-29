# 记录一些Python不一样的地方

### 无main方法...

### 弱类型，定义变量不允许定义类型...

### 字符串可以用''或""，中间可以存在转义字符（\n等）

### 字符串format.. （确实挺好用），但是一想还是不如java和cpp直接+来的简单...

```python
//例子
s="This is {}".format('PythonFormat')

//运行结果
s为'This is PythonFormat'
```

### 字符串切片（Cpp中的substr函数）

```python
//例子
s="This is a Python Example"
a=s[0] //把字符串当数组处理
b=s[0:3]  //可以指定substr的头尾~这个挺好用
```

### 恶心的流程控制

由于没有大括号，靠缩进判断范围...

#### if语句

```python
if condition :
    express
elif condition :
    express
else :
    express
```

#### for循环

```python
//python中的for不支持定义循环次数...
s = 'ABCD'
for ch in s :
    express
```

#### while

```python
//while 还是比较良心
while condition :
    express
```

##### continue与break照旧...

### 容器

#### list

```python
//链表，用中括号括起来即可...
//类型可以不一样(谁他妈能忍)
s=[0,'1',true,3.00]
//访问跟C一样，但是可以反向越界...，同样支持赋值
s[-1]为3.00
//同样支持切片，返回一个子链表
l = s[0:2]
//添加元素(尾部)
s.append(object)
//添加元素(插入)
s.insert(index,object)
//删除(默认弹出最后一个，否则按参数弹出)
i=s.pop(index=s.length)
//二维，就是一个list的list
s=[[],[],[]]
//其他一切照旧，很抽象
```

#### turple

```python
//叫做元组...但是其实和list差不多，只是所有list元素变为const
//但是很离谱的是，turple中的list是可以添加元素的...大概list存的是头指针的原因
//定义用()
s=(1,2,3)
```

#### dict

```python
//键值对，类比于stl中的map
//定义用{}括起来每个键值对
d={
    key:value,
    key:value
    ...
}
//查询方式，d[key]即可得到value，但是不安全...
//安全一点应该使用d.get(key),如果不存在，会返回none
//添加元素可以直接赋值...d[key]=value
//删除
d.pop(key)
但是这个几把也不安全...删除一个不存在的元素直接报错...
if key in d.keys()
	d.pop(key)
这样写安全，但是确实呆滞
//多的三个函数
d.keys()  return list
d.values()  return list
d.clear()  void
```

#### set

```python
//set没什么几把用，去重用的... 就跟stl的set一样
//定义方法 s=([1,2,3]) 这个定义也真够恶心的
//读取，没什么用，直接用 value in s ,return bool
//添加, s.add(object)  s.update(list)
//删除, s.discard(value)
//其他方法
s.clear() void
s.issubset(set) bool //是否为子集
s.issuperset(set) bool //是否为超集
s.isdisjoint(set) bool //是否有交集
```

