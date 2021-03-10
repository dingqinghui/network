local skynet  = require "skynet"
local comdefine = require "comdefine"

local SERVICE_NAME = comdefine.SERVICE_NAME

local CMD = {}

-- key host@数据库名称 value mysqlagents
local agentpool = {}
local blance = 1

--解析URL内容
local function parseURL(dburl)
	local urlarray = string.split(dburl,"#")
	assert(#urlarray == 5)

	local ip = assert(urlarray[1])
	local port = assert(urlarray[2])
	local user = assert(urlarray[3])
	local pwd = urlarray[4]
	local db = assert(urlarray[5])

	return { host = ip, port = port, user = user, password = pwd, database = db }
end

function CMD.init(config, poolsize)
    if not config then
        local dburl = skynet.getenv("mysqlconf")
        config = parseURL(dburl)
    end
    local dbconfig = config

    assert(dbconfig.host, "DB config host has error. host:" .. tostring(dbconfig.host))
	assert(dbconfig.port, "DB config port has error. port:" .. tostring(dbconfig.port))
	assert(dbconfig.user, "DB config user has error. user:" .. tostring(dbconfig.user))
	assert(dbconfig.password, "DB config password has error. password:" .. tostring(dbconfig.password))
	assert(dbconfig.database, "DB config database has error. database:" .. tostring(dbconfig.database))

    poolsize = poolsize or 1
    INFO_LOG("Init mysql db:%s pool size:%d.", dbconfig.database, poolsize)
    for i=1,poolsize do
        local agent_addr = skynet.newservice("mysqlagent")
        local ret = skynet.call(agent_addr, "lua", "init", dbconfig)
        if ret then
            table.insert(agentpool, agent_addr)
        end
    end

	return dbconfig.database
end


function CMD.query( sql)
    assert(sql)
    blance = blance + 1
    if blance > #agentpool then
        blance = 1
    end
    print(table.dump(agentpool),blance)
    local agent_addr = agentpool[blance] 
    assert(agent_addr)
    return skynet.call(agent_addr, "lua", "query", sql)
end


function CMD.getslave()
    assert(sql)
    blance = blance + 1
    if blance > #agentpool then
        blance = 1
    end
    return agentpool[blance] 
end

require "skynet.manager"
skynet.start(function()
    
    skynet.dispatch("lua", function(session, source, command, ...)
        local f = assert(CMD[command])
		local data = f(...)
		skynet.ret(skynet.pack(data))
    end)
    skynet.register(SERVICE_NAME.DB_MYSQL)
end)