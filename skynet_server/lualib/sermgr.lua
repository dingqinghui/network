
local skynet = require "skynet"
local sermgr = {
    __list = {}
}

local self = sermgr 
function sermgr.init(list)
    self.__list = list
end 


function sermgr.call(sername,...)
    local service = self.__list[sername]
    assert(service,sername)
    return skynet.call(service,...)
end 

function sermgr.send(sername,...)
    local service = self.__list[sername]
    assert(service,sername)
    return skynet.send(service,...)
end 


function sermgr.get(sername)
    return self.__list[sername]
end 

return sermgr