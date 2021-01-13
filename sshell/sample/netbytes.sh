
#!/bin/sh 

##############################
#
#
# 网卡流量统计
# 
#
##############################

read -p "please input net interface name:" interface

while  true 
do

    content=$(cat /proc/net/dev | grep "^[ ]*$interface:" )
    if [ $? -ne 0 ]
    then
        echo "not exist $interface net interface  "
        read -p  "please input net interface name:" interface
        continue
    fi

    orecv=$(echo $content | awk -F'[: ]+' '{print $3}'   )
    osend=$(echo $content | awk -F'[: ]+' '{print $11}' )

    sleep 1s

    nrecv=$(echo $content |  awk -F'[: ]+' '{print $3}' )
    nsend=$(echo $content |  awk -F'[: ]+' '{print $11}')

    perrecv=$((($nrecv-$orecv)/1024))
    persend=$((($nsend-$osend)/1024))

    echo "$interface send:$persend KB/s  recv:$perrecv KB/s"
done



