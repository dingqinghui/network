local user = __G_CLASS__(...)

function user:ctor()


end

function user:dctor()


end

function user:login(info,client)
    self.__baseinfo = info
    self.__client = client
    DEBUG_LOG("玩家登陆 %s",table.dump(info))
end

function user:enter_game()

    DEBUG_LOG("玩家进入游戏")
end

function user:send(packname,msg)
    local fd = self.__baseinfo.fd
    local pack = {name = packname,data = msg}
    self.__client.send(fd,pack)
end

function user:getuuid()
    return self.__baseinfo.uuid
end 


function user:getfd()
    return self.__baseinfo.fd
end 

return user