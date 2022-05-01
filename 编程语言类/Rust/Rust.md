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

## 结构体

### 定义与使用

```rust
//定义，可以看出与其他编程语言大差不差...
struct my_struct{
    i32_member:i32,
    String_member:String,
    f32_member:f32,
    bool_member:bool,
}

fn main(){
    //构造，rust中取消了构造函数，处理起来和C的直接赋值差不多
    let m_struct = my_struct{
      	i32_member:523,
        String_member:String::from("this is a String"),
        f32_member:77.3,
        bool_member:true,
    };
    
    //对于复制构造，rust提供了一种“更新”语法
    let m2_struct = my_struct{
        //更新的值
        i32_member:224,
        //其他的采用与m_struct一样的语法
        ..m_struct
    	//需要注意的是，没有实现copy traits的成员
        //（如String_member）会发生所有权转移
        //（在这之后访问m_struct.String_member会报错）
    };
    
    //数据成员的访问，与其他语言一样
    println!("{}",m2_struct.i32_member);
}
```

> 结构体中的成员可变性取决于结构体变量在绑定时的可变性

### Example

```rust
//又是典中典的计算长方形面积
struct Square{
    x: i32,
    y: i32,
}

fn get_area(s:&Square)->i32{
    s.x*s.y
}

fn main(){
    let my_s = Square{
        x:10,
        y:10,
    };
    println!("The Area is {}",get_area(&my_s));
}
```

### 为struct实现方法

rust里也提供了将struct与函数绑定的机制，绑定的函数被叫做方法

```rust
//像这样，就为Rectangle类提供了

#[derive(Debug)]
struct Rectangle{
    width:u32,
    length:u32,
}

impl Rectangle {
    
    //模拟了一个构造函数，用结构体名::方法名调用
    fn build(width:u32,length:u32) -> Rectangle{
        Rectangle { width, length }
    }

    //参数列表里的第一个参数为&self的可以直接用对象调用
    fn get_area(&self)->u32{
        self.width*self.length
    }
}

fn main(){
    let rec = Rectangle::build(10, 20);
    println!("{}",rec.get_area());
}
```

## 枚举

Rust里的枚举与C语言中的枚举有一定的差别，更类似于C中联合体与枚举体的结结合

### 定义与使用

```rust
enum WebEvent{
    //可以像一般的枚举一样使用
    PageLoad,
    PageUnload,
    //但是还有附加的功能，那就是像联合体一样绑定类型
    KeyPress(char),
    Click(i32,i32),//绑定类型可以用元组
    Move{begin:i32,end:i32},//也可以用结构体
}

//枚举体的使用
fn inspect(event:WebEvent){
    match event{
        WebEvent::PageLoad => print!("PageLoad"),
        WebEvent::Click(x,y) => print!("{}{}",x,y),
        default => (),
    }
}

//可以看见，rust中的枚举体可以轻松地模拟C中的枚举体
//此外，还可以让枚举体附带一些数据，更具有灵活性
fn main(){
    let event = WebEvent::Click(10,12);
    inspect(event);
}
```

### 枚举方法

枚举体作为一个类型，同样可以有自己的方法，同样使用impl关键字

```rust
//将上面一个写法修改为枚举体内方法

enum WebEvent{
    PageLoad,
    PageUnload,
    KeyPress(char),
    Click(i32,i32),
    Move{begin:i32,end:i32},
}


impl WebEvent {
    fn inspect(&self){
        match self {
            WebEvent::PageLoad => println!("PageLoad"),
            WebEvent::PageUnload => println!("PageUnload"),
            WebEvent::Click(x, y) => 
            	println!("Click ({},{})",x,y),
            default => (),
        }
    }
}

fn main(){
    let event = WebEvent::Click(10, 10);
    event.inspect();
}
```

### NULL的改进——Option枚举

在Cpp或其他编程语言中，经常会出现这样一个错误

```cpp
String a = NULL;
String b = "aa";
b = a + b;	//这里会带来错误（NULL是空值，不能运算）
```

而Rust采用将NULL与一般类型区分开的方式

```rust
//这是Rust标准库中提供的支持
enum Option<T>{
    //some成员代表不是空值
    some(T),
    None,
}

impl Option<T>{
    //解封装
    fn unwrap(&self)->T;
    //是否有效
    fn is_some(&self)->bool;
    //是否为空
    fn is_none(&self)->bool;
}
```

```rust
fn main(){
    let a = some(10);
    let b: Option<i32> = None;//这样就可以声明一个空的i32类型
    //需要注意的是，Option<T>与T并不是同一个类型
    //因此在运算前必须要进行转换，这样对于空值的处理就更完整了
    let c = 3;
    let d = a + c;//这里会报错，因此a和c不是同一个类型，需要转化
    let d = a.unwrap() + c;//这就可以了
}
```

