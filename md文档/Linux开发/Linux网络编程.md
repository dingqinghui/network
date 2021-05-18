# Linux网络编程

# 1. Socket API
## 1.1选项
### 1.1.1 SO_REUSEADDR
端口释放后可立刻被重用(bind) server应在bind之前设定此选项
### 1.1.2 O_NONBLOCK
设定为非阻塞
### 1.1.3 TCP_NODELAY
关闭Nagle算法 TCP层不用等待数据积累到一定程度再发送
### 1.1.4 SO_LINGER
SO_LINGER 是否等待数据发送完成 
struct linger { 
int l_onoff //0=off, nonzero=on(开关) 
int l_linger //linger time(延迟时间) 
} 
l_onoff  l_linger 	close
0 			0 		立刻返回 数据发送完
1 			0 		立刻返回 丢弃数据的发送 发送RST
1		    >0      等待一定时间发送 超时丢弃发送RST
### 1.1.5 SO_SNDBUF & SO_RCVBUF
https://www.jianshu.com/p/755da54807cd
这两个选项可以不用调整 ，linux2.4 以后的版本 自动调整到合适的大小
查看版本：cat /proc/version



## 1.2 accpet
从未完成队列中取出一个连接请求，创建和参数相同属性的套接字，为套接字分配文件句柄，并返回句柄。
阻塞：
	无数据则一直等待(出错也返回)。
非阻塞：
	无数据：返回errno=EAGAIN


## 1.3 connect
阻塞:阻塞等待三次握手成功
非阻塞:
	errno=EINPROGRESS:
		三次握手未立刻完成，监听可写事件.
		可写，getsockopt SOL_SOCKET, SO_ERROR  SO_ERROR 为0 握手完成
	返回值 > 0: 握手立刻完成，连接成功


## 1.4 read
n = read()
n > 0：实际读取字节数量
n == 0：对端关闭 本端关闭即可
n < 0
	errno=EINTR：系统中断中 忽略
	errno=EWOULDBLOCK || EAGAIN
		阻塞：读取数据超时，无法确定连接断开还是网络波动 直接断开连接重连
		非阻塞：没有数据可读


##  1.5 write
n = write()
n > 0：实际写入字节数量
n == 0：对端关闭 本端关闭即可
n < 0：
	errno=EINTR：系统中断中 忽略
	errno=EWOULDBLOCK || EAGAIN
		阻塞：写入数据超时，无法确定连接断开还是网络波动 直接断开连接重连
		非阻塞：缓冲区已满



## 1.6 epoll

### 1.6.1 et(边缘模式)
只有数据到来时触发回调，所以一次性要把数据读完 也就是不断的read
阻塞模式：read只有在超时时会返回 如果使用了 会导致线程阻塞到read
非阻塞：无问题

### 1.6.2 lt(水平模式)
有数据可读/能写入，read/write 一次即可，数据读不完还会继续触发


### 1.6.3 事件
EPOLLERR：对端异常关闭后 本段进行读写操作时触发


# 2.tcp连接状态
## 2.1 CLOSE_WAIT
对端关闭连接 本端没有调用close
## 2.2 TIME_WAIT
主动关闭连接 等待2ml
解决办法：改内核参数

# 3. SIGPIPE信号
产生原因：向一个已经收到RST套接字 写入数据时 发生信号，默认处理终止进程
处理：signal(SIGPIPE, SIG_IGN)

# 4. TCP RST包
## 4.1 产生原因
connect一个不存在的端口
send一个已经关掉的连接
send一个已经死掉的连接
设置SO_LINGER选项
close一个接收缓冲区中还有数据的连接，会给对方发一个RST
## 4.2 收到RST包后的表现
TCP socket在任何状态下，只要收到RST包，即可进入CLOSED初始状态

值得注意的是RST报文段不会导致另一端产生任何响应，另一端根本不进行确认。收到RST的一方将终止该连接

## 4.3程序中表现
阻塞模型：内核无法主动通知应用层出错，只有应用层主动调用read()或者write()这样的IO系统调用时，内核才会利用出错来通知应用层对端RST
非阻塞模型：select或者epoll会返回sockfd可读,应用层对其进行读取时，read()会报错RST

# 5.自连接
同主机 两边同时发送SYN包，同时回应 导致两边都进入连接状态

# 6.测试相关

## 6.1 句柄泄露
cat /proc/进程PID/fd/

## 6.2 内存泄露
工具：valgrind
检测方法：valgrind --tool=memcheck --leak-check=full --log- file=memchk.log Yourprogame 
检测结果：
definitely lost: 确定有内存泄漏，表示在程序退出时，该内存无法回收，也没指针指向该内存(首地址)；
indirectly lost: 间接内存泄漏，比如结构体中定义的指针指向的内存无法回收；
possibly lost： 可能出现内存泄漏，比如程序退出时，没有指针指向一块内存的首地址了，但由其他某个指针能推算出首地址；
still reachable: 程序没主动释放内存，在退出时候该内存仍能访问到，比如全局 new 的对象没 delete，由于操作系统会回收，所以此类问题可忽略;

## 6.3程序热点函数性能分析
生成报告：perf record -g
解析报告：perf report –g


# 7.内存库
## 7.1 jemalloc
多线程有优势
### 7.2 dlmalloc
linux默认内存分配库 单线程性能比jemalloc好，多线程效率低下

# 参考资料

## 非阻塞 socket操作
https://blog.csdn.net/zlfing/article/details/83030007

## TCP 全连接队列满 应对
https://blog.csdn.net/myy1066883508/article/details/106570632
ss -ntl：连接队列最大值 当前值
netstat -s | grep overflowed：连接队列溢出次数
队列溢出导致 连接被丢弃 服务器连接数上不来

## epoll事件
https://blog.csdn.net/q576709166/article/details/8649911

## socke关闭
https://blog.csdn.net/iteye_10227/article/details/82546016?utm_medium=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param&depth_1-utm_source=distribute.pc_relevant_t0.none-task-blog-BlogCommendFromMachineLearnPai2-1.channel_param

## 网络编程本质
 https://www.cnblogs.com/ict-wangwei/archive/2013/03/27/2984826.html

## 定时器
https://www.zhihu.com/question/32251997/answer/55338639

## linux 内核定时器实现
https://blog.csdn.net/gettogetto/article/details/80535667
