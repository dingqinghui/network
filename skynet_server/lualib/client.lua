

local skynet = require "skynet"
local queue = require "skynet.queue"
local xserialize  = require "xserialize"
--local msgimpl = require "msgimpl"
local socket = require "skynet.socket"
local errcode = require "errcode"

local PING_INTERVAL = 5
local PING_MAX_CNT = 3

local c_pool = {}              

local client ={
	MSG = {},
	handler = nil
}

local MSG = client.MSG


function MSG.ping(fd)
	local info = c_pool[fd]
	if not info then 
		return 
	end
	local now=skynet.time()
	info.tm = now
	return errcode.RT_OK,{now = now }
end

local function traceback(err)
	skynet.error("LUA ERROR: " .. tostring(err))
	skynet.error(debug.traceback())
end

local function send_package(fd,pack)
    local package = string.pack(">s2", xserialize.encode(pack) )
	local suc = socket.write(fd, package)
	if not suc then 
		skynet.error("send pack fail fd:" .. fd)
	end
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
    return f(fd,message.data)
end

local function dispatch_message(fd, address,data)
	-- 解包
	local message = xserialize.decode(data)
	skynet.error(string.format("request::%s",table.dump(message)))
	-- 消息分发
	local ok, errcode,result = xpcall(msg_dispatch, traceback, fd,message)
	-- 检测消息队列超长
	local mqlen = skynet.mqlen()
	if mqlen >= 100 then
		skynet.error("msgagent message queue length is too much,please check it.")
	end
	if not ok then
		skynet.error("message excute error messageid:",message.name)
	else

		local rpack = { 
			name =string.format("response_%s",message.name),  
			errcode=errcode, 
			result = result 
		}
		send_package(fd, rpack)
		skynet.error(string.format("respond_%s::%s",message.name,table.dump(rpack)))
	end
end



local function register_protocol(handler)
	skynet.register_protocol {
		name = "client",
		id = skynet.PTYPE_CLIENT,	
		unpack = skynet.tostring,   --- 将C point 转换为lua 二进制字符串
		dispatch = function (fd, address, msg)
			skynet.ignoreret()	
			dispatch_message(fd, address,msg)
		end
	}

	client.handler = handler
end


function client.gethandler()
	return client.MSG
end

function client.connect(fd) 
	local tm = skynet.time()
	c_pool[fd] = {
		cnt=0,
		tm = tm
	}
	local function check_dead()
		
		local tm = skynet.time()
		local info = c_pool[fd]
		if not info then 
			return 
		end
		if tm - info.tm  > PING_INTERVAL then 
			info.cnt = info.cnt + 1
		else
			info.cnt = 0
		end
		if info.cnt >= PING_MAX_CNT then 
			if client.handler then 
				client.handler.closeclient(fd)
			end 
			c_pool[fd] = nil
		else
			skynet.timeout( PING_INTERVAL * 100,check_dead)
		end 
		
	end

	skynet.timeout( PING_INTERVAL * 100,check_dead)
end



function client.disconnect(fd) 
	c_pool[fd] = nil
end




client.send = send_package
client.register_protocol = register_protocol

return client