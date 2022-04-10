# QT開發學習

## 第一個QT程序

### QT目錄

```
project------(工程目錄)
	|
	|-makefile   （調用qmake命令後生成）
	|-main.cpp   （源代碼）
	|-project.pro（qmake -project後生成）
	|-project    （可執行文件）
```

### 項目構建過程

```
1- 書寫源代碼
2- 在工程目錄下使用 qmake -project命令生成.pro文件
3- 使用qmake生成makefile文件
4- 用make構建可執行文件
```

### 項目代碼

```cpp
#include <QApplication>
#include <QLabel>

//命令行參數一定要寫
int main(int argc,char** argv){
    //QT應用對象
    QApplication app(argc,argv);
    //QT label對象
    QLabel Label("Hello QT！");
    //調用對象的顯示函數
    Label.show();
    //令程序進入交互循環
    return app.exec();
}
```



## 父窗口

### 常用的父窗口類

**QWidget**

QMainWindow （主窗口）	extends QWidget

QDialog  （對話框）			   extends QWidget

### 常用的接口(大部分顯示對象都有)

void remove(int x,int y);

void resize(int w ,int h);

### 項目代碼

```cpp
#include <QApplication>
#include <QWidget>
#include <QLabel>

int main(int argc,char** argv){
    QApplication app(argc,argv);
    
    QWidget Widget;
    QLabel label("label",&Widget);
    
    return app.exec();
}
```



## 重要的語法擴展--信號與槽

信號與槽是一組對應的關係

信號是當對象信號發生改變時對象主動發出的

槽是一類函數，在對象收到某些信號時自動調用作出響應

### 語法樣例

```cpp
//信號與槽的鏈接必須用到基類QObejct的靜態函數，因此必須繼承
class X:public QObject{
    
    Q_OBJECT
    
    public:
    //....
    signals://信號的說明符
    	void signal_func(paramlist);//信號函數，只可申明但是不能實現
    
    //槽函數可以有限定符（如public等）
    public slots://共有槽函數的說明符
    	//槽函數需要有實現，並可以被普通地調用
    	void slot_func(paramlist){
            //...
        }
};
```

### 信號與槽的鏈接

```cpp
//依賴於QObject的函數
QObejct::connect(const QObject* sender, const char* signal, const QObject receiver, const char* method);
//參數分別爲
//發送信號對象、信號、接受信號對象、槽
//其中signal需要將信號函數用 SIGNAL()宏 處理， method需要用 SLOT()宏 處理
//信號與槽函數的參數必須一致
```

```cpp
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QObject>
#include <QWidget>

int main(int argc,char** argv){
    
    QApplication app(argc,argv);
    
    QWidget parent;
   	parent.resize(500,500);
    
    QPushButton button("按下以關閉Label",&parent);
    QLabel label("要關閉的label",&parent);
    
    button.move(250,300);
    label.move(250,200);
    
    //類所有的signal與slot需要上手冊查詢
    QObject::connect(&button,SIGNAL(clicked(void)),&label,SLOT(close(void)));
    
    parent.show();
    
    return app.exec();
    
}
```

