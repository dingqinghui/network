
__G_CLASS_SET__ = __G_CLASS_SET__ or {}

__G_CLASS__ = function (module_name, class_name, mode)
    assert(type(module_name) == "string" and type(class_name) == "string")

    local cls_set = __G_CLASS_SET__
    local cls_name = module_name .. "@" .. class_name
    if cls_set[cls_name] then
        return cls_set[cls_name]
    end

    local cls = {
        inst_cnt = 0
    }
    cls_set[cls_name] = cls

    cls.class_name = function ()
        return cls_name
    end

    cls.meta = function (_, k)
        return cls_set[cls_name][k]
    end

    cls.new = function (...)
        local inst = {
        }
        setmetatable(inst, {
            __index = cls.meta,
            __gc = function (self)
                cls.inst_cnt = cls.inst_cnt - 1
                if self.dctor then
                    self:dctor()
                end
            end,
            __mode = mode,
        })
        cls.inst_cnt = cls.inst_cnt + 1
        inst:ctor(...)
        return inst
    end

    return cls
end