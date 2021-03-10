local ops = require("redisOps") --加载redis操作模块
local json = require("json")   --加载json模块
local db = require("dbOps") --加载数据库操作模块

local row = ops.rawRow()  --当前数据库的一行数据,table类型，key为列名称
local action = ops.rawAction()  --当前数据库事件,包括：insert、updare、delete

local key = "t_user:" .. row["uuid"]

if action == "delete" -- 删除事件
then
    ops.DEL(key) 
else
	local val = json.encode(row) -- 将result转为json
	ops.SET(key,val) -- 对应Redis的SET命令，第一个参数为key(支持string类型)，第二个参数为value
end