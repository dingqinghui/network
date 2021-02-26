local skynet = require "skynet"
local ap_mod = require "agentpool"
local comdefine = require "comdefine"

local INIT_AGENT_CAP = 100


local USER_STATUS = comdefine.USER_STATUS

local usermgr =  __G_CLASS__(...)

function usermgr:ctor(hub)
    self.__hub = hub
    self.__list = {} 
    self.__agentpool = ap_mod.new("useragent",INIT_AGENT_CAP,skynet.self(),__HUB__)
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
    self:getagentpool():release(agent)
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