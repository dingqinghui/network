local skynet = require "skynet"
require "skynet.manager"
local utils = require "utils"
local control = require "control"
local errcode = require "errcode"

local control_ins

local CMD = {}


function CMD.login(uuid,nodename)
    local usermgr_ins = control_ins:getusermgr()
    if not usermgr_ins then 
        ERROR_LOG("玩家管理模块不存在")
        return errcode.MODULE_ERROR
    end 
    usermgr_ins:adduser(uuid,nodename)
    return errcode.RT_OK
end

function CMD.logout(uuid,nodename)
    local usermgr_ins = control_ins:getusermgr()
    if not usermgr_ins then 
        ERROR_LOG("玩家管理模块不存在")
        return errcode.MODULE_ERROR
    end 
    usermgr_ins:deluser(uuid)
    return errcode.RT_OK
end


-- from login authd
function CMD.syc_verify_data(uuid,token)
    local verify_ins = control_ins:getverify()
    if not verify_ins then 
        ERROR_LOG("验证模块不存在")
        return errcode.MODULE_ERROR
    end 

    local usermgr_ins = control_ins:getusermgr()
    if not usermgr_ins then 
        ERROR_LOG("玩家管理模块不存在")
        return errcode.MODULE_ERROR
    end 

    -- 缓存验证信息
    verify_ins:syc_verify_data(uuid,token)

    local nodename ,host
    -- 未在线
    if not usermgr_ins:isonline() then 
        if not verify_ins:verifying(uuid) then 
            nodename ,host = control_ins:balance_gate()
            verify_ins:setgate(uuid,nodename)
        else
            -- 验证中
            nodename = verify_ins:getgate(uuid)
            if control_ins:gate_invaild(nodename) then
                host = control_ins:get_gate_host(nodename)
            else
                -- 登陆的网关不可用 
                nodename ,host = control_ins:balance_gate()
                verify_ins:setgate(uuid,nodename)
            end
        end
    else
        nodename = usermgr_ins:getgate(uuid)
        host = control_ins:get_gate_host(nodename)
    end 

    if not nodename or not host then 
        ERROR_LOG("没有可用的网关")
        verify_ins:del(token)
        return errcode.NOT_AVAILD_GATE
    end 


    return errcode.RT_OK,host
end

-- from gate authd
function CMD.verify_token(uuid,token)
    local verify_ins = control_ins:getverify()
    if not verify_ins then 
        ERROR_LOG("验证模块不存在")
        return errcode.MODULE_ERROR
    end 

    return verify_ins:verify_token(uuid,token)
end

skynet.start(function ()
    utils.dispatch_lua(CMD)

    control_ins = control.new()

    skynet.register(".controld")
end)