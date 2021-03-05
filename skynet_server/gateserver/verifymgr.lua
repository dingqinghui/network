local skynet = require "skynet"
local eventmgr = require "eventmgr"
local comdefine = require "comdefine"
local nodemgr = require "nodemgr"
local errcode = require "errcode"

local EVENT_NAME = comdefine.EVENT_NAME

local verifymgr =  __G_CLASS__(...)


function verifymgr:ctor()
    self.__verify = {                  
        -- [uuid] = {
        --     uuid = uuid,
        --     token = token,
        --     expire = expire,
        -- }
    }   
    self:load()
end 

function verifymgr:dector()

end 


function verifymgr:fram_loop()
    local curtm = skynet.time()
    for token,info in pairs(self.__verify) do 
        if info.expire and curtm > info.expire  then 
            DEBUG_LOG("TOKEN 过期")
            self:del(token)
        end 
    end
end

function verifymgr:load()
    skynet.fork(function (self)
        while true do 
            self:fram_loop()
            skynet.sleep(comdefine.FRAME_LOOP)
        end
    end,self)
end


function verifymgr:update(uuid,token,expire)
    self.__verify[uuid] = self.__verify[uuid] or {}
    local info = self.__verify[uuid]
    info.uuid   = uuid
    info.token  = token 
    info.expire = expire 

    DEBUG_LOG("添加验证消息 UUID:%d TOKEN:%d verify:%s ",
            uuid,
            token,
            table.dump(self.__verify))
end



function verifymgr:del(uuid)
    if not self.__verify[uuid] then 
        return 
    end 
    self.__verify[uuid] = nil
    DEBUG_LOG("删除验证消息 UUID:%d   verify:%s ",
        uuid,
        table.dump(self.__verify))
end

function verifymgr:verify_token(uuid,token)
    if not self.__verify[uuid] then 
        ERROR_LOG("玩家登陆验证失败 验证信息不存在 UUID:%d TOKEN：%d",uuid,token)
        return errcode.TOKEN_NO_EXIST
    end 

    if self.__verify[uuid].token ~= token then 
        ERROR_LOG("玩家登陆验证失败 Token错误 UUID:%d TOKEN：%d",uuid,token)
        return errcode.TOKEN_ERROR
    end 
    -- 验证通过只清空过期时间 在网络断开时重新计算过期时间
    self.__verify[uuid].expire = nil

    DEBUG_LOG("验证通过 删除验证信息 UUID:%d TOKEN：%d",uuid,token)
    return errcode.RT_OK
end

function verifymgr:assign_expire(uuid,expire)
    if not self.__verify[uuid] then 
        return 
    end 
    self.__verify[uuid].expire = expire
end


function verifymgr:get_user_list()
    local list = {}
    for uuid,_ in pairs(self.__verify) do 
        table.insert(list,uuid)
    end 
    return list
end


return verifymgr

