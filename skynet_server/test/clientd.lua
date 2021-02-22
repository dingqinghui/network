local skynet = require "skynet"

local xserialize  = require "xserialize"
local socket = require "client.socket"

math.randomseed(os.time())

local index = math.random(1,1000000000)

local account = "dingqinghis" .. index
local password = "dqh1993027" .. index

local fd = assert(socket.connect("127.0.0.1", 17000))


local function send_pack(client_fd,pack)
    if client_fd then 
        local package = string.pack(">s2", pack)
        socket.send(client_fd, package)
    end 
end


local function register()
    local data  = xserialize.encode(
        {
           name = "register",
           data = {
               account = account,
               password  = password
           }
       })
       
       send_pack(fd,data)
       --socket.close(fd)
end

local function ping()
    local data  = xserialize.encode(
        {
           name = "ping",
       })
       
       send_pack(fd,data)
       --socket.close(fd)
       skynet.error("send pack ping")
end

local function login()
    local data  = xserialize.encode(
        {
           name = "login",
           data = {
                account = account,
                password  = password
           }
       })
       
       send_pack(fd,data)
end


local function run(fd)
    --while true do 
        register()
        
        --skynet.exit()
        --skynet.sleep(500)
    --end
end

local MSG = {}
function MSG.response_register(pack)
    login()
end

function MSG.response_login(pack)
     -- local list = string.split(pack.data.gate_addr,":")
     -- local gatefd = assert(socket.connect(list[1], tonumber(list[2])))
end

function MSG.response_ping(pack)
end

local function read_pack(client_fd)
    while true do 
        local msg = socket.recv(fd)

        if not msg then 
            skynet.sleep(10)
        else
            local package = string.unpack(">s2", msg)
            local pack = xserialize.decode(package)
            skynet.error( table.dump(pack) )

            local f = MSG[pack.name]
            f(pack)
           
        end 
    end 
end 



local function ping_thread(fd )

    while true do 
        ping()
        skynet.sleep(500)
    end
end


skynet.start(function ()

    skynet.fork(run,fd)
    skynet.fork(ping_thread,fd)
    skynet.fork(read_pack,fd)
    
    -- local a = 1
    
    -- local function test_auth()
    --     skynet.error(a)
    --     a = a + 1
    --     skynet.timeout(100,test_auth)
    -- end
    -- skynet.timeout(100,test_auth)
end
)



  
  