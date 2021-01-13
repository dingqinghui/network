#!/bin/bash 

#设置变量
host=192.168.2.199
user=root
password=dingqinghui

expect <<EOF
set var [lindex $argv 1]


spawn  ssh   $user@$host
expect {
    #匹配 {动作}
    "yes/no" {send "yes\n" ;exp_continue}   
    "*password" {send "$password\n"}
}
send "exit\n"
expect eof
EOF