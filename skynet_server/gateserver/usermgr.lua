local skynet = require "skynet"
local ap_mod = require "agentpool"
local comdefine = require "comdefine"
local sermgr = require "sermgr"

local INIT_AGENT_CAP = 100


local USER_STATUS = comdefine.USER_STATUS

local usermgr =  __G_CLASS__(...)

function usermgr:ctor(hub)
    self.__hub = hub
    self.__list = {} 
    self.__agentpool = ap_mod.new("useragent",INIT_AGENT_CAP,skynet.self(),sermgr.get("hubd"))
end 

function usermgr:dector()

end 

function usermgr:getap()
    return self.__agentpool
end 

function usermgr:exit(uuid)
    local agent = self.__list[uuid]
    if not agent then 
        return 
    end 
    -- recyle agent
    self:getap():release(agent)
    DEBUG_LOG("玩家代理回收 玩家:%d",uuid)
end

function usermgr:getagent(uuid,fd)
    local agent = self.__list[uuid]
    if not agent then 
        agent = self:getap():get()
        if not self.__list[uuid] then 
            self.__list[uuid] = agent
        else
            self:getap():release(agent)
            agent = self.__list[uuid]
        end 
    end
    return agent
end 


return usermgr