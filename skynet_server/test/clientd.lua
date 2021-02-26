local skynet = require "skynet"
local client = require "client"


math.randomseed(os.time())




skynet.start(function ()

   for i=1,10 do
        local cli = client.new(i)
   end
end
)



  
  