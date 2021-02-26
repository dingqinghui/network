local skynet = require "skynet"


local AGENT_CHECK_INTERVAL = 6000

local agentpool = __G_CLASS__(...)


function agentpool:ctor(agentname,initcnt,...)
    self.__agentname = agentname
    self.__paramter = {...}
    self.__cap = 0
    self.__cnt = 0
    self.__pool = {}

    self:on_append(initcnt)
    -- 定时补充
    skynet.fork(agentpool.on_agent_cnt_check,self)
end 


function agentpool:dector()

end 


function agentpool:on_agent_cnt_check()
    while true do 
        if not self then 
            return 
        end 

        if self.__cnt <= 0 then 
            self:on_append(self.__cap)
        end

        skynet.sleep(AGENT_CHECK_INTERVAL)
    end
end 

function agentpool:on_newagent()
    local agent = skynet.newservice(self.__agentname,table.unpack(self.__paramter))
    assert(agent)
    table.insert(self.__pool,agent)
    self.__cnt  = self.__cnt + 1
    self.__cap  = self.__cap + 1

    --DEBUG_LOG("agent pool cap:%d cnt:%d",self.__cap,self.__cnt)
end 

function agentpool:on_append(appendcap)
    for i=1, appendcap do
        self:on_newagent(appendcap)
    end
end


function agentpool:get()
    if self.__cnt <= 0 then 
        self:on_newagent()
    end 
    self.__cnt  = self.__cnt - 1
    local service = table.remove(self.__pool)
    assert(service)
    return service 
end



function agentpool:release(service)
    self.__cnt  = self.__cnt + 1
    return table.insert(self.__pool,service)
end


return agentpool