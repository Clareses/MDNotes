## JDBC

一.导入JDBC包（IDEA在file->project structure里）

二.代码部分

```java
//1.加载类，并用drivermanager获取connection
Class.forname("com.mysql.jdbc.Driver");
Connection connection=DriverManager.
    getConnection("jdbc:mysql://127.0.0.1:3306/<database>?characterEncoding=UTF-8","username", "password");
//以上异常都需要处理，封装成方法
public static Connection getConnection(){
        Connection connection=null;
        try {
            Class.forName("com.mysql.jdbc.Driver");
            connection = DriverManager.getConnection
                ("jdbc:mysql://127.0.0.1:3306/<database>?characterEncoding=UTF-8","username", "password");
        } catch (ClassNotFoundException | SQLException e) {
            e.printStackTrace();
        }
        return connection;
}
```

```java
//2.运行SQL语句
String sql="<sql expression>";
PreparedStatement preparedStatement=connection.preparedStatement(sql);
preparedStatement.<settype>(int IndexOfParam,type Param);
preparedStatement.excuteUpdate();
```

```java
//3.执行查询语句并获得相关的值
```

```java
//4.关闭连接
```



建立一个JDBC DAO的基本步骤

第一步，先写一个baseDAO，里面实现几个通用的方法

```java
//1.连接数据库
public static Connection getConnection(/*无需传参，只需要用类内部自带的属性即可连接上*/);

//2.查询  考虑这么写的原因，尤其是参数为啥要传递connetion、preparedstatemnet和resultset进来？是因为如果直接在这个函数里获取connecion与其他俩个参数的话，就必须在这个函数内完成close，那么返回的值也必须确定为我们需要的参数了...这样写的目的是为了返回一个resultset，以方便解耦（所有查询函数都可以基于它），那么就不能在函数内关闭connection与resultset，因此采取传递参数的方式
public static ResultSet execute(Connection connection, String sql
                                , PreparedStatement preparedstatement,
                                Object[] params, ResultSet resultset);

//3.CRUD 这里返回的其实是更新的行数...那么其实这个函数不用传递这么多参数，CRUD完全可以在这个函数内完成并关闭connection而不用传参...原作这么写的原因可能是为了对称...
public static int ResultSet execute(Connection connection, String sql, 
                                    PreparedStatement preparedstatement, Object[] params);

//4.释放资源
//依次判断每个参数是否为null，如果不为null就调用close方法（注意关闭的顺序）
public static boolean closeResource(Connection connection
                                   PreparedStatement preparedstatement
                                   ResultSet resultset);

//最后关于上面的查询与CRUD函数应该如何使用
//例如实现某个查询函数的实例
public type Query(Connecion connection, some params){
    //写sql语句，并在参数处标记"？";
    //将some params转化为参数数组
    //建立空preparedstatement对象与空resultset对象
    //传递参数调用baseDAO中的excute(在basedao的execute中会自动读取参数并添加到sql语句里，并执行返回我们需要的resultset)
    //接收resultset并用next方法读取并完成对象建立
    //关闭connection、preparedstatement、resultset
    //将结果返回
}
//这里依然有一个问题，为什么与数据库链接不写在这个函数里？很明显，如果连续进行多次query的话，会多次新建connecion变量造成浪费...当然如果把connection写在这个函数里也不会有错，但是为了节约资源，我们将在service中建立connection，并在service中也需要关闭connection(其实有点麻烦...)
```

到此，DAO层的JDBC实现已经总结完毕

没完，关于实现事务的回滚

​	在MySQL中支持TRANSACTION以开启事务，在JDBC中，可以用Connection对象关闭Autocommit并在执行完所有MySQL的CRUD操作后再调用Connection.commit()进行事务提交，若过程中catch到error则调用Connection.rollback()进行事务回滚。

​	可是MySQL中，DAO层、Service层的各个函数都会自己创建Connection去完成连接，那么调用的回滚就会失败。解决方法是用ThreadLocal共享键值对，以统一同一个Connection。

```java
//因此我们可以这样改写DBUtils类和getConnection方法
class DBUtils{
    vars....;
    ThreadLocal<Connection> threadlocal=new ThreadLocal<>();
    
    public static Connection getConnection(){
        Connection connection=threadlocal.get();
        if(connection==null){
            connection = apply a connection;
            threadlocal.set(connection);
        }
        return connection;
    }
}
```

这样即可保证在同一线程中取得的connection都为同一个connection，从而很好地完成事务控制。

同时还可以对DBUtils做这样一个优化:

​	在DBUtils里加入begin()  commit()  rollback()三个方法，将connection的申请、关闭、从LocalThread中移除 都封装在内，这样在service中调用Dao层的时候可以屏蔽所有的connection。