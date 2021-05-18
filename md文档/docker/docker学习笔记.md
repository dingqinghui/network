docker安装：yum install -y docker

docker 运行环境+配置问题的软件容器，

docker使用的是虚拟机技术，他只打包运行所需要的必要资源配置... 所以比虚拟机要轻量

镜像 容器  仓库

镜像/容器 = 模板(类)/实例(对象)

每个容器都是相互独立的运行环境+程序

仓库 = 镜像List（命名空间）  保存多个镜像

可以把容器看做一个简易版的linux环境

run：工作方式  查找容器 查找镜像  从仓库拉去


配置文件/etc/docker/daemon.json来使用加速器
mkdir -p /etc/docker
vim /etc/docker/daemon.json 
{
  "registry-mirrors": ["https://7u5xgmdl.mirror.aliyuncs.com"]
}
systemctl daemon-reload
systemctl restart docker


docker images  //显示本地所有镜像
docker rmi     //删除镜像
docker search  //仓库中搜索镜像
docker pull	   //拉去镜像到本地


docker ps    			//列出docker所有正在运行的实例  -a -l
docker run -it image    //创建镜像实例并以交互式方式运行  
docker start     		//启动容器
docker stop      		//关闭容器
docker restart   		//重启容器
docker rm        		//删除暂停的容器

docker run -d           // 以后台进程运行 docker机制只有后台会立刻杀死并退出

docker rm -f $(docker ps -aq)   //一次删除多个容器

docker run -it  -v  主机路径:容器路径:权限(ro)  镜像名

镜像是一个分层机构  分层是为了不同镜像之间可以共享资源

运行镜像 其实就是在镜像外部包裹一个可写的容器层 内部的镜像层是只读的

容器卷：在主机上提供一个目录挂在在容器上，从而实现多个容器的数据共享和容器数据的持久化


docker exec -it firgame_cehua /bin/bash


[0-9]{4}-[0-9]{8}
