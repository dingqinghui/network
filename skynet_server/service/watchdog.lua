local skynet = require "skynet"
require "skynet.manager"
local args = { ... }
local agent_name = args[1]
local _conf = {}

local CMD = {}
local SOCKET = {}
local gate
local agent = {}
local ccnt = 0
function SOCKET.open(fd, addr)
	skynet.error("New client from : " .. addr)
	agent[fd] = skynet.newservice(agent_name)
	skynet.call(agent[fd], "lua", "start", { gate = gate, client = fd, watchdog = skynet.self() })

	ccnt  = ccnt + 1
end

local function close_agent(fd)
	local a = agent[fd]
	agent[fd] = nil
	if a then
		skynet.call(gate, "lua", "kick", fd)
		-- disconnect never return
		skynet.send(a, "lua", "disconnect")

		ccnt = ccnt - 1
	end
end

function SOCKET.close(fd)
	skynet.error("socket close",fd)
	close_agent(fd)
end

function SOCKET.error(fd, msg)
	skynet.error("socket error",fd, msg)
	close_agent(fd)
end

function SOCKET.warning(fd, size)
	-- size K bytes havn't send out in fd
	skynet.error("socket warning", fd, size)
end

function SOCKET.data(fd, msg)
end

local function get_gate_addr()
	return string.format("%s:%s",_conf.address,_conf.port)
end



function CMD.start(conf)
	_conf = conf
	skynet.error(table.dump(conf))
	skynet.call(gate, "lua", "open" , conf)
end

function CMD.close(fd)
	close_agent(fd)
end

function CMD.get_agent_cnt()
	return ccnt
end

function CMD.get_gate_addr()
	return get_gate_addr()
end


function CMD.get_info()
	return {
		host=get_gate_addr(),
		agentcnt=ccnt,
	}
end 

skynet.start(function()
	skynet.dispatch("lua", function(session, source, cmd, subcmd, ...)
		if cmd == "socket" then
			local f = SOCKET[subcmd]
			f(...)
			-- socket api don't need return
		else
			local f = assert(CMD[cmd])
			skynet.ret(skynet.pack(f(subcmd, ...)))
		end
	end)

	gate = skynet.newservice("gate")

	skynet.register(".watchdog")
end)
