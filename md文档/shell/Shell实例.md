## 1.批量添加用户
```shell
#!/bin/bash 

##############################
#
#
# 批量添加用户
# 
#
##############################

while  read username 
do
    # 用户是否存在
    id -u "$username"
    if [ $? -eq 0 ]; then
        echo "$username exist"
        continue
    fi
    
    echo $username
    # 创建用户
    adduser $username
    # 生成密码
    pw=$(echo $username| md5sum  | cut -c2-8)
    echo $pw
    # 修改密码
    echo $pw | passwd $username --stdin
    # 保存密码
    echo "$username $pw" >> newuser
done <userlist

```
## 2.网卡流量统计
```shell

#!/bin/sh 

##############################
#
#
# 网卡流量统计
# 
#
##############################

read -p "please input net interface name:" interface

while  true 
do

    content=$(cat /proc/net/dev | grep "^[ ]*$interface:" )
    if [ $? -ne 0 ]
    then
        echo "not exist $interface net interface  "
        read -p  "please input net interface name:" interface
        continue
    fi

    orecv=$(echo $content | awk -F'[: ]+' '{print $3}'   )
    osend=$(echo $content | awk -F'[: ]+' '{print $11}' )

    sleep 1s

    nrecv=$(echo $content |  awk -F'[: ]+' '{print $3}' )
    nsend=$(echo $content |  awk -F'[: ]+' '{print $11}')

    perrecv=$((($nrecv-$orecv)/1024))
    persend=$((($nsend-$osend)/1024))

    echo "$interface send:$persend KB/s  recv:$perrecv KB/s"
done
```
## 3.内存 CPU 使用最大的进程
```shell

#!/bin/bash

##############################
#
#
# 内存 CPU 使用最大的进程
# 
#
##############################

echo "########################内存排序#########################################"
ps aux --sort -%mem | head -10 | awk 'BEGIN{print "user" "\t" "pid" "\t" "mem" "\t" "commond" } NR>1&&$4>0 {print $1 "\t" $2 "\t" $4 "\t" $11}'    

echo "########################CPU排序#########################################"
ps aux --sort -%cpu | head -10 | awk 'BEGIN{print "user" "\t" "pid" "\t" "cpu" "\t" "commond" } NR>1&&$3>0{print $1 "\t"  $2 "\t" $3 "\t" $11}'    
```
## 4.服务器基础信息配置
```shell
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

```
## 5.CPU 内存 硬盘 TCP 统计信息
```shell
#!/bin/bash 

##############################
#
#
# CPU 内存 硬盘 TCP 统计信息
# 
#
##############################

function CPU()
{
    use=$(vmstat | awk 'NR==3{print $13+$14}')
    free=$(vmstat | awk 'NR==3{print $15}')
    iowait=$(vmstat | awk 'NR==3{print $16}')

    echo -e "Cpu:\t使用率：$use %\t空闲：$free %\tIO等待率：$iowait %"
}

function MEM()
{
    total=$(free -m | awk 'NR==2{print $2/1024}')
    use=$(free -m | awk 'NR==2{print $3/1024}')
    free=$(free -m | awk 'NR==2{print $4/1024}')
    
    ratio=$(echo "scale=2;  $use*100/$total" | bc)
    
    echo -e "内存:\t使用：$use\t空闲:$free\t总计:$total\t使用率：$ratio %"
}

function Disk()
{
    df -h | awk '/^\/dev/ {printf("磁盘:%s\t已用：%s\t空闲：%s\t总计：%s\t使用率：%s\t挂载点:%s\n",$1,$3,$4,$2,$5,$6) }'
}
function TcpStatus()
{
    A=$(netstat -apnt | awk 'NR>2{array[$6]=array[$6]+1}  END{for( key in array) { printf("%s-%s\t",key,array[key])}}'  )
    echo "TCP连接状态:$A"
}

CPU
MEM
Disk
TcpStatus

```

