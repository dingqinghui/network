local skynet =  require "skynet"
require "skynet.manager"
local client =  require "client"
require "login_gate_msg"
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

    local err = nodemgr.call("controlserver",".controld","verify_token",uuid,token)
    if err ~= errcode.RT_OK then 
        return err
    end 
	
	client.stopping(fd)

	-- 通知hub验证成功 uuid ， token
	skynet.send(__HUB__,"lua","authpass",uuid,fd)

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
    skynet.send(__HUB__,"lua","closeclient",fd)
end


skynet.start(function ()
    sermgr.init({
        ["hub" ]= paramter[1],
    })
    utils.dispatch_lua(CMD)
    client.start(handler)

    skynet.register(".auth")
end)



   

	


