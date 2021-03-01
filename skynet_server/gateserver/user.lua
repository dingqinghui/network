local skynet = require "skynet"
local comdefine = require "comdefine"
local sermgr = require "sermgr"

local USER_SURVIVAL_MAX_TM = comdefine.USER_SURVIVAL_MAX_TM

local user = __G_CLASS__(...)

function user:ctor(uuid,fd,client)
    self.__uuid = uuid
    self.__fd = fd
    self.__isonline = nil
    self.__data = {}
    self.__exit_co = nil
end

function user:dctor()


end

function user:login()

    self.__isonline = true
    skynet.wakeup(self.__exit_co)
    DEBUG_LOG("玩家登陆 uuid:%s fd:%d",self.__uuid,self.__fd)
end

-- 网络断开
function user:logout()
    DEBUG_LOG("玩家登出 uuid:%s fd:%d",self.__uuid,self.__fd)

    self.__fd = nil
    self.__isonline  = false

    skynet.fork(function ()
        skynet.sleep(USER_SURVIVAL_MAX_TM * 100,self.__exit_co)
        self.__exit_co = nil
        if self:isonline() then 
            return 
        end 
        -- 踢出玩家
        self:kick()
    end )


end

function user:kick()
    sermgr.send("usermgrd","lua","kick",self.__uuid)
    __USER__ = nil
end 


function user:enter_game()
    local data = {
        uuid = self.__uuid
    }

    DEBUG_LOG("玩家进入游戏 uuid:%s fd:%d",self.__uuid,self.__fd)
    return data
end

function user:send(packname,msg)
    if self:isonline() then 
        local fd = self:getfd()
        local pack = {name = packname,data = msg}
        self.__client.send(fd,pack)
    end 
end

function user:getuuid()
    return self.__uuid
end 


function user:getfd()
    return self.__fd
end 
function user:setfd(fd)
    self.__fd = fd
end 

function user:isonline()
    return self.__isonline
end 


return user