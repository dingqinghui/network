
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