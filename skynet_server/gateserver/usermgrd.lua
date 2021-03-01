local skynet = require "skynet"
local utils = require "utils"
local sermgr = require "sermgr"
require "skynet.manager"
local usermgr_mod = require "usermgr" 

local usermgr = nil

local paramter = {...}


local CMD = {}

-- from hub
function CMD.getagent(uuid,fd )
    if not usermgr then 
        return 
    end 
    return usermgr:getagent(uuid,fd)
end 

-- from agent
function CMD.kick(uuid)
    if not usermgr then 
        return 
    end 
    usermgr:exit(uuid)
end 



skynet.start(function ()
    sermgr.init({
        ["hubd"]       = paramter[1],
    })
    usermgr = usermgr_mod.new()
    utils.dispatch_lua(CMD)
    skynet.register("usermgr")
end )