local lfs = require "lfs"
local skynet = require "skynet"

local fileutile = {}


function fileutile.loadDir(rootPath,func)
    for entry in lfs.dir(rootPath) do
        skynet.error(entry)
        if entry~='.' and entry~='..' then
            local path = rootPath.."/"..entry
            local attr = lfs.attributes(path)
            assert(type(attr)=="table") 
            if(attr.mode == "directory") then
                --print("Dir:",path)
            elseif attr.mode=="file" then
                local block = loadfile(path)
                if func then func(entry,block) end
            end
        end
    end
end


function fileutile.lsDir(dirpath,func)
    for entry in lfs.dir(dirpath) do
        if entry~='.' and entry~='..' then
            local path = dirpath.."/"..entry
            local attr = lfs.attributes(path)
            assert(type(attr)=="table") 
            if(attr.mode == "directory") then
                --print("Dir:",path)
            elseif attr.mode=="file" then
                if func then func(entry) end
            end
        end
    end
end


function fileutile.mkDir(dirname)
    os.execute("mkdir -p \"" .. dirname .. "\"")
end


function fileutile.getFileName(filename)
    local idx = filename:match(".+()%.%w+$")
    if (idx) then
        return filename:sub(1, idx-1)
    else
        return filename
    end
end

--获取扩展名
function fileutile.getExtension(filename)
    return filename:match(".+%.(%w+)$")
end


return fileutile 