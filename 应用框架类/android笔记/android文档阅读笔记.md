# Activity

## Activity简介

### Activity的启动

Activity类似于**应用程序入口**的作用，只是它可能会有多个"入口"来**对付应用中不同生命周期**

同时这似乎也说明一个activity应该与别的操作系统中的一个带main的程序具有相同的地位

**不同activity之间的关系应该非常薄弱**，只提供最基础的数据传输和跳转等功能

### 配置Activity

在**mainfest.xml**文件中，添加如下的声明

```xml
    <manifest ... >
      <application ... >
          <activity android:name=".ExampleActivity" />
          ...
      </application ... >
      ...
    </manifest >
    
```

即可声明一个名为.ExampleActivity的Activity

**注：对于每个Activity，必需的声明的属性只有name，其他的可以根据情况添加**