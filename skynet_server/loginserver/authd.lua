local skynet =  require "skynet"
require "skynet.manager"
local client =  require "client"
local dbmgr = require "dbmgr"
local rediskey = require "rediskey"
local generator = require "generator" 
--local nodemgr  = require "nodemgr"
--local gatemgr = require "gatemgr"
local errcode = require "errcode"

local hub =  ...

local NODEID = skynet.getenv("nodeid")

local MSG = client.gethandler()


function MSG.register(fd,data)
    local account = data.account
	local password = data.password

	-- 检验账户是否存在
	local ret = dbmgr.sadd(rediskey.account_list,account)
	if ret == 0 then 
		skynet.error("注册失败账号已经存在 账号:" .. account )
		return errcode.ACCOUNT_EXIST
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
		skynet.error("添加账号失败")
		return errcode.ADD_ACOUNT_FAIL
	end
	ret = dbmgr.hget(rediskey.account,account)
	table.dump(ret)

	skynet.error(string.format("注册账号：%s UUID：%d ",account,uuid))
	return errcode.RT_OK
end



function MSG.login(fd,data)
    local account = data.account
	local password = data.password

	-- 验证用户是否存在
	local ret = dbmgr.sadd(rediskey.account_list,account)
	if ret == 1 then
		skynet.error("账号不存在 账号：" .. account)
		return errcode.ACCOUNT_NOT_EXIST
	end  
	-- 验证登陆密码正确性
	ret = dbmgr.hget(rediskey.account,account)
	table.dump(ret)
	if not ret  then
		skynet.error("数据库获取密码失败 账号：" .. account)
		return errcode.DB_GET_PASSWORD_FAIL
	end  

	local info = table.unserialize(ret)
	if info.password ~= password then
		skynet.error("账号密码错误 账号：" .. account)
		return errcode.PASSWORD_INCORRECT
	end

	-- uuid -- token  expire
	local token = math.floor( generator.uuid(NODEID) )
	local key = generator.keygen(rediskey.token,info.uuid)
	dbmgr.set(key,token) 
	dbmgr.expire(key,10) 

	local gateinfo = "127.0.0.1:8005"--gatemgr.balance()
	if gateinfo == nil then 
		return {  {} }
	end 

	local data = {
		result = true,
		uuid = info.uuid,
		token = token,
		gate_addr = gateinfo.host,
	}

	return {  data = data}
end


local function slave_laucher()

	local CMD = {}
	function CMD.connect(fd)
		client.connect(fd)
	end

	function CMD.disconnect(fd)
		client.disconnect(fd)
	end

    skynet.dispatch("lua", function(session, source, cmd, ...)
        local f = CMD[cmd]
        assert(f,cmd) 
        skynet.retpack( f(...) ) 
    end)

	local handler = {}
	function handler.closeclient(fd)
		skynet.send(hub,"lua","closeclient",fd)
	end
    client.register_protocol(handler)
end
   

local function master_laucher()
    local slaves = {}
    local scnt = 10
    for i=1,scnt do
        table.insert(slaves,skynet.newservice("authd",hub))
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