#!/usr/bin/expect  
#注意第一行  这不是sh脚本 而是expect脚本 有自己的语法规则
################################################################
# expect中相关命令
# 　　spawn：
# 　　　　启动新的进程
# 　　send：
# 　　　　用于向进程发送字符串
# 　　expect：
# 　　　　从进程接收字符串
# 　　interact：
# 　　　　允许用户交互
# 　　exp_continue 
# 　　　　匹配多个字符串在执行动作后加此命令
################################################################

#引用参数
set var [lindex $argv 1]
#设置变量
set host 192.168.2.199
set user root
set password dingqinghui

spawn  ssh   ${user}@${host}
expect {
    #匹配 {动作}
    "yes/no" {send "yes\n" ;exp_continue}   
    "*password" {send "${password}\n"}
}

#登陆后执行命令
expect "*]#" { send "who\n" }

#结束ssh会话
send "exit\n"
#脚本结束
expect eof