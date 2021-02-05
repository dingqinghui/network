local skynet = require "skynet"
local msgimpl = require "msgimpl"
local msgdefine = require "msgdefine"
local dbmgr = require "dbmgr"
local rediskey = require "rediskey"
local generator = require "generator" 
local nodemgr  = require "nodemgr"
local errcode = require "errcode"


local C2L = msgdefine.C2L
local L2G = msgdefine.L2G
local C2GT = msgdefine.C2GT

local function on_login_gate_req(message)
	local token = message.token
	local uuid = message.uuid

	local response = { 
		id = L2G.LOGIN_GATE_RET,
		data = {
		} ,
		errcode = errcode.RT_OK
	}

	-- 验证TOKEN
	local tokenkey = generator.keygen(rediskey.token,uuid)
	if dbmgr.exists(tokenkey) == 0 then 
		response.result = errcode.TOKEN_KEY_NO_EXIST
		skynet.exit()
		return response
	end 
	
	if dbmgr.get(tokenkey) ~= token then 
		response.result = errcode.TOKEN_ERROR
		skynet.exit()
		return response
	end 


	return response
end


msgimpl.register(C2GT.LOGIN_GATE_REQ,on_login_gate_req)