## 握手过程

![image-20210514114845818](https://i.loli.net/2021/05/14/iCEvbxkh45nfqL2.png)





## 数据传输过程

mss:建立连接时通知对端tcp一个包的最大长度

rwnd：对端可用接受缓冲区窗口大小，对端在ack时携带这个信息

cwnd：发送窗口大小，根据慢启动拥塞控制调整cwnd

滑动窗口大小：min(rwnd,cwnd)



1M数据传输过程

![image-20210514151459310](https://i.loli.net/2021/05/14/6Pmctup3s74dwyb.png)

![image-20210514151525065](https://i.loli.net/2021/05/14/6KgeJHAoWwOvdNZ.png)