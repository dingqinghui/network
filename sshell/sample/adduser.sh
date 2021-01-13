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
