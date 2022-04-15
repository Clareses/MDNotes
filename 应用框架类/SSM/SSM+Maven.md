# Maven

## 简介

Maven是一个**基于项目对象模型（POM）用于进行项目依赖管理与项目构建管理**的工具软件

### 如何加载jar包？

- 下载jar包
- 将jar包复制到WEB-INF\lib文件夹中
- 在开发环境中，将jar包add as library

### 传统导入jar包的问题

- 步骤多（繁琐）
- 不同项目中，Jar包将重复配置（重复、冗余）
- 在不同环境里Jar包的版本冲突问题（移植性差）

### 项目的生命周期

生命周期，即从编译到运行的整个过程

- 清理缓存、校验、编译、测试、打包、安装、部署

即为项目的整个构建过程。在集成开发环境中，可能不需要整个过程，但是集成开发环境仅提供一键构建，不够灵活。无现存工具去执行某一步骤

### 核心功能

1. 项目依赖管理
2. 项目生命周期管理
3. 聚合工程

## Maven项目结构

### 结构

Maven项目具有完全一致的项目结构，因此用Maven配置的项目可以在不同的开发环境间移植（比如用IDEA或Eclipse开发）

```
项目文件夹
--src
  --main（项目源文件）
  	--java（java代码）
  	--resources（动态或静态资源文件，如页面、图片、css等）
  --test（单元测试文件）
--pom.xml（项目对象模型）
```

### pom.xml

pom，即Project Object Model（项目对象模型）

pom.xml是依赖配置文件，maven根据此文件进行项目依赖管理

```xml
<?xml version = "1.0" encoding = "utf-8"?>
<project>
    
    <!--指定项目模型版本-->
	<modelVersion> 4.0 </modelVersion>
	
    <!--指定项目标识 G:企业标识，一般用包名 A:项目标识 V:版本标识 -->
	<groupId>     </groupId>
    <artifactId>  </artifactId>
    <version>     </version>
    
    <!--依赖配置-->
    <dependencies>
    	<!--dependency坐标在mvn网站上搜索复制即可-->
        <dependency>  </dependency>
    </dependencies>

</project>
```



## Maven的使用

### 项目生命周期管理

- mvn clean
- mvn check
- mvn compile
- mvn test
- mvn package
- mvn deploy



# MyBatis

## MyBatis部署

新建Maven项目后添加依赖

- MyBatis
- MySQL Driver

配置数据库连接

- 在resource文件夹中添加mybatis-conf文件

- 填入url、username、password等必要信息