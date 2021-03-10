
local skynet = require "skynet"
local comdefine = require "comdefine"
local rediskey = require "rediskey"
local generator = require "generator" 
local sqldef = require "sqldef"
local json = require "json"

local SERVICE_NAME = comdefine.SERVICE_NAME
local ONE_DAY_SECOND = comdefine.ONE_DAY_SECOND
local TABLE_NAME = comdefine.TABLE_NAME

local userdb = {}

local self =  userdb


skynet.init(function()
    if not self.__redis then 
        self.__redis = skynet.localname(SERVICE_NAME.DB_REDIS)
    end 
    if not self.__mysql then 
        self.__mysql = skynet.localname(SERVICE_NAME.DB_MYSQL)
    end

    assert(self.__redis)
    assert(self.__mysql)
end)



function userdb.load(uuid)
    assert(uuid)
    local key = generator.keygen(rediskey.userkey,uuid)

    local ret = skynet.call(self.__redis,"lua","EXISTS",key)
    if ret == 1 then 
        return skynet.call(self.__redis,"lua","get",key)
    else
        local sql = generator.keygen(sqldef.LOAD_USER,uuid)
        local res = skynet.call(self.__mysql,"lua","query",sql)
        skynet.call(self.__redis,"lua","set",key,json.encode(res))
        skynet.call(self.__redis,"lua","expire",key,ONE_DAY_SECOND)
        return res
    end 
end

function userdb.save(uuid,data,binsert)
    local key = generator.keygen(rediskey.userkey,uuid)
    skynet.call(self.__redis,"lua","del",key)

    local sql
    if binsert then 
        sql = generator.insert_sql(TABLE_NAME.USER ,data)
    else
        sql = generator.update_sql(TABLE_NAME.USER ,data,{uuid = uuid})
    end
    print(sql)
    local res = skynet.call(self.__mysql,"lua","query",sql)
end

return userdb