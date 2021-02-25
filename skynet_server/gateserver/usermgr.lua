local skynet = require "skynet"
local agentpool = require "agentpool"
local comdefine = require "comdefine"

local USER_STATUS = comdefine.USER_STATUS

local usermgr =  __G_CLASS__(...)

function usermgr:ctor(hub)
    self.__hub = hub
    self.__list = {} or self.__list
    self.__fdmap = {}
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

    self.__fdmap[info.fd] = uuid
end

function usermgr:del(uuid)
    local info  = self.__list[uuid]
    if info and info.agent then 
        self:getagentpool().unref(info.agent)
    end
    self.__list[uuid] = nil

    self.__fdmap[info.fd] = nil
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



function usermgr:kick(uuid)
    if not self.__list[uuid] then 
        return 
    end 
    self:del(uuid)
end


function usermgr:getuid(fd)
    return self.__fdmap[fd]
end

function usermgr:setstatu(uuid,statu)
    if not self.__list[uuid] then 
        return 
    end
    local info = self.__list[uuid]
    info.statu = statu
end 

function usermgr:getstatu(uuid)
    if not self.__list[uuid] then 
        return USER_STATUS.NOUSER
    end
    local info = self.__list[uuid]
    return info.statu
end 

function usermgr:setfd(uuid,fd)
    if not self.__list[uuid] then 
        return 
    end
    local info = self.__list[uuid]
    info.fd = fd
end 



--[[
    @desc: 网络连接断开
    author:{author}
    time:2021-02-25 10:34:51
    --@uuid: 
    @return:
]]
function usermgr:disconnect(uuid)
    local statu = self:getstatu(uuid)
    if statu ==  USER_STATUS.LOADED then 
        self:setstatu(uuid,USER_STATUS.WAIT_RECONNECT)
    elseif statu ==  USER_STATUS.LOADING then  
    elseif statu ==  USER_STATUS.WAIT_RECONNECT then 
    elseif statu ==  USER_STATUS.NOUSER then 
    end 

    self:setfd(uuid,nil)
end 



function usermgr:authpass(uuid,info)
    local statu = self:getstatu(uuid)
    if statu == USER_STATUS.NOUSER then         -- 新连接
        -- 添加玩家
        self:add(uuid,info)
        -- 分配agent 并初始化数据
        self:setstatu(uuid,USER_STATUS.LOADING)

        info.agent = self:getagentpool():ref()    
        skynet.call(info.agent,"lua","newuser")
        assert(info.agent)

        self:setstatu(uuid,USER_STATUS.LOADED)
        self.__hub:setagent(info.fd,info.agent) 
    elseif statu == USER_STATUS.LOADING          -- 连接重置
    then
        self.__hub.kick(self:getfd(uuid))
        self:setfd(info.fd)
    elseif statu == USER_STATUS.WAIT_RECONNECT   -- 断线重连
    then 
        self:setfd(info.fd)
        self.__hub:setagent(info.fd,self:getagent(uuid)) 
    elseif statu == USER_STATUS.LOADED           -- 顶号
    then 
        self.__hub.kick(self:getfd(uuid))
        self:setfd(info.fd)
        self.__hub:setagent(info.fd,self:getagent(uuid)) 
    end 
end


return usermgr