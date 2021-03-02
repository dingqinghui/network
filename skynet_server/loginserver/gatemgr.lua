local nodemgr = require "nodemgr"
local skynet = require "skynet"
require "skynet.manager"
local utils = require "utils"
local eventmgr = require "eventmgr"
local comdefine = require "comdefine"

local EVENT_NAME = comdefine.EVENT_NAME

local gatemgr = {
    __connect = {}
} 
local self = gatemgr 
local function node_statu_change(nodename,statu)
    if not string.find(nodename,"gate") then 
        return 
    end 
    if statu then 
        self.__connect[nodename] = 0
    else
        self.__connect[nodename] = nil
    end
    DEBUG_LOG("node_statu_change node:%s status:%d",nodename,statu and 1 or 0)
end 



skynet.init(function ()
    -- 获取当前状态
    local status = nodemgr.status()
    for nodename,statu in pairs(status) do
        node_statu_change(nodename,statu)
    end

    eventmgr.subscribe(EVENT_NAME.CLUSTER_CHANGE,node_statu_change)
end)


function gatemgr.balance()
    local min =nil
    local gatename = nil
    for nodename,cnt in pairs(self.__connect) do
        if not min or min > cnt then 
            min = cnt
            gatename = nodename
        end 
    end
    if not gatename then 
        return nil
    end 
    self.__connect[gatename] = self.__connect[gatename] + 1
    return gatename
end 




