local skynet =  require "skynet"
require "skynet.manager"
local client =  require "client"
local dbmgr = require "dbmgr"
local rediskey = require "rediskey"
local generator = require "generator" 
--local nodemgr  = require "nodemgr"
--local gatemgr = require "gatemgr"

local NODEID = skynet.getenv("nodeid")

local MSG = client.gethandler()


function MSG.register(fd,data)
    local account = data.account
	local password = data.password


	-- 检验账户是否存在
	local ret = dbmgr.sadd(rediskey.account_list,account)
	
	if ret == 0 then 
		skynet.error("dbmgr.sadd ret:",ret)
		return {}
	end 
	
	-- 生成UUID
	local uuid = generator.uuid(NODEID)
	local info = {
		account = account,
		password = password,
		uuid = uuid
	}
	
	-- 保存账号信息
	ret = dbmgr.hset(rediskey.account,account,table.serialize(info))
	if ret == 0 then 
		dbmgr.srem(rediskey.account_list,account)
		return {}
	end

	return { data = {result = true} }
end

function MSG.login(fd,data)
    local account = data.account
	local password = data.password

	-- 验证用户是否存在
	local ret = dbmgr.sadd(rediskey.account_list,account)
	if ret == 1 then
		skynet.error("account not exist")
		return 
	end  
	-- 验证登陆密码正确性
	ret = dbmgr.hget(rediskey.account,account)
	if not ret  then
		return 
	end  

	local info = table.unserialize(ret)
	if info.password ~= password then
		skynet.error("password not correct")
		return 
	end

	-- uuid -- token  expire
	local token = math.floor( generator.uuid(NODEID) )
	local key = generator.keygen(rediskey.token,info.uuid)
	dbmgr.set(key,token) 
	dbmgr.expire(key,10) 

	local gateinfo = "127.0.0.1:8005"--gatemgr.balance()
	if gateinfo == nil then 
		return {  {}}
	end 

	local data = {
		result = true,
		uuid = info.uuid,
		token = token,
		gate_addr = gateinfo.host,
	}
	skynet.error("data:",table.dump(data))

	return {  data = data}
end


local function slave_laucher()
    local CMD = {}

    skynet.dispatch("lua", function(session, source, cmd, ...)
        local f = CMD[cmd]
        assert(f,cmd)
        skynet.retpack( f(...) ) 
    end)

    client.register_protocol()
end
   

local function master_laucher()
    local slaves = {}
    local scnt = 10
    for i=1,scnt do
        table.insert(slaves,skynet.newservice("authd"))
    end
    
    local CMD  = {}
	function CMD.balance(fd)
		local i  = math.floor( fd % scnt )
        local s = slaves[i]
        return s
    end
	
	
    skynet.dispatch("lua", function(session, source, cmd,...)
        local f = CMD[cmd]
        assert(f,cmd)
        skynet.retpack( f(...) ) 
    end)
end

skynet.start(function ()
    local handle = skynet.localname(".auth")
    if handle then 
        slave_laucher()
    else
        skynet.register(".auth")
        master_laucher()
    end 
end)