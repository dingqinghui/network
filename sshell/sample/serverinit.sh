#!/bin/bash

##############################
#
#
# 服务器基础信息配置
# 
#
##############################

# 硬连接 不同文件名指向同一个inode， 软连接 数据块中存放指向的链接文件路径

#时间同步
if [ ! -e /etc/localtime ] 
then 
    ln -s /usr/share/zoneinfo/Asia/Shanghai  /etc/localtime 
fi

crontab -l  2>/dev/null | grep ntpdate
if [ $? -ne 0 ]
then
    (echo "* 2 * * * /usr/sbin/ntpdate ntp.aliyun.com 2>/dev/null" ; crontab -l 2>/dev/null)  | crontab
fi

# profile 默认的环境变量配置文件 作用于所有用户
#历史命令显示时间   grep 找到了选择的行，退出时状态值为0，否则为1
grep "HISTTIMEFORMAT" /etc/profile
if [ $? -eq 0  ]
then
   sed -i "/HISTTIMEFORMAT/d" /etc/profile
fi
echo 'export HISTTIMEFORMAT="%Y-%m-%d %H:%M:%S `whoami`  "' >> /etc/profile
source /etc/profile

#防火墙状态清空
#systemctl命令是系统服务管理器指令，它实际上将 service 和 chkconfig 这两个命令组合到一起。
#systemctl是RHEL 7 的服务管理工具中主要的工具，它融合之前service和chkconfig的功能于一体。可以使用它永久性或只在当前会话中启用/禁用服务。
#ConterOS7.0 以下
cat /etc/redhat-release  | egrep "CentOS Linux release [0-6]"
if [ $? -eq 0  ]
then
    service iptables stop   
else
    service firewalld stop
fi


#设置文件最大打开数量
grep "soft nofile" /etc/security/limits.conf
if [ $? -ne 0 ]
then
    # 追加信息
        cat >> /etc/security/limits.conf <<EOF
    soft nofile 65535
EOF
else
    # 替换 一行
    sed -i '/soft nofile/ c\* soft nofile 65535' /etc/security/limits.conf 
fi
grep "hard nofile" /etc/security/limits.conf
if [ $? -ne 0 ]
then
    # 追加信息
    cat >> /etc/security/limits.conf <<EOF
    hard nofile 65535
EOF
else
    sed -i '/hard nofile/ c\* hard nofile 65535' /etc/security/limits.conf 
fi

#swap关闭
swapoff -a

#内核优化

#系统性能分析工具安装
