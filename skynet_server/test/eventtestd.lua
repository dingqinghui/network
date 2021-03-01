local skynet = require "skynet"
local eventmgr = require "eventmgr"

local eventname = "testevent"

local mc = require "skynet.multicast"

skynet.start(function ()


    eventmgr.subscribe(eventname,function (...)
        print("event call back ",...)
    end)
    eventmgr.subscribe(eventname,function (...)
        print("event call back ",...)
    end)

    eventmgr.publish(eventname,1,2,34,5)
    eventmgr.publish(eventname,1,2,34,5)

    eventmgr.unsubscribe(eventname)

end )
