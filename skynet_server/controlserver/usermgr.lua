local skynet = require "skynet"

local comdefine = require "comdefine"

local errcode = require "errcode"


local EVENT_NAME = comdefine.EVENT_NAME

local usermgr =  __G_CLASS__(...)



function usermgr:ctor()
    -- 在线玩家信息
    self.__userlist = {}    
    self.__nodelist = {}
    -- 网关地址
    self.__gates = {}

    self:load()

end 

function usermgr:dector()

end 


function usermgr:load()

end



function usermgr:adduser(uuid,nodename)
    if self.__userlist[uuid] then 
        return 
    end 

    self.__userlist[uuid] = nodename
    self.__nodelist[nodename] = self.__nodelist[nodename] or {}
    self.__nodelist[nodename][uuid] = true

    DEBUG_LOG("添加玩家 UUID：%d 节点：%s",uuid,nodename)
end
function usermgr:getgate(uuid)
    return self.__userlist[uuid] 
end

function usermgr:isonline(uuid)
    return self.__userlist[uuid] ~=nil
end

function usermgr:deluser(uuid)
   
    if not self.__userlist[uuid] then 
        return 
    end 
    local nodename = self.__userlist[uuid]
    self.__userlist[uuid] = nil

    DEBUG_LOG("删除玩家 UUID：%d 节点：%s",uuid,nodename)

    local nl = self.__nodelist[nodename]
    if not nl[uuid] then 
        return 
    end 
    nl[uuid] = nil
end


function usermgr:syc_user_list(userlist,nodename)
    DEBUG_LOG("同步节点玩家列表 节点：%s List:%s ",nodename,table.dump(userlist))
    for uuid,_ in pairs(userlist) do 
        self:adduser(uuid,nodename)
    end 
end


-- 网关下线
function usermgr:gatedown(nodename)
    DEBUG_LOG("删除节点玩家列表 nodename :%s nodelist:%s",nodename,table.dump(self.__nodename))
    local nl = self.__nodelist[nodename]
    if not nl then 
        return 
    end 
    DEBUG_LOG("删除节点玩家列表 节点：%s List:%s ",nodename,table.dump(nl))
    for uuid,_ in pairs(nl) do 
        self:deluser(uuid)
    end 
end





return usermgr