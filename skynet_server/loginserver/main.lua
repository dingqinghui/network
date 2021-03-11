local skynet =  require "skynet"
require "skynet.manager"

--local nodemgr = require"nodemgr"

local redistool = require "redistool"

local GETENV = skynet.getenv

skynet.start(function ()
	local handle = skynet.self()

    -- 守护进程模式无法开启console
	if GETENV("daemon") == nil then
		skynet.newservice("console")
	end

	skynet.newservice("debug_console", GETENV("console_port") or 8001)
	
	skynet.newservice("debug_console", GETENV("console_port") or 8001)
	local hub = skynet.newservice("hub" )
	local conf={
		address = GETENV("address") or "127.0.0.1",
		port = GETENV("port") or 17000,
		maxclient = GETENV("maxclient") or 65535,
		nodelay = GETENV("nodelay") or true,
	}
	skynet.call(hub, "lua", "open", conf)


	--skynet.newservice("gate_mgr")

	skynet.exit()

end )



