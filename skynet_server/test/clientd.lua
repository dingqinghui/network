local skynet = require "skynet"
local client = require "client"
local coroutine = require "coroutine"

math.randomseed(os.time())




skynet.start(function ()
   for i=1,1 do
        local cli = client.new(i)
   end
end
)



  
  