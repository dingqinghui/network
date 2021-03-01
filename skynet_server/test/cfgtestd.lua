local skynet = require "skynet"
local cfgmgr = require "cfgmgr"

skynet.start(function ()
    cfgmgr.initall("./config")
    local common = cfgmgr.get("Common")
    --print(table.dump( common) )
    --skynet.sleep(1000)
    --cfgmgr.reload("./config")

    --print(table.dump(cfgmgr.get("Common") ) )

    --print(table.dump( common) )
end )