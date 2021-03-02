local skynet = require "skynet"
require "skynet.manager"
local utils = require "utils"

local usermgr = require "usermgr"


local usermgr_mod

local CMD = {}

-- function CMD.syc_user_list(uuidlist,nodename)
--     usermgr_mod:syc_user_list(uuidlist,nodename)
-- end


function CMD.login(uuid,nodename)
    usermgr_mod:adduser(uuid,nodename)
end

function CMD.logout(uuid,nodename)
    usermgr_mod:deluser(uuid)
end


skynet.start(function ()
    utils.dispatch_lua(CMD)

    usermgr_mod = usermgr.new()

    skynet.register(".usermgr")
end)