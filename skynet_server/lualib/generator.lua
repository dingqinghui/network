local skynet = require "skynet"
local json = require "json"

local generator = {}

local service
skynet.init(function()
    service = skynet.uniqueservice("generatord")
end)


function generator.uuid(...)
    return skynet.call(service,"lua","uuid",...) 
end

function generator.keygen(str,...)
    return string.format(str,...)
end

function generator.insert_sql(tabname,data)
    local field_str = ""
    local val_str   = ""
    for field,val in pairs(data) do
        if field_str ~= "" then 
            field_str  = field_str .. ","
            val_str  = val_str .. ","
        end

        local t = type(val)
        if t == "boolean" then 
            val_str = string.format("%s \'%d\'", val_str,val and 1 or 0)
        elseif t == "number" then 
            val_str = string.format("%s \'%f\'", val_str,val )
        elseif t == "string" then 
            val_str = string.format("%s \'%s\'", val_str,val )
        elseif t == "table" then 
            val_str = string.format("%s \'%s\'", val_str,json.encode(val) )
        end 
        
        field_str = field_str .. string.format("`%s`",field)
    end
    local  sql = string.format("insert into %s (%s) values (%s)",tabname,field_str,val_str)
    return sql
end

-- 默认更新全表
function generator.update_sql(tabname,data,conditions)
    local function convert(field,val)
        local t = type(val)
        if t == "boolean" then 
            return string.format(" `%s`=\'%d\'", field,val and 1 or 0)
        elseif t == "number" then 
            if is_float(val) then 
                return string.format(" `%s`=\'%f\'", field,val )
            else
                return string.format(" `%s`=\'%d\'", field,val )
            end
        elseif t == "string" then 
            return string.format(" `%s`=\'%s\'", field,val )
        elseif t == "table" then 
            return string.format(" `%s`=\'%s\'", field,json.encode(val) )
        end 
        return ""
    end

    local basestr= ""
    for field,val in pairs(data) do
        if basestr ~= "" then 
            basestr  = basestr .. ","
        end
        basestr = basestr .. convert(field,val)
    end

    local  sql = string.format("update  `%s` set %s ",tabname,basestr)

    local condstr = ""
    if conditions then 
        condstr = ""
        for field,val in pairs(conditions) do
            if condstr ~= "" then 
                condstr  = condstr .. ","
            end
            condstr = condstr .. convert(field,val)
        end
        if condstr ~= "" then
            sql = string.format("%s where %s",sql,condstr)
        end
    end
    
    return sql
end



return generator