#!/bin/bash 

##############################
#
#
# CPU 内存 硬盘 TCP 统计信息
# 
#
##############################

function CPU()
{
    use=$(vmstat | awk 'NR==3{print $13+$14}')
    free=$(vmstat | awk 'NR==3{print $15}')
    iowait=$(vmstat | awk 'NR==3{print $16}')

    echo -e "Cpu:\t使用率：$use %\t空闲：$free %\tIO等待率：$iowait %"
}

function MEM()
{
    total=$(free -m | awk 'NR==2{print $2/1024}')
    use=$(free -m | awk 'NR==2{print $3/1024}')
    free=$(free -m | awk 'NR==2{print $4/1024}')
    
    ratio=$(echo "scale=2;  $use*100/$total" | bc)
    
    echo -e "内存:\t使用：$use\t空闲:$free\t总计:$total\t使用率：$ratio %"
}

function Disk()
{
    df -h | awk '/^\/dev/ {printf("磁盘:%s\t已用：%s\t空闲：%s\t总计：%s\t使用率：%s\t挂载点:%s\n",$1,$3,$4,$2,$5,$6) }'
}
function TcpStatus()
{
    A=$(netstat -apnt | awk 'NR>2{array[$6]=array[$6]+1}  END{for( key in array) { printf("%s-%s\t",key,array[key])}}'  )
    echo "TCP连接状态:$A"
}

CPU
MEM
Disk
TcpStatus
