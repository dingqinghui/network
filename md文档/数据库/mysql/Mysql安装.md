客户端：
yum install mysql

下载mysql的repo源
wget http://repo.mysql.com/mysql-community-release-el7-5.noarch.rpm 

安装包
rpm -ivh mysql-community-release-el7-5.noarch.rpm

安装mysql
yum install mysql-server
yum install mysql-devel

启动服务
service mysqld start


开机自动启动
chkconfig --add mysqld 

修改初始密码
mysqladmin -uroot password '密码'

修改远程连接权限
GRANT ALL PRIVILEGES ON *.* TO 'root'@'%' IDENTIFIED BY 'tower@12356'；   
FLUSH   PRIVILEGES; 


如果连接不上查看端口是否开放




----------------------------------
vim  /etc/my.cnf 
添加 log-bin=/var/lib/mysql/mysql-bin

MySQL备份系列（2）-- mysqldump备份（全量+增量）方案操作记录
https://www.cnblogs.com/kevingrace/p/6114979.html