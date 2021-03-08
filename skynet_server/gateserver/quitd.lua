local skynet =  require "skynet"
require "skynet.manager"
local utils = require "utils"
local timehelper = require "timehelper"
local comdefine = require "comdefine"

local SERVICE_NAME = comdefine.SERVICE_NAME
-- 管理服务器退出流程

local CMD = {}


function CMD.quit()
    INFO_LOG("服务器开始关闭 %s ......",timehelper.nowstr())

    -- 关闭网关监听
    local hub = skynet.localname(SERVICE_NAME.GATE_HUB) 
    if hub then 
        INFO_LOG("关闭网关")
        skynet.call(hub,"lua","close")      
        INFO_LOG("关闭玩家连接")
        skynet.call(hub,"lua","quitserver")      
    end 
    -- 清除玩家对象
    local usermgr = skynet.localname(SERVICE_NAME.GATE_USERMGR) 
    if usermgr then 
        INFO_LOG("玩家数据保存")
        skynet.call(usermgr,"lua","quitserver")      
    end 

    
    INFO_LOG("服务器关闭完成 %s ......",timehelper.nowstr())
end



skynet.start(function ()
    utils.dispatch_lua(CMD)

    skynet.register(".quitd")
end)




