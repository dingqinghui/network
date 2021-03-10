local agentserver = require "agentserver"
local skynet = require "skynet"

local client = require "client"
local user_mod = require "user"
local utils = require "utils"
local sermgr = require "sermgr"
require "enter_game_msg" 

local paramter = {...}

__USER__        = nil



local CMD  = {}
local handler = {}



function CMD.newuser(uuid,fd,token)
    assert(__USER__ == nil)
    assert(fd ~= nil)

    client.startping(fd)

    __USER__ = user_mod.new(uuid,fd,token,client)

    sermgr.send("hubd","lua","redirect",fd,skynet.self())

    DEBUG_LOG("新玩家上线： uuid：%d fd：%d",uuid,fd)
end


function CMD.assign(uuid,fd,token)
    assert(__USER__)
    assert(fd ~= nil)
    assert( __USER__:getuuid() == uuid )

    client.startping(fd)
    
    __USER__:setfd(fd)
    __USER__:settoken(token)

    sermgr.send("hubd","lua","redirect",fd,skynet.self())

    DEBUG_LOG("玩家重复上线： uuid：%d fd：%d",uuid,fd)
end


-- 释放AGENT
function CMD.release(uuid)
    if not __USER__ then 
        return 
    end 

    if uuid ~= __USER__:getuuid() then 
        return 
    end 
    -- 存档
    __USER__ = nil
end

-- 网络连接断开
function CMD.disconnect(fd)
    client.stopping(fd)
    if not __USER__ then 
        return 
    end 
    __USER__:setfd(nil) 
end



local handler = {}

-- PING 超时 关闭网络
function handler.closeclient(fd)
    sermgr.send("hubd","lua","closeclient",fd)
end


skynet.start(function ()
    
    sermgr.init({
        ["usermgrd"]   = paramter[1],
        ["hubd"]       = paramter[2],
    })

    utils.dispatch_lua(CMD)

    client.start(handler)
end)
