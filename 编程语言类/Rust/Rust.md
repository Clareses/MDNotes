# Rust

## 变量与常量

### 声明

```rust
fn main(){
    let x = 10;	//声明了一个不可变的变量x
	let mut n = 10; //声明了一个可变变量n
    let const m: i32 = 16; //声明了一个常量m（常量后要跟类型）
}
```

### shadow

shadow是Rust中一个有用的特性

```rust
fn main(){
    let x = "   "; //此处x的类型为&str
    let x = x.len(); //这里再一次声明了同名变量x，类型会转成u32
}
```

如上所示，重新声明一个同名变量会销毁前一个变量

这样就可以少很多类似于 x_size = x_str.len() 等麻烦的写法了

## 数据类型

### 标量类型

- 整数 ：有符号表示为isize，如i32；无符号表示为usize，如u32
- 浮点数：表示为fsize，如f32代表32位浮点数
- 布尔 ：占用1Byte，值为true 和 false
- 字符 ：采用Unicode编码

标量的运算与其他语言一致。

### 复合类型

#### 元组

用于存储多个类型的多个值，且长度固定，一旦声明就无法改变。

##### Turple的创建与访问

```rust
fn main(){
    //创建了一个turple
    let tup = (1, "ss", 'a');
    //也可以声明类型
    let tup2:(i32, f32, &str) = (1, 3.0, "Rust");
	//tuple的访问
    println!("{}{}{}",tup.0, tup.1, tup.2);
}
```

##### Turple的解构

```rust
fn main(){
    let tup = (1,2,3);
    //let a = tup.0;
    //let b = tup.1;
    //..
    
    //这样就可以一次直接取出所有的值（解构）
    let (a,b,c) = tup;
}
```

#### 数组

##### 数组类型

- 采用[type；size]的格式来表示

- 内存被分配在栈上

- 在编译时就会检查越界（内存安全）

##### Array的创建与访问

```rust
fn main(){
   	//array的创建
    let array = [1,2,3];
    //创建带初始值的数组
    let array = [0;10];
    //创建长度为100的数组
    let array:[i32;100] = [0;100];
	//array的访问
    println!("{}",array[0]);
}
```

## 控制流

### 函数与注释

#### 函数的定义与调用

```rust
//rust中调用不分先后，实现不必一定写在调用前

fn main(){
	anther_func();
}

fn anther_func(){}

//有参数的函数必须写明参数类型
fn func_with_parameter(x:i32){}
```

#### 语句与表达式

函数体由一系列**语句与表达式构成**

Rust是**基于表达式**的语言

语句是执行一些动作的指令，如赋值等；它**不能被赋值给变量**

**表达式会计算产生一个值，可以赋值给变量**

**函数的定义也是语句**

```rust
fn main(){
    let y = {
        let x = 3;
        x + 1//这里类似于返回值，y最后等于x+1，即4
    };
    
    let z = {
      	let x = 3;
        x + 1;//这里变成了一个语句,因此z最后等于一个空turple
    };
}
```

#### 带返回值的函数

```rust
//返回值类型写参数列表后
fn func() -> i32{
    let x = 10;
    x + 1
    let y = 11;
    y + 1//一般来说，返回值是最后一条表达式的值
}

//同样也可以利用return提前返回（返回也代表直接退出整个函数，之后的语句不会再执行）
```

### if-else**表达式**

```rust
fn main(){
    //if最普通的样子
    if 1>3 {
        println!("true");
    }else {
        println!("false");
    }
    
    //最后value = 5，注意一致性，返回值类型必须相同
    let value = if true {5} else {6};
}
```

### 循环**表达式**

同样支持break与continue

#### loop

永久循环，直到break

```rust
fn main(){
    let mut cnt = 0;
    
    //运行结果 result == 20
    let result = loop{	//loop的返回值是break语句后的表达式的值
      	cnt += 1;//注意没有++了
        if cnt == 10{
            break cnt*2;	//注意这里是返回值
        }
    };
}
```

#### while

```rust
fn main(){
    while condition{
     	//大致相同   
    }
}
```

#### for

for用于遍历容器

```rust
fn main(){
    let a = [1,2,3,4,5];
    //使用iter可以以引用的方式遍历，直接a也可以，但是遍历的是元素的值而不是引用
    for element in a.iter(){
        println!("{}",element);
    }
}
```

对于其他语言中的for

```rust
fn main(){
    let a = [1,2,3,4,5];
    //（1..5）表示区间，左开右闭，[1,5)，rev表示转置
    for i in (1..5).rev(){
        println!("{}",a[i]);
    }
    //输出为 5 4 3 2 
}
```

