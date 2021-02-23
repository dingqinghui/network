local skynet = require "skynet"

local logic_agent_pool = {
    pool = {},
    cap = 0,
    cnt = 0,
    agentname = "",
}

local self = logic_agent_pool

function logic_agent_pool.init(agentname,count)
    self.agentname = agentname
    for i=1,count do
        local handle = skynet.newservice(agentname,skynet.self())
        assert(handle)
        table.insert(self.pool,handle)
        self.cnt  = self.cnt + 1
        self.cap  = self.cap + 1
    end
end

function logic_agent_pool.append()
    local appendcap = self.cap 
    for i=1, appendcap do
        local handle = skynet.newservice(self.agentname)
        assert(handle)
        table.insert(self.pool,handle)
        self.cnt  = self.cnt + 1
        self.cap  = self.cap + 1
    end
end

function logic_agent_pool.ref()
    if self.cnt <= 0 then 
        self.append()
    end 
    self.cnt  = self.cnt - 1
    local handle = table.remove(self.pool)
    assert(handle)
    return handle 
end

function logic_agent_pool.unref(handle)
    self.cnt  = self.cnt + 1
    return table.remove(self.pool,handle)
end


return logic_agent_pool