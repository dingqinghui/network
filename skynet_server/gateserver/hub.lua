local skynet = require "skynet"
local gateserver = require "snax.gateserver"

local usermgr_mod = require "usermgr" 

local authmaster        -- 验证主服务
local hub = {}
local agent = {}        -- <fd,auth agent /logic agent>
local userlist = {}     -- <uuid,logic agent>

local usermgr 

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
    agent[fd] = s
    skynet.call(s,"lua","connect",fd)
end

function handler.message(fd, msg, sz)
    DEBUG_LOG("con msg  fd:%d msg:%s sz:%d",fd,msg,sz)

    local s = agent[fd]
    if not s then 
        return 
    end 
    -- 调用验证服务验证
    skynet.redirect(s,skynet.self(),"client",fd,msg,sz) 
end

function handler.disconnect(fd)
    local s = agent[fd]
    if not s then 
        return 
    end 

    usermgr:disconnect(fd) 
    skynet.call(s,"lua","disconnect",fd)
    close_fd(fd)
    DEBUG_LOG("con disconnect fd:%d",fd)
end


function handler.error(fd, msg)
    local s = agent[fd]
    if not s then 
        return 
    end
    
    usermgr:disconnect(fd) 
    skynet.call(s,"lua","disconnect",fd)
    close_fd(fd)
    DEBUG_LOG("con error fd:%d",fd)
end


function handler.warning(fd, size)
    DEBUG_LOG("con warning fd:%d",fd)
end


local CMD = {}


--[[
    @desc: 验证通过 分配logic agent
    author:{author}
    time:2021-02-23 17:17:49
    --@info: 
    @return:
]]
function CMD.authpass(srouce, info )
    usermgr:authpass(info.uuid,info)
end


--[[
    @desc: 清除玩家logic agent
    author:{author}
    time:2021-02-23 17:16:18
    --@uuid: 
    @return:
]]
function CMD.kick(source,fd,uuid)
    usermgr:kick(uuid)
    hub.kick(fd)

    DEBUG_LOG("主动关闭客户端 fd:" .. fd)
end


function handler.command(cmd, source, ...)
	local f = assert(CMD[cmd],cmd)
	return f(source, ...)
end


function hub.kick(fd)
    if fd and agent[fd] then 
        close_fd(fd)
        gateserver.closeclient(fd)
    end
end

function hub.setagent(fd,s)
    if not fd then 
        return 
    end 
    agent[fd] = s
end


skynet.init(function ()
    for i=1,5 do
        local s = skynet.newservice("authd",skynet.self())
        table.insert(authpool,s)
        authagentcnt  = authagentcnt + 1
    end
    --authmaster = skynet.uniqueservice("authd",skynet.self())
   

    usermgr = usermgr_mod.new(hub)
end)

gateserver.start(handler)

