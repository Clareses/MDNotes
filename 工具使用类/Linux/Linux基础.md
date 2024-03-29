# Linux基础

## 一、目录结构

![img](F:\MDNotes\_Images\d0c50-linux2bfile2bsystem2bhierarchy.jpg)

- **/bin**：

	bin 是 Binaries (二进制文件) 的缩写, 这个目录存放着最经常使用的命令。

- **/boot：**

	这里存放的是启动 Linux 时使用的一些核心文件，包括一些连接文件以及镜像文件。

- **/dev ：**

	dev 是 Device(设备) 的缩写, 该目录下存放的是 Linux 的外部设备，在 Linux 中访问设备的方式和访问文件的方式是相同的。

- **/etc：**

	etc 是 Etcetera(等等) 的缩写,这个目录用来存放所有的系统管理所需要的配置文件和子目录。

- **/home**：

	用户的主目录，在 Linux 中，每个用户都有一个自己的目录，一般该目录名是以用户的账号命名的，如上图中的 alice、bob 和 eve。

- **/lib**：

	lib 是 Library(库) 的缩写这个目录里存放着系统最基本的动态连接共享库，其作用类似于 Windows 里的 DLL 文件。几乎所有的应用程序都需要用到这些共享库。

- **/lost+found**：

	这个目录一般情况下是空的，当系统非法关机后，这里就存放了一些文件。

- **/media**：

	linux 系统会自动识别一些设备，例如U盘、光驱等等，当识别后，Linux 会把识别的设备挂载到这个目录下。

- **/mnt**：

	系统提供该目录是为了让用户临时挂载别的文件系统的，我们可以将光驱挂载在 /mnt/ 上，然后进入该目录就可以查看光驱里的内容了。

- **/opt**：

	opt 是 optional(可选) 的缩写，这是给主机额外安装软件所摆放的目录。比如你安装一个ORACLE数据库则就可以放到这个目录下。默认是空的。

- **/proc**：

	proc 是 Processes(进程) 的缩写，/proc 是一种伪文件系统（也即虚拟文件系统），存储的是当前内核运行状态的一系列特殊文件，这个目录是一个虚拟的目录，它是系统内存的映射，我们可以通过直接访问这个目录来获取系统信息。
	这个目录的内容不在硬盘上而是在内存里，我们也可以直接修改里面的某些文件，比如可以通过下面的命令来屏蔽主机的ping命令，使别人无法ping你的机器：
	
	```
	echo 1 > /proc/sys/net/ipv4/icmp_echo_ignore_all
	```

- **/root**：

	该目录为系统管理员，也称作超级权限者的用户主目录。

- **/sbin**：

	s 就是 Super User 的意思，是 Superuser Binaries (超级用户的二进制文件) 的缩写，这里存放的是系统管理员使用的系统管理程序。

- **/selinux**：

	 这个目录是 Redhat/CentOS 所特有的目录，Selinux 是一个安全机制，类似于 windows 的防火墙，但是这套机制比较复杂，这个目录就是存放selinux相关的文件的。

- **/srv**：

	 该目录存放一些服务启动之后需要提取的数据。

- **/sys**：

	这是 Linux2.6 内核的一个很大的变化。该目录下安装了 2.6 内核中新出现的一个文件系统 sysfs 。
	
	sysfs 文件系统集成了下面3种文件系统的信息：针对进程信息的 proc 文件系统、针对设备的 devfs 文件系统以及针对伪终端的 devpts 文件系统。
	
	该文件系统是内核设备树的一个直观反映。
	
	当一个内核对象被创建的时候，对应的文件和目录也在内核对象子系统中被创建。

- **/tmp**：

	tmp 是 temporary(临时) 的缩写这个目录是用来存放一些临时文件的。

- **/usr**：

	 usr 是 unix shared resources(共享资源) 的缩写，这是一个非常重要的目录，用户的很多应用程序和文件都放在这个目录下，类似于 windows 下的 program files 目录。

- **/usr/bin：**

	系统用户使用的应用程序。

- **/usr/sbin：**

	超级用户使用的比较高级的管理程序和系统守护程序。

- **/usr/src：**

	内核源代码默认的放置目录。

