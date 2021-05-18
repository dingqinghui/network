# zookeeper

## 一.简介

### 1.概述

用于分布式管理的工具，文件系统+通知机制

### 2.特点
####  1.集群可用
一领导者，多跟随者。集群中<u>**半数以上**</u>的服务可用，则整个集群可用。

####  2.强一致性，
访问任意服务获取到的数据都是最新的。

####  3.顺序性
同一个客户端发送的消息，按照顺序处理。

####  4.事务性
客户端的请求要么成功要么失败


### 3.数据结构
数据结构模型与UNIX文件系统类似，每个节点为ZNODE，ZNODE默认可以存储1M数据，使用路径作为节点的唯一标识。


## 二.安装配置
### 1.单机安装配置

#### 1.下载
直接下载个bin包就行了。没有安装jdk的需要先安装jdk
`wget https://zookeeper.apache.org/releases.html`
`tar -xvfz apache-zookeeper-3.6.2-bin.tar.gz`
#### 2.更改配置文件
`cd  ./apache-zookeeper-3.6.2-bin/conf`
`cp zoo_sample.cfg  zoo.cfg `
#### 3.启动
`cd  ./apache-zookeeper-3.6.2-bin/bin`
`./zkServer.sh  start`

### 2.集群安装配置
####  1. 复制文件夹
 ``mkdir zkcluster`
 `cp apache-zookeeper-3.6.2-bin ./zkcluster/zk1`
 `cp -r apache-zookeeper-3.6.2-bin ./zkcluster/zk1`
 `cp -r apache-zookeeper-3.6.2-bin ./zkcluster/zk2`
 `cp -r apache-zookeeper-3.6.2-bin ./zkcluster/zk3``

####  2.创建数据文件夹

 ``mkdir ./zkcluster/zk1/data`
 `mkdir ./zkcluster/zk2/data`
 `mkdir ./zkcluster/zk3/data``

#### 3.添加myid文件

 ``touch ./zkcluster/zk1/data/myid`
 `touch ./zkcluster/zk2/data/myid`
 `touch ./zkcluster/zk3/data/myid`
 `echo "1" > ./zkcluster/zk1/data/myid`
 `echo "2" > ./zkcluster/zk2/data/myid`
 `echo "3" > ./zkcluster/zk3/data/myid``

#### 4.分别修改服务配置
修改：

`dataDir=/root/zkcluster/zk1/data`
`dataLogDir=/root/zkcluster/zk1/log`
`clientPort=2183`

添加：

`#1 2 3 代表myid中配置的`
`#第一个端口集群内部数据复制 第二个选举`
`server.1=0.0.0.0:2888:3888`
`server.2=0.0.0.0:2889:3889`
`server.3=0.0.0.0:2890:3890`

#### 5. 启动集群
 ``./zk1/bin/zkServer.sh start` 
 `./zk2/bin/zkServer.sh start` 
 `./zk3/bin/zkServer.sh start`` 

## 三.内部原理
### 1.选举机制
集群服务依次启动，当半数以上服务启动后，整个集群可用，选举一个id最大的服务作为领导者，其他服务以及后续启动的服务为跟随者。


### 2.节点类型
`1：PERSISTENT                                //  持久化节点`
`2：PERSISTENT_SEQUENTIAL       //  持久化排序节点`
`3：EPHEMERAL                                 //  临时节点`
`4：EPHEMERAL_SEQUENTIAL        //  临时排序节点`

永久节点：节点创建后会被持久化，只有主动调用delete方法的时候才可以删除节点。

临时节点：节点创建后在**创建者超时连接或失去连接**的时候，节点会被删除。**临时节点下不能存在字节点**。

排序节点：创建的节点名称后自动添加序号，如节点名称为"node-"，自动添加为"node-1"，顺序添加为"node-2"...。排序序号由父节点维护。


### 3.集群服务数量建议
zookeeper集群建议启动奇数个服务。例如5个服务，3个不可用整个集群不可用。6个服务，3个不可用整个集群也不可用。所以启动6个相当于浪费了资源。

### 4.数据写入流程
客户端向服务写入数据，如果服务不是领导者，则转发给领导者。
领导者广播给其他跟随者，跟随者回复给领导者是否写入成功。
领导者统计成功数量，如果超过一半的服务写入成功则，通知与客户端连接的服务写入成功。
服务发送结果给客户端通知结果。



## 四.CAP理论:

### 1.一致性(Consistency):
分布式系统中，数据副本在任意时刻都一一样的。也就是说不会获取到旧数据。
### 2.可用性(Availability):
客户端请求后一定时间内必须给出回应。
### 3.分区容错(Partition tolerance):
子网络之间可能无法通信，系统必须在这种情况下继续运行。

**CAP不可能同时满足。分区容错是不可避免的。所以只能存在CP、Ap。**



