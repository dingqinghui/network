local skynet = require "skynet"
local socket = require "skynet.socket"
local xserialize  = require "xserialize"




local function launch_mysql()
    local mysql = skynet.newservice("mysqld")
    skynet.call(mysql,"lua","init",nil,10)
end

local function launch_redis()
	skynet.newservice("redisd")
end

skynet.init(function()
    launch_mysql()
    launch_redis()

end)

local userdb = require "userdb"

skynet.start(function ()
    for i=1,1 do
        skynet.newservice("clientd",i)
        print(i)
    end
    

    
    -- print( table.dump( userdb.load(1) ) )


    -- userdb.save(1111,{
    --     uuid = 1111,
    --     reg_time = skynet.time(),
    --     name = "测试名字2111"
    -- },false)
end
)
