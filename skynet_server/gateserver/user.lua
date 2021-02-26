local user = __G_CLASS__(...)

function user:ctor(uuid,fd,client)
    self.__uuid = uuid
    self.__fd = fd

end

function user:dctor()


end

function user:login()

    DEBUG_LOG("玩家登陆 uuid:%s fd:%d",self.__uuid,self.__fd)
end

function user:enter_game()


    DEBUG_LOG("玩家进入游戏 uuid:%s fd:%d",self.__uuid,self.__fd)
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
    return self.__fd ~= nil
end 



return user