
local skynet = require "skynet"
local comdefine = require "comdefine"
local verifymgr = require "verifymgr"
local usermgr = require "usermgr"
local eventmgr = require "eventmgr"
local nodemgr = require "nodemgr"


local EVENT_NAME = comdefine.EVENT_NAME


local control =  __G_CLASS__(...)

function control:ctor()
    self.__list = {}
    self.__gates = {}
    self.__gatecnt = 0
    self:load()
    self:init_data()
end 


function control:dector()

end 

function control:init_data()
    -- 获取初始状态
    local status = nodemgr.status()
    DEBUG_LOG("节点初始状态 :%s",table.dump(status))
    for nodename,statu in pairs(status) do
        self:node_statu_change(nodename,statu)
    end
    -- 订阅状态改变事件
    eventmgr.subscribe(EVENT_NAME.CLUSTER_CHANGE,function (nodename,statu) 
        self:node_statu_change(nodename,statu)
    end)
end


function control:load()
    self.__verify_mod = verifymgr.new()
    self.__usermgr_mod = usermgr.new()
end

function control:getverify()
    return self.__verify_mod
end


function control:getusermgr()
    return self.__usermgr_mod
end

function control:addgate(nodename,host)
    self.__gates[nodename] = host
    table.insert(self.__list,nodename)
    self.__gatecnt = self.__gatecnt  + 1
end


function control:delgate(nodename)
    self.__gates[nodename] = nil
    for i,name in pairs(self.__list) do
        if name == nodename then 
            table.remove(self.__list,i)
        end
    end
    self.__gatecnt = self.__gatecnt  - 1
end


function control:node_statu_change(nodename,statu)
    DEBUG_LOG("节点状态改变 节点：%s 状态：%s",nodename,table.dump(statu))
    local ret  = string.find(nodename,"gate")
    if  not ret then 
        return 
    end
    if statu then 
        -- 获取 玩家列表
        local ok,userlist,host = nodemgr.call(nodename,".usermgr","get_gate_info")
        if ok then 
            self:getusermgr():syc_user_list(userlist,nodename)
            self:addgate(nodename,host)
        end
    else
        self:getusermgr():gatedown(nodename)
        self:delgate(nodename)
    end
end

function control:balance_gate()
    if self.__gatecnt <= 0 then 
        return 
    end 
    local index = math.random(1,self.__gatecnt)
    local nodename = self.__list[index]
    local host = self.__gates[nodename]
    return nodename,host
end


function control:gate_invaild(nodename)
    if not nodename then 
        return false
    end 
    return self.__gates[nodename] ~= nil 
end

function control:get_gate_host(nodename)
    if not nodename then 
        return false
    end 
    return self.__gates[nodename]
end

return control