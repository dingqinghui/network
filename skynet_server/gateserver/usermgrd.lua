local skynet = require "skynet"
local utils = require "utils"
local sermgr = require "sermgr"
require "skynet.manager"
local nodemgr = require "nodemgr"

local NODE_NAME = skynet.getenv("nodename")

local usermgr_mod = require "usermgr" 

local usermgr = nil

local paramter = {...}


local CMD = {}

-- from hub
function CMD.getagent(uuid,fd )
    if not usermgr then 
        return 
    end 
    local agent = usermgr:getagent(uuid,fd)
    assert(agent)
    nodemgr.send("word",".usermgr","login",uuid,NODE_NAME)
    return agent
end 

-- from agent
function CMD.kick(uuid)
    if not usermgr then 
        return 
    end 
    nodemgr.send("word",".usermgr","logout",uuid,NODE_NAME)
    usermgr:exit(uuid)
end 

-- from word
function CMD.getuserlist()
    return usermgr:userlist()
end 

skynet.start(function ()
    sermgr.init({
        ["hubd"]       = paramter[1],
    })
    usermgr = usermgr_mod.new()
    utils.dispatch_lua(CMD)
    skynet.register(".usermgr")
end )