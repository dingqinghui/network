
#!/bin/bash

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