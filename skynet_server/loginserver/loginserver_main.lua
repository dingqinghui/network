local skynet =  require "skynet"
require "skynet.manager"

local nodemgr = require"nodemgr"

local GETENV = skynet.getenv

skynet.start(function ()

    -- 守护进程模式无法开启console
	if GETENV("daemon") == nil then
		skynet.newservice("console")
	end
	
	--skynet.newservice("debug_console", GETENV("console_port") or 8001)
	local watchdog = skynet.newservice("watchdog","login_agent" )
	local conf={
		address = GETENV("address") or "127.0.0.1",
		port = GETENV("port") or 17000,
		maxclient = GETENV("maxclient") or 65535,
		nodelay = GETENV("nodelay") or true,
	}

	skynet.newservice("gate_mgr")

	skynet.call(watchdog, "lua", "start", conf)
	
	skynet.exit()

end )