local skynet =  require "skynet"
require "skynet.manager"

local nodemgr = require"nodemgr"

local GETENV = skynet.getenv


local function launch_mysql()
	local mysql = skynet.newservice("mysqld")
end

local function launch_redis()
	skynet.newservice("redisd")
end

skynet.start(function ()
	skynet.dispatch("lua", function(session, source, cmd, ...)
			local f = assert(CMD[cmd])
			skynet.ret(skynet.pack(f(cmd, ...)))
	end)

    -- 守护进程模式无法开启console
	if GETENV("daemon") == nil then
		skynet.newservice("console")
	end
	
	skynet.newservice("debug_console", GETENV("console_port") or 9010)
	

	launch_mysql()
	launch_redis()



    skynet.exit()
    
end )