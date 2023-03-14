# STL源码分析

## iterator

### 什么是iterator

​	迭代器，即**容器提供**给用户的可以**访问容器**而无需知道容器内部细节的一种**智能指针**。在Cpp里，更是**连接容器与算法**的桥梁。

​	迭代器的实现通常是通过**对裸指针进行包装**而做到的，这种包装一方面需要**重载与迭代器相关的运算符**（如自增、dereference）等，另一方面，迭代器需要某种机制**提供给迭代器的使用者（algorithm）与迭代器相关的信息**。

### 智能指针

这里先简单实现一个标准容器 list

```cpp
template <class item_t>
struct list_node_t {
    item_t data;
    list_node_t next;
};

template <class item_t>
class list {
  private:
    typedef list_node_t<item_t> node_t;
    node_t* head;
    node_t* tail;
    
  public:
    // constructor
	list(){ head = tail = new node_t(null, nullptr); }
    // destructor
    ~list();
    
    void push_front(item_t data) {
        node_t* node = new node_t(data, head->next);
        if(head == tail) tail = node;
        head = node;
    }
    
    item_t get_element(size_t pos) {
        node_t* ptr = head->next;
        for(size_t i = 0; i < pos; i++)
            ptr = ptr->next;
        return *ptr;
    }
};
```

​	上面实现的list是一个比较常规的数据结构，对外提供了一些基本的读写操作的接口，但是考虑下面这样一个函数。

```cpp
void print_all(list<int> l, size_t len) {
    for(int i = 0; i < len; i++) {
        cout << l.get_element(i);
    }
}
```

​	这里操作list的方式明显非常低效：每次都要从头开始再访问，长度为n的链表的遍历却消耗了n^2^的时间。那么直接提供给用户list内部元素的裸指针？这将破坏list的封装性。如果能提供某种类型，使它能够既能灵活访问list，又不破坏其基本的封装性，问题就能解决。

```cpp
template <class item_t>
class list_iterator {
private:
    list_node_t<item_t>* ptr;
public:
    list_iterator(list_node_t<item_t>* ptr = nullptr) {
        this->ptr = ptr;
    }

    list_iterator<item_t> operator++() {
        ptr = ptr->next;
        return *this;
    }

    list_iterator<item_t> operator++(int) {
        list_iterator<item_t> ret = *this;
        ptr = ptr->next;
        return ret;
    }

    item_t& operator*() {
        return ptr->data;
    }

    bool operator==(list_iterator<item_t> iter) {
        return ptr == iter.ptr;
    }

    bool operator!=(list_iterator<item_t> iter) {
        return ptr != iter.ptr;
    }

    ~list_iterator();
};
```

​	并在list中添加提供迭代器的函数

```cpp
list_iterator<item_t> list::begin(){
    return list_iterator<item_t>(head->next);
}

list_iterator<item_t> list::end(){
    return list_iterator<item_t>(tail->next);
}
```

​	这样就可以修改上面的print_all函数了

```cpp
template <class item_t>
void list_print_all(list<item_t> l) {
    for(list_iterator<item_t> iter = l.begin(); iter != l.end(); iter++) {
        cout << *iter << endl;
    }
}
```

​	这样，就在不破坏封装性（用户并不清楚内部实现，而迭代器仅能由list提供（因为没人能知道list的head的地址，所以不存在自己构造一个list迭代器的可能））的情况下，提供了灵活的访问模式。



### 容器与算法的桥梁

#### 使用函数模板

​	对于上面的 print_all 函数，也许有更合适的实现：

```cpp
template <class iter_t>
void print_all(iter_t begin, iter_t end) {
    for(iter_t i = begin; i != end; i++) {
        cout << *i << endl;
    }
}
```

​	改用iterator作为了泛型参数，不同的容器只需要为自己封装好iterator，即可共用函数模板。这样，不论是 list 或是 vector 又或是 set，都可以使用 print_all 函数。

