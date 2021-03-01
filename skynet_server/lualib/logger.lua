
local skynet = require "skynet"
local comdefine = require "comdefine"

local LOG_LEVEL = comdefine.LOG_LEVEL

local function send2logger(level,...)
    local msg
    if select("#",...) == 1 then 
        msg = ...
    else
        msg = string.format(...)
    end 
    
    local logger = skynet.localname(".logger")
    if logger then 
        skynet.send(logger,"lua","log",level,msg)
    else
        print(msg)
    end 
end


function DEBUG_LOG(...)
    send2logger(LOG_LEVEL.DEBUG,...)
end


function INFO_LOG(...)
    send2logger(LOG_LEVEL.INFO,...)
end

function WARN_LOG(...)
    send2logger(LOG_LEVEL.WARNNING,...)
end


function ERROR_LOG(...)
    send2logger(LOG_LEVEL.ERROR,...)
end


function FATAL_LOG(...)
    send2logger(LOG_LEVEL.FATAL,...)
end