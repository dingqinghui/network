local skynet = require "skynet"
local gateserver = require "snax.gateserver"

local usermgr
local authmaster        -- 验证主服务
local hub = {}
local agent = {}        -- <fd,auth agent /logic agent>


skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
}



local authpool = {}
local authagentcnt = 0

local function close_fd(fd)
    agent[fd] = nil
end


local handler = {}
function handler.open(source, conf)

end

local function getauthagent(fd) 
    local index = fd % authagentcnt + 1
    return authpool[index]
end



function handler.connect(fd, addr)
    DEBUG_LOG("新客户端连接 fd:%d host:%s",fd,addr)
    gateserver.openclient(fd)
    local s = getauthagent(fd)
    assert(s)
    agent[fd] = s
    skynet.send(s,"lua","connect",fd)
end

function handler.message(fd, msg, sz)
    DEBUG_LOG("收到客户端消息  fd:%d msg:%s sz:%d",fd,msg,sz)

    local s = agent[fd]
    if not s then 
        return 
    end 

    skynet.redirect(s,skynet.self(),"client",fd,msg,sz) 
end

function handler.disconnect(fd)
    local s = agent[fd]
    if not s then 
        return 
    end 

    skynet.send(s,"lua","disconnect",fd)        -- 通知验证服务

    close_fd(fd)
    DEBUG_LOG("客户端断开连接 fd:%d",fd)
end


function handler.error(fd, msg)
    local s = agent[fd]
    if not s then 
        return 
    end

    skynet.send(s,"lua","disconnect",fd)        -- 通知验证服务

    close_fd(fd)
    DEBUG_LOG("客户端出错 fd:%d",fd)
end


function handler.warning(fd, size)
    DEBUG_LOG("con warning fd:%d",fd)
end


local  authcnt = 0

local CMD = {}
-- from auth service 
function CMD.authpass(srouce, uuid,fd )
    agent[fd] = nil
    local s = skynet.call(usermgr,"lua","getagent",uuid) 
    if not agent[fd] then 
        -- redirect fd to useragent
        agent[fd] = s
        -- 赋值连接
        skynet.send(s,"lua","assign",uuid,fd)

        authcnt  = authcnt + 1
        INFO_LOG("完成验证数量 %d agent:%s",authcnt,table.dump(agent) )
    end 
end


function CMD.closeclient(srouce, fd )
    if not agent[fd] then 
        return 
    end 

    gateserver.closeclient(fd)
    close_fd(fd)
    INFO_LOG("踢出连接 fd:%d agent:%s",fd,table.dump(agent))
end 


function handler.command(cmd, source, ...)
	local f = assert(CMD[cmd],cmd)
	return f(source, ...)
end


skynet.init(function ()
    for i=1,5 do
        local s = skynet.newservice("authd",skynet.self())
        table.insert(authpool,s)
        authagentcnt  = authagentcnt + 1
    end

    usermgr = skynet.newservice("usermgrd",skynet.self())
end)

gateserver.start(handler)

