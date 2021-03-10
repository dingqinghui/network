local skynet = require "skynet"
local comdefine = require "comdefine"
local sermgr = require "sermgr"
local userdb = require "userdb"

local USER_STATUS = comdefine.USER_STATUS
local USER_SAVE_INTERVAL = comdefine.USER_SAVE_INTERVAL

local user = __G_CLASS__(...)

function user:ctor(uuid,fd,token,client)
    self.__uuid = uuid
    self.__fd = fd
    self.__data = {}
    self.__token = token
    self.__status = USER_STATUS.INIT_INSTANCE
    self.__isnew = false
    self.__savecnt = 0
end


function user:dctor()
    

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

function user:set_statu(statu)
    self.__status = statu
end

function user:get_statu()
    return self.__status
end

function save_timer()
    if not __USER__ then 
        return 
    end 
    __USER__:save()

    skynet.timeout(USER_SAVE_INTERVAL,save_timer)
end

function user:load_data(data)
    data = data or {}
    if not data.uuid then 
        self.__isnew = true
    end 

    data.reg_time = data.reg_time or skynet.time()
    data.name     = data.name or "初始名字"


    self.__data = data
    -- 开启存档定时器
    skynet.timeout(USER_SAVE_INTERVAL,save_timer)
end

function user:enter_game()
    DEBUG_LOG("玩家进入游戏 uuid:%s fd:%d",self.__uuid,self.__fd)

    -- 开启存档定时器

    return self.__data
end


function user:save()
    if self.__isnew and self.__savecnt <= 0 then 
        userdb.save(self.__uuid,self.__data,true)   -- 插入
    else
        userdb.save(self.__uuid,self.__data,false)  -- 更新
    end 
    self.__savecnt = self.__savecnt + 1
end

return user