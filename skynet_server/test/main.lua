local skynet = require "skynet"


skynet.start(function ()
    
    skynet.newservice("debug_console", skynet.getenv("console_port") or 8002)

    skynet.newservice("eventtestd")

end )