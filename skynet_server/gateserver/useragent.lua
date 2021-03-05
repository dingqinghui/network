local agentserver = require "agentserver"
local skynet = require "skynet"

local client = require "client"
local user_mod = require "user"
local utils = require "utils"
local sermgr = require "sermgr"

local paramter = {...}

__USER__        = nil



local CMD  = {}
local handler = {}



function CMD.newuser(uuid,fd,token)
    assert(__USER__ == nil)
    assert(fd ~= nil)

    client.startping(fd)

    __USER__ = user_mod.new(uuid,fd,token,client)
    __USER__:login()

    sermgr.send("hubd","lua","redirect",fd,skynet.self())
end


function CMD.assign(uuid,fd,token)
    assert(__USER__)
    assert(fd ~= nil)

    client.startping(fd)
    
    __USER__:setfd(fd)
    __USER__:settoken(token)

    sermgr.send("hubd","lua","redirect",fd,skynet.self())
end



function CMD.release(uuid)
    if not __USER__ then 
        return 
    end 

    if uuid ~= __USER__:getuuid() then 
        return 
    end 

    __USER__ = nil
end


function CMD.disconnect(fd)
    client.stopping(fd)
end



local handler = {}


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
