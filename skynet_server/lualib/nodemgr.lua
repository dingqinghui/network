local skynet = require "skynet"


local nodemgr = setmetatable({},{
    __index = function (self,key)
        return setmetatable({},    {__call = function (tab,...)
            assert(self.service)
            return skynet.call(self.service,"lua",key,...)
        end
        })
    end ,
})
  
nodemgr.service = nil
local self = nodemgr

skynet.init(function ()
    self.service = skynet.localname(".nodemgr")
    if not rawget(self,"service") then 
        self.service = skynet.newservice("nodemgrd")
    end 
end )

function nodemgr.call(node,addrname,...)
    assert(rawget(self,"service"))
    return skynet.call(self.service,"lua","call",node,addrname,...)
end 


function nodemgr.send(node,addrname,...)
    assert(rawget(self,"service"))
    skynet.send(self.service,"lua","send",node,addrname,...)
end 


return nodemgr
