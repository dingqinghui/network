local skynet = require "skynet"
local ap_mod = require "agentpool"
local comdefine = require "comdefine"
local sermgr = require "sermgr"


local INIT_AGENT_CAP = 10


local USER_STATUS = comdefine.USER_STATUS

local usermgr =  __G_CLASS__(...)

function usermgr:ctor(hub)
    self.__hub = hub
    self.__userlist = {
        -- [uuid] = {
        --     __agent    = nil,
        --     __fd       = nil
        --     __token    = nil
        --     __expire   = nil
        -- }
    }        -- 已经登录的玩家列表

    self.__conlist = {
        --[fd] = uuid
    }

    self:load()
end 

function usermgr:dector()

end 

function usermgr:fram_loop()
    local curtm = skynet.time()
    for uuid,user in pairs(self.__userlist) do 
        -- 离线
        if not user.__fd then 
            if user.__expire < curtm then 
                -- 清除玩家
                self:exit(uuid)
            end
        end
    end 
end

function usermgr:load()
    -- 开启每帧检测
    skynet.fork(function (self)
        while true do 
            self:fram_loop()
            skynet.sleep(comdefine.FRAME_LOOP)
        end
    end,self)
    -- 创建连接池
    self.__agentpool = ap_mod.new("useragent",INIT_AGENT_CAP,skynet.self(),sermgr.get("hubd"))
end 
 

function usermgr:getap()
    return self.__agentpool
end 

function usermgr:get_user(uuid)
    return self.__userlist[uuid]
end

function usermgr:add_user(uuid,fd,token)
    self.__userlist[uuid] = {
        __uuid = uuid,
        __fd = fd,
        __token = token
    }

    self.__conlist[fd] = uuid
end


function usermgr:exit(uuid)
    local user = self.__userlist[uuid]
    if not user then 
        return 
    end 
    assert( user.__agent )

    if user.__agent then 
        skynet.send(user.__agent ,"lua","release",uuid)
    end
    -- recyle agent
    self:getap():release(user.__agent)
    
    if user.__fd then 
        self.__conlist[user.__fd] = nil
    end
    self.__userlist[uuid] = nil

    -- 通知controlserver
    if __CONTROL_CLIENT__ then 
        __CONTROL_CLIENT__:send(".controld","logout",uuid,__NODE_NAME__)
    end

    DEBUG_LOG("玩家代理回收 玩家:%d",uuid)
end


function usermgr:alloc_agent(uuid)
    return self:getap():get()
end 


function usermgr:release_agent(agent)
    return self:getap():release(agent)
end 


function usermgr:assign_agent(uuid,agent)
    local user = self.__userlist[uuid]
    
    user.__agent = agent
end

function usermgr:get_agent(uuid)
    local user = self.__userlist[uuid]
    if not user then 
        return nil
    end 
    return user.__agent
end


function usermgr:assign_con(uuid,fd)
    local user = self.__userlist[uuid]
    user.__fd = fd

    self.__conlist[fd] = uuid
end


function usermgr:get_con(uuid)
    local user = self.__userlist[uuid]
    if not user then 
        return nil
    end 
    return user.__fd
end




function usermgr:assign_token(uuid,token)
    local user = self.__userlist[uuid]
    user.__token = token
end



function usermgr:get_user_list()
    local list = {}
    for uuid,_ in pairs(self.__userlist) do 
        table.insert(list,uuid)
    end 
    return list
end


-- fd 网络断开连接
function usermgr:disconnect(fd)
    if not self.__conlist[fd] then 
        return 
    end 
    local uuid = self.__conlist[fd]
    local user = self:get_user(uuid)
    if not user then 
        return 
    end 

    -- 清空网络句柄
    self:assign_con(uuid,nil)


    -- 保存离线时间
    local expire = skynet.time() + comdefine.USER_OFFLINE_EXPIRE
    user.__expire = expire

    -- 通知验证服务
    sermgr.send("verifyd","lua","update_expire",uuid,expire)

     if user.__agent then 
        skynet.send(user.__agent ,"lua","disconnect",fd)
    end
end



function usermgr:alloc_agent_success(uuid,fd ,token,agent)
    local user = self:get_user(uuid)
    -- 玩家下线
    if not user then 
        self:release_agent(agent)
        return 
    end 
    local oldagent =  self:get_agent(uuid)
    -- agent 已经存在
    if oldagent then 
        self:release_agent(agent)
        skynet.send(agent,"lua","assign",uuid,fd,token)
    else
        self:assign_agent(uuid,agent)
        skynet.send(agent,"lua","newuser",uuid,fd,token)
    end 
end




function usermgr:user_login(newuuid,newfd ,newtoken)
    local fd = self:get_con(newuuid)
    local uuid = self.__conlist[fd]

    if fd == newfd and uuid == newuuid then 
        WARRING_LOG("重复的登陆验证消息 uuid:%s fd:%d",uuid,fd)
        return 
    end 
    DEBUG_LOG("玩家登录 NEW UUID：%d NEW FD:%d TOKEN:%d",newuuid,newfd,newtoken)

    if fd ~= nil and fd ~= newfd then 
        assert( newuuid == uuid)
        sermgr.send("hubd","lua","closeclient",fd)
        INFO_LOG("玩家被顶号 踢出连接 uuid:%s fd:%d",uuid,fd)
    end 

    local user = self:get_user(newuuid)
    if not user then 
        self:add_user(newuuid,newfd,newtoken)
        -- 请求分配agent
        local agent = self:alloc_agent()
        assert(agent)
        self:alloc_agent_success(newuuid,newfd ,newtoken,agent)
    else
        -- 重连/顶号
        self:assign_con(newuuid,newfd)
        self:assign_token(newuuid,newtoken)
        local agent = self:get_agent(newuuid)
        -- 通知更新fd 和token
        skynet.send(agent,"lua","assign",newuuid,newfd,newtoken)
    end 

    DEBUG_LOG("userlist=%s",table.dump(self.__userlist))
end


return usermgr