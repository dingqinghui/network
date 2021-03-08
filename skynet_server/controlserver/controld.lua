local skynet = require "skynet"
require "skynet.manager"
local utils = require "utils"
local control = require "control"
local errcode = require "errcode"
local comdefine = require "comdefine"

local SERVICE_NAME = comdefine.SERVICE_NAME

local control_ins

local CMD = {}


function CMD.init_user_list(userlist,nodename,host)
    local usermgr_ins = control_ins:getusermgr()
    if not usermgr_ins then 
        ERROR_LOG("玩家管理模块不存在")
        return errcode.MODULE_ERROR
    end 
    -- 保存玩家列表
    usermgr_ins:syc_user_list(userlist,nodename)
    -- 标记网关数据初始完成
    control_ins:gate_init_finish(nodename,host)
    DEBUG_LOG("初始化同步网关玩家数据完成 NODE：%s HOST：%s",nodename,host)
    return errcode.RT_OK
end

-- token 过期 / 玩家在网关上的实例销毁
function CMD.logout(uuid,nodename)
    local usermgr_ins = control_ins:getusermgr()
    if not usermgr_ins then 
        ERROR_LOG("玩家管理模块不存在")
        return errcode.MODULE_ERROR
    end 
    usermgr_ins:deluser(uuid)
    return errcode.RT_OK
end


function CMD.syc_verify_data(uuid,token)
    local usermgr_ins = control_ins:getusermgr()
    if not usermgr_ins then 
        ERROR_LOG("玩家管理模块不存在")
        return errcode.MODULE_ERROR
    end 

    -- 负载网关
    local nodename,host = control_ins:balance_gate(uuid)
    if not nodename or not host then 
        return errcode.NOT_AVAILD_GATE
    end 
     -- 保存玩家
     usermgr_ins:adduser(uuid,nodename)
    -- 同步信息到网关 BLOCK
    local ok = control_ins:sync_login_info_to_gate(nodename,uuid,token)
    if not ok then 
        ERROR_LOG(" 同步数据到网关 网关不可用 GATENAME:%s",nodename)
        return errcode.GATE_SERVER_ERROR
    end 

    return errcode.RT_OK,host
end



skynet.start(function ()
    utils.dispatch_lua(CMD)

    control_ins = control.new()

    skynet.register(SERVICE_NAME.CTR_CONTROL)
end)