​	事实上，所有 std::algorithm 库函数都是通过这种方式与容器相连接的。

#### type traits

​	考虑一个函数: std::max_element

```cpp
// 下面的 "?" 中应该填什么类型？
template <class iter_t>
? max_element(iter_t begin, iter_t end) {
    ? max = *begin;
    for(iter_t i = begin; i != end; i++) {
        if(*i > max)
            max = *i;
    }
    return max;
}
```

​	问题在于，有些算法需要定义（使用）对应iterator所指向的类型，iterator需要某种机制，去向使用者提供这些信息，这就是type traits（类型萃取）

>   **C++ nested type**
>
>   ​	类型萃取依赖于C++内嵌类型的机制。所谓内嵌类型，即Cpp类内部的类型，可以通过定义内部类或是在类内部进行**typedef**来产生，nested type可以像静态成员一样，**通过类名限定符进行访问**（即 类名::nested type 访问）。

​	类型萃取通过在iterator类内部定义nested type实现

```cpp
template <class item_t>
class list_iterator {
private:
    list_node_t<item_t>* ptr;
public:
    // type traits
    typedef item_t value_type;
    typedef item_t* value_ptr;
    typedef item_t& value_ref;
    
    // operator overloading...
    //++, *, ==, != ....
};
```

​	此时再实现 max_element 函数

```cpp
template <class iter_t>
typename iter_t::value_type
max_element(iter_t begin, iter_t end) {
    typedef typename iter_t::value_type value_t;
    value_t max = *begin;
    for(iter_t i = begin; i != end; i++) {
        if(*i > max) max = *i;
    }
    return max;
}
```

#### Partial Specialization

​	但是这个体系依然有问题，假设有如下一个调用：

```cpp
int array[] = {1, 2, 3, 4, 5, 6, 7};
int max = max_element(array, array + 7);
```

​	理论上，int* 是指针，这个函数从语义上同样应该作用于裸指针。但是，当max_element向裸指针要求 value_type 时，裸指针明显无法给出。

​	针对这一问题，应采用偏特化的方式来解决。

>   **Partial Specialization**
>
>   ​	偏特化，即通过指定泛型参数，从而为某些泛型参数实现特定的版本。
>
>   ```cpp
>   // 泛化版本
>   template <class T>
>   void print_type() {
>       cout << "normal version";
>   }
>   
>   // 对int类型的偏特化
>   template <>
>   void print_type<int>() {
>   	cout << "int version";
>   }
>   
>   // 对指针类型的偏特化
>   template <class T>
>   void print_type<T*>() {
>       cout << "pointer version"
>   }
>   
>   int main(){
>       int tmp;
>       print_type(1.0);
>       print_type(tmp);
>       print_type(&tmp);
>   }
>   ```
>
>   ​	输出为：normal version， int version， pointer version

​	因此，在 iterator 和 type traits 之间再加一个中间层，即可实现

```cpp
// 泛化
template <typename iter_t>
struct iterator_traits {
    typedef typename iter_t::value_type value_type;
    typedef typename iter_t::value_ref reference;
    typedef typename iter_t::value_ptr pointer;
}

// 对指针类型的偏特化
template <typename iter_t>
struct iterator_traits<iter_t*> {
    typedef iter_t value_type;
    typedef iter_t* pointer;
    typedef iter_t& reference;
}
```

​	使用偏特化，max_element 也应该改写为如下形式：

```cpp
template <class iter_t>
typename iterator_traits<iter_t>::value_type
max_element(iter_t begin, iter_t end) {
    typedef typename iterator_traits<iter_t>::value_type value_type;
    value_type ret = *begin;
    for(iter_t i = begin; i != end; i++) {
        if(*i > ret) ret = *i;
    }
    return ret;
}
```

​	加入了 iterator_traits 后，就可以满足裸指针、迭代器公用了。

### STL中的iterator

```cpp
// 
```

