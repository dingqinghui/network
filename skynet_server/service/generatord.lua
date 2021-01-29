local skynet = require "skynet"
local dbmgr = require "dbmgr"
local queue = require "skynet.queue"
local rediskey = require "rediskey"

local cs = queue()

local CMD = {}

local index = 0
function CMD.token()
    local tm = math.floor(skynet.time() )
    local token = tm << 20
    token = token | index
    index = index + 1
    return token
end

function CMD.uuid()
    dbmgr.incr(rediskey.account_cnt)
    local cnt = dbmgr.get(rediskey.account_cnt)
    return cnt * 4095
end

skynet.start(function ()
    skynet.dispatch("lua",function (session,source,cmd,...)
        local f = CMD[cmd]
        if f then 
            skynet.retpack( cs( f,...) )
        end
    end)
end)