local lfs = require "lfs"


local fileutile = {}

--[[
    @desc: 加载目录下所有文件
]]
function fileutile.load_dir(rootPath)
   
    for entry in lfs.dir(rootPath) do
        skynet.error(entry)
        if entry~='.' and entry~='..' then
            local path = rootPath.."/"..entry
            local attr = lfs.attributes(path)
            assert(type(attr)=="table") 
            if(attr.mode == "directory") then
                --print("Dir:",path)
            elseif attr.mode=="file" then
                loadfile(path)
            end
        end
    end
end


return fileutile 