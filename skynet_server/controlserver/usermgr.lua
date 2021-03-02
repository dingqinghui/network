local skynet = require "skynet"
local eventmgr = require "eventmgr"
local comdefine = require "comdefine"
local nodemgr = require "nodemgr"

local EVENT_NAME = comdefine.EVENT_NAME

local usermgr =  __G_CLASS__(...)



function usermgr:ctor()
    self.__userlist = {}
    self.__nodelist = {}

    self:load()
end 

function usermgr:dector()

end 

function usermgr:load()
    -- 获取初始状态
    local status = nodemgr.status()
    DEBUG_LOG("节点初始状态 :%s",table.dump(status))
    for nodename,statu in pairs(status) do
        self:node_statu_change(nodename,statu)
    end
    -- 订阅状态改变事件
    eventmgr.subscribe(EVENT_NAME.CLUSTER_CHANGE,function (nodename,isconnect) 
        self:node_statu_change(nodename,statu)
    end)
end

function usermgr:node_statu_change(nodename,statu)
    DEBUG_LOG("节点状态改变 节点：%s 状态：%d",nodename,statu and 1 or 0)
    if statu then 
        -- 获取 玩家列表
        local ok,userlist = nodemgr.call(nodename,".usermgr","getuserlist")
        if ok then 
            self:syc_user_list(userlist,nodename)
        end
        return 
    else
        local ret  = string.find(nodename,"gate")
        if  ret then 
            self:gatedown(nodename)
            return 
        end 
    end
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