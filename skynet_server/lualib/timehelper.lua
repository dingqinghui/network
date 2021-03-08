local timehelper = {}

function timehelper.nowstr()
    return os.date("%Y-%m-%d %H:%M:%S")
end

return timehelper