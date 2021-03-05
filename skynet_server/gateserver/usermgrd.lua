local skynet = require "skynet"
local utils = require "utils"
local sermgr = require "sermgr"
require "skynet.manager"
local nodemgr = require "nodemgr"
local usermgr_mod = require "usermgr" 
local nodeclient = require "nodeclient"

__NODE_NAME__ = skynet.getenv("nodename")
local usermgr = nil

local paramter = {...}


__CONTROL_CLIENT__ = nil

local CMD = {}


local function control_connect()
    if not __CONTROL_CLIENT__ then 
        return 
    end 

    local vlist = sermgr.call("verifyd","lua","get_user_list")
    local ulist = usermgr:get_user_list()
    for _,uuid in ipairs(vlist) do 
        table.insert(ulist,uuid)
    end 
    local host  = sermgr.call("hubd","lua","gethost")  
    __CONTROL_CLIENT__:send(".controld","init_user_list",ulist,__NODE_NAME__,host)
end



-- from auth  验证通过管理agent
function CMD.assign(uuid,fd ,token)
    if not usermgr then 
        return 
    end 
    usermgr:user_login(uuid,fd,token)
end 

-- 客户端网络断开连接
function CMD.disconnect(fd)
    usermgr:disconnect(fd)
end


skynet.start(function ()
    utils.dispatch_lua(CMD)

    sermgr.init({
        ["hubd"]            = paramter[1],
        ["verifyd"]         = paramter[2],
    })

    usermgr = usermgr_mod.new()
    __CONTROL_CLIENT__ = nodeclient.new("controlserver",control_connect)
    __CONTROL_CLIENT__:start()
    
    skynet.register(".usermgr")
end )