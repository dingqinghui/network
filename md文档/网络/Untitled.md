



proactor

线程 多线程



最大句柄解决办法

websocket

udp

tcp



阻塞  非阻塞

阻塞IO  同步IO

reactor ：多路复用 + 非阻塞IO   事件循环 事件回调处理逻辑

EPOLL_HUB :读写通道都关闭

EPOLLRDHUB:读通道关闭

write == -1 errno==epipe 写关闭

errno== EINTR   中断打断了系统调用

errno == ETIMEOUT  tcp探活包超时

探活和应用层心跳：

探活包实现在传输层，只能感知连接存活，不能感知连接可用。如传输层无法感知应用层程序死锁 、阻塞状态。

重传包优先级大于探活包，断点导致大量的包重传，无法及时发送探活包，不能及时发现连接断开。