local skynet =  require "skynet"
local dbmgr = require "dbmgr"
local rediskey = require "rediskey"
local generator = require "generator" 
local errcode = require "errcode"
local client =  require "client"
local comdefine = require "comdefine"
local userdb = require "userdb"
local USER_STATUS = comdefine.USER_STATUS

local SERVICE_NAME = comdefine.SERVICE_NAME
local MSG = client.gethandler()

local NODEID = skynet.getenv("nodeid")

function MSG.enter_game_msg(fd,data)
    local uuid = data.uuid

    if not __USER__ then 
        return errcode.USER_NOT_EXIST
    end 

    if uuid ~= __USER__:getuuid() then 
        return errcode.USER_UUID_ERROR
    end 

    local statu =  __USER__:get_statu()
    if statu == USER_STATUS.LOADING_DATA then 
        return errcode.USER_LOADING_DATA
    else if statu == USER_STATUS.LOADED then 

    else
        __USER__:set_statu(USER_STATUS.LOADING_DATA)
        local data = userdb.load(uuid)
        __USER__:load_data(data)
        __USER__:set_statu(USER_STATUS.LOADED)
    end 

    local data = __USER__:enter_game()
	return errcode.RT_OK,data
end






