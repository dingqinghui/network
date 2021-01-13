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
