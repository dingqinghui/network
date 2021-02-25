local agentserver = require "agentserver"
local gate_msg = require "gate_msg"
local skynet = require "skynet"

local client = require "client"
local user = require "user"

__USER__    = nil
__HUB__     = ...

local CMD  = {}
local handler = {}

--[[
    @desc: 验证通过
    author:{author}
    time:2021-02-23 17:19:11
    @return:
]]
function CMD.authpass(info)

    client.startping(info.fd)

    if  __USER__ and __USER__:getuuid() == info.uuid then 
        __USER__:enter_game()
       
    else
        __USER__ = user.new(info)
        __USER__:login(info,client)
    end 
end

--[[
    @desc: 网络断开
    author:{author}
    time:2021-02-23 17:19:06
    @return:
]]
function CMD.disconnect()
    client.stopping(__USER__:getfd())

end



local handler = {}

--[[
    @desc: 主动断开
    author:{author}
    time:2021-02-25 15:08:37
    --@fd: 
    @return:
]]
function handler.closeclient(fd)
    skynet.send(__HUB__,"lua","closeclient",fd)
end


skynet.start(function ()
    skynet.dispatch("lua", function(session, source, cmd, ...)
        local f = CMD[cmd]
        assert(f,cmd) 
        skynet.retpack( f(...) ) 
    end)

    client.start(handler)
end)
