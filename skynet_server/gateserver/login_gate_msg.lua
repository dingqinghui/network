local skynet =  require "skynet"
local dbmgr = require "dbmgr"
local rediskey = require "rediskey"
local generator = require "generator" 
local errcode = require "errcode"
local msgimpl = require "msgimpl"
local client =  require "client"

local MSG = client.gethandler()

function MSG.login_gate(fd,message)
	local token = message.token
	local uuid = message.uuid

	-- -- 验证TOKEN
	-- local tokenkey = generator.keygen(rediskey.token,uuid)
	-- if dbmgr.exists(tokenkey) == 0 then 
	-- 	ERROR_LOG("tokent 不存在")
	-- 	return errcode.TOKEN_KEY_NO_EXIST
	-- end 
	-- local t = tonumber( dbmgr.get(tokenkey) )
	-- if  t ~= token then 
	-- 	ERROR_LOG("tokent 错误 t:%d %d",t,token)
	-- 	return errcode.TOKEN_ERROR
	-- end 

	-- -- 删除key
	-- dbmgr.del(rediskey.token)

	client.stopping(fd)

	-- 通知hub验证成功 uuid ， token
	skynet.call(__HUB__,"lua","authpass",uuid,fd)

	DEBUG_LOG("玩家：%d 登陆成功",uuid)
	return errcode.RT_OK
end


