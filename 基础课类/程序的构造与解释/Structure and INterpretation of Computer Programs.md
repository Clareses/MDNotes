# 计算机程序的构造与解释

## 过程抽象

### 程序设计的基本元素

​ 不论哪种编程语言，都需要为使用者提供以下三种最基本的机制：

- **基本表达形式**，用以表述出编程语言所关心的最简单的个体，如数据类型、基本过程等
- **组合的方法**，由此将简单的元素构造产生更复杂的元素，如 struct in C
- **抽象的方法**，提供为复合对象命名的机制，使得这些对象（不论是数据或是过程）可以用以操作（复用）

#### 表达式

​ 在Lisp中，**输入的数是基本表达式之一**（输入的数也是表达式，它表示以10作为基数的数）；此外，**基本过程也是一类基本表达式**。

​ 过程，即应用于数据上的某些操作（其本身也是一种数据），在Lisp中，使用过程的方法如下：

```scheme
; + - * / 都是基本过程
(+ 1 2)
; output = 3
```

​ 即 将过程与参数 用括号括起来，表示将过程应用于这些数据上；这一操作，形成了一个新的表达式，即 **复合表达式**。

​ 以上操作已经说明了 Lisp 提供的前两个机制（基本表达式与组合的方法）

#### 命名（定义）与环境

​ Lisp 也提供了 **通过名字去使用计算对象的方式**，这里的名字也被称为 **变量**，**变量的值就是它所对应的那个计算对象**（计算对象可以是任意一个基本表达式或是复合表达式）

```scheme
(define a 1)
(+ a 2) = 3    ; 可以与数绑定

(define plus +)
(plus a 2) = 3  ; 可以与一个基本过程绑定

(define plus2 (plus a 2))
(plus2) = 3   ; 可以与一个复合过程绑定

(define a define) ; error,说明define并不是一个表达式
```

​ 这样就可以达到以一个简单名字去**引用组合运算的结果**（注意引用的对象是结果，即其中的所有值都已经确定，并不是过程）

​ 解释器在存储中维护这些命名的关系而营造出的上下文就是**环境**，组合式的求值依赖于它

> **语法糖**
>
> ​ 类似于 define 这类既不是基本表达式，也不是一个基本过程（因为 define 不能进行表达式求值。此外，在解释器中类似于 (define a define) 的操作将引发错误），这一类型的固定语法是为了“方便”而产生的，因此也被称为“语法的糖衣”。
>
> ​ 与其他编程语言相比，Lisp只有很少的固定语法，这归因于它的灵活性（在后续篇幅中介绍）

#### 复合过程

​ 表达式定义只能完成一部分抽象工作，因此需要一种更加强大的机制来提供抽象，即**过程定义**

```scheme
; 定义一个 平方 过程
(define (square x) (* x x))

; 定义一个 四次方 过程
(define (four_power x) (square (square x)))
```

​ 定义一个过程，就是使用基本或是复合过程进行组合，形成一个复合过程。此前已经见过基本运算过程。

#### 条件表达式与谓词

```scheme
(cond  (<p1> <e1>)
       (<p2> <e2>)
    .....)

; 关于 cond 的语法糖
(if <p> <e1> <e2>)

```

​ 该表达式的值为p成立的e的值，如果没有找到成立的p，则cond的值为未定义

### 过程

#### 什么是过程？

​ 所谓过程就是一种**模式**，描述了一个**计算过程的局部演化方式**，描述了计算过程中的每个步骤是如何基于前面的步骤建立起来的。接下来考虑由一些简单过程产生的计算过程的**形状**。

#### 线性递归和迭代

​ 考虑一个**阶乘函数**：

```lisp
(define (fac n)
    (if (= n 1)
        1
        (* n (fac (- n 1)))))
; 这个递归函数很容易理解，那么它将产生什么样的形状？
; fac(n)
; fac(n - 1) * n
; fac(n - 2) * (n - 1) * n
; fac(n - 3) * (n - 2) * (n - 1) * n
; ....
; fac(1) * 2 * 3 ..... (n - 1) * n
; 1 * 2 * 3 ..... (n - 1) * n
; 2 * 3 ... (n - 1) * n

; 同样，可以将它改写为迭代的方式
(define (fac n)
    (define (iter res cur)
        (if (= cur n)
            res
            (iter (* res cur) (- cur 1))))
    (iter 1 1)
)
; 产生的计算过程如下
; fac(6)
; iter(1 1)
; iter(2 2)
; iter(6 3)
; iter(24 4)
; iter(70 5)
; iter(420 6)
```

​ 查看第一个计算过程，代换模型展示出一个**逐步展开再收缩的计算过程**，构造出一个**推迟进行的操作所形成的链条**，这类计算过程被称为**递归计算过程**

​ 第二个计算过程，利用两个**状态变量**描述了**过程变化的所需的所有信息**，这类过程被称为**线性迭代过程**

> **递归过程与递归计算过程**
>
> ​ 递归过程并不一定就是递归计算过程，递归计算过程和迭代计算过程的区别主要在于是怎样描述状态的转移的。
>
> ​ 如上面的第二个过程，它使用了递归过程，但是在递归过程的参数中记录了计算过程的状态变化，因此它是一个迭代计算过程。

#### 树形递归

​ 考虑下面这个过程

```lisp
; 求第n个斐波那契数，fib(0) = 0, fib(1) = 1, fib(n) = fib(n - 1) + fib(n - 2)
(define (fib  n)
    (cond ((= n 0) 0)
        ((= n 1) 1)
        (else (+ (fib (- n 1) (fib (- n 2)))))))
; 根据代换模型，可以清晰看出一个树形的计算过程
; 当然...也不是不可以将它改为迭代计算过程（因为它的状态比较简单）
(define (fib n)
    (define (iter res cur pre m)
        (if (= m n)
            res
            (iter (+ cur pre) res cur (+ m 1)))))
```

### 高阶过程

​ 以上已经定义了过程**是一类描述对数的复合操作而又不依赖于具体的数的抽象**，但是限制参数的类型为数严重限制了抽象能力。我们**需要构造出以过程为参数、或以过程为返回值的过程**。这类操作过程的过程被成为**高阶过程**，是一种更加强有力的抽象机制。

#### 过程作参数

