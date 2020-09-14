NAME
       socket - create an endpoint for communication

SYNOPSIS
       #include <sys/types.h>          /* See NOTES */
       #include <sys/socket.h>

       int socket(int domain, int type, int protocol);

DESCRIPTION
       socket() creates an endpoint for communication and returns a descriptor.


	
		int socket(AF_INET, SOCK_STREAN, 0);
		
		现在编写一个 客户端的程序看看？
		
		
		第一步: 你要去打开网络设备 
		第二步: 你要去配置端口信息
		第三步：你要去发送信息
		
		程序逻辑要清楚
		现在到底是在客户端还是服务端
		
		配置这个
		IP PORT 和 协议
		--------------
		字节序处理函数： 主要用于 端口 

		#include <arpa/inet.h>
		uint32_t htonl(uint32_t hostlong);
		uint16_t htons(uint16_t hostshort);
		uint32_t ntohl(uint32_t netlong);
		uint16_t ntohs(uint16_t netshort);
		可以进行测试 
		测试一个正常的端口就好 一般都是 两个字节
		比如 
		uint16_t hostshort_port = 0x0102;
		uint16_t net_short = htons(hostshort_port);
		printf("net_short is %02x  hostshort_port is %02x\r\n", net_short, hostshort_port);
		--------
		IP地址转换函数  主要用于IP 地址转换
		#include <arpa/inet.h>
		int inet_pton(int af, const char *src, void *dst);
		const char *inet_ntop(int af, const void *src, char *dst, socklen_t size);
		-----------------
		接下来呢?
		
		-------
		nt socket(int domain, int type, int protocol);
		int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
		int listen(int sockfd, int backlog);设置最大监听上限
		int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);//三次握手之后返回
		int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
		
		
		------------------------------------------------------------------
		
		
		
		
		bind()的作用是将参数sockfd和addr绑定在一起，使sockfd这个用于网络通讯的文件描述符监听addr所描述的地址和端口号。
		
		struct sockaddr_in servaddr;
		bzero(&servaddr, sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		servaddr.sin_port = htons(6666);
		
		首先将整个结构体清零，然后设置地址类型为AF_INET，网络地址为INADDR_ANY，这个宏表示本地的任意IP地址，因为服务器可能有多个网卡，每个网卡也可能绑定多个IP地址，这样设置可以在所有的IP地址上监听，直到与某个客户端建立了连接时才确定下来到底用哪个IP地址，端口号为6666。
		
		==================================
		从现在的信息来讲： 
		服务器需要的是 暴露出自己的 IP和端口 以及 记录连接进来的客户端端口和地址
		客户端 要知道 服务器的端口和地址罢了
		
		====================================================================
		服务器需要的是 监听 描述符号 和 返回的 连接描述符号
		
		=====================================================================
		在学习socket API时要注意应用程序和TCP协议层是如何交互的： 应用程序调用某个socket函数时TCP协议层完成什么动作，比如调用connect()会发出SYN段 应用程序如何知道TCP协议层的状态变化，比如从某个阻塞的socket函数返回就表明TCP协议收到了某些段，再比如read()返回0就表明收到了FIN段
		
		====================================================
		测试命令：
		netstatus -
		-a或–all：显示所有连线中的Socket；
		-l或–listening：显示监控中的服务器的Socket；
		-t或–tcp：显示TCP传输协议的连线状况；
		-u或–udp：显示UDP传输协议的连线状况；
		-----
		netstat -a     //列出所有端口
		netstat -at    //列出所有tcp端口
		netstat -au    //列出所有udp端口 
		---------------
		
		tcpdump  这个命令可以好好看一看
		比如需要抓取目标主机是192.168.56.1，通过端口22的传输数据包：
		
		sudo tcpdump  -XvvennSs 0 -i eth0 tcp[20:2]=0x4745 or tcp[20:2]=0x4854
		
		--------------------------
		nc 命令 也可以 学一下
		一个经典的用法是端口扫描。比如我要扫描192.168.56.2主机1~100端口，探测哪些端口开放的（黑客攻击必备）：
		
		nc -zv 192.168.56.2 1-100 |& grep 'succeeded!'
		Connection to 192.168.56.2 22 port [tcp/ssh] succeeded!
		
		========================================================
		
		#include <netinet/in.h> //结构体
		#include <arpa/inet.h> //字节序函数 
		后面这个 包含了 前面的 #include <netinet/in.h> //结构体
		
		建立连接的那些 
		都是直接使用 
		网络字节序的数据去处理的
		
		=======================================================
		这些好像都是在 /usr/include/netinet/in.h --->htons();
		htonl(INADDR_ANY); 可以使用这个 ip
		man 了一下  
		NAME
       htonl, htons, ntohl, ntohs - convert values between host and network byte order

	   SYNOPSIS
       #include <arpa/inet.h>

       uint32_t htonl(uint32_t hostlong);

       uint16_t htons(uint16_t hostshort);

       uint32_t ntohl(uint32_t netlong);

       uint16_t ntohs(uint16_t netshort);

	   192.168.193.25:41356    192.168.193.25:9527 
		
		取地址的话
		&server_addr.sin_addr 和 server_addr.sin_addr.s_addr 是一样的
		但是 如果是成员的话就不一样了
		
		服务器获取 客户端信息失败？
		
		因为 accept 的第三个参数是 传入传出参数。
		--------------------------------------------------------------------------------
		
		cliaddr_len = sizeof(server_addr);
		connect_fd = accept(listen_fd, (struct sockaddr *)&client_addr, &cliaddr_len);
		
		----------------------------------------------------------------------------------
