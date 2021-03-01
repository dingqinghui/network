local skynet = require "skynet"
local sharedata = require "skynet.sharedata"
local fileutile = require "fileutile"


local cfgmgr ={}

function cfgmgr.initall(path)
    path = path or "./config"
    fileutile.lsDir(path,function (realName)
        local filename = fileutile.getFileName(realName) 
        sharedata.new(filename, string.format("@%s/%s",path,realName))
        --DEBUG_LOG("加载配置文件成功 %s",realName)
    end )
end 


function cfgmgr.get(name)
    return sharedata.query(name)
end


function cfgmgr.reload(reload,path)
    path = path or "./config"
    fileutile.lsDir(path,function (realName)
        local filename = fileutile.getFileName(realName) 
        sharedata.update(filename, string.format("@%s/%s",path,realName))
        --DEBUG_LOG("加载配置文件成功 %s",realName)
    end )
end



return cfgmgr