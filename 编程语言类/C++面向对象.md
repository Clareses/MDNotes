# C++面向对象

## 封装

### 构造函数

```cpp
class test{
	test(_param_list):_init_list{
    	//todo
  	}  
};
```

- **默认构造函数**

	Cpp默认给出，无参，对成员变量的初始化不确定，会根据新建变量的位置发生变化。（一旦实现了构造函数，默认的构造函数会失效）

	```cpp
	class test{
		test():_init_list{}  
	};
	```

- **委托构造函数**

	一个构造函数在初始化列表中直接使用同一个类中的构造函数重载

	```cpp
	class test{
	    test(int param):_init_list{}
	  	test():test(0){}  
	};
	```

- **复制构造函数**

	在为新对象赋初值时调用

	```cpp
	class test{
	  	test(const test& param):_init_list{}
	};
	```

### 析构函数

```cpp
class test{
    ~test(param_list){
        //todo
    }
};
```

