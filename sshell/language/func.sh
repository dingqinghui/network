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

