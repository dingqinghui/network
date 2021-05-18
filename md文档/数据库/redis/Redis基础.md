# redis

## 1.基础数据类型

### 1.1 string
setex 设定超时时间后 重新set 超时时间会失效
key命名规则：表名：主键名：主键值：字段名

### 1.2 list 
底层实现：双向链表
blpop：阻塞取 等待timeout
应用场景：多用于队列等

### 1.3 set
底层：map存储 只使用key value为nil 
spop：随机并移除
srandmember：随机指定数量不移除
集合 交并补

### 1.4 注意
使用redis时应只用来保存数据 不进行数据的校验等业务逻辑操作，降低redis与业务的耦合性


## 2.启动基础配置
#### 2.1 启动
#####  2.1.1单机启动
redis-server -p 6379
#####  2.1.2 集群启动
先逐个启动单节点 然后通过redis-cli --cluster create 47.235.228.98:9001 47.235.228.98:9002  将节点连接成为集群

### 2.2配置
```
#### daemonize：是否后台启动
#### logfile：日志文件名

################ rdb相关 ########################
##### dir:日志文件 持久化文件目录
##### dbfilename:数据库文件名
##### rdbcompression：数据是否压缩 压缩会增加CPU压力 但是减少占用的磁盘空间
##### rdbchecksum：加载过程校验文件 防止文件损坏
################ rdb相关 ########################

#### maxmemory:最大物理内存大小 0不受限制
#### maxmemory-samples:数据逐出 每次选取检测的数量
#### maxclients:客户端最大连接数 达到上限关闭新连接
#### timeout:闲置等待时间 超时关闭连接  0关闭功能
#### include:继承配置文件
#### requirepass:密码
#### masterauth:从连主的 主密码
```
## 3.持久化

### 3.1 rdb
将内存中所有数据进行存档一次
#### 3.1.1触发条件
自动执行：
save second change  n秒m个key 发生变化则进行存储（在配置文件中配置）
使用bgsave 进行保存
save：save指令阻塞线程，直到rdb完成，不建议使用
bgsave： redis fork 子进程 进行rdb 不会阻塞
shutdown save
#### 3.1.2优点
二进制文件 存储率高
恢复速度快
用于容灾定时备份
#### 3.1.3缺点
容易丢失数据
ork子进程处理 牺牲性能

### 3.2 aof
使用文件记录每次的操作记录，用于数据恢复。
#### 3.2.1 aof文件写入策略
always：每次写入 每条执行都要执行磁盘写入操作 效率低 准确性高
eversec：每秒写入一次  宕机丢失一秒数据 推荐
no： 系统控制写入时机  不可控 不推荐

#### 3.2.1 配置
appendonly：是否开启
appendfsync：aof同步策略
#### 3.2.3 aof文件重写
整理aof文件 将多条指令操作合并成一条最终指令，降低aof文件大小，提高数据恢复效率
重写方式：
	手动重写
	bgrewriteaof
	自动重写

### 3.3 混合持久化
结合rdb 和 aof，每次进行重写时，保存重写时内存的rdb内容和增量aof命令。每次当aof文件大小超过配置限制或内存增大n倍时触发重写。
使用混合持久化可以加快redis恢复数据时间，并最大程度减少数据的丢失。

## 4.事务
### 4.1 语法
multi exec discard
### 4.2 注意
中有一个命令有语法错误 则事务自动被取消
执行错误 错误语句命令不会执行  其他命令可以执行
无回滚

## 5.主从复制
### 5.1.概述
一主多从，每台从服务器保存一份数据。主写，从读。主向从进行数据同步。
### 5.2作用
读写分离
热备份
宕机快速恢复

### 5.3 原理流程

##### 5.3.1 建立连接
slave主动与master建立连接，slave保存master ip port，master保存slave port。
##### 5.3.2 初始数据同步
全量复制：
slave->master:请求数据
master：执行bgsave，创建rdb快照。创建命令缓冲区，保存后续命令。
master->slave:同步rdb快照
slave：清空本地数据，执行rdb恢复数据。

增量复制：
slave->master:通知rdb完成
master->slave:同步命令缓冲区内容
slave：执行bgrewrite，恢复数据

注意事项
master：
命令缓冲区会满，导致命令丢失.重复进行全量复制，可能导致死循环
repl-backlog-size：命令缓冲区大小

slave：
多个salve同时请求同步，导致master带宽不足。
数据同步阶段，slave不能写。
slave-server-stale-data：yes | no

##### 5.3.3 命令同步
slave->master:replconf ack runid offset
master:runid不同或offset不在命令缓冲区中执行全量复制。否则发送发送缓冲区中内容和offset
master->slave:缓冲区内容+offset
##### 5.3.4 心跳机制
master配置：
repl-ping-slave-period 10：每十秒一次用于检测slave存活
min-slaves-to-write 2：slave存活搓两小于2时禁止master写
min-slaves-max-lag 8：slave时延都超过8禁止master写
slave：
每秒一次，发送offset，用于检测master存活，和数据同步

