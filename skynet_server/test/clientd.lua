local skynet = require "skynet"
local client = require "client"
local coroutine = require "coroutine"
local usertime = require "usertime"


local CLIENT_CNT_MAX = 10
local curCnt = 0

math.randomseed(os.time())

function LoginFinish()

   curCnt = curCnt + 1
   if curCnt >= CLIENT_CNT_MAX then 
      INFO_LOG("登陆完成 milltime：%d 数量：%d",usertime.getmilliseconds(),curCnt)
   end 
end

skynet.start(function ()


   INFO_LOG("开始登陆 milltime：%d",usertime.getmilliseconds())
   for i=1,CLIENT_CNT_MAX do
        local cli = client.new(i)
   end
end
)



  
  