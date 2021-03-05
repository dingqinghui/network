
local nodemgr = require "nodemgr"
local eventmgr = require "eventmgr"
local comdefine = require "comdefine"


local EVENT_NAME = comdefine.EVENT_NAME

local nodeclient =  __G_CLASS__(...)


function nodeclient:ctor(name,connect,disconnect)
    assert(name ~= nil )
    self.__name = name
    self.__isconnect = false
    self.__onconnect = connect
    self.__disconnect = disconnect
    --self:load()
end 


function nodeclient:dector()

end 

function nodeclient:start()
    -- 获取初始状态
    local status = nodemgr.status()
    for nodename,statu in pairs(status) do
        self:node_statu_change(nodename,statu)
    end

    -- 订阅状态改变事件
    eventmgr.subscribe(EVENT_NAME.CLUSTER_CHANGE,function (nodename,statu) 
        self:node_statu_change(nodename,statu)
    end)
end



function nodeclient:node_statu_change(nodename,statu)
    if self.__name ~= nodename then 
        return 
    end 
    if statu then 
        self:on_connect()
    else
        self:on_disconnect()
    end
end


function nodeclient:on_connect()
    self.__isconnect = true
    if self.__onconnect then 
        self.__onconnect()
    end 
end

function nodeclient:on_disconnect()
    self.__isconnect = false
    if self.__disconnect then 
        self.__disconnect()
    end 
end


function nodeclient:call(...)
    if not self.__isconnect then 
        return false
    end  

    return nodemgr.call(self.__name,...)
end


function nodeclient:send(...)
    if not self.__isconnect then 
        return false
    end  
    nodemgr.send(self.__name,...)
end

return nodeclient