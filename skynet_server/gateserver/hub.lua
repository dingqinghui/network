local skynet = require "skynet"
local gateserver = require "snax.gateserver"
local logic_agent_pool = require "logic_agent_pool"


local authmaster        -- 验证主服务

local agent = {}        -- <fd,auth agent /logic agent>
local userlist = {}     -- <uuid,logic agent>

skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
}

local handler = {}

local authpool = {}
local authagentcnt = 0

local function close_fd(fd)
    agent[fd] = nil
end

function handler.open(source, conf)

end


local function getauthagent(fd) 
    local index = fd % authagentcnt + 1
    return authpool[index]
end



function handler.connect(fd, addr)
    DEBUG_LOG("new con fd:%d host:%s",fd,addr)

    gateserver.openclient(fd)
    
    local s = getauthagent(fd)
    assert(s)
    skynet.call(s,"lua","connect",fd)
    agent[fd] = s
end

function handler.message(fd, msg, sz)
    DEBUG_LOG("con msg  fd:%d msg:%s sz:%d",fd,msg,sz)

    local s = agent[fd]
    assert(s)
    -- 调用验证服务验证
    skynet.redirect(s,skynet.self(),"client",fd,msg,sz) 
end

function handler.disconnect(fd)
    local s = agent[fd]
    skynet.call(s,"lua","disconnect",fd)

    close_fd(fd)
    DEBUG_LOG("con disconnect fd:%d",fd)
end


function handler.error(fd, msg)
    local s = agent[fd]
    skynet.call(s,"lua","disconnect",fd)

    close_fd(fd)
    DEBUG_LOG("con error fd:%d",fd)
end


function handler.warning(fd, size)
    DEBUG_LOG("con warning fd:%d",fd)
end


local CMD = {}

--[[
    @desc: 关闭socket连接 但是不清除agent
    author:{author}
    time:2021-02-23 17:15:42
    --@source:
	--@fd: 
    @return:
]]
function CMD.closeclient(source,fd)
    close_fd(fd)
    gateserver.closeclient(fd)
    DEBUG_LOG("主动关闭客户端 fd:" .. fd)
end

--[[
    @desc: 验证通过 分配logic agent
    author:{author}
    time:2021-02-23 17:17:49
    --@info: 
    @return:
]]
function CMD.authpass(srouce, info )
    local s = nil
    if userlist[info.uuid] then 
        s = userlist[info.uuid].agent
        -- 关闭旧socket连接
        local fd = userlist[info.uuid].fd
        if agent[fd] then 
            close_fd(fd)
            gateserver.closeclient(fd)
        end 
    else
        s = logic_agent_pool.ref()
    end

    skynet.call(s,"lua","authpass",{
        uuid = info.uuid,
        token = info.token,
        fd = info.fd
    })

    agent[info.fd] = s
    info.agent = s
    userlist[info.uuid] = info
end

--[[
    @desc: 清除玩家logic agent
    author:{author}
    time:2021-02-23 17:16:18
    --@uuid: 
    @return:
]]
function CMD.kick(source,uuid)
    if userlist[uuid] then 
        logic_agent_pool.unref(userlist[uuid].agent)
        userlist[uuid] = nil
    end
end


function handler.command(cmd, source, ...)
	local f = assert(CMD[cmd],cmd)
	return f(source, ...)
end



skynet.init(function ()
    for i=1,5 do
        local s = skynet.newservice("authd",skynet.self())
        table.insert(authpool,s)
        authagentcnt  = authagentcnt + 1
    end
    --authmaster = skynet.uniqueservice("authd",skynet.self())
    logic_agent_pool.init("gate_agent",10)
end)

gateserver.start(handler)

