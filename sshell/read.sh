#/bin/bash

read -p "用户名:" -t 5000 username

read -p "密码:" -t 5000 -s password

echo "username:$username"
echo "password:$password"
