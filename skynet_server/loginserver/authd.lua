local skynet =  require "skynet"
require "skynet.manager"
local client =  require "client"
require "authmsg"

--local nodemgr  = require "nodemgr"
--local gatemgr = require "gatemgr"


local hub =  ...

local function slave_laucher()

	local CMD = {}
	function CMD.connect(fd)
		client.start_ping(fd)
	end

	function CMD.disconnect(fd)
		client.stopping(fd)
	end

    skynet.dispatch("lua", function(session, source, cmd, ...)
        local f = CMD[cmd]
        assert(f,cmd) 
        skynet.retpack( f(...) ) 
    end)

	local handler = {}
	function handler.closeclient(fd)
		skynet.send(hub,"lua","closeclient",fd)
	end
    client.start(handler)
end
   

local function master_laucher()
    local slaves = {}
    local scnt = 10
    for i=1,scnt do
        table.insert(slaves,skynet.newservice("authd",hub))
    end
    
    local CMD  = {}
	function CMD.balance(fd)
		local i  = math.floor( fd % scnt )
        local s = slaves[i]
        return s
    end
	
	
    skynet.dispatch("lua", function(session, source, cmd,...)
        local f = CMD[cmd]
        assert(f,cmd)
        skynet.retpack( f(...) ) 
    end)
end



skynet.start(function ()
    local handle = skynet.localname(".auth")
    if handle then 
        slave_laucher()
    else
        skynet.register(".auth")
        master_laucher()
    end 
end)