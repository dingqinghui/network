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

	-- 验证TOKEN
	local tokenkey = generator.keygen(rediskey.token,uuid)
	if dbmgr.exists(tokenkey) == 0 then 
		return errcode.TOKEN_KEY_NO_EXIST
	end 
	if  tonumber( dbmgr.get(tokenkey) ) ~= token then 
		return errcode.TOKEN_ERROR
	end 

	-- 删除key
	dbmgr.del(rediskey.token)
	-- 通知hub验证成功 uuid ， token
	skynet.send(__HUB__,"lua","authpass",{
		uuid = uuid,
		token = token,
		fd = fd
	})

	client.stopping(fd)

	DEBUG_LOG("玩家：%d 登陆成功",uuid)
	return errcode.RET_OK
end


