local skynet = require "skynet"
local client = require "client"


math.randomseed(os.time())




skynet.start(function ()
   for i=1,1000 do
        local cli = client.new(i)
   end
end
)



  
  