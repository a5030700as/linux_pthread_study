  makefile 
  先去理解 GCC G++编译器 这些 -c -o -I -l -g 相关的指令说明
  以及 静态库 动态库的 概念 
  
  然后再去看一下makefile 比较好一点
  
  首先，对于程序而言 编译 链接 两步 对于 库而言 编译 打包
  
  为什么采用编译分开呢？ 因为多个程序 用一个源文件的时候 不需要重新编译 浪费时间
  
  第二 .o的格式是内存四区的格式吧？ 链接程序的时候 最小的链接单位是 模块 不是库 也不是函数 是.o文件 按照内存四区的格式去链接
  
  第三 动态库 rpath的意思 是运行时候路径 是指运行的时候动态库的存储路径 和 链接程序的时候 链接的动态库 往往是一个 程序的头部信息 包含了动态的名字 ldd readelf
  程序的加载应该是 先加载动态库 然后加载 程序main这个过程 makefiel里面需要添加 -Wl,-rpath=....的操作咯
  
  第四 .o文件 链接的时候 是提供 符号而已。。。。
  
  =============================================================
  第一 通过mk可以修改linux内核的启动脚本 这个很重要
  
  第二 文件系统也是要挂载的 可以指定挂载的权限 比如只读
  
  =========================================================
  动态库？静态库？
  如何创建动态库？ 静态库？

========================================================================
使用ar r 创建 静态库
ar -r libtest.a my_add.o my_sub.o
结果：
libtest.a  main.c  main.o  Makefile  mian.c  my_add.c  my_add.o  my_sub.c  my_sub.o
然后
重新编译 main.c 连接时候 指定链接库 相当于 指定 .o这样而已 

方式1：指定连接库名称 
	gcc -c main.c
	gcc -o main main.o libtest.a 
方法二： 指定路径 指定库
	gcc -o main main.o -L. -ltest
如果使用 -l小写 那么就是 要去掉 前缀 和 后缀了

=======================================================
===================================================================================
现在实验动态库

共享库 使用的格式 和 m进程格式 也就是 ELF格式
可以看成是一个 小 线程了 
编译的时候 需要了解 两步  一个是 位置无关码的编译选项，运行时候的加载

gcc -fPIC my_sub.c my_add.c -shared -o libtest.so //一句话实现 不分步

gcc -o main main.o libtest.so //可以成功 

尝试着运行 ./main 
发现报错：
	./main: error while loading shared libraries: libtest.so: cannot open shared object file: No such file or directory
-------------------------------------------------------
因为 只是 生成了 动态库  
但是 没有告知 动态连接器 
让程序 在运行的时候去连接 加载函数

方法 一： 把库放到 动态链接库的 默认目录 /lib 或者 /usr/lib 
方法 二： 追加 当前目录 到动态链接器的 默认目录表 中 （修改动态连接器的 环境变量 自己创建 LD_LIBRARY_PATH）
方法 三： 编译的时候 指定连接库目录  和 库文件 -Ldir -lname
 
 编译时候 指定路径 使用-wl，-rpath= 来指定动态库的搜索路径
 
-------------------------------------
	gcc -o main main.o -L. -ltest
	./main: error while loading shared libraries: libtest.so: cannot open shared object file: No such file or directory
	ldd main >>
	linux-vdso.so.1 (0x00007ffe7db10000)
	libtest.so => not found
	libc.so.6 => /lib/x86_64-linux-gnu/libc.so.6 (0x00007fda8d7d7000)
	/lib64/ld-linux-x86-64.so.2 (0x00007fda8dbc8000)

===============================================================================
总而言之： 
	动态库的实现是： 
	制作动态库：
	需要使用 位置无关码的方式(相对跳转) 生成 动态库 -fPIC选项
	
	使用动态库：
	编译的时需要指定 动态库，
	运行程序时候 需要确定 动态链接器 可以找到 动态库的位置 （ 也就是 要么 把动态库 放到指定的地点 要么给动态库创建一个环境变量 ）
=============================================================================

