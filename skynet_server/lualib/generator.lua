local skynet = require "skynet"

local generator = {}

local service
skynet.init(function()
    service = skynet.uniqueservice("generatord")
end)


function generator.token()
    return skynet.call(service,"lua","token") 
end



function generator.uuid()
    return skynet.call(service,"lua","uuid") 
end

function generator.keygen(str,...)
    return string.format(str,...)
end

return generator