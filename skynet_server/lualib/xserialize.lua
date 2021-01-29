
local cmsgpack = require "cmsgpack"

local xserialize = {}


function xserialize.encode(buf)
    local dst = cmsgpack.pack(buf)
    return dst
end 

function xserialize.decode(buf)
    local dst = cmsgpack.unpack(buf)
    return dst
end 

return xserialize