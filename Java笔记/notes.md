## notes

#### I/O流

​	1.文件

```java
文件有File类来表示
class File{
    File(String pathname);//接收一个字符串表示的绝对路径，eg."F:\\temp\\1.txt"
    public boolean createNewFile() throws IOException;//在当前file对象的path上创建文件，若文件名重复，则不创建。返回boolean判断是否创建成功。
    public boolean exists();//判断当前file对象的path是否存在对应文件
    public long length();//返回当前file对象的byte长度
}
```

​    2.I/O流

```java
I/O流分为字符流与字节流，多用字节流，因此只作字节流笔记
以主机作为源头，分为输出与输入，因此有input与output之分
字节流下分为两个抽象类:InputStream/OutputStream;
这两个类都有对应的两种实现FIleInputStream/BufferedInputStream;
{//用法大致相同，因此分别只拿FileInputStream与FileOutputStream作例子
   
    class FileInputStream{
        FileInputStream(File file);//构造函数，接收一个file，作为读取的源头
        public int read();//从文件中读取下一个字节
        public int read(byte b[]);//从文件中读取数据并存入array b中,并返回读取的总字节数
        public int read(byte b[],int off,int length);/从array b的第off位开始存入从文件中读取的长度为length的数据，返回读取的总字节数
        public void close();//关闭当前的流对象
    }
    
    class FileOutputStream{
        FileOutputStream(File file);//构造函数
        public void write(int b,boolean append=false);//向文件中写入字节b，append用于判断是否从文件尾开始写入，若为true，则相当于不覆盖原内容，在尾部添加读入的数据
        public void write(byte b[],int off=0,int length,boolean append);//向文件中写入起点为off，长度为length的b[]片段，append同上
        public void close;
    }
}
```

```java
关于这部分内容的示例代码
package Pakage;
import java.io.*;

public class Main_Ac {
    public static void main(String[] args) throws IOException {
        File file1=new File("F:\\temp\\1.txt");
        File file2=new File("F:\\temp\\2.txt");
        file2.createNewFile();
        if(file1.exists()&&file1.length()!=0){
            FileInputStream fin=new FileInputStream(file1);
            FileOutputStream fout=new FileOutputStream(file2);
            int c=0;
            while((c=fin.read())!=-1) fout.write(c);
            fin.close();
            fout.close();
        }
    }
}

```

#### 多线程

​	多线程即同时处理多条指令

```java
示例代码
//方法1，创建myThread类，以便管理复用
class myThread extends Thread{
    @Override
    //在run方法中写入要执行的代码
    public void run() {
        super.run();
        System.out.println("线程myThread启动");
    }
}

public class Main_Ac {
    public static void main(String[] args){
        //创建对象，调用start方法启动线程
        myThread myThread=new myThread();
        myThread.start();
        
        //方法2，直接创建对象并运行
        new Thread(new Runnable() {
            @Override
            public void run() {
                //写入执行的代码
            }
        }).start();//启动线程
        
    }
}

```

#### 接口回调

​	接口回调问题，即是不实现类中与其他类交互的具体方法，而是声明一个接口，让别的类去实现接口以	降低耦合性。可类比为电脑类不用具体去实现对每个外设的接收信息的方法，而是设置一个接口，让其他	外设类去实现接口。

```java
实例代码（这个内容带点泛型的意思...不论是什么类，实现成接口，就可以和某个类交互）
//用于其他类实现该接口，使其他类"变成"一个dataListener，方便computer对不同的类作同样的处理（都当做dataListener来处理）
interface dataListener{
    public void listen();//监听数据的方法
};
//主类
class computer{
    dataListener listener;
    public void setListener(dataListener){
        this.listener=listener;
    }
    public void release(dataListener listener){
        if(listener!=null) listener.listen();
    }
};
//显示屏类，实现dataListener接口
class Screen implements dataListener{
    @override
    //实现listen方法
    public void listen(){
        sout("显示屏监听到数据")
    }
}

```

