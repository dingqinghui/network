local agentserver = require "agentserver"
local login_msg = require "login_msg"
local skynet = require "skynet"
local fileutile = require "fileutile"

-- 加载消息
fileutile.load_dir("./login/msghandler")

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


