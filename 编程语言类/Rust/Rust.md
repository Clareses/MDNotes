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

## Rust所有权

Rust能够保证内存安全的关键就在于编译时处理了内存分配相关的操作，这与Rust中的**所有权规则**分不开关系。

#### 所有权规则

- **每个值都与一个变量绑定**，这个变量就是该值的所有者

- 每个值**同时只能有一个所有者**
- 当**所有者离开定义域后，值将自动销毁**

#### 内存与分配

Rust中的内存分配比较奇怪

对于一些简单类型，会直接分配在栈上；而对于复杂的类似于String等类型，会在堆中分配内存，并让栈中的变量指向它（这一点与其他语言大致相同）

问题出在Rust并不显式提供释放内存的函数，而是在离开作用域后自动调用Drop函数进行销毁

#### 变量与数据的交互

变量是给内存上的数据起的一个别名，在别的编程语言上也许体现的不明显，但在rust中绝对是非常重要的

- **移动（Move）**

  ```rust
  let x = 5;
  let y = x;
  //这种情况下，栈中会存上两个5
  //并且变量x与变量y都可用
  //对于栈中的数据，移动的结果就是复制一份数据，并与新变量绑定
  ```

  ```rust
  let s = String::from("hello");
  let x = s;
  //这种情况下，s是栈上的数据，但是其中包含了指向堆的指针
  //而移动后，x会复制s的所有数据（包括指针，这种复制类似于浅复制）
  //移动后，s会失效（避免double free）
  ```

- **克隆（Clone）**

  ```rust
  //但是如果真的想将堆中的数据复制一份呢？（deep copy）
  //那就需要专门实现一个接口：Clone
  //再调用该接口即可实现深层复制
  let s = String::from("hello");
  let x = s.clone();
  ```

- **Copy与Drop**

  如果一个类型，想像在栈上那样完成复制（复制后本体还能使用），就需要实现一个Copy接口。而Copy接口与Drop接口是互斥的（实现了Copy就不能实现Drop，反之也一样）

与Cpp对比后发现

- **默认的复制都是浅复制**
- 但是与Cpp对比，为了能够自动释放内存，**浅复制后本体会被弃用**
- 为了实现深复制，**也加入了类似复制构造的机制**，但是似乎是利用返回值再给变量赋值

#### 与函数相关的所有权

在rust里，函数调用时的**传参与返回**也遵循上面的一套所有权规则

对于栈数据，没有任何区别

对于堆数据，那就会经常发生移动等现象了

换言之，在栈空间里，传参数默认传的就是值

而在堆空间里，由于有move的存在，默认传的是别的编程语言里的“引用”

（Rust有自己对引用的解释，在下一节里）

考虑以下代码

```rust
fn main(){
    let s0 = String::from("hello");
    let s2 = test_str(s0);
    println!("{}",s2);
}

fn test_str(mut s1:String)->String{
    println!("{}",s1);
    s1.push_str("mstr");
    return s1;
}
```

在这里，控制权发生了3次转移

- S0接收了from函数返回的String
- S0通过函数调用将控制权交给了test_str里的S1，这里S0将失效，同时可变性变成了mut
- S1通过返回值将参数又交给了S2，S1失效
- 最后只有S2在有效的情况下离开了作用域，会调用Drop函数释放空间

> 可以发现Rust中的mut与const等确实不一样，是否mutable是可以随意修改的

#### 引用与借用

可是如果**只是想使用这个变量**，而**不希望它的所有权转移**（所有权转移，如果想转回本体身上，则还需要返回值赋值给它本身...），则可以利用Rust提供的引用语法

```rust
//不使用引用，来避免所有权转移
fn main(){
    let mut s0 = String::from("hello");
    let s0 = test_str(s0);//所有权转移后又转移回s0
    println!("{}",s0);
}

fn test_str(s1:String)->String{
    println!("{}",s1);
    return s1;
}
//这样的写法繁琐而复杂
```

Rust中的引用，更类似于指针的概念了，在参数中传递引用，类似于传递指针，自然有一定的限制（指针也有常引用指针等分类）

```rust
fn main(){
    let s0 = String::from("hello");
    test_str(&s0);//传递s0的引用
    println!("{}",s0);//s0依然可用
}

fn test_str(s1:&String){
    println!("{}",s1);//接收引用并使用而不改变所有权
}
```

引用同样有两种类型

- **mut &Type** 可变引用，可以理解为一般的指针。注意，可变引用的本体也必须可变。
- **&Type** 不可变引用，理解为const修饰的指针

```rust
fn main(){
    let mut s0 = String::from("hello");
    let immut_quote_s0 = &s0;	//创建了一个不可变引用
    let mut_quote_s0 = &mut s0; //创建了一个可变引用
    //这里的写法是有问题的，不可以同时拥有可变和不可变引用
}
//需要注意的是，引用的可变性表示引用指向的位置的可变性，而引用本身并不需要运算（不像指针那样可以++--...），如果写了一个let mut x = &mut y；会有warning提醒说x不需要mut
```

```rust
//下面的程序输出为 hello world
fn main(){
    let mut s0 = String::from("hello");
    test_quote_func(&mut s0);
    println!("{}", s0);   
}

fn test_quote_func(s: &mut String){
    s.push_str(" world");
}
```

**但是对于可变引用，有一个重要的限制！**

**即一个作用域中，对同一块数据，只能有一个可变引用！**

或者说，对同一块数据，只能同时有一个可变引用处于有效状态

这样的机制可以避免数据竞争

> **数据竞争**
>
> 满足以下三个条件即有可能发生数据竞争
>
> - 两个或多个指针访问同一个数据
> - 至少有一个指针用于写入数据
> - 没有使用任何机制来同步对数据的访问

**不可以同时拥有一个可变引用和一个不可变引用！**

#### 切片

由于字符串记录在堆上，如果每次需要使用字符串的某个子串时，都在堆上创建一个新的空间，那将十分麻烦且浪费。而根据数据只能对应一个变量的原则，又不能使用另外一个新的字符串指向同一块空间。因此，rust提供了一种**切片（slice）**机制，**使用一种特殊的类型，用以引用字符串的一个子串部分**。

```rust
//函数的功能是返回字符串的第一个单词
fn main(){
    let s = String::from("Hello world");
    let index = get_first_word_index(&s);
    println!("{}", index);
}

//注意返回值类型的写法，可以发现平时写的str常量就是这个类型
fn get_first_word_index(str: &str) -> &str{	
    let bytes = str.as_bytes();
    for (i, &item) in bytes.iter().enumerate(){
        if item == b' '{
            return &str[0..i];
        }
    }
    &str[..]
}
```

在涉及字符串处理的函数时，使用字符串切片可以使函数的泛用性更广（既可以接受字面值参数（&str），也可以接受字符串（String， 整个转成&str即可））

**除了字符串以外，很多别的类型也有切片，比如数组**
