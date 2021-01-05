#/bin/bash
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
