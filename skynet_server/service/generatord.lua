local skynet = require "skynet"

local usertime = require "usertime"

local CMD = {}



local index = 0


--[[   
    snowflake算法：41（时间戳）+ 10 (nodeid) + 12 (自增索引)
]]

--64-41=15
function CMD.uuid(nodeid)
    local ms = usertime.getmilliseconds()
    index = (index + 1) % 0x3FF
    return (ms << 22) | (nodeid << 10) | (index)
end



skynet.start(function ()
    skynet.dispatch("lua",function (session,source,cmd,...)
        local f = CMD[cmd]
        if f then 
            skynet.retpack( f(...) )
        end
    end)
end)