这种设计强制开发者无法忽视数据为空时的情况...因为必须做一个转换

### Match表达式

```rust
match expression {
    pattern1 => expression,
    ....
    default => expression,
}
//其中每个pattern对应的expression就是match表达式的值
```

```rust
fn main(){
    let a = 10;
    let b = match a {
        10 => true,
        default => {
            println!("值不对！");
            false
        },
    };
    println!("{}",b);
}
```

特别地，对于绑定了数据的枚举体，match可以从中提取出对应的值

```rust
enum HttpRequest{
    Get(String),
    Post(String),
}

impl HttpRequest{
    fn parse(&self){
        match self{
            //Get和Post附加的值被提取到s中了
            HttpRequest::Get(s) => println!("{}",s),
            HttpRequest::Post(s) => println!("{}",s),
        }
    }
}

fn main(){
    let request = HttpRequest::Get(
        String::from("HTTP1.1 / GET")
    );
    request.parse();
}
```

match必须穷举所有的可能性

### if let

if let用于仅匹配一种情况时的处理

```rust
if let pattern = expression {
    expression
}else{
    expression
}
```

```rust
enum HttpRequest{
    Get(String),
    Post(String),
}

fn main(){
    let request = HttpRequest::Get(
        String::from("HTTP1.0 / GET"));
    //这里只匹配了一种情况，但是却需要多写一个_
    // match request{
    //     HttpRequest::Get(&s) => println!("{}",s),
    //     _ => (),
    // }
    //改用if let
    if let HttpRequest::Get(s) = request {
        println!("{}",s)
    }
	//注意和if的不同在于，它可以模式匹配，提取出enum的值
}
```

## Rust 组织管理

任何一门编程语言如果不能组织代码都是难以深入的，几乎没有一个软件产品是由一个源文件编译而成的。

对于一个工程来讲，**组织代码**是十分重要的。

Rust 中有三个重要的组织概念：**箱、包、模块**。

### 箱（Crate）

**"箱"是二进制程序文件或者库文件**，**存在于"包"中**。

**"箱"是树状结构的**，它的树根是编译器开始运行时编译的源文件所编译的程序。

> 注意："二进制程序文件"不一定是"二进制可执行文件"，只能确定**是包含目标机器语言的文件，文件格式随编译环境的不同而不同**。

### 包（Package）

当我们使用 Cargo 执行 new 命令创建 Rust 工程时，工程目录下会建立一个 Cargo.toml 文件。**工程的实质就是一个包**，包必须**由一个 Cargo.toml 文件来管理**，该文件**描述了包的基本信息以及依赖项**。

**一个包最多包含一个库"箱"，可以包含任意数量的二进制"箱"，但是至少包含一个"箱"**（不管是库还是二进制"箱"）。

当使用 cargo new 命令创建完包之后，**src 目录下会生成一个 main.rs 源文件**，**Cargo 默认这个文件为二进制箱的根**，**编译之后的二进制箱将与包名相同**。

### 模块（Module）

对于一个软件工程来说，我们往往按照所使用的编程语言的组织规范来进行组织，组织模块的主要结构往往是树。Java 组织功能模块的主要单位是类，而 JavaScript 组织模块的主要方式是 function。

这些先进的语言的**组织单位可以层层包含，就像文件系统的目录结构一样**。**Rust 中的组织单位是模块**（Module）。

```rust
mod nation {
    mod government {
        fn govern() {}
    }
    mod congress {
        fn legislate() {}
    }
    mod court {
        fn judicial() {}
    }
}
```

这是一段描述法治国家的程序：国家（nation）包括政府（government）、议会（congress）和法院（court），分别有行政、立法和司法的功能。我们可以把它转换成树状结构：

```
nation
 ├── government
 │ └── govern
 ├── congress
 │ └── legislate
 └── court
   └── judicial
```

在文件系统中，目录结构往往以斜杠在路径字符串中表示对象的位置，**Rust 中的路径分隔符是 ::** 。

**路径分为绝对路径和相对路径。绝对路径从 crate 关键字开始描述。相对路径从 self 或 super 关键字或一个标识符开始描述。**例如：

```
crate::nation::government::govern();
```

是描述 govern 函数的绝对路径，相对路径可以表示为：

```
nation::government::govern();
```

现在你可以尝试在一个源程序里定义类似的模块结构并在主函数中使用路径。

如果你这样做，你一定会发现它不正确的地方：government 模块和其中的函数都是私有（private）的，你不被允许访问它们。

------

### 访问权限

**Rust 中有两种简单的访问权：公共（public）和私有（private）**。

