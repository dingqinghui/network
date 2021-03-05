local skynet = require "skynet"
local comdefine = require "comdefine"
local sermgr = require "sermgr"

local user = __G_CLASS__(...)

function user:ctor(uuid,fd,token,client)
    self.__uuid = uuid
    self.__fd = fd
    self.__data = {}
    self.__exit_co = nil
    self.__token = token
end


function user:dctor()
    

end


function user:login()



end

function user:enter_game()
    local data = {
        uuid = self.__uuid
    }

    DEBUG_LOG("玩家进入游戏 uuid:%s fd:%d",self.__uuid,self.__fd)
    return data
end

function user:send(packname,msg)
    local fd = self:getfd()
    if not fd  then 
        return 
    end 
    local pack = {name = packname,data = msg}
    self.__client.send(fd,pack)
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

function user:settoken(token)
    self.__token = token
end



return user