local skynet = require "skynet"
local eventmgr = require "eventmgr"
local comdefine = require "comdefine"
local nodemgr = require "nodemgr"
local errcode = require "errcode"

local EVENT_NAME = comdefine.EVENT_NAME

local verifymgr =  __G_CLASS__(...)


function verifymgr:ctor()
    self.__verify = {                  -- token 列表
        -- [token] = {
        --     uuid = uuid,
        --     token = token,
        --     expire = expire,
        -- }
    }   

    self.__uuidlist = {                -- 正在验证中的玩家列表
        --[uuid] = nodename
    }      

    self:load()
end 

function verifymgr:dector()

end 


function verifymgr:fram_loop()
    local curtm = skynet.time()
    for token,info in pairs(self.__verify) do 
        if curtm > info.expire  then 
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

function verifymgr:update(uuid,token)
    self.__verify[token] = self.__verify[token] or {}
    local info = self.__verify[token]
    info.uuid   = uuid
    info.token  = token 
    info.expire = skynet.time() + comdefine.LOGIN_TOKEN_EXPIRE

    DEBUG_LOG("添加验证消息 UUID:%d TOKEN:%d verify:%s  uuidlist%s",
            uuid,
            token,
            table.dump(self.__verify),
            table.dump(self.__uuidlist))
end

function verifymgr:del(token)
    if not self.__verify[token] then 
        return 
    end 
    local uuid = self.__verify[token].uuid
    self.__uuidlist[uuid] = nil
    self.__verify[token] = nil
    DEBUG_LOG("删除验证消息 UUID:%d TOKEN:%d  verify:%s  uuidlist%s",
        uuid,
        token,
        table.dump(self.__verify),
        table.dump(self.__uuidlist))
end





function verifymgr:setgate(uuid,nodename)
    DEBUG_LOG("设置验证网关 玩家UUID：%d 网关名:%s ",uuid,nodename)
    if  self.__uuidlist[uuid] then 
        return 
    end 

    self.__uuidlist[uuid] = nodename
end


function verifymgr:getgate(uuid)
    return self.__uuidlist[uuid]
end


function verifymgr:verifying(token)
    if not self.__verify[token] then 
        return false
    end 
    return true
end


--- login 同步登陆验证信息信息
function verifymgr:syc_verify_data(uuid,token,expire)
    self:update(uuid,token,expire)
end


function verifymgr:verify_token(uuid,token)
    if not self.__verify[token] then 
        ERROR_LOG("玩家登陆验证失败 验证信息不存在 UUID:%d TOKEN：%d",uuid,token)
        return errcode.TOKEN_NO_EXIST
    end 

    if self.__verify[token].token ~= token then 
        ERROR_LOG("玩家登陆验证失败 Token错误 UUID:%d TOKEN：%d",uuid,token)
        return errcode.TOKEN_ERROR
    end 
    self:del(token)
    DEBUG_LOG("验证通过 删除验证信息 UUID:%d TOKEN：%d",uuid,token)
    return errcode.RT_OK
end



return verifymgr

