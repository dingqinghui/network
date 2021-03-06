local skynet = require "skynet"
local gateserver = require "snax.gateserver"
require "skynet.manager"
local comdefine = require "comdefine"

local SERVICE_NAME = comdefine.SERVICE_NAME
local start = false
local usermgr
local verifyd 
local authagent = {}        
local useragent = {}
local hub = {}

local hubconf
local authpool = {}
local authagentcnt = 0

skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
}



local function close_fd(fd)
    authagent[fd] = nil
    useragent[fd] = nil
end


local handler = {}


local function getauthagent(fd) 
    local index = fd % authagentcnt + 1
    return authpool[index]
end



function handler.connect(fd, addr)
    DEBUG_LOG("新客户端连接 fd:%d host:%s",fd,addr)
    gateserver.openclient(fd)
    local s = getauthagent(fd)
    assert(s)
    authagent[fd] = s
    skynet.send(s,"lua","connect",fd)
end

function handler.message(fd, msg, sz)
    DEBUG_LOG("收到客户端消息  fd:%d msg:%s sz:%d",fd,msg,sz)

    if useragent[fd] then 
        skynet.redirect(useragent[fd],skynet.self(),"client",fd,msg,sz) 
    else
        skynet.redirect(authagent[fd],skynet.self(),"client",fd,msg,sz) 
    end
end


local function disconnect(fd)
    if authagent[fd]  then 
        skynet.send(authagent[fd],"lua","disconnect",fd) 
    else
        skynet.send(usermgr,"lua","disconnect",fd)      
    end 
    close_fd(fd)
end

function handler.disconnect(fd)
    disconnect(fd)
    DEBUG_LOG("客户端断开连接 fd:%d",fd)
end


function handler.error(fd, msg)
    disconnect(fd)
    DEBUG_LOG("客户端出错 fd:%d",fd)
end


function handler.warning(fd, size)
    DEBUG_LOG("con warning fd:%d",fd)
end


local  authcnt = 0

local CMD = {}


--useragent 初始化完成 重定向
function CMD.redirect(source,fd,s)
    useragent[fd] = s
    authagent[fd] = nil

    DEBUG_LOG("重定向 FD:%s Agent:%d",fd,s)
end


function CMD.gethost()
    return string.format("%s:%s",hubconf.address,hubconf.port)
end

-- ping 超时 通知关闭客户端
function CMD.closeclient(fd)
    gateserver.closeclient(fd)
    disconnect(fd)
end


function handler.command(cmd, source, ...)
	local f = assert(CMD[cmd],cmd)
	return f(source, ...)
end

-- 监听成功
function handler.open(source, conf)
    hubconf = conf

    start = true

    verifyd = skynet.newservice("verifyd")
    usermgr = skynet.newservice("usermgrd",skynet.self(),verifyd)

    for i=1,5 do
        local s = skynet.newservice("authd",skynet.self(),usermgr,verifyd)
        table.insert(authpool,s)
        authagentcnt  = authagentcnt + 1
    end
end


function CMD.quitserver()
    if not start then 
        return 
    end 
    -- 踢出连接
    for fd ,__ in pairs(authagent) do
        gateserver.closeclient(fd)
        close_fd(fd)
    end 
    for fd ,__ in pairs(useragent) do
        gateserver.closeclient(fd)
        close_fd(fd)
    end 
end


skynet.init(function ()

    skynet.register(SERVICE_NAME.GATE_HUB)
end)

gateserver.start(handler)