```lisp
; 考虑以下两个过程
; 1. calc the sum from a to b
(define (sum-integer a b)
    (if (> a b)
        0
        (+ a (sum-integer (+ a 1) b))))
; or iteration version
(define (sum-integer a b)
    (define (iter res cur)
        (if (> cur b)
            res
            (iter (+ res cur) (+ cur 1))))
    (iter 0 a))

; 2. calc sum of cubes from a to b
(define (sum-cubes a b)
    (if (> a b)
        0
        (+ (cube a) (sum-cube (+ a 1) b))))

; 不难发现其中的pattern
(define (<name> a b)
    (if (> a b)
        0
        (+ (<term> a) (<name> (<next> a) b))))

; 抽象出其中的 term 和 next, 可以得到这样一个函数
(define (sum a b term next)
    (if (> a b)
        0
        (+ (term a) (sum (next a) b))))

; 以上两个函数就可以写为
(define (sum-integer a b)
    (define (self a) a)
    (define (next a) (+ a 1))
    (sum a b self next))

(define (sum-cubes a b)
    (define (cube a) (* a a a))
    (define (next a) (+ a 1))
    (sum a b cube next))

; 甚至可以把sum也抽象出来，变成"将某一区间内的数据按某种方式归约"
(define (reduce op from to step term)
    (if (> from to)
        0
        (op (term a) (reduce op (step a) b term next))))
```

#### lambda expression

##### 快速构建过程

​ 在使用函数作为参数时，总需要define才可以传递参数。但是在Lisp中，函数作为一等数据，与数有着同样的地位，因此需要一种方式快速构造临时过程的方法（就像使用数一样）。

```lisp
; lambda expression 的格式
(lambda (args) (expression))

; 案例： 使用 lambda 构造求 a ～ b 之和的过程
(define (sum-integer a b)
    (reduce + a b 
            (lambda (x) x)
            (lambda (x) (+ x 1))))
```

##### 构建局部变量

```lisp
; 在其他语言中，可以很简单地创建变量，在lisp中并不提供这一功能，但是使用lambda也可以轻松实现
void add(int a, int b, int c) {
 int t = a + b;
 return k * c + k * b;
}

; 可以用lambda这样实现
(define (test a b c)
  ((lambda (k) (+ (* k c) (* k c)))
   (+ a b)))
; 通过传参给lambda完成了对变量k的赋值

; lisp 提供了语法糖，使用let语句就可以完成这样的操作
(let ((arg1 val1)
      (arg2 val2)
      (arg3 val3))
     (body))
; 等价于
(lambda 
 (arg1 arg2 arg3) 
  (body) 
  (val1 val2 val3))
```

#### 过程作返回值

​ 考虑上面过程作为参数所产生的抽象，可以发现，从 reduce -> sum -> sum-integer 的过程实际上是将参数不断填满的过程；并且其中的每一步都需要另外定义函数。这同样不适用于一些仅使用一次的函数。但是如果换一种抽象方式，将 reduce 改为返回一个过程，那将大大改善这种情况。

```lisp
(define (reduce op next term)
  (lambda (from to)
    (define (iter cur res)
      (if (> cur to)
        res
        (iter (next cur) (op (term cur) res))))
    (iter (next from) from)))

(display ((reduce + (lambda (x) (+ 1 x)) (lambda (x) x)) 1 10))
```

> **函数式编程**
>
> ​ 函数式编程（Functional Programing），是一种以函数为核心的编程思想。通过将函数的地位提升至操作数这一级别，使得函数不仅可以作为参数传递、作为返回值返回，还可以像数一样根据某些规则进行复合运算。
>
> ​ 函数式编程带来了相比面向过程的方式更加强大的抽象能力，使得代码复用率提高，灵活性得到提升。

#### 一些例子

```lisp
; 实现一个double函数，它接收一个 一参数 函数作为参数，返回执行效果相当于执行两次该函数的函数
(define (double f) (lambda (x) (f (f x))))

; 实现一个 repeat 函数， 接收 一个一参数函数、 一个整数n 作为参数，返回执行效果相当于执行n次该函数的函数
(define (repeat f n) 
  (if (= n 1) f (lambda (x) (f ((repeat f (- n 1)) x)))))

; 实现一个 compose 函数， 接收 两个一参函数， 返回复合函数
(define (compose f g) (lambda (x) (f (g x))))
```

## 数据抽象

### 什么是数据抽象

​ 数据抽象的基本思想，是**设法构造出一些使用复合数据对象的程序**，**使它们就像是在抽象数据上操作一样**。在数据抽象和操作数据数据抽象两个界面之间，是一组被**称为 选择函数 和 构造函数** 的过程。

> ​ 简而言之，其他语言中对结构体的访问在Lisp中可以通过使用函数返回值的方式模拟。

### 序对

```lisp
; 为了实现组合数据，Lisp提供了cons语法
(cons 1 2)
; 返回复合数据类型，这种数据类型可以通过 car 和 cdr 进行访问
(car (cons 1 2)) -> 1
(cdr (cons 1 2)) -> 2
; 虽然lisp内置提供了cons,但事实上完全可以自己实现
(define (cons x y) (lambda (z) (if (= 0 z) x y)))
(define (car c) (c 0))
(define (cdr c) (c 1))
```

### 实例-有理数

```lisp
; 构造函数
(define (make-rat num denom) (cons num denom))

; 选择函数
(define (get-num rat) (car rat))
(define (get-denom rat) (cdr rat))

; 使用rat的函数
(define (print-rat rat)
  (newline)
  (display (get-num rat))
  (display "/")
  (display (get-denom rat)))

(define (add-rat ra rb) (cons (+ (get-num ra) (get-num rb)) (+ (get-denom ra) (get-denom rb))))
```

​ 从数据的角度看，有理数的抽象可以分为三块： **问题域中的有理数（用户使用的有理数）， 作为分子、分母的有理数（由构造函数和选择函数使用），作为序对的有理数**

​ 三层之间**使用对应的函数隔离**开来，每一层次中的过程构成了所定义的**抽象屏障**的界面，通过抽象屏障联系起系统之间的不同层次。

> **数据的本质**
>
> ​ 在函数式编程中，数据并不再是独立的一部分，而**被定义为一组适当的选择函数和构造函数**（可以通过 cons、car、cdr 的简单实现看出）

### 层次性数据和闭包性质

#### 闭包

​ Lisp允许建立元素本身也是序对的序对，这就是 cons 的**闭包性质**，即通过它组合起数据对象得到的结果还可以通过同样的操作进行组合