**默认情况下，如果不加修饰符，模块中的成员访问权将是私有的**。

如果**想使用公共权限，需要使用 pub 关键字**。

**对于私有的模块，只有在与其平级的位置或下级的位置才能访问，不能从其外部访问**。

#### 实例

```rust
mod nation {
   pub mod government {
     pub fn govern() {}
   }
 
   mod congress {
     pub fn legislate() {}
   }
   
   mod court {
     fn judicial() {
       super::congress::legislate();
     }
   }
 }
 
 fn main() {
   nation::government::govern();
 }
```

这段程序是能通过编译的。请注意观察 court 模块中 super 的访问方法。

如果模块中定义了结构体，结构体除了其本身是私有的以外，其字段也默认是私有的。所以如果想使用模块中的结构体以及其字段，需要 pub 声明：

#### 实例

```rust
mod back_of_house {
   pub struct Breakfast {
     pub toast: String,
     seasonal_fruit: String,
   }
 
   impl Breakfast {
     pub fn summer(toast: &str) -> Breakfast {
       Breakfast {
         toast: String::from(toast),
         seasonal_fruit: String::from("peaches"),
       }
     }
   }
 }
 pub fn eat_at_restaurant() {
   let mut meal = back_of_house::Breakfast::summer("Rye");
   meal.toast = String::from("Wheat");
   println!("I'd like {} toast please", meal.toast);
 }
 fn main() {
   eat_at_restaurant()
 }
```

运行结果：

```
I'd like Wheat toast please
```

枚举类枚举项可以内含字段，但不具备类似的性质:

#### 实例

```rust
mod SomeModule {
   pub enum Person {
     King {
       name: String
     },
     Quene
   }
 }
 
 fn main() {
   let person = SomeModule::Person::King{
     name: String::from("Blue")
   };
   match person {
     SomeModule::Person::King {name} => {
       println!("{}", name);
     }
     _ => {}
   }
 }

```

运行结果：

```
Blue
```

------

### 难以发现的模块

使用过 Java 的开发者在编程时往往非常讨厌最外层的 class 块——它的名字与文件名一模一样，因为它就表示文件容器，尽管它很繁琐但我们不得不写一遍来强调"这个类是文件所包含的类"。

不过这样有一些好处：起码它让开发者明明白白的意识到了类包装的存在，而且可以明确的描述类的继承关系。

在 Rust 中，模块就像是 Java 中的类包装，但是文件一开头就可以写一个主函数，这该如何解释呢？

每一个 Rust 文件的内容都是一个"难以发现"的模块。

让我们用两个文件来揭示这一点：

#### main.rs 文件

```rust
*// main.rs*
 mod second_module;
 
 fn main() {
   println!("This is the main module.");
   println!("{}", second_module::message());
 }
```

#### second_module.rs 文件

```rust
*// second_module.rs*
 pub fn message() -> String {
   String::from("This is the 2nd module.")
 }
```

运行结果：

```
This is the main module.
This is the 2nd module.
```

------

### use 关键字

use 关键字能够将模块标识符引入当前作用域：

#### 实例

```rust
mod nation {
   pub mod government {
     pub fn govern() {}
   }
 }
 
 use crate::nation::government::govern;
 
 fn main() {
   govern();
 }
```

这段程序能够通过编译。

因为 use 关键字把 govern 标识符导入到了当前的模块下，可以直接使用。

这样就解决了局部模块路径过长的问题。

当然，有些情况下存在两个相同的名称，且同样需要导入，我们可以使用 as 关键字为标识符添加别名：

#### 实例

```rust
mod nation {
   pub mod government {
     pub fn govern() {}
   }
   pub fn govern() {}
 }
   
 use crate::nation::government::govern;
 use crate::nation::govern as nation_govern;
 
 fn main() {
   nation_govern();
   govern();
 }
```

这里有两个 govern 函数，一个是 nation 下的，一个是 government 下的，我们用 as 将 nation 下的取别名 nation_govern。两个名称可以同时使用。

use 关键字可以与 pub 关键字配合使用：

#### 实例

```rust
mod nation {
   pub mod government {
     pub fn govern() {}
   }
   pub use government::govern;
 }
 
 fn main() {
   nation::govern();
 }
```



------

### 引用标准库

Rust 官方标准库字典：https://doc.rust-lang.org/stable/std/all.html

在学习了本章的概念之后，我们可以轻松的导入系统库来方便的开发程序了：

#### 实例

```rust
use std::f64::consts::PI;
 
 fn main() {
   println!("{}", (PI / 2.0).sin());
 }
```

运行结果：所有的系统库模块都是被默认导入的，所以在使用的时候只需要使用 use 关键字简化路径就可以方便的使用了。
