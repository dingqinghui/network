local skynet = require "skynet"
require "skynet.manager"
local utils = require "utils"
local verifymgr = require "verifymgr"
local errcode = require "errcode"
local comdefine = require "comdefine"

local verifymgr_ins = nil


local CMD = {}

function CMD.get_user_list()
    if not verifymgr_ins then 
        retuirn {}
    end
    return verifymgr_ins:get_user_list()
end 

function CMD.syc_verify_data(uuid,token,expire)
    verifymgr_ins:update(uuid,token,expire)
end

function CMD.verify_token(uuid,token)
    return verifymgr_ins:verify_token(uuid,token)
end

-- 玩家网络断开 更新过期时间
function CMD.update_expire(uuid,expire)
    verifymgr_ins:assign_expire(uuid,expire)
end

skynet.start(function ()
    utils.dispatch_lua(CMD)

    verifymgr_ins = verifymgr.new()

    skynet.register(".verifyd")
end)