```java
实例代码 在android中的奇妙用途
interface dataListener{
    public void listen();
};

class dialog{
    dataListener listener;
    public void setListener(dataListener){
        this.listener=listener;
    }
    public void release(dataListener listener){
        if(listener!=null) listener.listen();
    }
};

class MainActivity{
    public void showDialog(){
        dialog dg=new dialog();
        dg.setdatalistener(new dataListener(
        	public void listen(){
                //此处为执行的代码
            }
        ));
        dg.show();
    }
}

class test{
    int main(){
        //模拟用户操作
        MainActivity ac=new MainActivity();//启动了activity
        ac.showDialog();//用户点击按键，ac调用showDialog方法，在方法中新建dialog并调用dialog的setListener方法，传入new出来的dataListener对象并在对象里设置好listen函数，此时dialog里已经设置好了listener成员变量
        dialog.release();//用户点击提交，dialog调用release方法，判断listener是否为空，不为空就调用它的listen方法，其中listen方法是ac通过setListener传过来的
    }
}

```

#### 网络编程

​	在java中，主机被抽象为套接字(Socket)，由套接字可以直接获取到I/O流，通过网络进行文件传输

```java
套接字，即通信的端点，可以由ip和端口描述;
class Socket{
	public Socket(String host, int port);//构造函数，参数为要建立连接的Sever的ip地址与端口号;
    public InputStream getInputStream() throws IOException;//获取当前通信的InputStream
    public OutputStream getOutputStream() throws IOException;//获取OutputStream
    public void close();//关闭
};
服务器的套接字
class ServerSocket{
    public ServerSocket(int port);//构造函数，参数为响应的端口号
    public Socket accept();//接收发来请求的Socket并返回
    public void close();//关闭
}
```

### Servlet

​	servlet是java后端学习的重点知识，其本质是一类”程序“，是运行在服务器上的程序，就如同”控制台程序“一般地位。

​	一个java服务器的文件布局为

```java
main{
    java{
        package{
            servlet程序代码1
            servlet程序代码2
        }
    }
    webapp{
        webINF{
            web.xml//web的配置文件，该文件夹外部无法访问
        }
        index.jsp//首页...
        a.jsp
        b.jsp
    }
}   	
```

 关于web.xml

```xml
每个servlet都需要在web.xml中进行配置
	<servlet>
        <servlet-name>HelloServlet</servlet-name>  servlet的类的别名，用于与mapping绑定
        <servlet-class>com.example.demonet.HelloServlet</servlet-class> 类的真名
    </servlet>
    <servlet-mapping>
        <servlet-name>HelloServlet</servlet-name> 	别名
        <url-pattern>/hello</url-pattern>			访问路径
    </servlet-mapping>
此外，web.xml还保存以下配置文件
	<context-param>
		<param-name>context上下文参数的name</param-name>
        <param-value>上下文参数的值</param-value>
	</context-param>
	<!--上下文参数可以通过servletcontext对象获取到,对于整个工程文件，context参数的值都唯一--!>
	

```

##### Servlet类的继承关系

```java
interface Servlet{
    public void init(ServletConfig config) throws ServletException;
    public ServletConfig getServletConfig();//获取servletconfig对象（servlet配置对象）
    public void service(ServletRequest req, ServletResponse res);
    public void destroy();
}

public abstract class GenericServlet //实现了很多空函数（像适配器...），并且加入了config成员变量
    implements Servlet, ServletConfig, java.io.Serializable{
	public void destroy() {};//重写的空函数
    public String getInitParameter(String name);
    public ServletConfig getServletConfig();
    public ServletContext getServletContext();
    public abstract void service(ServletRequest req, ServletResponse res)
	throws ServletException, IOException;
}

public abstract class HttpServlet extends GenericServlet{//对适配器继承
    //对get和post方法实现分发
    protected void doGet(HttpServletRequest req, HttpServletResponse resp);
    protected void doPost(HttpServletRequest req, HttpServletResponse resp);
}
```

##### 	因此，在实现一个servlet时，需要这么做...	

```java
1.新建类并继承HttpServlet
class MyServlet extends HttpServlet{
    @override
    public void doGet(HttpServletRequest request, HttpServletResponse response);
    @override
    public void doPost(HttpServletRequest request, HttpServletResponse response);
    @override....
}
2.配置web.xml文件或在类头上配置
    web.xml配置见上文
    另一个配置方法
@WebServlet(name = "helloServlet", value = "/hello-servlet")//分别为别名和访问地址
class MyServlet extends HttpServlet{}
```

