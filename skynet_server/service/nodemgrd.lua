local skynet = require "skynet"
local cluster = require "skynet.cluster"
require "skynet.manager"
local utils = require "utils"


local ping_interval = 50        -- 500 ms

local nodename = skynet.getenv("nodename")

local config_name = skynet.getenv "cluster"
local config = {}
local commond = {}

local node_status = {}


local rsp = {}

local function loadconfig(tmp)
    if not tmp then 
        local f = assert(io.open(config_name))
        local source = f:read "*a"
        f:close()
        tmp = {}
        assert(load(source, "@"..config_name, "t", tmp))()
    end 
    for name,address in pairs(tmp) do
        if type(address) == "string" then
            config[name] = address
        end 
    end 
end

local function status_change(nodename,statu)
   for i=#rsp,1,-1 do
        local ret,msg = pcall(skynet.send,rsp[i],"lua","node_statu_change",nodename,statu)
        if not ret then 
            skynet.error("node_statu_change fail.",msg,debug.traceback())
            table.remove(rsp,i)
        end 
   end
end

local function ping()
    for name,address in pairs(config) do
        if nodename ~= name then 
            skynet.fork(function ()
                while true do 
                    -- 出错只有可能是节点未启动 不可能是代码出错
                    local ret,msg = pcall(cluster.call,name,".nodemgr","ping")
                    local old_statu = node_status[name]
                    node_status[name] = ret
                    if old_statu ~= ret then 
                        status_change(name,ret)
                    end 
                    skynet.sleep(ping_interval)
                end
            end )
        end 
    end
end


function commond.reload(config)
    loadconfig(config)
end 


function commond.ping()
    return true
end 

function commond.status()
    return node_status
end 

function commond.call(node,...)
    if not node_status[node]  then 
        skynet.error( string.format("cluster.call node %s network loss ",node) )
        return false
    end 
    return pcall(cluster.call,node,...)
end 

function commond.send(node,...)
    if not node_status[node]  then 
        skynet.error( string.format("cluster.send node %s network loss ",node) )
        return false
    end 

    return pcall(cluster.send,node,...)
end 

function commond.getconfig()
    return config
end

function commond.register(addr)
    table.insert(rsp,addr)
    skynet.error(table.dump(rsp))
end

function commond.unregister(addr)
    for i=#rsp,1,-1 do
        if addr == rsp[i] then 
            table.remove(rsp,i)
        end  
   end
end

skynet.start(function ()
    if skynet.localname(".nodemgr") then 
        skynet.exit() 
    end 

    cluster.open(nodename)
    skynet.register(".nodemgr")

    loadconfig()
    ping()

    utils.dispatch_lua(commond)
end )


