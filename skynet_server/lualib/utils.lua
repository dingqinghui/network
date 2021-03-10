local skynet = require "skynet"


local utils = {}

function utils.dispatch_lua(handler,show_source)
    skynet.dispatch("lua", function(session, source, cmd, ...)
        if handler then 
            local f = handler[cmd]
            assert(f,cmd) 
            if  show_source then 
                skynet.retpack( f(source,...) ) 
            else
                skynet.retpack( f(...) ) 
            end 
        end 
    end)
end 






return utils

