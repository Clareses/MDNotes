## MySQL语法记录

##### 1.使用一个数据库	

```sql
USE <databasename>
```

##### 2.选择表中的数据

```sql
SELECT (DISTINCT) <dataname> or *（代表所有data） 
FROM  <tablename>  (WHERE  <condition>)  
(ORDER BY  <dataname> (DESC))  (LIMIT  (<offset> ,)  <lines> )
```

​	select子句会返回条件下查询出的内容，可能是一个数据或是一个表。这个数据或表也可以作为其他语句的条件内容。

##### 3.INNER JOIN

​	如果已经use了一个database，则可以省略一个database不写（也可以链接同一个数据库中的两个表）

```sql
<databaseA>.<tablename> DTA JOIN  <databaseB>.<tablename> DTB ON  DTA.<keyname> = DTB.<keyname> 
(condition,可以用逻辑运算符来处理)
```

​	同样，表可以做到self-JOIN，即用表某一列信息对应上另一列信息，生成连表（这需要给同一张表起两个不同的别名）

``` sql
<tablename> TA JOIN <tablename> TB ON TA.<keyname> = TB.<keyname>
```

​	支持连续JOIN

```sql
<table1> 
JOIN <table2> ON <condition>
JOIN <table3> ON <condition>
......
```

##### 4.OUTER JOIN

​	SQL在连接两个表时会自动剔除部分值为null的数据，这时候就需要OUTER JOIN来选择以哪张表为选择的标准

```sql
<table>
<type> JOIN <table> ON <condition> 
-- type为LEFT或RIGHT关键字，表示以上or下表为标准
```

​	USING子句可以替代ON

```sql
<tablename> JOIN <tablename> USING (<column1>,<column2>.....)
```

##### 5.SQL数据类型

​	文本、数字、时间

​	文本	 VARCHAR(<length>)		ENUM(<type>、<type>)  //枚举类型		BLOB //Binary Large Obj

​	数字	 INT		FLOAT		DOUBLE		DECIMAL //作为字符串存储的double

​	时间	DATE	  TIME		DATETIME

##### 6.插入语句

```sql
INSERT INTO
	<table> (<column1>,<column2>,<column3>...)
VALUES
	(<value1>,<value2>,<value3>......),
	(<value1>,<value2>,<value3>......),
	(<value1>,<value2>,<value3>......)...;
-- 如果是对该行的所有值都插入，并且每一个value都与column对应，那么可以省略插入列不写
-- value需要对应上表中数据的类型，主键可以用default表示，以防冲突
-- 插入的行数可选
```

​	插入分层行，即插入带亲子关系的表（即在父表与子表中插入有关联的数据，如订单表与订单商品表）

```sql
-- 关键在于如何确定父表插入的新的与子表关联的id（在订单案例中就是父表（订单id）与子表（订单商品表的订单id）如何确定。父表的id会auto increase，则子表要获取父表的新增订单的值，因此需要函数 LAST_INSERT_ID）
--e.g.
INSERT INTO orders (customer_id,order_date,status)
VALUES (1,'2021-10-10',1);
INSERT INTO order_items
VALUES (LAST_INSERT_ID(),1,1,2.95),
VALUES (LAST_INSERT_ID(),2,2,9.95);
--e.g.中为父表与子表分别添加了数据
```

```sql
-- 除去标准的输入插入值以外，select出的值也可以作为插入的VALUES
INSERT INTO <table>
SELECT subquery;
```

##### 7.创建表

```sql
-- 创建表
CREATE TABLE <tablename> AS 
--1.利用()，写出表定义
(<type name>,<type name>....);
--2.利用SELECT子句，对另一张表实现复制
SELECT subquery;
```

##### 8.更新表

```sql
UPDATE <tablename> 
SET <columnname1>=<value1>,<columnname2>=<value2>...
WHERE <condition>;
--这里的where condition决定了更新的行数
```

##### 9.删除表内容

```sql
DELETE FROM <tablename>
WHERE <condition>
```

##### 10.聚合函数

```sql
MAX();	MIN();	AVG();	SUM();	COUNT();
--聚合函数只对非空值起作用，空值将被忽略
```

##### 11.GROUP  BY 子句

group by子句可以对数据分组

```sql
SELECT client_id,SUM(invoice_total)
FROM orders 
GROUP BY client_id;
--统计每个client_id的invoice总和
```

##### 12.HAVING子句

WHERE子句用于在GROUP BY之前进行筛选，HAVING用于GROUP BY后筛选 (WHERE在这种情况下会报错)

```sql
SELECT SUM(<column>) AS total
FROM <table>
(WHERE <condition>)
GROUP BY <column>
(HAVING <condition>);
```

##### 13.ROLLUP子句

```sql
--只存在于MySQL中，用于统计每一列的总和
SELECT SUM(<column>) AS total
FROM <table>
(WHERE <condition>)
GROUP BY <column> WITH ROLLUP
(HAVING <condition>);
```

##### 14.触发器

​	触发器用于更新表中一些相关的操作，如更新了某员工表中员工薪资，另一张表中统计所有员工薪资的变量total也应该自动发生改变，使用trigger即可将它们绑定，在发生一个事件时自动执行另一个事件（但是要注意，trigger不可以作用于同一个表，否则会引起无限更新）

```sql
CREATE Trigger <Triggername> -- 一般命名方式为，发生变化的表名_BEFORE/AFTER_操作
	AFTER/BEFORE <action> On <tablename> --action的种类有INSERT/DELETE/UPDATE 
	FOR EACH ROW --对发生了action的行进行更新
BEGIN
	--code block
END;

--eg.
CREATE Trigger payment_after_insert
	AFTER INSERT On payment
BEGIN
	UPDATE invoice SET payment_total = payment_total + NEW.salary --NEW可以获取最近更新的一行
END;
```

##### 15.事务

​	事务可以保证一组SQL完全执行完毕才更新表，否则将回到所有语句都不执行的状态

```sql
START TRANSACTION;
--SQL EXPRESSION * n
COMMIT;
```

