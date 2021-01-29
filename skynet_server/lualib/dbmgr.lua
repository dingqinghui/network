local skynet = require "skynet"


local dbservice


skynet.init(function()
	dbservice = skynet.uniqueservice "redisd"
end)


local function exec(key,...)
    return skynet.call(dbservice,"lua",key,...)
end


local dbmgr =  setmetatable({}, {__index = function(tab,key)
    local f = function (...)
        return skynet.call(dbservice,"lua",key,...)
    end
    return f
end}) 


return dbmgr

