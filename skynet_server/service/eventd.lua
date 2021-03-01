local skynet = require "skynet"
local mc = require "skynet.multicast"
local utils = require "utils"
require "skynet.manager"
require "logger"

local CMD = {}

local etmap = {
    -- [eventname] = {
    --     channel,
    --     ref,
    -- }
}

function CMD.new(etname)
    local info = etmap[etname]
    if not info then 
        local channel = mc.new()
        etmap[etname] = { 
            channel = channel,
            ref = 1
        }
        DEBUG_LOG("创建事件频道 事件：%s 频道：%d",etname ,channel.channel)
        return channel.channel
    else
        info.ref = info.ref + 1
        return info.channel.channel
    end
end

function CMD.del(etname)
    local info = etmap[etname]
    if not info then 
        return 
    end 
    info.ref = info.ref - 1
    if info.ref <= 0 then 
        local channel = info.channel
        DEBUG_LOG("删除事件频道 事件：%s 频道：%d",etname ,channel.channel)
        channel:delete()
        etmap[etname] = nil
    end 
end


skynet.start(function ()
    utils.dispatch_lua(CMD)
    skynet.register(".eventd")
end)

