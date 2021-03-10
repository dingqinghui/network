local skynet = require "skynet"
local comdefine = require "comdefine"

local SERVICE_NAME = comdefine.SERVICE_NAME

local redis


skynet.init(function()
    redis = skynet.uniqueservice("redisd")
end)



local redisop =  setmetatable({}, {__index = function(tab,key)
    local f = function (...)
        return skynet.call(redis,"lua",key,...)
    end
    return f
end}) 


return redisop