- **/var**：

	var 是 variable(变量) 的缩写，这个目录中存放着在不断扩充着的东西，我们习惯将那些经常被修改的目录放在这个目录下。包括各种日志文件。

- **/run**：

	是一个临时文件系统，存储系统启动以来的信息。当系统重启时，这个目录下的文件应该被删掉或清除。如果你的系统上有 /var/run 目录，应该让它指向 run。

## 二、Vim基础指令

基本上 vi/vim 共分为三种模式，分别是**命令模式（Command mode）**，**输入模式（Insert mode）**和**底线命令模式（Last line mode）**。 这三种模式的作用分别是：

### 命令模式

用户刚刚启动 vi/vim，便进入了命令模式。

此状态下敲击键盘动作会被Vim识别为命令，而非输入字符。比如我们此时按下i，并不会输入一个字符，i被当作了一个命令。

以下是常用的几个命令：

- **i** 切换到输入模式，以输入字符。
- **x** 删除当前光标所在处的字符。
- **:** 切换到底线命令模式，以在最底一行输入命令。
- **yy** 复制该行
- **p**  黏贴
- **dd** 删除该行

若想要编辑文本：启动Vim，进入了命令模式，按下i，切换到输入模式。

命令模式只有一些最基本的命令，因此仍要依靠底线命令模式输入更多命令。

### 输入模式

在命令模式下按下i就进入了输入模式。

在输入模式中，可以使用以下按键：

- **字符按键以及Shift组合**，输入字符
- **ENTER**，回车键，换行
- **BACK SPACE**，退格键，删除光标前一个字符
- **DEL**，删除键，删除光标后一个字符
- **方向键**，在文本中移动光标
- **HOME**/**END**，移动光标到行首/行尾
- **Page Up**/**Page Down**，上/下翻页
- **Insert**，切换光标为输入/替换模式，光标将变成竖线/下划线
- **ESC**，退出输入模式，切换到命令模式

### 底线命令模式

在命令模式下按下:（英文冒号）就进入了底线命令模式。

底线命令模式可以输入单个或多个字符的命令，可用的命令非常多。

在底线命令模式中，基本的命令有（已经省略了冒号）：

- q 退出程序
- w 保存文件

按ESC键可随时退出底线命令模式。

简单的说，我们可以将这三个模式想成底下的图标来表示：

![img](F:\MDNotes\_Images\vim-vi-workmodel.png)

## 三、用户管理相关

### 用户基础操作

- **新建用户**  useradd （-g 组名） 用户名

- **删除用户**  
	- 删除但保留home   userdel 用户名
	- 删除且不保留home  userdel -r 用户名
- **查询用户**  id 用户名
- **查看当前用户**  who am i
- **退出当前登录**  logout
- **切换用户**  su 用户名
- **设置密码**  pwd 用户名 （不写用户名默认给自己设置）

### 用户分组操作

- **新增组**  groupadd 组名
- **删除组**  groupdel 组名
- **修改用户组**  usermod -g 组名 用户名



### 用户与组相关文件

- /etc/passwd 存放用户密码
- /etc/shadow 口令配置文件
- /etc/group   组配置文件



## 四、文件管理

- mkdir （-p） 文件夹名  （-p是创建多级文件夹）  **新建文件夹**
- touch  文件名  **新建文件**

- rmdir  **删除文件夹**
- rm -rf  **强制删除整个文件夹或文件**

- cp（-r）文件名  目标位置  	**复制文件到目标位置**（如果-r则递归复制整个文件夹）
- mv  旧文件名  新文件名   **重命名**
- mv 原文件位置  新位置     **移动文件**
- cat 文件位置  **查看文件**

- |more  **查看文件的管道命令**

- less 文件名  **用less查看文件（pageup、pagedown翻页）**

| 命令    | 说明                          |
| ------- | ----------------------------- |
| pgup    | 上一页                        |
| pgdn    | 下一页                        |
| /字符串 | 向下查找      n下一个 N上一个 |
| ?字符串 | 向上查找     同上             |
| q       | 离开less                      |

- echo 选项  内容    **输出到控制台**

- head  文件 **显示头部内容**

- tail 文件  **显示尾部内容**
- \>   **覆盖输入**
- \>>  **尾部追加输入**
- ln -s 文件位置  快捷方式名   **创建快捷方式**
- history  **查看历史操作**
