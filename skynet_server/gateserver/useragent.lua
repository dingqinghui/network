local agentserver = require "agentserver"
local skynet = require "skynet"

local client = require "client"
local user_mod = require "user"
local utils = require "utils"
__USER__        = nil

__USERMGR__     = select(1,...)
__HUB__         = select(2,...)

local CMD  = {}
local handler = {}

-- 验证通过赋值连接
function CMD.assign(uuid,fd)

    -- 开启心跳检测
    client.startping(fd)
    -- 重新登陆
    if  __USER__ then 
        assert(__USER__:getuuid() == uuid)
        local curfd = __USER__:getfd()
        if curfd == fd then 
            INFO_LOG("重复的登陆验证消息 uuid:%s fd:%d",uuid,fd)
        else
            if curfd == nil then 
                INFO_LOG("玩家重连 uuid:%s fd:%d",uuid,fd)
            else
                INFO_LOG("玩家顶号 uuid:%s fd:%d",uuid,fd)
                client.stopping(curfd)
                skynet.send(__HUB__,"lua","closeclient",curfd)
            end 
        end 
        __USER__:setfd(fd)
    else
    -- 新登陆
        __USER__ = user_mod.new(uuid,fd,client)
        INFO_LOG("新玩家登陆 :%s %d",uuid,fd)
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
    __USER__:setfd(nil)
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

    utils.dispatch_lua(CMD)

    client.start(handler)
end)