​ 在本节，我们着手研究复合数据的闭包所引出的问题和技术，包括用序对表示表和树，以及一种能以很生动的形式表示闭包的图形语言

#### 序列

##### **表示**

​ 如何在Lisp中表示一个序列？在C语言中，可以通过指针来构建链表；在Lisp中甚至要更简单——将cons的第二个元素设置为下一个表项cons即可。

```scheme
; 通过闭包性质可以很容易地构建出一个表
(cons 1 (cons 2 (cons 3 (cons 4 (cons 5 nil)))))
```

##### **操作**

```scheme
; constructor
(define (make-list k v) 
  (define (iter res cur)
    (if (= cur k)
      res
      (cons v res)))
  (iter 0 (cons v null)))

; insert at begin
(define (push-front l v) (cons v l))

; insert at end
(define (push-back l v)
  (if (null? l)
    (cons v null)
    (cons (car l) (push-back (cdr l) v))))

; get length of list
(define (get-length l) 
  (if (pair? l)
    (+ (get-length (cdr l)) 1) 0))

; get the last pair of list
(define (last-pair l)
  (if (null? (cdr l))
    l (last-pair (cdr l))))

; reverse the list
(define (reverse l) 
  (define (iter res remain)
    (if (null? remain)
      res
      (iter (push-front (car remain) res) (cdr remain))))
  (iter '() l))
```

> **反转链表**
>
> ​ 反转链表的思想可能会有些难理解，但事实上并不难
>
> ​ 从第一位开始取，并不断对一个空表（res）进行头插，直到原表的尽头（remain == null），此时res逆序存储了链表的值

##### **对表的映射**

​ 函数式常见的操作数据结构的方式中就有 map，即接收一个函数作为参数，对表中的每一个元素运用该函数，并返回作用后的新表

```scheme
(define (map l f)
  (if (null? l)
    null (cons (f (car l)) (map (cdr l) f))))

(define (for-each l f)
  (define (iter)
    (if (not (null? l))
      (f (car l)) null))
  (iter))
```

#### 约定界面

##### **概念**

```scheme
; 计算一棵树的所有奇叶子的平方和
(define (sum-odd-squares tree)
  (cond ((null? tree) 0)
        ((not (pair? tree))
         (if (odd? tree) (sqr tree) 0))
        (else (+ (sum-odd-squares (car tree)) 
                 (sum-odd-squares (cdr tree))))))

; 计算第n个斐波那契数之前的所有偶斐波那契数，并输出表
(define (fib n)
  (cond ((= n 0) 0)
        ((= n 1) 1)
        (else (+ (fib (- n 1)) (fib (- n 2))))))

(define (even-fibs n)
  (define (iter res i)
    (let ((fibi (fib i)))
      (if (zero? i) 
        res
        (iter (if (odd? fibi) 
                res 
                (cons fibi res)) (- i 1)))))
  (iter '() n))
```

​ 观察这两个结构非常不同的算法，写出计算过程，会发现一些相似性...

​ 对第一个算法：

- **枚举**出一棵树的所有树叶
- **过滤**，选出其中的奇数
- 对选出的每个数进行平方**计算**
- 用 + **累积**得到的结果，从 0 开始

​ 对第二个算法

- **枚举**从 0 到 n 的所有正整数
- 对每个数**计算**斐波那契数
- **过滤**，选出其中的偶数
- 用 cons **累积**结果，从空表开始

​ 将这类对序列的操作抽象出来，即可得到**约定界面**，在进行类似的算法时，只需要提供对应的函数作为参数即可

> **与面向对象对比**
>
> ​ 面向对象同样可以实现这一操作，但是是通过继承、多态和泛型来实现的：子类继承算法父类，再通过重写父类的虚函数，从而为父类的入口算法提供完整的上下文，从而完成运算。

##### **实现**

```scheme
;累积
(define (accumulate op initial sequence)
  (define (iter res seq)
    (if (null? seq)
      res
      (iter (op (car seq) res) (cdr seq))))
  (iter initial sequence))

; 过滤
(define (filter predicate sequence) 
  (if (null? sequence)
    null
    (if (predicate (car sequence))
      (filter predicate sequence)
      (cons (car sequence) (filter predicate sequence)))))

; 映射计算
(define (map func seq)
  (if (null? seq)
    null (cons (func (car seq)) (map (cdr seq) func))))

; 枚举
(define (enumerate-nums from to)
  (defineV (iter res i)
    (if (= i from)
      res
      (iter (cons i (- i 1)))))
  (iter null to))

; 最终结果
(define (even-fibs n)
  (accumulate cons '()
              (filter even?
                      (map fib
                           (enumerate-nums 0 n )))))

; 通过这种方式，以序列作为数据流动的介质，即可实现抽象
```

> **Lisp对这类函数的支持**
>
> accumulate类：
>
> - foldl，向左累积
> - foldr，向右累积
>
> fliter类：
>
> - filter，过滤器
> - filter-not，反向过滤器
> - filter-map，过滤并映射操作
>
> map类：
>
> - map，映射器

#### 嵌套映射

​ 考虑以下一个需求，需要求出 任意 0 < i < j < n

```scheme
(define (enumerate from to) 
  (define (iter res i)
    (if (< i from)
      res
      (iter (cons i res) (- i 1))))
  (iter '() to))

(define (get-pair n)
  (foldr append null
         (map (lambda (i)
                (map (lambda (j) (list i j))
                     (enumerate i n)))
              (enumerate 1 n))))

(define (get-result n)
  (foldr append null
         (map (lambda (pair) (list (cons pair (+ (car pair) (cadr pair)))))
              (filter (lambda (pair) (odd? (+ (car pair) (cadr pair))))
                      (get-pair n)))))
```

### 符号数据

​ 每种语言中都提供了字符串，Lisp也提供了符号数据，且较其他语言要更加灵活：

```scheme
(define a 1)
(define b 2)

(list a b) => (1 2)

; 使用 ' 标识符号数据
(list 'a 'b) => (a b)

; ' 也可以用于标识复合对象，采用的是表的输出方式
(car '(a b c)) => (car (list 'a 'b 'c)) => a
```

### 抽象数据多重表示

​ 对于同一个对象，可能有不同的实现方式，在面向对象中，可以使用继承抽象类或是实现interface的方式实现复用，在FP中通过纯函数同样可以实现类似的效果；

#### 带标识的数据

​ 考虑复数抽象数据类型：复数可以通过直角坐标（即实部和虚部）的方式表示，也可以通过极坐标的方式表示；因此为复数抽象数据设计通用的接口就成为了一个问题。

