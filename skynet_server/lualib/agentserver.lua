local skynet = require "skynet"
local netpack = require "skynet.netpack"
local socket = require "skynet.socket"
--local profile = require "profile"
local queue = require "skynet.queue"
local xserialize  = require "xserialize"
local msgimpl = require "msgimpl"



local agentserver = {}


local WATCHDOG
local client_fd

--------------------------------------
-- 用于协议的编码解码
local cs = queue()
local os_time = os.time
---------------------------------------
local function traceback(err)
	skynet.error("LUA ERROR: " .. tostring(err))
	skynet.error(debug.traceback())
end

function agentserver.start(handler)

	skynet.register_protocol {
		name = "client",
		id = skynet.PTYPE_CLIENT,	
		unpack = skynet.tostring,   --- 将C point 转换为lua 二进制字符串
	}
	
	local function send_package(pack)
		local package = string.pack(">s2", pack)
		skynet.error("sendpack size:",#package)
		socket.write(client_fd, package)
	end
	
	local function push_message(msgid, data)
		send_package(xserialize.encode({id = msgid,data = data}))
	end
	
	-- 消息分发
	local function msg_dispatch(message)
		local f = msgimpl.get(message.id)
		if not f then 
			skynet.error("message not define callback")
			return nil
		end 
		-- 这里有待优化不是所有的消息都需要加锁来处理
		return f(message.data)
	end
	
	
	local function dispatch_message(session, address,data)
		-- 解包
		local message = xserialize.decode(data)
		skynet.error(string.format("request::%s",table.dump(message)))
		-- 消息分发
		local ok, result = xpcall(msg_dispatch, traceback, message)
		-- 检测消息队列超长
		local mqlen = skynet.mqlen()
		if mqlen >= 100 then
			skynet.error("msgagent message queue length is too much,please check it.")
		end
		if not ok then
			skynet.error("message excute error messageid:",message.id)
		else
			skynet.error(string.format("respond::%s",table.dump(result)))
			send_package( xserialize.encode(result) )
		end
	end



	local CMD = {}
	-- 发送数据
	function CMD.push_message(msgid, data)
		push_message(msgid, data)
	end
	
	-- socket连接成功
	function CMD.start(conf)
		local fd = conf.client
		local gate = conf.gate
		WATCHDOG = conf.watchdog
	
		client_fd = fd
		skynet.call(gate, "lua", "forward", fd)

		if handler.connect then 
			handler.connect()
		end
	end
	
	-- socket断开连接
	function CMD.disconnect()
		if handler.disconnect then 
			handler.disconnect()
		end
		skynet.exit()
	end
	
	
	skynet.start(function()
		-- 注册消息处理
		skynet.dispatch("lua", function(session, source, cmd, ...)
			local f = CMD[cmd]
			if  f then 
				skynet.retpack(f(...))
			else
				skynet.retpack(handler.commond(cmd,...))
			end
		end)
	
		skynet.dispatch("client", function (fd, address, msg)
			assert(fd == client_fd)	-- You can use fd to reply message
			skynet.ignoreret()	-- session is fd, don't call skynet.ret
			dispatch_message(fd, address,msg)
		end)
	end)
end


return agentserver








