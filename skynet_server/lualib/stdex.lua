local loadstring = rawget(_G, "loadstring") or load

function table.dump(obj,deep)
    deep = deep or 1
    local out = ""  
    local t = type(obj)  
    if t == "number" then  
        out = out .. obj
    elseif t == "string" then  
        out = out .. string.format("\"%s\"",obj)
    elseif t == "table" then  
        out = out .. "\n" .. string.rep("\t", deep - 1) .. "{\n"  
        for k, v in pairs(obj) do  
            out = out .. string.rep("\t", deep) .. "[" .. table.dump(k ,deep + 1) .. "]=" .. table.dump(v,deep + 1) .. ",\n"  
        end  
        out = out .. string.rep("\t", deep - 1) .. "}"  
    elseif t == "nil" then  
        return nil  
    elseif t == "function" 
        or t == "userdata"
        or t == "thread" then 
        out = out .. tostring(obj)
    end  

    return out  
end





function table.serialize(obj)
    local lua = ""
    local t = type(obj)
    if t == "number" then
        lua = lua .. obj
    elseif t == "boolean" then
        lua = lua .. tostring(obj)
    elseif t == "string" then
        lua = lua .. string.format("%q", obj)
    elseif t == "table" then
        lua = lua .. "{\n"
    for k, v in pairs(obj) do
        lua = lua .. "[" .. table.serialize(k) .. "]=" .. table.serialize(v) .. ",\n"
    end
    local metatable = getmetatable(obj)
        if metatable ~= nil and type(metatable.__index) == "table" then
        for k, v in pairs(metatable.__index) do
            lua = lua .. "[" .. table.serialize(k) .. "]=" .. table.serialize(v) .. ",\n"
        end
    end
        lua = lua .. "}"
    elseif t == "nil" then
        return nil
    else
        error("can not serialize a " .. t .. " type.")
    end
    return lua
end

function table.unserialize(lua)
    local t = type(lua)
    if t == "nil" or lua == "" then
        return nil
    elseif t == "number" or t == "string" or t == "boolean" then
        lua = tostring(lua)
    else
        error("can not unserialize a " .. t .. " type.")
    end
    lua = "return " .. lua
    local func = loadstring(lua)
    if func == nil then
        return nil
    end
    return func()
end



function  string.split(str,sep)
    local list = {}
    local beginpos = 1
    while true do
      local pos,epos = string.find(str,sep)
      if not pos then 
          if str ~= "" then
            table.insert(list,str)
          end
        return list
      end
      table.insert(list,string.sub(str,beginpos,pos - 1))
      str = string.sub(str,epos + 1,#str)
    end
  end 
  