##  一.变量

### 1.系统变量

### 2.自定义变量

####  2.1赋值 A=1

##### 等号两边不能有空格

##### 所有变量为字符串类型，不可直接进行算数运算

#### 2.2取消 unset A

####  2.3只读变量 `readonly unset A`

#### 2.4提升为全局变量 其他脚本中可用 `export A`

#### 2.5引用命令结果 `A=$(conmmond)`

### 3.特殊变量

#### 3.1$n   0 脚本名 1-n 脚本输入参数

#### 3.2$# 输入的参数个数

#### 3.3 $* $# 所有输入参数

当 $* 和 $@ 不被双引号`" "`包围时，它们之间没有任何区别，都是将接收到的每个参数看做一份数据，彼此之间以空格来分隔。

但是当它们被双引号`" "`包含时，就会有区别了：

- `"$*"`会将所有的参数从整体上看做一份数据，而不是把每个参数都看做一份数据。
- `"$@"`仍然将每个参数都看作一份数据，彼此之间是独立的。

#### 3.4 $?  上次执行的命令是否正确 0正确



###  4. 数组
```shell
#数组定义
array=(2 3 4 9 1)
#数组取值
echo ${array[0]}
#数组遍历
for val in ${array[*]}
do
   echo "$val"
done
#数组长度
echo ${#array[*]}
#数组合并
array2=(11 12 13)
array3=(${array[*]} ${array2[*]})
echo ${array3[*]}
#删除元素
unset array[1]
#删除数组
unset array3
```

## 二.运算符

### 1.语法

1.1 $(())   $[]  

1.2 expr + -   **\\***   /    

### 2.注意事项

**注意乘法是\\***

**expr 运算符间要有空格**

shell 运算符都是用整形处理。如果要处理浮点数使用 bc
```shell
# scale=2保留小数点两位
ratio=$(echo "scale=2; $use*100/$total" | bc)
# 复合运算
perrecv=$((($nrecv-$orecv)/1024))
```

## 三.条件判断

### 1.语法

[ condition ]

### 2.常用判断
#### 2.1 整数判断  -lt -gt -le -ge -eq -ne
#### 2.2 文件权限  -r -w -x
#### 2.3 文件类型 -f -d -e

### 3.多条件判断 && ||
[ 1 -lt 2 && 1 -lt 2 ]

### 3.注意
3.1 [ condition ] 要有空格
3.2 condition非空则为true

## 四.控制语句
### 1.if
```shell
#!/bin/bash
#if后边要有空格
if [ $1 -eq 1 ];then
        echo "you input is 1"
else
        echo "you input isn't 1"
fi

if [ $1 -eq 1 ]
then
        echo "you input is 1"
else
        echo "you input isn't 1"
fi

if [ $1 -eq 1 ]
then
        echo "you input is 1"
elif [ $1 -eq 2 ];then
        echo "you input is 2"
else
        echo "you input isn't 1"
fi
```
### 2.case
```shell
#!/bin/bash 
A=1
case $1 in
        "$A")
        echo "this is 1"
        ;;
        *)
        echo "other value"
        ;;
esac
```
### 3.for
```shell
#!/bin/bash
#基本写法1
echo "for base 1"
for((i=0;i<10;++i))
do
        echo "$i"
done
#基本写法2  这里不用(())
echo "for base 2"
for i in $@ 
do
        echo "$i"
done
#配合seq
echo "for seq"
for i in 'seq' 1 10
do
        echo $i
done
#文件
echo "for file ls *"
for i in 'ls'
do
        echo "$i"
done 

```

### 4.while
```shell
#/bin/bash
#基础写法
i=1
s=0
while [ $i -lt 100 ]
do
        s=$(($i+$s))
        i=$(($i+1))
done
echo $s
#读入文件,处理每行
filename=$0
while read line
do
        echo $line
done < $filename
#无限循环
while [ 1 ]
do
        echo "loop"
done
```
## 五.函数

### 1.系统函数

#### 1.1 basename

`[root@localhost sshell]#  basename   /etc/case.sh`
`case.sh`

`[root@localhost sshell]# basename   /etc/case.sh  .sh`
`case`

#### 1.2 dirname

`[root@localhost sshell]# dirname .`
`.`
`[root@localhost sshell]# dirname /etc/aaaaa/bbbb.sh`
`/etc/aaaaa`

### 2.自定义函数
```shell
#/bin/bash
#function和()可以不写
function sum()
{
        #通过$n引用函数参数
        local tal=$[ $1 + $2]
        #函数默认返回值 为最后一条命令执行的结果，返回值只能是[0-255]
        return $tal
}
read -p "please input paramter1:" a
read -p "please input paramter2:" b
#函数调用一定在函数定义之后
sum $a $b
#通过$?引用函数返回值
echo $?
```
## 六.工具函数

