local nodemgr = require "nodemgr"
local skynet = require "skynet"
require "skynet.manager"

local connect = {}

local function node_statu_change(nodename,statu)
    if not string.find(nodename,"gate") then 
        return 
    end 
    if statu then 
        connect[nodename] = 0
    else
        connect[nodename] = nil
    end
    skynet.error(string.format("node_statu_change node:%s status:%d",nodename,statu and 1 or 0))
end 


local CMD = {}

function CMD.node_statu_change(nodename,statu)
    node_statu_change(nodename,statu)
end 

function CMD.balance()
    local min =nil
    local gatename = nil
    for nodename,cnt in pairs(connect) do
        if not min or min > cnt then 
            min = cnt
            gatename = nodename
        end 
    end
    if not gatename then 
        return nil
    end 
    connect[gatename] = connect[gatename] + 1
    return gatename
end 


skynet.start(function ()
    skynet.dispatch("lua", function(session, source, cmd, ...)

			local f = assert(CMD[cmd])
			skynet.ret(skynet.pack(f( ...)))
    end)
    skynet.register(".gatemgr")
    -- 获取当前状态
    local status = nodemgr.status()
    for nodename,statu in pairs(status) do
        node_statu_change(nodename,statu)
    end
    -- 注册状态变化通知
    nodemgr.register(skynet.self())

end)

