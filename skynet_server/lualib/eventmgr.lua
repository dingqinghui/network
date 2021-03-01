local skynet = require "skynet"
local mc = require "skynet.multicast"
require "logger"

local eventmgr = {
    __service = nil,
    __chlist = {},
}

local self = eventmgr

skynet.init(function ()
    self.__service = skynet.localname(".eventd")
    if not self.__service then 
        self.__service = skynet.newservice("eventd")
    end 
end )


function eventmgr.publish(eventname,...)
    local channel = self.__chlist[eventname]
    if not self.__chlist then 
        return 
    end 
    DEBUG_LOG("触发事件  事件：%s  频道：%d",eventname,channel.channel)
    channel:publish(...)
end


function eventmgr.subscribe(eventname,func)
    local channel = self.__chlist[eventname]
    if not channel then 
        local id  = skynet.call(self.__service,"lua","new",eventname)
        channel = mc.new {
            channel = id,                       -- 绑定上一个频道
            dispatch = function (channel, source, ...)       -- 设置这个频道的消息处理函数
                func(...)
            end,  
        }
        channel:subscribe()
        self.__chlist[eventname] =  channel
        DEBUG_LOG("订阅事件 事件：%s",eventname)
    else
        DEBUG_LOG("不可重复订阅事件  事件：%s",eventname)
    end 
end


function eventmgr.unsubscribe(eventname)
    local channel = self.__chlist[eventname]
    if not self.__chlist then 
        return 
    end 
    self.__chlist[eventname] = nil
    channel:unsubscribe()
    skynet.send(self.__service,"lua","del",eventname)

    DEBUG_LOG("取订事件  事件：%s",eventname)
end




return eventmgr