local skynet =  require "skynet"
local dbmgr = require "dbmgr"
local rediskey = require "rediskey"
local generator = require "generator" 
local errcode = require "errcode"
local msgimpl = require "msgimpl"
local client =  require "client"

local MSG = client.gethandler()

local NODEID = skynet.getenv("nodeid")

function MSG.register(fd,data)
    local account = data.account
	local password = data.password

	-- 检验账户是否存在
	local ret = dbmgr.sadd(rediskey.account_list,account)
	if ret == 0 then 
		ERROR_LOG("注册失败账号已经存在 账号:" .. account )
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
		ERROR_LOG("添加账号失败")
		return errcode.ADD_ACOUNT_FAIL
	end
	ret = dbmgr.hget(rediskey.account,account)

	INFO_LOG("注册账号：%s UUID：%d ",account,uuid)
	return errcode.RT_OK
end



function MSG.login(fd,data)
    local account = data.account
    local password = data.password
    
    INFO_LOG("登陆%s ",account)
	-- 验证用户是否存在
	local ret = dbmgr.sadd(rediskey.account_list,account)
	if ret == 1 then
		ERROR_LOG("账号不存在 账号：" .. account)
		return errcode.ACCOUNT_NOT_EXIST
	end  
	-- 验证登陆密码正确性
	ret = dbmgr.hget(rediskey.account,account)
	if not ret  then
		ERROR_LOG("数据库获取密码失败 账号：" .. account)
		return errcode.DB_GET_PASSWORD_FAIL
	end  

	local info = table.unserialize(ret)
	if info.password ~= password then
		ERROR_LOG("账号密码错误 账号：" .. account)
		return errcode.PASSWORD_INCORRECT
	end

	-- uuid -- token  expire
	local token = math.floor( generator.uuid(NODEID) )
	local key = generator.keygen(rediskey.token,info.uuid)
	dbmgr.set(key,token) 
	dbmgr.expire(key,10) 

	local gateinfo = "127.0.0.1:27000"--gatemgr.balance()
	if gateinfo == nil then 
		return 
    end 
    
    skynet.send(__HUB__,"lua","closeclient",fd)

	return  errcode.RT_OK,  {
		uuid = info.uuid,
		token = token,
		gate_addr = "127.0.0.1:27000" --gateinfo.host,
	}

end


