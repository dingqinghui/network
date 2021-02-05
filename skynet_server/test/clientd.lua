local skynet = require "skynet"

local xserialize  = require "xserialize"
local socket = require "client.socket"

local index = ...

local account = "dingqinghui" .. index
local password = "dqh19930227" .. index

local fd = assert(socket.connect("127.0.0.1", 17000))


local function send_pack(client_fd,pack)
    if client_fd then 
        local package = string.pack(">s2", pack)
        socket.send(client_fd, package)
        print("111111111111111",client_fd)
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
        login()
        --skynet.exit()
        --skynet.sleep(500)
    --end
end


local function read_pack(client_fd)
    while true do 
        local msg = socket.recv(fd)

        if not msg then 
            --print("5555555555555555")
        else
            local package = string.unpack(">s2", msg)
            local pack = xserialize.decode(package)
            skynet.error( table.dump(pack) )
            local list = string.split(pack.data.gate_addr,":")
            local gatefd = assert(socket.connect(list[1], tonumber(list[2])))
        end 
    end 
end 



skynet.start(function ()

    skynet.fork(run,fd)
    --skynet.fork(read_pack,fd)
end
)



  
  