

local skynet = require "skynet"
local traceback = debug.traceback
local XPCALL = xpcall
local msgimpl = {}

local MSG_HANDLER_LIST = {}

function msgimpl.register(msgid, f)
    if MSG_HANDLER_LIST[msgid] then
        return
    end
    MSG_HANDLER_LIST[msgid] = f
end

function msgimpl.get(msgid)
    local handler = MSG_HANDLER_LIST[msgid]
    return handler
end

return msgimpl