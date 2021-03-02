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


local function newchannel(eventname,dispatch)
    local id  = skynet.call(self.__service,"lua","new",eventname)
    local channel = mc.new {
        channel = id,                                    -- 绑定上一个频道
        dispatch = dispatch                              -- 设置这个频道的消息处理函数
    }
    return channel
end

function eventmgr.publish(eventname,...)
    local channel = self.__chlist[eventname]
    if not channel then 
        channel = newchannel(eventname,nil)
        self.__chlist[eventname] =  channel
    end 
    DEBUG_LOG("触发事件  事件：%s",eventname)
    channel:publish(...)
end


function eventmgr.subscribe(eventname,func)
    local channel = self.__chlist[eventname]
    if not channel then 
        channel = newchannel(eventname,function (channel, source, ...) 
            func(...)
        end)

        channel:subscribe()
        self.__chlist[eventname] =  channel
        DEBUG_LOG("订阅事件 事件：%s",eventname)
    else
        DEBUG_LOG("不可重复订阅事件  事件：%s",eventname)
    end 
end


function eventmgr.unsubscribe(eventname)
    local channel = self.__chlist[eventname]
    if not channel then 
        return 
    end 
    self.__chlist[eventname] = nil
    channel:unsubscribe()
    skynet.send(self.__service,"lua","del",eventname)

    DEBUG_LOG("取订事件  事件：%s",eventname)
end




return eventmgr