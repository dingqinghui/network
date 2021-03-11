local redis = require "skynet.db.redis"
local skynet =  require "skynet"
require "skynet.manager"
local utils = require "utils"
local comdefine = require "comdefine"

local SERVICE_NAME = comdefine.SERVICE_NAME
local GETENV = skynet.getenv

local function masterinit()
    local slaves = {}
    local balance = 0
    for i=1,10 do
        local addr = skynet.newservice("redisd")
        slaves[i] = addr
        skynet.error("slave :",addr)
    end

    skynet.dispatch("lua", function (_,source, cmd, ...)
        balance = balance + 1
        if balance > 10 then 
            balance = 1
        end
        
        local ret = skynet.call(slaves[balance],"lua",cmd,...)
        skynet.retpack(ret)
    end)

end



local function slaveinit()
    local conf = {
        host = GETENV("redishost") or "127.0.0.1",
        port = GETENV("redisport") or 6379,
        db = GETENV("redisdb") or 0,
        auth = GETENV("redisauth") ,
    }

    local db = { }

    local function connect(conf)
        db.con = redis.connect(conf)
        if db.con == nil then 
            reconnect(conf)
        else
            setmetatable(db, {__index = function (_,cmd)
                local f = function (...)
                    return db.con[cmd](db.con,...)
                end
                return f 
            end})
        end 
    end

    local function reconnect(conf)
        skynet.timeout(500,function (conf)
            connect(conf)
        end)
    end

    -- 连接redis
    connect(conf)

    utils.dispatch_lua(db)
end



skynet.start(function ()
    local master = skynet.localname(SERVICE_NAME.DB_REDIS)
    if master then 
        slaveinit()
    else
        skynet.register(SERVICE_NAME.DB_REDIS)
        masterinit()
    end 
end )