综上所述： 
  需要区分 你是创建库 (往库里添加 .o文件) 还是 使用 库==
  gcc -fPIC -c *.c
  gcc -shared *.o -o libMytest.so
  是对 .o文件 打包 -fipc是生成 位置无关码 -shred是生成一个共享库 
  
  与静态库不同的是，程序运行时需要动态库的支持，如果找不到相应的动态库，程序运行会报错。
  ================================================================================================================
  < targets ....> : < target-pattern > : < prereq-patterns ...>
    <commands>
  $(OBJS) : $(BULID_DIR)/A520/project/Standard/bulid/board/%.o: %.c
	$(CC) -D_GNU_SOURCE -DBULID_LIB $(BOARD_DEFINE) $(HA_DEFINE) $(CFLAGS) $(LIBS) $(INC) -c $(patsubst %.o,%.c,$(subst 
模式的意思就是 条件的意思;
=====================================================================================
    


================================
文件系统就是 一个工具管理了一些存储设备而已； 或者说 一套函数 去管理了一些存储设备而已；
有 open操作的 也有 f_open操作的
================================

================================
makefile

include 就是 C语言的展开形式， 只要里面没有 终极目标 ，数据信息就没有扫描完成。 
====================不要被名字给忽悠了 .config也是makefile==================
看形式 而不是看名字 就好像 xxx.sh是给程序员看的 编译器的视角 是里面的第一行 #!/bin/bash

makefile 变量同名冲突 默认是 覆盖的形式 除非使用了 +=的追加符号


=======================================================================================
嵌入式系统里面
busybox 是一个小程序 linuxrc->busybox 
系统命令都是 软连接 指向了 linuxrc 
利用 fork去执行相应的 系统命令 
175 ARCH ?= arm
 176 CROSS_COMPILE ?= arm-linux-
=======================================================================================

=============================================================================================
为什么要制作镜像？
	1 文件格式问题
	2 存储体积问题
	3 方便移植而已
============================================================================================
把文件系统的制作 和 文件系统镜像的制作都自动化处理， 并开放了接口， 这样用户就可以进行所谓的 二次开发了

文件系统镜像 = 文件系统（目录和文件） + 分区引导信息(什么格式的文件系统) 压缩算法 (可以理解为 文件系统的格式打包)
 
简单来讲 就是 制作内核镜像 之后 再去制作 文件系统镜像 ； 

文件系统里面 有内核启动的第一个 程序 以及相关的启动脚本， 
启动脚本里面 去完成用户的程序接口之类的，
这个就是二次开发了

用户通过 修改制作 文件系统镜像的脚本 就可以实现 用户的自启动程序了

这么一说 busybox只是一个 交叉工具 编译出来的 工具集 而不是 文件系统 ，然后 再使用 文件系统镜像制作工具 去制作文件系统镜像；
简而言之： busybox生成 文件系统的文件内容(各种命令 和 lib) 
			然后 文件系统制作工具 再对这些 项目文件 进行 打包处理 
		就是一个文件系统镜像了
		
会使用 dd 以及 df命令
==========================================================================================================

为什么需要根文件系统
(1)init进程的应用程序在根文件系统上
(2)根文件系统提供了根目录/
(3)内核启动后的应用层配置(etc目录)在根文件系统上。几乎可以认为：发行版=内核+rootfs
(4)shell命令程序在根文件系统上。譬如ls、cd等命令
总结：一套linux体系，只有内核本身是不能工作的，必须要rootfs（上的etc目录下的配置文件、/bin /sbin等目录下的shell命令，还有/lib目录下的库文件等···）相配合才能工作。



根文件系统的制作过程
      1).通过busybox工具,生成/bin、/sbin、linuxrc、/usr/bin、/usr/sbin.

      2).使用交叉编译工具链，构建/lib目录.

      3).创建其他空目录.

      4).配置etc目录,并初始化/dev目录.

      5).利用工具制作根文件系统映像文件.
  
 ======================================
  通过交叉编译工具readelf可查看我们的busybox应用依赖了哪些库文件
date "+%Y-%m-%d %H-%M-%S "
  =========================================================
  静态库 就是 .o的文件 集合 这个意思 就是说   不像 动态库的制作过程
  需要生成 位置无关码 然后生成 共享库 的打包指令 (共享库 需要指定一些头信心 符号 以及等等)

 程序执行的表现： 第一 静态编译的程序 就是  把需要用到的 模块加载进来而已
 而 动态库 编译的时候 只是把 动态库的 头信息给编译进去 (符号等等)， 运行的 时候 整个函数库 加载进内存然后 查找调用

 ===============================================================================================
 
 ==================================
.o  和 .h  和 .c 的关系 起码得理解了
=====================================
编译允许没有 实现 有个符号就可以，
但是连接不行 连接必须要有实现
==========================================

编译器 要知道 头文件 和 实现的位置 不然编译不出来

arm-linux-gnueabihf-gcc -o upgrade_daemo.o -c upgrade_daemo.c -I/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/arm-linux-gnueabihf/include -I./inc -I../../lib/include -I../../A520/lib/include -I../../HAPlatform/Code/Devices/Arch/Win32/Include
arm-linux-gnueabihf-gcc  -DWATCH_DOG_ENABLE -DA520 -I/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2014.04_linux/arm-linux-gnueabihf/include -I./inc -I../../lib/include -I../../A520/lib/include -I../../HAPlatform/Code/Devices/Arch/Win32/Include -o upgrade_daemo ./upgrade_daemo.o -lpthread -lrt -lproc_msg -L../lib/lib
./upgrade_daemo.o: In function `main':
upgrade_daemo.c:(.text+0x1a): undefined reference to `_WatchdogDevInit'
collect2: error: ld returned 1 exit status
Makefile:25: recipe for target 'upgrade_daemo' failed
make: *** [upgrade_daemo] Error 1
========================================================================================================

find . -maxdepth 9 -type d 这样就可以去确定每一个 目录了 

剩下就是 扫描这些目录的 .c文件进行编译？

SRC_DIR = 	../../app \
			../../../../firmware \
			../../../../bsp \
			../../../../../PlatformApp/DefaultApp \
			../../../../../HAPlatform/Code/StandardLib \
			../../../../../HAPlatform/Code/Devices \
			../../../../../HAPlatform/Code/UI \
			../../../../../HAPlatform/Code/Module/AppConfig \
			../../../../../HAPlatform/Code/Module/QRcode \
			../../../../../HAPlatform/Code/Module/CodeTest \
			../../../../../HAPlatform/Code/Module/MotionDetection \
			../../../../../HAPlatform/Code/Module/Media \
			../../../../../HAPlatform/Code/Module/RTC 
			
SRC_DIRS = $(foreach dir,$(SRC_DIR), $(shell find $(dir) -maxdepth 9 -type d))

SRC = $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
SRC_OBJ_FILES = $(patsubst %.c,%.o,$(SRC))

SRC_FILES = $(notdir $(SRC))
	  
OBJ_FILES = $(patsubst %.c,%.o,$(SRC_FILES))
OBJS = $(addprefix $(BULID_DIR)/A520/project/Standard/bulid/board/,$(SRC_OBJ_FILES))


