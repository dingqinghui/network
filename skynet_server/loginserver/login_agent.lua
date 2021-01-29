local agentserver = require "agentserver"
local login_msg = require "login_msg"
local skynet = require "skynet"

local CMD  = {}
local handler = {}

function handler.connect()


end

function handler.disconnect()

    skynet.exit()
end


function handler.commond(cmd,...)
    local f = CMD[cmd]
    if f then 
        return f(...)
    end 
end



agentserver.start(handler)