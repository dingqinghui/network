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

    skynet.timeout(USER_SAVE_INTERVAL * 100,save_timer)
end

function user:load_data(data)
    data = data or {}
    if not data.uuid then 
        self.__isnew = true
    end 
    data.uuid     = data.uuid or self.__uuid
    data.reg_time = data.reg_time or math.ceil(skynet.time())
    data.name     = data.name or "初始名字"


    self.__data = data

    -- 立刻存档一次
    if self.__isnew then 
        self:save(true)
    end
    -- 开启存档定时器
    skynet.timeout(USER_SAVE_INTERVAL* 100,save_timer)
end

function user:enter_game()
    DEBUG_LOG("玩家进入游戏 uuid:%s fd:%d",self.__uuid,self.__fd)

    -- 开启存档定时器

    return self.__data
end


function user:save(binsert)
    print(table.dump(self.__data))
    userdb.save(self.__uuid,self.__data,binsert)   
end

return user