### 1. cut
```shell
# -d分隔符 -f列索引
cut -f1,2 -d":" /file.txt
```

### 2.  sed

工作流程：每次读取一行到缓冲区中，对缓冲区中的内容执行 commonds，缓冲区的内容用于打印，不会直接修改文件除非使用-i选项。

`sed -opt 'commond;commond;commond'`

```shell
########################删除行#########################
## commond = "行位置/d;行位置/d"

# 在删除中可以用1-$ 标识要删除的行，也可用正则表达式匹配删除的行
#删除最后一行
sed -i '$d' sed.file   
# 删除所有行
sed '1,$/d' sed.file
#删除5行到匹配的行
sed -i "5,/^$/d" sed.file   #删除空行
########################添加行#########################
#commond= '行位置 [i,a]/内容'

#3行前添加
sed '3i\this is a insert line' test.txt
#3行后添加
sed '3a\this is a insert line' test.txt

########################替换行内容#########################
## commond = "行位置 s/要替换的内容/替换成什么/每行的替换次数"

##2到4行 ，A=2替换成A=3， I 不区分大小写，替换三次` g替换所有
sed -i "2,4 s/A=2/A=3/3I" sed.file
#多个sed命令组合
sed -e 's/linuxunix/LINUXUNIX/g' -e 's/CentOS/RHEL8/g' sed-test.txt
#可使用正则表达式进行匹配

#整行替换
sed -e '/soft nofile/ c\root soft nofile 65535;/hard nofile/ c\root soft nofile 65535' /etc/security/limits.conf 

######################插入内容到行内###########################
# 删除空行 在1111前插入AAA 在1111后插入BBB   &标识引用前面的字段
sed '/^$/d;s/1111/AAA&/;s/1111/&BBB/'


```

### 3. awk

sed处理行数据，awk提供更加强大的列处理。

`awk [操作选项]  '[BEGIN {命令}] pattern{命令} [END{命令}]' filename`

BEGIN和END的命令分别在开始处理前和处理结束后执行一次。每行会先匹配pattern，成功匹配则执行命令。

```shell
#awk命令编程语法是借鉴C/C++ if while  for等。数组key可以是任意值，按照hash值进行排序

# -F指定列分隔符 NR当前处理的行 NF当前行列数
awk -F, 'BEGIN{print "id,shape"} \
NR>10&&NR<20 \
{\
if($1<1200)\
	print $1 "," $2;\
else \
	print $1 "," "优秀"\
}' awk.file 
# 定义变量total 做统计
awk -F, 'BEGIN{print "id,shape";total=0} NR>10&&NR<20{total +=$1} END{print "total:"total}' awk.file 

#数组  循环
awk -F, 'BEGIN{print "id,shape"} {print NR"," $1;total[NR]=$1} END{for(i in total){ print total[i]} }' awk.file 

awk -F, 'BEGIN{print "id,shape"} {print NR"," $1;total[NR]=$1} END{for(i=0;i<10;++i){ print total[i]} }' awk.file 

# 数组排序 
# slen = asort(a,b) 对a的值进行排序，排序后的索引存放到b 丢弃a的索引值 简单说就是排序value 丢失key
awk '{a[$1]=$2}END{slen=asort(a,b);for(i=1;i<=slen;i++) print i"\t"a[i]"\t"b[i]}' test 
# slen = asorti(a,b) 对数组a的下标进行排序，并把排序后的下标存入新生成的数组b中，并把数组a的长度赋值给变量l
awk '{a[$1]=$2}END{slen=asorti(a,b);for(i=1;i<=slen;i++) print i"\t"b[i]"\t"a[b[i]]}'

# 指定多个分隔符可以是一个正则表达式
awk -F ‘[-|]+’ ‘{print $3;}’ data
```

### 4. xargs

命令可以从标准输入，和命令行参数中获取数据。| 管道是将某个命令的标准输出作为另一个命令的标准输入。但是有些命令是只支持命令行参数。

`xargs` 将标准输入内容切割成多块作为命令的参数

```shell
# xargs [选项] [被执行命令] [被执行命令初始化参数]

# 从标准输入读入参数创建目录
xargs mkdir
# 管道读入
echo "test" | xargs mkdir
```
### 5.expect
处理交互命令，变为自动化工具
expect 脚本

```shell
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
```
shell 脚本
```shell
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
```



## 七.特殊字符展开规则
*:任意符合条件的字符
~:家目录
{}:  echo {a..z}-{a,b}
$：$USER
$((expr)):算数展开
$()：一个命令的输出作为输入
"":双引号中的字符串除了 $ \ `` 其他字符都进行原样输出 不进行展开。所以"" 禁用了{} * ~ 空格的展开
'':单引号 禁用所有的展开
\:阻止单个字符的展开 原样引用



##  杂项

### 一张图搞懂head tail

![这里写图片描述](https://img-blog.csdn.net/20170405211633859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2lnbmppbmc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

