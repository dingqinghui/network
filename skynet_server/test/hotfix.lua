local skynet = require "skynet"
local codecache = require "skynet.codecache"

local test = require "test"

for k, v in pairs(test) do
    skynet.error(k,v)
    
end

local function reload_module(module_name)

    local function get_upvalues(f)
        local u = {}
        if not f then return u end
        local i = 1
        while true do
            local name, value = debug.getupvalue(f, i)
            if name == nil then
                return u
            end
            skynet.error("upvalue",name,value)
            u[i] = value
            i = i + 1
        end
    end
    local function set_upvalus(f,upvalues)
        for k, v in pairs(upvalues) do
            debug.setupvalue(f,k,v)
        end
    end 

    local old_module = package.loaded[module_name] or {}
    package.loaded[module_name] = nil

    local source = nil
    local err = {}
    for pat in string.gmatch(package.path, "([^;]+);*") do
        local filename = string.gsub(pat, "?", module_name)
        skynet.error("filename:",filename)
        local ret,f = pcall(io.open,filename)
        if not ret then
            table.insert(err, f)
        else
            if f then 
                source = f:read "*a"
                f:close()
                break
            end 
        end
    end

    if not source then 
        skynet.error(table.concat(err, "\n"))
        return 
    end 

    local new_module = load(source, "@"..module_name, "t")()

    for k, v in pairs(new_module) do
        local o_value = old_module[k]
        if type(o_value) == "function" then 
            local o_upvalues = get_upvalues(o_value)
            set_upvalus( v,o_upvalues);
        end 
        old_module[k] = v
    end

    package.loaded[module_name] = old_module

    return old_module
end



skynet.start(function ()
   
    skynet.fork(function ()
        while true do 
            skynet.sleep(500)
            
            skynet.error(test)
            reload_module("test")
            
            test.print()
            skynet.error(test.data)
            
            --codecache.clear()
            --skynet.newservice("hotfix")
        end 
    end )


end )