local skynet = require "skynet"



local CMD = {}


local index = 0


--[[
    29（时间戳） +  (区号 + 机器号(标识进程)) 12 + (功能ID) 5 + 自增ID  
]]


function CMD.uuid(id)
    local tm = math.floor(skynet.time() * 100 )
    local token = tm << 22
    token = token | ( (id or 1) <<  24 )
    token = token | index 
    index = index + 1
    return token
end



skynet.start(function ()
    skynet.dispatch("lua",function (session,source,cmd,...)
        local f = CMD[cmd]
        if f then 
            skynet.retpack( f(...) )
        end
    end)
end)