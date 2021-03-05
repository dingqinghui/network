
local skynet = require "skynet"
local comdefine = require "comdefine"
local usermgr = require "usermgr"
local eventmgr = require "eventmgr"
local nodemgr = require "nodemgr"
local balance = require "balance"

local EVENT_NAME = comdefine.EVENT_NAME


local control =  __G_CLASS__(...)

function control:ctor()
    self.__gates = {}   -- 完成初始化的网关
    self:load()
    self:init_data()
end 


function control:dector()

end 

function control:init_data()
    
end


function control:load()
    self.__usermgr_mod = usermgr.new()

    self.__balance = balance.new("gateserver",nil,function (nodename)
        self:on_gate_disconnect(nodename)
        self.__gates[nodename] = false
    end)
    
end

function control:on_gate_disconnect(nodename)
    self.__usermgr_mod:remove_gate_user(nodename)
end


function control:getusermgr()
    return self.__usermgr_mod
end

function control:gate_init_finish(nodename,host)
    if not string.find(nodename,"gateserver") then 
        return 
    end 
    self.__gates[nodename] = host
end


function control:invalid_gate(nodename)
    -- 是否初始完成
    if not self.__gates[nodename] then 
        return false  
    end 
    -- 是否节点可用
    if not self.__balance:isconnect(nodename) then 
        return false
    end 
    return true
end


function control:balance_gate(uuid)
    DEBUG_LOG("gate = ：%s",table.dump(self.__gates))
    local nodename,host
    if self.__usermgr_mod:isonline(uuid) then 
        nodename = self.__usermgr_mod:get_gate_name(uuid) -- 一定是可用的
        host = self.__gates[nodename]
    else
        local mincnt = 0
        self.__balance:transfer(function (name)
            local cnt = self.__usermgr_mod:get_user_cnt_by_node(name)
            if not nodename or cnt < mincnt then 
                nodename = name
                mincnt = cnt
            end 
        end)
        host = self.__gates[nodename]
    end 

    return nodename ,host 
end

-- 向网关同步登陆信息
function control:sync_login_info_to_gate(nodename,uuid,token)
    local expire = skynet.time() + comdefine.LOGIN_TOKEN_EXPIRE
    return self.__balance:call(nodename,".verifyd","syc_verify_data",uuid,token,expire)
end

return control