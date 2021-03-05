local skynet = require "skynet"

local comdefine = require "comdefine"

local errcode = require "errcode"


local EVENT_NAME = comdefine.EVENT_NAME

local usermgr =  __G_CLASS__(...)



function usermgr:ctor()
    -- 在线玩家信息
    self.__userlist = {}    
    self.__nodes = {}
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

    self.__nodes[nodename] = self.__nodes[nodename] or {
        __list = {},
        __cnt = 0,
    }
    local nodeinfo = self.__nodes[nodename]
    nodeinfo.__list[uuid] = true
    nodeinfo.__cnt = nodeinfo.__cnt + 1
    DEBUG_LOG("添加玩家 UUID：%d 节点：%s",uuid,nodename)
end



function usermgr:get_gate_name(uuid)
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


    local nodeinfo  = self.__nodes[nodename]
    if not nodeinfo then 
        return 
    end 

    if not nodeinfo.__list[uuid] then 
        return 
    end 

    nodeinfo.__list[uuid] = nil
    nodeinfo.__cnt = nodeinfo.__cnt - 1

    DEBUG_LOG("删除玩家 UUID：%d 节点：%s",uuid,nodename)
end



function usermgr:syc_user_list(userlist,nodename)
    for uuid,_ in pairs(userlist) do 
        self:adduser(uuid,nodename)
    end 
end


-- 网关下线
function usermgr:remove_gate_user(nodename)
    local nodeinfo = self.__nodes[nodename]
    if not nodeinfo then 
        return 
    end 

    for uuid,_ in pairs(nodeinfo.__list) do 
        self:deluser(uuid)
    end 
end


function usermgr:get_user_cnt_by_node(nodename)
    local nodeinfo = self.__nodes[nodename]
    if not nodeinfo then 
        return 0
    end 
    return nodeinfo.__cnt
end



return usermgr