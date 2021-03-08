local skynet =  require "skynet"
require "skynet.manager"
local client =  require "client"

local utils = require "utils"
local sermgr = require "sermgr"
local errcode = require "errcode"
local nodemgr = require "nodemgr"

local paramter =  { ... }
local CMD = {}
local MSG = client.gethandler()
local handler = {}

function MSG.login_gate(fd,message)
	local token = message.token
	local uuid = message.uuid
    -- 验证TOKEN
    local err = sermgr.call("verifyd","lua","verify_token",uuid,token)
    if err ~= errcode.RT_OK then 
        return err
    end

    sermgr.send("usermgr","lua","assign",uuid,fd,token)

    client.stopping(fd)
	DEBUG_LOG("玩家：%d 登陆成功",uuid)
	return errcode.RT_OK
end

	
function CMD.connect(fd)
    client.startping(fd)
end

function CMD.disconnect(fd)
    client.stopping(fd)
end

function handler.closeclient(fd)
    sermgr.send("hub","lua","closeclient",fd)
end


skynet.start(function ()
    sermgr.init({
        ["hub" ]        = paramter[1],
        ["usermgr" ]    = paramter[2],
        ["verifyd"]     = paramter[3]
    })
    utils.dispatch_lua(CMD)
    client.start(handler)
end)



   

	


