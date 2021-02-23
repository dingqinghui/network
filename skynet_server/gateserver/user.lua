
require "class"

local user = __G_CLASS__(...)

function user:ctor()


end

function user:dctor()


end

function user:login(info)
    self.__baseInfo = info

    DEBUG_LOG("玩家登陆 %s",table.dump(info))
end

function user:enter_game()

    DEBUG_LOG("玩家进入游戏")
end



function user:getuuid()
    return self.__baseInfo.uuid
end 

return user