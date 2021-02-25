local skynet = require "skynet"


local agentpool = __G_CLASS__(...)


function agentpool:ctor(agentname,count)
    self.__agentname = agentname
    self.__cap = 0
    self.__cnt = 0
    self.__pool = {}

    self:append(count)
end 

function agentpool:dector()

end 


function agentpool:append(appendcap)
    for i=1, appendcap do
        local handle = skynet.newservice(self.__agentname,skynet.self())
        assert(handle)
        table.insert(self.pool,handle)
        self.__cnt  = self.cnt + 1
        self.__cap  = self.cap + 1
    end
    --INFO_LOG("logic agent appen %d",appendcap)
end


function agentpool:ref()
    if self.__cnt <= 0 then 
        self:append()
    end 
    self.__cnt  = self.__cnt - 1
    local service = table.remove(self.__pool)
    assert(service)
    return service 
end



function agentpool.unref(service)
    self.__cnt  = self.__cnt + 1
    return table.remove(self.__pool,service)
end


return agentpool