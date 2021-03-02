local skynet =  require "skynet"
require "skynet.manager"

local nodemgr = require"nodemgr"

local GETENV = skynet.getenv


local watchdog = nil
local CMD = {}



skynet.start(function ()
	skynet.dispatch("lua", function(session, source, cmd, ...)
			local f = assert(CMD[cmd])
			skynet.ret(skynet.pack(f(cmd, ...)))
	end)



    -- 守护进程模式无法开启console
	if GETENV("daemon") == nil then
		skynet.newservice("console")
	end
	
	skynet.newservice("debug_console", GETENV("console_port") or 9003)
	

	skynet.newservice("usermgrd")


    skynet.exit()
    
end )