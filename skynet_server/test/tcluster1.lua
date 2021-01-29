local skynet = require "skynet"
local cluster = require "skynet.cluster"
local nodemgr = require "nodemgr"
skynet.start(function ()
    skynet.exit()
end )