##### 	ServletConfig类

```java
//顾名思义，就是servlet的配置信息类
public interface ServletConfig {
    public String getServletName();//获取servlet的别名
    public ServletContext getServletContext();//获取servletcontext对象
    public String getInitParameter(String name);//获取初始化参数
    public Enumeration<String> getInitParameterNames();
}
```

##### 	ServletContext类

```java
//顾名思义就是servlet的上下文对象，可以用来存键值对，也可以用来获取上下文信息
//context对于每个工程文件都只有一个，在服务器启动时生成，在服务器停止时销毁，因此不同servlet创建的键值对可以共享，但是一旦重新启动，则所有键值对都消失
public interface ServletContext{
    public String getInitParameter(String name);//获取初始化数据
    public String getContextPath();//获取当前工程路径
    public String getRealPath(String path);//获取当前工程的绝对路径
    public void setAttribute(String name, Object object);//设置键值对
    public void removeAttribute(String name);//删除键值对
    public Object getAttribute(String name);//获取键值对的值
}
```

##### HttpServletRequest类

```java
//很明显这个类用于获取请求及进行相关的操作
//The servlet container creates an HttpServletRequest object and passes it as an argument to the servlet's service methods (doGet, doPost, etc).
public interface HttpServletRequest extends ServletRequest {
    String getRequestURI();//返回请求文件的相对地址
    String getRequestURL();//返回请求文件的绝对地址
    String getRemoteHost();//返回发起请求的ip
    String getHeader();//返回请求内容
    Object getParameter(String name);//获取用户请求的参数
    Object[] getParameters(String name);//获取用户请求的参数列表
    String getMethod();//返回用户的请求方式（有点鸡肋...）
    void setAttribute(String key,Object value);//建立键值对
    Object getAttribute(String key);//获取键值对的值
    RequestDispatcher getRequestDispatcher(String path);//获取请求转发对象
    void enCoding(String code);//用于解决post方法接收中文乱码的问题
}
```

##### HttpServletResponse类

```java
//该类用于回传信息
//The servlet container creates an HttpServletResponse object and passes it as an argument to the servlet's service methods (doGet, doPost, etc).
public interface HttpServletResponse extends ServletResponse{
    void addCookie(Cookie cookie);//添加cookie
    boolean containersHeader(String name);//判断响应头参数是否已被设置
    void sendRedirect(String location) throws IOException;//发送请求重定向信息
    void setHeader(String name, String value);//设置响应头
    void addHeader(String name, String value);//添加响应头
    void setStatus(int sc);//设置状态码
    int getStatus();//返回状态码
    String getHeader(String name); //返回响应头
    void setContentType(String type);//设置响应头中的ContentType属性
}
```



##### 请求转发

```java
@webServlet(name="dispatch",value="/dispatch")
class dispatch extends HttpServlet{
    @override
    doGet(Request req,Response resp){
        //使用req.getparameter获取参数并处理
        req.setAttribute("name","value");//设立键值对以便校验
        req.getRequestDispatcher("/dispacher2").forward(req,resp);//转发到dispach2
    }
}

@webServlet(name="dispatch2",value="/dispatch2")
class dispatch2 extends HttpServlet{
    @override
    doGet(Request req,Response resp){
        //使用req.getparameter获取参数并处理
        req.getAttribute("name");//校验
       	//执行后续步骤
    }
}
```

##### 请求重定向

```java
//请求重定向用于原地址被迁移，由一个Servlet返回信息转向访问另一个servlet
doGet(Request req,Response resp){
    req.setStatus(302);//设置状态码
    req.setHeader("location","新地址");
    
    //方法2
    req.sendRedirect(String location);
}
```

##### 中文乱码处理

```java
//起因是字符集编码不同
doGet(req,resp){
	resp.setCharsetenCode("UTF-8");//设置发送的字符集
    resp.setHeader("content-type","text/html;charset-UTF-8");//为浏览器设置字符集
    
    //简写
    resp.setContentType("text/html;charset-UTF-8");
}
```

