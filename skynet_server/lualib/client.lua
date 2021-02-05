

local skynet = require "skynet"
local queue = require "skynet.queue"
local xserialize  = require "xserialize"
--local msgimpl = require "msgimpl"
local socket = require "skynet.socket"

local client ={
	MSG = {}
}

local function traceback(err)
	skynet.error("LUA ERROR: " .. tostring(err))
	skynet.error(debug.traceback())
end

local function send_package(fd,pack)
    local package = string.pack(">s2", xserialize.encode(pack) )
    socket.write(fd, package)
end

-- 消息分发
local function msg_dispatch(fd,message)
	if not client.MSG then 
		skynet.error("not resigster client msg handler")
	end 
    local f = client.MSG[message.name]
    if not f then 
        skynet.error("message not define callback")
        return nil
    end 
    -- 这里有待优化不是所有的消息都需要加锁来处理
    return f(fd,message.data)
end

local function dispatch_message(fd, address,data)
	-- 解包
	local message = xserialize.decode(data)
	skynet.error(string.format("request::%s",table.dump(message)))
	-- 消息分发
	local ok, result = xpcall(msg_dispatch, traceback, fd,message)
	-- 检测消息队列超长
	local mqlen = skynet.mqlen()
	if mqlen >= 100 then
		skynet.error("msgagent message queue length is too much,please check it.")
	end
	if not ok then
		skynet.error("message excute error messageid:",message.id)
	else
		skynet.error(string.format("respond::%s",table.dump(result)))
		send_package(fd, result)
	end
end



local function register_protocol()
	skynet.register_protocol {
		name = "client",
		id = skynet.PTYPE_CLIENT,	
		unpack = skynet.tostring,   --- 将C point 转换为lua 二进制字符串
		dispatch = function (fd, address, msg)
			skynet.ignoreret()	
			dispatch_message(fd, address,msg)
		end
	}
end


function client.gethandler()
	return client.MSG
end

client.send = send_package
client.register_protocol = register_protocol

return client