local skynet = require "skynet"
local gateserver = require "snax.gateserver"



local authmaster    -- 验证主服务
local agent = {}    -- 为了保证同一客户端的请求的处理顺序，同一客户端的消息只能发送给同一个auth服务


skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
}

local handler = {}


local function close_fd(fd)
    agent[fd] = nil
end

function handler.open(source, conf)

end

local function getagent(fd)
    local s = nil
    if not agent[fd] then 
        s = skynet.call(authmaster,"lua","balance",fd)
        agent[fd] = s 
    else
        s = agent[fd]
    end
    return s 
end


function handler.message(fd, msg, sz)
    local s = getagent(fd)
    assert(s)
    -- 调用验证服务验证
    skynet.redirect(s,skynet.self(),"client",fd,msg,sz)
end

function handler.connect(fd, addr)
    gateserver.openclient(fd)
    local s = getagent(fd)
    skynet.call(s,"lua","connect",fd)
    DEBUG_LOG("客户端新连接 fd:%d",fd)
end

function handler.disconnect(fd)
    local s = getagent(fd)
    if s then skynet.call(s,"lua","disconnect",fd) end

    close_fd(fd)
    DEBUG_LOG("客户端连接断开 fd:%d",fd)
end


function handler.error(fd, msg)
    local s = getagent(fd)
    if s then skynet.call(s,"lua","disconnect",fd) end

    close_fd(fd)
    DEBUG_LOG("客户端连接出错 fd:%d",fd)
end


function handler.warning(fd, size)
    DEBUG_LOG("客户端连接警告 fd:%d",fd)
end

local logincnt = 0

local CMD = {}
function CMD.closeclient(source,fd)
    gateserver.closeclient(fd)
    close_fd(fd)
    INFO_LOG("服务器主动关闭客户端 fd:" .. fd)

    logincnt  = logincnt + 1
    if logincnt % 100 == 0 then 
        INFO_LOG("完成登陆验证数量 %d   %f" , logincnt , skynet.time()) 
    end 
end

function handler.command(cmd, source, ...)
	local f = assert(CMD[cmd],cmd)
	return f(source, ...)
end

skynet.init(function ()
    authmaster = skynet.uniqueservice("authd",skynet.self())
end)



gateserver.start(handler)

