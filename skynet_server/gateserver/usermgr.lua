local skynet = require "skynet"
local agentpool = require "agentpool"

local usermgr =  __G_CLASS__(...)

function usermgr:ctor(hub)
    self.__hub = hub
    self.__list = {} or self.__list

    self.__agentpool = agentpool.new("gate_agent",1000)
end 

function usermgr:dector()

end 

function usermgr:getagentpool()
    return self.__agentpool
end 

function usermgr:add(uuid,info)
    if not self.__list[uuid] then 
        self.__list[uuid] = info
    end
    self.__list[uuid] = info
end

function usermgr:del(uuid)
    local info  = self.__list[uuid]
    if info and info.agent then 
        self:getagentpool().unref(info.agent)
    end
    self.__list[uuid] = nil
end

function usermgr:getagent(uuid)
    if not self.__list[uuid] then 
        return 
    end 
    return self.__list[uuid].agent
end

function usermgr:getfd(uuid)
    if not self.__list[uuid] then 
        return 
    end 
    return self.__list[uuid].fd
end

function usermgr:isonline(uuid)
    if not self.__list[uuid] then 
        return false
    end 
    local info  = self.__list[uuid]
    if not info.fd then 
        return false
    end 
    if not info.agent then 
        return false
    end 
    return true
end

function usermgr:disconnect(uuid)
    if not self.__list[uuid] then 
        return 
    end 
    local info  = self.__list[uuid]
    info.fd = nil
end 



function usermgr:authpass(uuid,info)
    local s = nil
    local uuid = info.uuid
    -- 强制登陆
    if self:isonline(uuid) then 
        local oinfo = self:get(uuid)
        s = self:getagent(uuid)

        local fd = self:getfd(uuid)
        self.__hub.kick(fd)
    else
        s = self:getagentpool():ref()
    end
    -- 通知客户端代理
    skynet.call(s,"lua","authpass",{
        uuid = info.uuid,
        token = info.token,
        fd = info.fd
    })

    info.agent = s
    self:add(uuid,info)
    return s
end

function usermgr:kick(uuid)
    if not self.__list[uuid] then 
        return 
    end 
    self:del(uuid)
end

return usermgr