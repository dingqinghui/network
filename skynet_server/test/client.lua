
local xserialize  = require "xserialize"
local socket = require "client.socket"
local errcode = require "errcode" 
local skynet  = require "skynet"
local usertime = require "usertime"

local finishcnt = 0

local MSG = {}
function MSG.response_register(pack,cli)
    cli:login_msg()
end

function MSG.response_login(pack,cli)
    local message = pack.result
    cli:login_success(message.gate_addr,message.token,message.uuid)
end

function MSG.response_ping(pack,cli)

end


function MSG.response_login_gate(pack,cli)
    if pack.errcode ~= errcode.RT_OK then 
        return 
    end 

    -- finishcnt  = finishcnt + 1
    -- --if finishcnt % 100 == 0 then 
    --     --INFO_LOG("登陆成功 数量%d %f",finishcnt,skynet.time())
    -- --end 
    -- LoginFinish()
    skynet.sleep(300)
    cli:enter_game()
end



local client = __G_CLASS__(...)

function client:ctor(prefix)
    self.__prefix = prefix

    self:connect()
    self:start()
end 

function client:dector()

end 

function client:connect()
    local index = math.random(1,1000000000)
    self.__account = self.__prefix .. "dingqinghis" .. index
    self.__password = self.__prefix .."dqh1993027" .. index
    self.__fd = assert(socket.connect("127.0.0.1", 17000))

end 


function client:start()
    skynet.fork(client.run_thread,self)
    --skynet.fork(client.ping_thread,self)
    skynet.fork(client.read_thread,self)
end 


function client:send(pack)
    pack = xserialize.encode(pack)
    local package = string.pack(">s2", pack)
    socket.send(self.__fd, package)
   
end 

function client:ping_thread()
    while true do 
        self:send({name = "ping"})
        skynet.sleep(500)
    end
end 

function client:read_thread()
    while true do 
        local msg = socket.recv(self.__fd)
        if not msg then 
            skynet.sleep(1)
        else
            local ok,package = pcall( string.unpack,">s2", msg)
 
            if ok then 
                local pack = xserialize.decode(package)
                --print(table.dump(pack))
                if pack.errcode ~= errcode.RT_OK then 
                    --ERROR_LOG(table.dump(pack))
                    --return 
                end 
                local f = MSG[pack.name]
                f(pack,self)
            else
                return 
            end
        end 
    end 
end


function client:run_thread()
    --while true do 

        self:register_msg()
            
        --skynet.exit()
        --skynet.sleep(500)
    --end
end



function client:register_msg()
    self:send( 
    {
        name = "register",
        data = {
            account = self.__account,
            password  = self.__password
        }
    })
end 


function client:login_msg()
    self:send( 
    {
        name = "login",
        data = {
             account = self.__account,
             password  = self.__password
        }
    })
end 

function client:login_gate_msg(token,uuid)
    self:send( 
    {
        name = "login_gate",
        data = {
                token = token,
                uuid  = uuid
        }
    })

    ms = usertime.getmilliseconds()
end 

function client:login_success(gate_addr,token,uuid)
    --for i = 1,3 do 
       
        local list = string.split(gate_addr,":")
        local gatefd = assert(socket.connect(list[1], tonumber(list[2])))
        socket.close(self.__fd) 

        
        self.__fd = gatefd
        self.__uuid = uuid
        self:login_gate_msg(token,uuid)
    --end
end

function client:enter_game()
    self:send( 
        {
            name = "enter_game_msg",
            data = {
                 uuid = self.__uuid,
            }
        })

end



return client