
local nodemgr = require "nodemgr"
local eventmgr = require "eventmgr"
local comdefine = require "comdefine"
local skynet = require "skynet"


local EVENT_NAME = comdefine.EVENT_NAME

local balance =  __G_CLASS__(...)


function balance:ctor(name,connect,disconnect)
    assert(name ~= nil )
    self.__name = name
    self.__onconnect = connect
    self.__disconnect = disconnect
    self.__nodelist = {}
    self:load()
end 


function balance:dector()

end 

function balance:load()
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



function balance:node_statu_change(nodename,statu)
    if not string.find(nodename,self.__name) then 
        return 
    end 
    if statu then 
        self:on_connect(nodename)
    else
        self:on_disconnect(nodename)
    end
    self.__nodelist[nodename] = statu
end


function balance:on_connect(nodename)
    self.__isconnect = true
    if self.__onconnect then 
        self.__onconnect(nodename)
    end 
end

function balance:on_disconnect(nodename)
    self.__isconnect = false
    if self.__disconnect then 
        self.__disconnect(nodename)
    end 
end


function balance:isconnect(nodename)
    return self.__nodelist[nodename]
end

function balance:transfer(func)
    for nodename,statu in pairs(self.__nodelist) do 
        if statu then 
            func(nodename)
        end 
    end 
end


function balance:call(nodename,...)
    if not self.__nodelist[nodename] then 
        return false
    end  
    print(nodename,...)
    return nodemgr.call(nodename,...)
end


function balance:send(nodename,...)
    if not self.__nodelist[nodename] then 
        return false
    end  

    nodemgr.send(nodename,...)
end

return balance