​ 朴素地，可以考虑为每个数据提供一个包装，提供过程 attach-tag，以cons ('tag , data)的形式，通过tag区分出不同的数据，并提供 type-tag 过程和 contents 过程分别用于解包

```scheme
; 一种带标识数据的实现方式
(define (attach-tag type-tag content)
  (cons type-tag content))

(define (type-tag data)
  (car data))

(define (contents data)
  (cdr data))
```

  由此，就可以通过tag区分不同的类型，设计一个统一的接口来调用了：

```scheme
; 假设对于直角坐标表示法的复数存在以下接口
(make-from-real-imag x y)
(real-part-rec data) => (car data)
(imag-part-rec data) => (cdr data)
(angle-rec data) => (transform and get angle)
(magni-rec data) => (...)

; 对极坐标表示的同理
(make-from-angle-magni x r)
(real-part-polar data) => (transform)
(imag-part-polar data) => (...)
(angle-polar data) => (car data)
(magni-polar data) => (cdr data)

; 设计的统一接口
(define (rec? data)
  (eq? (car data) 'rec))

(define (polar? data)
  (eq? (car data) 'polar))

(define (real-part data)
  (cond ((rec? data) (real-part-rec (contents data))
         (polar? data) (real-part-polar (contents data))
         (else error))))
```

​ 这样即可实现类似面向对象中多态的效果；但是也有一个问题，即：下层和上层之间的耦合程度过高，导致每为下层添加一种实现，上层都必须在通用接口中添加某一种实现的调用，给维护造成困难。对这类操作的调用，我们更希望在上层作出约束，而由下层主动注册操作，因此有了如下的解决方案：

```scheme
; 上层为下层提供这样一类接口
(put <func-name> <type-tag-table> <func>)
(get <func-name> <type-tag-table>) => func
; 这类函数在通用接口中维护一张表，通过 func-name 和 type-tag-table 定位到具体的函数对象
; 其中 type-tag-table 是一张表，用于存储函数需要的所有参数类型
; 具体数据实现通过这两个接口即可注册具体的操作

; 如：
(define (install-rec-package)
  ; 实现
  (define (make-from-real-imag x y) (cons x y))
  (define (make-from-angle-magni x r)=>(transform))
  (define (image-part data) (car data))
  (define (add-rec x y) (cons x.a + y.a, x.b + y.b))
  
  ; actions to register
  (define (tag x) (attach-tag 'rec x))
  (put 'make-from-real-imag 'rec (lambda (x y) (tag make-from-real-imag x y)))
  (put 'add '(rec rec) add-rec)
  ....)

; 而在通用接口中，可以这样实现
; 其中 参数 . 代表动态个数的参数
(define (apply-func func-name . args)
  (let ((args-type-list (map type-tag args)))
    (let ((proc (get func-name args-type-list)))
      (if proc
          (apply proc (map contents args))
          (error)))))

(define (add-complex x y) (apply-generic 'add x y))
```

​ 通过这种方式，实现了面向对象中的重载与重写...

> **其他语言中类似机制的实现**
>
> ​ 在Cpp中，重载和泛型是通过编译期给符号添加固定的tag到可重定向文件中实现的，而多态是通过虚函数表实现的。这些操作与FP中的实现非常类似。

### 泛型

​ 更拓展一步，对于所有数，都可以有加、减等运算操作，为所有数提供统一的接口会更加合理；继续之前的思想，不难通过定义tag数据来完成这一操作

```scheme
; 通用算术接口
(define (add x y) (apply-generic 'add x y))
(define (sub x y) (apply-generic 'sub x y))

; 实现复数类型的加减接口
(define (install-complex-calc-package)
  (put 'add add-complex))
```

​ 观查整个系统，不难发现，通过tag将参数一层一层解包，最后定位到具体需要执行的函数，由此构建出的通用接口就是所谓的泛型函数

### 跨数据操作

​ 在数据抽象的前半部分中，我们通过建立各种屏障用来隔离各种不同的数据，但是却忽略了不同类型的数据之间同样可以进行各式各样的操作：比如复数和自然数完全可以进行相加。

​ 为解决这种问题，最容易想到的解决方法就是在算术包中为这一类型特地重载相关的函数，即注册一个('add '(complex-number natural-number))的函数，从而使两类数相加时可以定位到对应的函数。但是这也凸显出一个问题，如果参数顺序调转，变成 自然数 加 复数，我们则又需要添加一个条目。再进一步，我们需要为所有可以使用自然数作为一种复数的地方添加对应的条目，使得自然数在任何地方都可以替代复数。

​ 另一个更好的解决方案是为自然数添加一个类型转换函数

```scheme
(define (natrual->complex x)
  (make-complex-from-real-imag x 0))
```

​ 这种方式也被称为**强制**，同样地，我们可以为某个类型提供一个类型转换函数表：

```scheme
(put-coercion <from-type-tag> <to-type-tag> <func>)
(get-coercion <from-type-tag> <to-type-tag>)
```

​ 类型在注册自己的实现时，同时也注册转换表到通用接口中，通用接口只需要修改一下

apply-generic函数即可

```scheme
(define (apply-generic func-name . args)
  (let ((args-type-table (map type-tag args)))
    (let ((proc (get func-name args-type-table)))
      (if proc
          (apply proc (map contents args))
          (if (= (length args) 2)
            (let ((type1 (car args-type-table))
                  (type2 (cadr args-type-table))
                  (arg1 (car args))
                  (arg2 (cadr args)))
              (let ((type1->type2 (get-coercion type1 type2))
                    (type2->type1 (get-coercion type2 type1)))
                (cond (type1->type2 (apply-generic func-name (type1->type2 a1) a2))
                      (type2->type1 (apply-generic func-name a1 (type2->type1 a2)))
                      (else error))))
            (error))))))
```

​ 比起之前的实现，对于两个参数的函数，如果发生不匹配，则对两个参数尝试进行转型，如果成功，则调用

​ 再进一步，我们建立起类型的层次结构，让每层的数据都实现 raise 函数，这样 apply-generic 可以进一步简化，出现类型不匹配则不断尝试提升某一个参数的层次，直到匹配为止；而不用为每两个类型之间实现强制函数

## 模块化、对象和状态

​ 目前对大型系统的抽象主要有两种思路：

- 将注意力集中在**对象**上，将大型系统**看作一批会发生状态变化的对象**，但是这样编程语言就必须提供机制去**保证符号与对象的绑定**；对符号的理解也从之前的**代换模型**转为更为机械的**环境模型**
- 将注意力集中在**流过系统的信息流**上，将数据作为流过大型系统的信息流；采用流模式去处理大型系统可以**松解模型中对时间的模拟与求值过程中各种事件发生的顺序**

### 赋值与局部对象

#### 什么是状态

​ 所谓一个对象"有状态"，是指它当前的行为会受到它的历史行为的影响。对于一个有状态的对象，并不一定需要记录其所有的历史行为，而可以通过部分信息刻画出历史行为得到的结果，从而为后来的行为提供依据，即**状态变量**。

​ 但是，对象模型中，大部分的对象都不是完全独立的，每个对象都能通过某些特定的交互作用，影响其他对象的状态。

​ 为了减小不同对象之间的耦合度，我们希望可以得到一些互相屏蔽的局部变量，而仅通过接口对对象进行操作；这就对局部状态变量产生了需求。

#### 局部状态变量实例

​ 设想此时需要实现一个银行账户系统：

```scheme
; 我们期望得到这样一系列函数
(define (withdraw amount)
  (the value of remain amount))

(withdraw 10) -> 20
(withdraw 10) -> 10
...
```

​ 根据此前的代换模型，应当不存在这样的过程：对于两个同样参数的调用，代换后求值的结果应该是完全一致的

​ 为此，Lisp 引入两个特殊的形式（就像 define 一样）

```scheme
(set! <name> <new-value>)
(begin <expression1> <expression2> ... <expression.>)
```

​ 其中，**set! 会给符号重新绑定一个新值；而 begin 为后续的表达式构造了一系列顺序执行的区域，begin 语句的值为最后一个表达式的值**

​ 有了这两种特殊形式，就可以轻松写出期望的函数了：

```scheme
(define remain 100)

(define (withdraw amount)
  (begin (set! remain (- remain amount))
         remain))

; 以上形式也可写为
(define (withdraw amont)
  (set! remain (- remain amount))
  remain)
; scheme 中，表达式序列的返回值即为最后一个表达式，可以不用 begin 包裹
```

​ 但是上面的实现方式并非是**局部变量**，修改实现为：

```scheme
(define new-withdraw
  (let ((balance 100))
    (lambda (amount)
      (begin (set! balance (- balance amount))
             balance))))
; 这个实现中，使用let绑定了一个局部变量 balance, 就可以隐藏一些实现了

; 同时，还希望为不同的账户提供不同的 balance 记录
(define (make-withdraw balance)
  (lambda (amount)
    (set! balance (- balance amount))
    balance))

(define w1 (make-withdraw 100))
(define w2 (make-withdraw 100))
(w1 10)
(w2 20)
```

​ 这里的求值模型就变得越来越奇怪了，至少用代换模型已经完全无法解释...之后将引入一个全新的基于环境的求值模型，用于解释这样的情况

### 求值环境模型

​ 不好描述，直接看书。

### 用变动数据做模拟

#### 队列

```scheme
#lang racket

(define (make-queue)
  (let ((ptrs (mcons '() '())))

    (define (enqueue! val)
      (let ((new-cons (mcons val '())))
        (if (empty?) (begin 
                       (set-mcar! ptrs new-cons)
                       (set-mcdr! ptrs new-cons))
          (begin 
            (set-mcdr! (mcdr ptrs) new-cons)
            (set-mcdr! ptrs new-cons)))))

    (define (dequeue!)
      (if (empty?) (display 'queue_is_empty)
        (set-mcar! ptrs (mcdr (mcar ptrs)))))

    (define (empty?)
      (null? (mcar ptrs)))

    (define (front)
      (if (empty?) (display 'queue_is_empty)
        (mcar ptrs)))

    (define (debug)
      (display (mcar ptrs)))

    (define (dispatcher action . args)
      (cond [(eq? action 'enqueue!) (apply enqueue! args)]
            [(eq? action 'dequeue!) (dequeue!)]
            [(eq? action 'empty?) (empty?)]
            [(eq? action 'front) (front)]
            [(eq? action 'debug) (debug)]
            (else (display 'undefined_action))
            ))
    dispatcher))

(define queue (make-queue))

(queue 'enqueue! 10)
(queue 'enqueue! 20)
(queue 'enqueue! 30)
(queue 'dequeue!)
(queue 'dequeue!)
(queue 'dequeue!)
(queue 'dequeue!)
```

#### 多维表格

```scheme
#lang racket

(define (make-one-dim-table)
  (let ((table-header (mcons '*table* null)))

    (define (look-up table key)
      (cond [(null? table) null]
            [(eq? (mcar (mcar table)) key) table]
            (else (look-up (mcdr table) key))))

    (define (put! key val)
      (let ((item (look-up (mcdr table-header) key)))
        (if (null? item)
          (let ((new-item (mcons key val)))
            (set-mcdr! table-header (mcons new-item (mcdr table-header))))
            (set-mcdr! (mcar item)))))

    (define (get key)
      (let ((item (look-up (mcdr table-header) key)))
        (if (null? item) (display 'KEY_NOT_FOUND)
          (mcdr (mcar item)))))

    (define (debug)
      (display table-header))

    (define (dispatcher action . args)
      (cond [(eq? action 'put!) (apply put! args)]
            [(eq? action 'get) (apply get args)]
            [(eq? action 'debug) (debug)]
            (else (display 'NO_SUCH_ACTION: action))))
  dispatcher))

(define table (make-one-dim-table))

(table 'put! 'a 1)
(table 'put! 'b 2)
(display (table 'get 'a))
```

#### 电路模拟

```scheme
#lang racket

(define (make-wire)
  (let ((signal-val 0) 
        (action-proc '()))

    (define (call-each actions) 
      (if (null? actions) (displayln 'call-each)
        (begin
          ((mcar actions))
          (call-each (mcdr actions)))))

    (define (set-signal! val) 
      (if (= val signal-val) (displayln 'set-signal)
        (begin
          (set! signal-val val)
          (call-each action-proc))))

    (define (get-signal) signal-val)

    (define (add-action! action) (set! action-proc (mcons action action-proc)))

    (define (dispatcher action)
      (cond [(eq? action 'get-signal) get-signal]
            [(eq? action 'set-signal!) set-signal!]
            [(eq? action 'add-action!) add-action!]
            (else (display 'INVAILED_ACTION: action))))
    dispatcher))


(define (logic-not val)
  (cond [(= val 0) 1]
        [(= val 1) 0]
        (else (display 'INVAILED_INPUT:SHOULD_BE_1_OR_0))))

(define (logic-and a b)
  (cond [(and a b) 1]
        [(not (and a b)) 0]
        (else (display 'INVAILED_INPUT:SHOULD_BE_1_OR_0))))

(define (logic-or a b)
  (cond [(or a b) 1]
        [(not (or a b)) 0]
        (else (display 'INVAILED_INPUT:SHOULD_BE_1_OR_0))))

(define (after-delay a b) (b))

(define (get-signal wire) ((wire 'get-signal)))

(define (set-signal! wire val) ((wire 'set-signal!) val))

(define (add-action! wire action) ((wire 'add-action!) action))

(define inverter-delay 0)
(define or-gate-delay 0)
(define and-gate-delay 0)

(define (inverter i o) 
  (define (invert-input)
    (let ((new-val (logic-not (get-signal i))))
      (after-delay inverter-delay
                   (lambda () (set-signal! o new-val)))))
  (add-action! i invert-input))

(define (and-gate a b o)
  (define (and-action-proc)
    (let ((new-val (logic-and (get-signal a) (get-signal b))))
      (after-delay and-gate-delay
                   (lambda () (set-signal! o new-val)))))
  (add-action! a and-action-proc)
  (add-action! b and-action-proc))

(define (or-gate a b o)
  (define (or-action-proc)
    (let ((new-val (logic-or (get-signal a) (get-signal b))))
      (after-delay or-gate-delay
                   (lambda () (set-signal! o new-val)))))
  (add-action! a or-action-proc)
  (add-action! b or-action-proc))


(define (half-adder a b s c) 
  (let ((d (make-wire))
        (e (make-wire)))
    (or-gate a b d)
    (and-gate a b c)
    (inverter c e)
    (and-gate d e s)))

(define (full-adder a b c-in sum c-out)
  (let ((s (make-wire))
        (c1 (make-wire))
        (c2 (make-wire)))
    (half-adder a s sum c2)
    (half-adder b c-in s c1)
    (or-gate c1 c2 c-out)))


(define a (make-wire))
(define b (make-wire))
(define c (make-wire))
(define out (make-wire))

(or-gate a b c)

(and-gate c a out)

(set-signal! a 1)

(displayln (get-signal out))
```

### 流

#### 使用序列的问题

​ 考虑曾经作为程序界面的**序列**，会发现它存在很大的问题，即性能低下，考虑以下两个程序

```scheme
; ------------------------------------------------------------
; use seq to calculate                                       
(define (enumerate-interval low high)
  (if (= low high) '()
    (cons low (enumerate-interval (+ 1 low) high))))

(define (prime? x)
  (define (prime-iter x y)
    (cond [(= x y) true]
          [(= 0 (remainder x y)) false]
          (else (prime-iter x (+ 1 y)))))
  (prime-iter x 2))

(define (accumulate l action)
  (if (not (cons? l)) 0
    (action (car l) (accumulate (cdr l) action))))

(define (accumulate-all-prime-from-to-use-seq from to)
  (accumulate (filter prime? (enumerate-interval from to)) +))

;------------------------------------------------------------
; use iter to calculate                                     
(define (accumulate-all-prime-from-to-use-iter from to)
  (define (iter cur end ret)
    (cond [(= cur end) ret]
          [(prime? cur) (iter (+ cur 1) end (+ ret cur))]
          (else (iter (+ cur 1) end ret))))
  (iter from to 0))
```

​ 容易看出，使用序列的实现**更加美观且可复用性更高**，但是从性能角度考虑，序列在经过几次映射函数（accumulate、filter）时，发生了整个表的复制

​ 使用流就是为了既使程序保持美观且高效

#### 流和延迟求值

​ 给流一个定义：**使用了延迟求值技术的表**，传递一个流相当于传递了一个部分序列，只有当使用到这个序列的某些内容时才发生求值

​ 流提供流以下操作：

```scheme
(delay expr), 返回一个 promise, 并不对 expr 立即求值
(force promise), 对 promise 进行求值

(define p (delay 1)) -> promise
(force p) -> 1

(stream-cons first rest), 返回一个流，rest是还未计算的部分
; stream-cons 可以实现为
(define (stream-cons first rest)
  (cons first (delay rest)))

(stream-first stream), 返回流的第一个元素
(stream-rest stream), 返回流的其余元素
; 可以实现为
(define (stream-first stream) (car stream))
(define (stream-rest stream) (force (cdr stream)))
```

​ 同时也提供流对流的映射，如 stream-filter，stream-map 等，举例来说

```scheme
(define (stream-enumerate-interval from to)
  (if (= from to) empty-stream
    (stream-cons from (stream-enumerate-interval (+ 1 from) to))))

(define (prime? x)
  (define (prime-iter x y)
    (cond [(= x y) true]
          [(= 0 (remainder x y)) false]
          (else (prime-iter x (+ 1 y)))))
  (prime-iter x 2))

(define (list-prime-from-to from to)
  (stream->list (stream-filter prime? (stream-enumerate-interval from to))))

; 求出 10 ~ 20 间的所有质数
(display (stream->list (stream-filter prime? (stream-enumerate-interval 10 20))))
```

#### delay和映射的实现

```scheme
; 从最简单的实现来看，delay可以作为一个简单的lambda的语法糖，force只不过简单调用
(define (delay expr)
  (lambda () expr))

(define (force delay-item)
  (delay-item))

; 但是这种实现没有处理重复调用的问题，可以对已经计算过的值做存储
; 就会变成以下形式
(define (delay expr)
  (let ((already-run? false) (result false))
    (lambda ()
      (if (not already-run?)
          (begin (set! result (proc))
                 (set! already-run? true)
                 result)
          result))))

;而 force 依然只是简单调用 delay-item

; 由此可以实现很多对流的映射操作
(define (stream-filter p s)
  (cond [(stream-empty? s) empty-stream]
        [(p (stream-first s))
         (stream-cons (stream-first s)
                      (stream-filter p (stream-rest s)))]
        (else (stream-filter p (stream-rest s)))))
```

#### 无穷流和隐式流

```scheme
; 不难猜测无穷流的定义方式
(define (ints start) (stream-cons start (ints (+ 1 start))))
; 这可以轻松定义出一个从 start 开始的无穷连续整数列
; 通过无穷流也可以产生无穷流，如
(define (odds s) (stream-filter (lambda (x) (= 0 (remainder x 2))) s))
; 通过这些方式可以组合出很多程序

; 隐式流可以从另一个角度看到流的组合
(define ones (stream-cons 1 ones))
(define (ints start) (stream-cons start (add-streams ones ints)))
```

#### 案例：使用流构造埃筛质数序列

```scheme
; s 是一个从 2 开始的质数序列
(define (sieve s)
  ; 返回的第一个元素是2
  (stream-cons 
    (stream-first s)
    ; 其后的元素是对s进行延迟计算后产生的：
    ; 对当前的序列进行质数筛选运算
    (sieve (stream-filter
             (lambda (x)
               (not (= 0 (x (stream-first s)))))
             (stream-rest s)))))
```

```cpp
// C++ 也在 20 标准中提供了 ranges 和 views 机制，理论上也可以简便地书写类似的代码
// 可惜我写不来，拿一个简单的例子来用
#include <iostream>
#include <numeric>
#include <ranges>

auto main() -> int {
    auto res = std::views::iota(2, 100) | std::views::filter([](int x) { return x % 2; });
    std::cout << std::accumulate(res.begin(), res.end(), 0, [](auto x, auto y) { return x * y; });
    return 0;
}

```

### 函数式程序的模块化

​ 模块化是为了更好地复用，通过引进赋值系统，可以将大系统的某些状态封装为内部变量。但是同样地，这也带来了引用透明性失效的问题，在某个程度上提高了程序的复杂性（因为被迫引入时间作为度量）。

​ 通过将时间转变成序列关系上的先后，可以产生流这一数据结构；使用流可以避免使用赋值，从而更加优雅地书写等效的代码。

​ 与通过引入赋值带来的模块化一样，函数式程序通过流也可以实现模块化，如下面这一例：

```scheme
; 先考虑已经实现过的银行模拟程序
(define (make-account balance)
  (lambda (action amount)
    (cond [(eq? action 'withdraw) (set! balance (- balance amount))]
          [(eq? action 'deposit) (set! balance (+ balance amount))]
          [(eq? action 'query) (display balance)]
          (else (display 'NO_SUCH_ACTION)))))

; 这种方式通过赋值完成了状态的维护
; 从流的角度来看，对银行账户的操作可以看成是输入一系列操作流
; 而账户的余额也同样是一系列输出流
; 随着时间变化（操作输入和余额输出），流在不断推进
; 根据这个思想，可以写出如下代码

; 取款的实现
(define (stream-withdraw balance amount-stream)
  (stream-cons 
    balance
    (stream-withdraw (- balance (stream-first amount-stream))
                     (stream-rest amount-stream))))
```

​ 这种方式避免了赋值却也模拟出了真实的状态变化，本质上看状态发生的“变化”只是流在不断推进，将其中某一点作为“状态”而已。

​ 但是一旦涉及模拟一些独立对象之间的交互（如两个人共用上面的银行账户时），对两个输入流的归并依然会带来时间上的问题。因此现在还不存在能够完美解决这一情况的编程范式。

## 元语言抽象

### 概念

​ 在之前的部分中，通过提取基本元素，抽象出模块，从这些模块再构建更高一级的模块，从而限制软件系统的复杂性；但是总会存在一些情况，或是软件复杂度膨胀到一定位置时，上述的所有方式都将失效（如使用汇编也可以提供一些基本的抽象，利用寄存器，构造一些简单的计算过程等；但是其可维护性是极差的），为此需要构造出全新的语言，**通过提供不同的原语来提供不同的思考方式**，来描述所面对的问题

​ **元语言抽象**就是设计新的语言，并通过**构造求值器**的方式实现这些语言，求值器也是一个过程，它通过接收语言的某种表达式来执行求值这个表达式需要的动作；求值器决定了一个程序设计语言中的各种表达式的意义，而它本身也不过是另一个程序

### 元循环求值器

​ 用与被求值的语言同样的语言写出的求值器被称为**元循环**（类似自举？），以下将用 Lisp 实现一个简单的 Lisp 求值器

​ 求值器采用的求值模型为**环境模型**，该模型包含两个部分：

- 求值一个组合式（一个不是特殊形式的复合表达式）时，先**求值其中的所有子表达式（包括运算符表达式和运算对象表达式）**，而后将**运算符子表达式的值作用于运算对象子表达式的值**
- 将一个复合过程应用于一集参数时，先通过实际参数约束形式参数，构造出一个新环境，在这个新的环境里求值这个过程的体

​ 整个求值过程可以看作一个循环，即对于一个表达式，在具体环境中的求值被归约到过程对实际参数的应用（部分二），而对实际参数的应用被归约到新表达式在新环境中的求值（部分一），以此循环直到在环境中找到可以直接应用的基本过程时再递归回去

#### 求值器的内核

​ 其内核主要也由上述的两个操作组成，即 eval（求值）和 apply（过程应用）

```scheme
; 可以看到，eval 做的工作就是对表达式进行解析，并分发到对应的操作中
(define (eval exp env)
  (cond [(self-evaluating? exp) exp]
        [(variable? exp) (lookup-variable-value exp env)]
        [(quoted? exp) (text-of-quotation exp)]
        [(assignment? exp) (eval-assignment exp env)]
        [(definition? exp) (eval-definition exp env)]
        [(if? exp) (eval-if exp env)]
        [(lambda? exp) (make-procedure (lambda-params exp)
                                       (lambda-body exp)
                                       env)]
        [(begin? exp)
         (eval-sequence (begin-action exp) env)]
        [(cond? exp) (eval (cond->if exp) env)]
        [(application? exp) 
         (apply (eval (operator exp) env)
                (list-of-values (operands exp) env))]
       (else (error "UNKOWN EXPRESSION TYPE -- EVAL " exp))))

; 而 apply 通过 procedure 和 arguments 对两类过程进行分发
; 一类是基础过程，这类过程的值已经在全局环境中，可以直接应用
; 否则就创建新环境，并以新环境为基础，对过程体序列进行求值（eval-sequence）
(define (apply procedure arguments)
  (cond [(primitive-procedure? procedure) (apply-primative-procedure procedure arguments)]
        [(compound-procedure? procedure)
         (eval-sequence
           (procedure-body procedure)
           (extend-environment
             (procedure-parameters procedure)
             arguments
             (procedure-environment procedure)))]
        (else (error "UNKOWN PROCEDURE TYPE -- APPLY " procedure))))
```

​ 现在考虑环境求值模型的实现，环境是求值的基础，每个环境里会存所有的过程、变量与值的绑定

```scheme
; env:   (frame,  enclosing-env)   每个环境是一个cons，car 是框架，用来存储binding，cdr 存储到父环境的指针
;    |
; frame  (variables,     values)   每个框架也分为car 和cdr，分别存储变量名和值
;   |    |
; vals   |    (list val1 val2 val3...)
; vars    (list var1 var2 var3...)

;---------------implementation for frame------------------------------
; 制作一个框架，简单把 vars 和 vals 组合起来即可
(define (make-frame vars vals)
  (cons vars vals))

; 对 frame 的选择函数
(define (frame-variables frame) (car frame))
(define (frame-values frame) (cdr frame))

; 给 frame 添加 binding
(define (add-binding-to-frame! var val frame)
  (set-car! (cons var (car frame)))
  (set-cdr! (cons val (cdr frame))))

;---------------implementation for environment------------------------
; 从一个旧环境中派生出新环境
; 先检查新增加的binding是否合法，若合法则添加
(define (extend-environment vars vals base-env)
  (let ((var-len (length vars))
        (val-len (length vals)))
    ; 会发生环境派生时，只会是发生过程调用的时候...
    ; 因此 error 可以写 too few / many arguments
    (cond ((> var-len val-len) (error "TOO FEW ARGUMENTS" vars vals))
          ((< var-len val-len) (error "TOO MANY ARGUMENTS" vars vals))
          ; 合法，构造出一个新环境，其中带有新的binding并指向父环境
          (else (cons (make-frame vars vals) base-env)))))

; 对环境的选择函数
(define (first-frame env) (car env))
(define (enclosing-env env) (cdr env))

; 空环境
(define the-empty-environment '())
```

​ 考虑对环境的操作，主要就是三个：增加、修改、查找变量

```scheme
; 对环境的搜索可以简单归结为，每进入一个环境，就遍历它的frame
; 如果找到了同名的，则进行一系列操作

;-----------------implementation for evironment operations--------------
(define (look-up-variable-value var env)
  (if (null? env) (error "UNBOUND SYMBOL" var)
      (define (scan-loop vars vals)
        (cond 
         ; 在当前环境的框架下没能找到，则到父环境中找
         ((null? vars) (look-up-variable-value var (enclosing-env env)))
         ; 找到，返回值
         ((eq? (car vars) var) (car vals))
         ; 接着比对下一个变量
         (else scan-loop (cdr vars) (cdr vals))))
       (let ((frame (first-frame env)))
         (scan-loop (frame-variables frame)
                    (frame-values frame)))))

(define (set-variable-value var val env)
  (if (null? env) (error "UNBOUND SYMBOL" var)
      (define (scan-loop vars vals)
        (cond 
         ; 在当前环境的框架下没能找到，则到父环境中找
         ((null? vars) (look-up-variable-value var (enclosing-env env)))
         ; 找到，则修改
         ((eq? (car vars) var) (set-car! val vals))
         ; 接着比对下一个变量
         (else scan-loop (cdr vars) (cdr vals))))
       (let ((frame (first-frame env)))
         (scan-loop (frame-variables frame)
                    (frame-values frame)))))

; 原书的实现为如果已经定义则重新修改，考虑搜索是从新到旧，直接新添项目也可以实现
(define (define-variable! var val env)
  (add-binding-to-frame! var val (first-frame env)))
```

​ 这时候就可以为整个解释器准备全局环境了

```scheme
; 先为全局环境准备好基本过程的名称和列表
(define primitive-procedure-names
  (list 'car 'cdr 'cons '+ '- '* '/ 'null?))
(define primitive-pricedure-objects
  (list car cdr cons + - * / null?))

; 添加 true 和 false
(define (true? x) (not (eq? x false)))
(define (false? x) (eq? x false))

; 建立全局环境
(define (setup-environment)
  (let ((initial-env
         (extend-environment (primitive-procedure-names)
                             (primitive-procedure-objects)
                             the-empty-environment)))
    (define-variable! 'true true initial-env)
    (define-variable! 'false false initial-env)
    initial-env))
```

​ 基本过程已经实现，但是还有一些非过程的关键字，如：begin块、lambda、if、cond、（）过程应用等需要实现

```scheme
; 以 if 为例，展示整个实现过程
; 先考虑 if 在代码中如何表示？ (if (<predicate>) (<cons>) (<alter>))
; make-if 在 cond转if 时会使用到，但是如果不使用 cond，可以不实现 make-if
(define (make-if predicate consequent alternative)
  (list 'if predicate consequent alternative))
; 简单实现为一个带 tag 的 list 即可
; 由此，可以定义出对 if 块的一些基本选择函数
(define (if-predicate exp) (cadr exp))
(define (if-consequent exp) (caddr exp))
(define (if-alternative exp)
  (if (null? cdddr exp) ('false) (cadddr exp)))
; 再接下去就可以定义求值 if 块的函数
(define (eval-if exp env)
  (if (true? (eval (if-predicate exp) env))
      (eval (if-consequent exp) env)
      (eval (if-alternative exp) env)))
; 同时再补上 eval 中判断 是否是if块 时使用的 if? 谓词
; 判断是否是 带<tag>的 list
(define (tag-list? exp tag)
  (if (pair? exp)
      (eq? (car exp) tag)
      false))

(define (if? exp) (tag-list? exp 'if))

; 整个求值的顺序是：
; main-loop -> input -> eval -> cond(if?) -> eval-if -> result

; 同理，其他语句的实现也可以是同样的方式：

; begin: ('begin (action1 action2 action3...))
(define (begin-action begin-block) (cdr begin-block))

(define (begin? exp) (tag-list? exp 'begin))

(define (eval-sequence exps env)
  (cond ((last-exp? exps) (eval (first-exp exps) env))
        (else (eval (first-exp exps) env)
              (eval-sequence (rest-exps exp) env))))

(define (last-exp? exps) (null? (cdr exps)))
(define (first-exp exps) (car exps))
(define (rest-exps exps) (cdr exps))
; eval -> cond(begin) -> begin-action -> eval-sequence

; procedure,即过程应用的表达式

; lambda
; in eval, [(lambda? exp) (make-procedure (lambda-parameter exp) (lambda-body exp) env)]
```

