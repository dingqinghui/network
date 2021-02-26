local skynet = require "skynet"
require "skynet.manager"
local fileutile = require "fileutile"
local comdefine = require "comdefine"
local utils = require "utils"

local os_date = os.date
local CMD = {}

local LOG_LEVEL = comdefine.LOG_LEVEL

local logpath =  skynet.getenv("logdir") or "./logger/"
local log_level = LOG_LEVEL[skynet.getenv("loglevel")] or LOG_LEVEL.INFO

fileutile.mkdir(logpath)

local COLOR_RESET = "\x1b[0m"
local COLOR_RED = "\x1b[31m"
local COLOR_GREEN = "\x1b[32m"
local COLOR_BLUE = "\x1b[34m"
local COLOR_YELLO = "\x1b[33m"
local COLOR_L_BLUE = "\x1b[36m"

local level_colors = {
	[LOG_LEVEL.DEBUG]    = COLOR_L_BLUE,
	[LOG_LEVEL.INFO]     = COLOR_GREEN,
	[LOG_LEVEL.WARNNING] = COLOR_YELLO,
	[LOG_LEVEL.ERROR]    = COLOR_RED,
	[LOG_LEVEL.FATAL]    = COLOR_RED
}

local function level2string(level)
    if LOG_LEVEL.DEBUG == level then 
        return "DEBUG"
    elseif LOG_LEVEL.INFO == level then 
        return "INFO"
    elseif LOG_LEVEL.WARNNING == level then 
        return "WARNNING"
    elseif LOG_LEVEL.ERROR == level then 
        return "ERROR"    
    elseif LOG_LEVEL.FATAL == level then 
        return "FATAL"
    end
end


local filepath
local file

local function save(msg)
    local newfilepath = string.format("%s%s.log",logpath,os_date("%Y-%m-%d"))
    if filepath ~= newfilepath then 
        if file then 
            file:close()
        end
        file = io.open(newfilepath,"a")
        assert(file)
        filepath = newfilepath
    end 
    file:write(msg .. "\n")
    file:flush()
end

local function logger(source,lv,msg)
    
    if log_level > lv then 
        return 
    end
    --[日志级别][时间][服务地址][消息]
    msg = string.format("[%s][%s][:%08x] %s",level2string(lv),os_date("%Y-%m-%d %H:%M:%S"),source,msg)
    -- 颜色
    local color = level_colors[log_level]
    msg = color .. msg .. COLOR_RESET
    print(msg)
    save(msg)
end

-- register protocol text before skynet.start would be better.
skynet.register_protocol {
	name = "text",
	id = skynet.PTYPE_TEXT,
	unpack = skynet.tostring,
    dispatch = function(_, address, ...)
		logger(address,log_level,...)
	end
}

skynet.register_protocol {
	name = "SYSTEM",
	id = skynet.PTYPE_SYSTEM,
	unpack = function(...) return ... end,
	dispatch = function()
		-- reopen signal
		print("SIGHUP")
	end
}

function CMD.log(source,level,...)
    logger(source,tonumber(level),...)
end


skynet.start(function()
    
    utils.dispatch_lua(CMD,true)

    skynet.register(".logger")
    
end)