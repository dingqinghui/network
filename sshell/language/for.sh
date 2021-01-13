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
for i in `seq 1 10`
do
	echo $i
done

#文件
echo "for file ls *"
for i in `ls`
do
	echo "$i"
done 

