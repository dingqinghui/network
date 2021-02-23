local skynet = require "skynet"
local fileutile = require "fileutile"

skynet.start(function ()
    DEBUG_LOG("%s %d","ssssssssss",1)

    skynet.newservice("debug_console",8000)

    skynet.fork(function()
        while true do
            skynet.error("test")
            skynet.sleep(200)
        end
    end )
end)