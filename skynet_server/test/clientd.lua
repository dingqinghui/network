local skynet = require "skynet"
local client = require "client"
local coroutine = require "coroutine"
local usertime = require "usertime"


local tm
local CLIENT_CNT_MAX = 100
local curCnt = 0

math.randomseed(os.time())

function LoginFinish()

   curCnt = curCnt + 1
   if curCnt >= CLIENT_CNT_MAX then 
      INFO_LOG("登陆完成 cost:[%d] 数量：[%d]",usertime.getmilliseconds() - tm,curCnt)
   end 
end

skynet.start(function ()

   tm = usertime.getmilliseconds()
   INFO_LOG("开始登陆 milltime：%d",tm)
   for i=1,CLIENT_CNT_MAX do
        local cli = client.new(i)
   end

end
)



  
  