##### 5.3.5 runid，复制缓冲区,offset 
runid每次服务器启动时生成的值，用于标识一台redis服务器
服务器在使用aof持久化，或作为master时创建，先入先出的队列，存储服务器所有执行过的命令，并用序列号标识
offset命令缓冲区中的序列号

### 5.4问题
#### 5.4.1  频繁全量复制
master重启导致runid不同
解决：redis内部进行优化，master会保存runid+offset到rdb中
#### 5.4.2 offset不在缓冲区
解决：更改命令缓冲区大小

#### 5.4.3 数据不同步
网络延迟导致数据不同步，从不同slave中数据不一致
解决：部署同一机房


##  6.哨兵模式
### 6.1 作用
对主从模式，master宕机导致集群无法进行写操作。哨兵模式用于监控整个集群的状态，当主节点宕机时选举出新的master，并进行通知。
### 6.2 配置
详见sentinel.conf 文件
###  6.3 原理
#### 6.3.1 初始
sentinel连接master，向master获取信息(slave sentinels等)
sentinel依据从master获取的信息，连接slave和其他sentinel
#### 6.3.2通知
经过初始阶段，各个sentinel直接相互连通。每个sentinel定时获取集群(master slave)中的工作状态信息,然后同步给其他sentinel。
#### 6.3.3故障转移
发现:
sentinel通过ping发现master断开连接，当有n个sentinel发现master断开连接则认为master宕机。
选举: 
sentinel之间选举出一个"领导者",处理这次宕机。
转移:
被选举出的sentinel依据算法从slave中选举一个作为master，并将新master信息通知给其他slave和master

##  7.集群
### 7.1 集群作用
扩展内存
分摊访问压力，负载均衡
### 7.2 配置：
cluster-enabled yes
cluster-config-file nodes-6379.conf  #非手动配置 节点启动后自动写入
cluster-node-timeout 15000

### 7.3 连接集群中的节点
redis-cli --cluster create 47.235.228.98:9001 47.235.228.98:9002 

### 7.4 原理
集群中每个节点相互连接，每个节点拥有一定的槽。节点直接相互交换槽信息。
当对key进行操作时，先计算key的槽值，然后向任意节点请求执行操作。如果是当前节点的槽则直接执行，否则通知客户端重定向到新的节点执行操作。

### 7.5 异常处理
#### 7.51 slave宕机
无影响
#### 7.52 master宕机
slave变为master，原master重启后变为slave

### 7.6 集群key命名
如果key中有{}那么只计算{}内的CRC16 进行槽位映射

## 8.问题 优化

### 8.1 缓存预热
原因：redis宕机后失去所有数据，导致数据库压力增大
解决： redis持久化热点数据

### 8.2 缓存穿透
原因：redis只缓存底层数据库查询到的数据。数据库中不存在，则每次请求这个数据都要到数据库中查找，redis失去作用
解决：
redis缓存数据库中不存在的数据，设置一个过期时间
建立一个bitmap，将要查询的数据进行hash，保存bitmap中。通过bitmap进行拦截。

### 8.3 缓存击穿
原因： 某个数据过期删除后，短时间内大量的这个数据的请求到来，导致数据库压力增大
解决：过热数据永不过期

### 8.4 缓存雪崩
原因：大量key短时间内到底删除，导致数据库压力增大
解决：过期时间设置随机分散

### 8.5 总结
redis的作用是作为底层数据库的"保护伞"。当保护伞失效，就会引发底层数据库压力剧增。因为这些问题的情况，无非就是缓存过期/不存在。

## 9.工具
### 9.1 monitor 性能检测测试
### 9.2 redis-benchmark   压测工具
### 9.3 slowlog  慢日志查询

## 参考资料
### 思维导图
https://www.processon.com/view/link/5e73239ee4b03b9965240e6a#map
### REDIS 五中数据类型的底层实现
https://www.cnblogs.com/MouseDong/p/11134039.html
https://www.jianshu.com/p/fe2b4b9387b9
###  REDIS集群配置讲解
https://baijiahao.baidu.com/s?id=1643461909684941861&wfr=spider&for=pc
https://www.cnblogs.com/vieta/p/11192137.html
https://www.zhihu.com/question/21419897
https://www.jianshu.com/p/ee1ccbddb020
### 数据持久化
https://www.cnblogs.com/chenliangcl/p/7240350.html
###  MYSQL+REDIS数据一致性
https://www.zhihu.com/question/319817091
https://my.oschina.net/jiagouzhan/blog/2990423
###  阿里云 Redis 开发规范深入解读
https://zhuanlan.zhihu.com/p/108645085
###  一致性哈希和哈希槽对比
https://www.jianshu.com/p/4163916a2a8a
###  Redis Cluster 集群扩容与收缩
https://blog.csdn.net/men_wen/article/details/72896682
