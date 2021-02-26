local skynet = require "skynet"
local utils = require "utils"
local usermgr_mod = require "usermgr" 

local usermgr = nil

__HUB__ = ...

local CMD = {}

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
end 



skynet.start(function ()
    usermgr = usermgr_mod.new()
    utils.dispatch_lua(CMD)
end )