local skynet =  require "skynet"
require "skynet.manager"

local nodemgr = require"nodemgr"

local GETENV = skynet.getenv


local watchdog = nil


local function launcher_hub()
	local hub = skynet.newservice("hub")
	local conf={
		address = GETENV("address") or "127.0.0.1",
		port = GETENV("port") or 27000,
		maxclient = GETENV("maxclient") or 65535,
		nodelay = GETENV("nodelay") or true,
	}
	skynet.call(hub, "lua", "open", conf)
end

local function launch_mysql()
    local mysql = skynet.newservice("mysqld")
    skynet.call(mysql,"lua","init",nil,10)
end

local function launch_redis()
	skynet.newservice("redisd")
end

skynet.start(function ()

    -- 守护进程模式无法开启console
	if GETENV("daemon") == nil then
		skynet.newservice("console")
	end
	
	skynet.newservice("debug_console", GETENV("console_port") or 8002)
	
	skynet.newservice("quitd")
	
	launch_redis()
	launch_mysql()
	launcher_hub()


    skynet.exit()
    
end )