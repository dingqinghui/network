local skynet = require "skynet"
local gateserver = require "snax.gateserver"

local auth

skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
}

local handler = {}

local wait_auth = {}

local function close_fd(fd)
    wait_auth[fd] = nil
end

function handler.open(source, conf)

end

function handler.message(fd, msg, sz)
    skynet.error(string.format("con msg  fd:%d msg:%s sz:%d",fd,msg,sz))
    if wait_auth[fd] then 
        local s = skynet.call(auth,"lua","balance",fd)
        -- 调用验证服务验证
        skynet.redirect(s,skynet.self(),"client",fd,msg,sz)
    else

    end
    
end

function handler.connect(fd, addr)
    wait_auth[fd] = true
    skynet.error(string.format("new con fd:%d host:%s",fd,addr))

    gateserver.openclient(fd)
end

function handler.disconnect(fd)

    close_fd(fd)
    skynet.error(string.format("con disconnect fd:%d",fd))
end


function handler.error(fd, msg)
    close_fd(fd)
    skynet.error(string.format("con error fd:%d",fd))
end


function handler.warning(fd, size)
    skynet.error(string.format("con warning fd:%d",fd))
end

local CMD = {}

function handler.command(cmd, source, ...)
	local f = assert(CMD[cmd],cmd)
	return f(source, ...)
end


skynet.init(function ()

    auth = skynet.uniqueservice("authd");

end)

gateserver.start(handler)

