local skynet = require "skynet"
local comdefine = require "comdefine"

local SERVICE_NAME = comdefine.SERVICE_NAME

local redis
local mysql

skynet.init(function()
    redis = skynet.localname(SERVICE_NAME.DB_REDIS)
    mysql = skynet.localname(SERVICE_NAME.DB_MYSQL)

end)




local dbmgr =  setmetatable({}, {__index = function(tab,key)
    local f = function (...)
        return skynet.call(dbservice,"lua",key,...)
    end
    return f
end}) 


return dbmgr

