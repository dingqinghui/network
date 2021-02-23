local skynet = require "skynet"
local gateserver = require "snax.gateserver"



local authmaster    -- 验证主服务
local agent = {}    -- 为了保证同一客户端的请求的处理顺序，同一客户端的消息只能发送给同一个auth服务


skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
}

local handler = {}

local wait_auth = {}

local function close_fd(fd)
    wait_auth[fd] = nil
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
    DEBUG_LOG("con msg  fd:%d msg:%s sz:%d",fd,msg,sz)
    if wait_auth[fd] then 
        local s = getagent(fd)
        assert(s)
        -- 调用验证服务验证
        skynet.redirect(s,skynet.self(),"client",fd,msg,sz)
    else

    end
    
end

function handler.connect(fd, addr)
    wait_auth[fd] = true
    DEBUG_LOG("new con fd:%d host:%s",fd,addr)

    gateserver.openclient(fd)

    local s = getagent(fd)
    skynet.call(s,"lua","connect",fd)
end

function handler.disconnect(fd)
    close_fd(fd)
    DEBUG_LOG("con disconnect fd:%d",fd)

    local s = getagent(fd)
    skynet.call(s,"lua","disconnect",fd)
end


function handler.error(fd, msg)
    close_fd(fd)
    DEBUG_LOG("con error fd:%d",fd)

    local s = getagent(fd)
    skynet.call(s,"lua","disconnect",fd)
end


function handler.warning(fd, size)
    DEBUG_LOG("con warning fd:%d",fd)
end


local CMD = {}

function CMD.closeclient(source,fd)
    close_fd(fd)
    gateserver.closeclient(fd)
    DEBUG_LOG("主动关闭客户端 fd:" .. fd)
end

function handler.command(cmd, source, ...)
	local f = assert(CMD[cmd],cmd)
	return f(source, ...)
end



skynet.init(function ()
    authmaster = skynet.uniqueservice("authd",skynet.self())
end)

gateserver.start(handler)

