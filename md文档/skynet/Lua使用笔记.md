## 5.3参考手册
https://www.runoob.com/manual/lua53doc/manual.html#pdf-load

## 注册表  _G    _ENV

用于在C代码中保存Lua的值。注册表在Lua中不可见，索引1存放Lua mainthread ，索引2 存放全局表，也是默认加载代码的_ENV。

_ENV 在5.2以后存放在 chunk的upvalue(1)

a = 1

5.1  _G['a'] =  1     5.2 _ENV[''a] = 1

![](Lua使用笔记/lua _G _ENV.png)


## lua_State *lua_newstate (lua_Allocf, void *ud)

```c
//可以通过自定义内存分配函数控制lua虚拟机的内存使用量，和内存使用统计
//lua_newstate  lua_Allocf:自定义内存分配函数 ud：内存分配函数的User Data
void* __alloc(void *ud, void *ptr, size_t osize, size_t nsize){
    lua_State *L = ud;
    return realloc(ptr, nsize);
}
lua_State *L  = lua_newstate(__alloc,L);
                
```

## gsub 匹配 替换字符串
```lua
-- string.gsub (s, pattern, repl [, n])

--  .：任意字符 ()：捕获   %1-%9:引用捕获的内容
string.gsub("hello Lua", "(.)(.)", "%2%1")) -- <== ehll ouLa   

-- 使用函数处理匹配的字符串
s, n = string.gsub("hello world", "l+", function(s) return "xxx" end)
print(s, n) <== hexxxo worxxxd 2
```

## load 加载代码
```lua
--[[
load (chunk [, chunkname [, mode [, env]]])
chunk:字符串/函数
chunkname:错误消息和调试消息中代码块名字
mode:t 文本 b 二进制
env: module ENV
]]
local code = [[
  A = 1
  B = 2
  Tab = {
    a = 1,
    b =2
  }

]]

local result = {}
load(code,[[testcode]],[[t]],result)()  -- 当code中对全局变量赋值引用时，实际是操作result。
```

## os.getenv 获取进程环境变量

## C导出包含Upvalue的Lua库函数
```c
	luaL_Reg l[] = {
		{ "A" , lsend },
		{ NULL, NULL },
	};
	lua_createtable(L, 0, 1);
	lua_pushboolean(L,1);   //压入一个bool值的upvalue，所有l中的C函数都包含这个upvalue 
	luaL_setfuncs(L,l,1);

lua_upvalueindex(L,1); // 返回函数指定upvalue索引的堆栈伪索引， 然后就可以通过lua_toXXXXX 获取这个upvalue值
```

## 协程池
```lua

local coroutine_resume     = coroutine.resume 
local coroutine_create     = coroutine.create 
local coroutine_running    = coroutine.running 
local coroutine_yield      = coroutine.yield 

local coroutines = setmetatable({}, { __mode = "kv" })
coroutines.pool = {}

local self = coroutines

function coroutines.create(f)
   local co = table.remove(self.pool,1)
   if co then 
      -- 从协程池中取出一个协程  调用resume(f) f作为协程的新函数
      coroutine_resume(co,f)
      print("coroutine_resum remove")
   else
      co = coroutine_create(function (...)
         -- 第一次resume 执行线程函数
         f(...) 
         while true do
            -- 执行完 回收到池中
            local cur = coroutine_running()
            table.insert(self.pool,#self.pool + 1,cur)
            -- 下次resume 时传入f
            f = coroutine_yield() 
            -- 执行线程函数
            f( coroutine_yield() )
         end 
      end)
   end 
   return co
end


function coroutines.fork(f)
   local co = self.create(f)
   coroutine_resume(co)
end 


self.resume = coroutine_resume
self.running = coroutine_running
self.yield = coroutine_yield


return coroutines
```

## 热更新代码
```lua
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
            u[i] = value
            i = i + 1
        end
    end

    local function set_upvalus(f,upvalues)
        for k, v in pairs(upvalues) do
            debug.setupvalue(f,k,v)
        end
    end 

    -- 缓存旧模块
    local old_module = package.loaded["module_name"] or {}
    package.loaded[module_name] = nil
    -- 加载新模块
    local new_module = require(module_name)
    -- 将新模块合并到老模块
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
```

## Lua 5.3 最大整数值
```lua
local max = 0x7FFFFFFFFFFFFFFF
print( (1 << 63) -1 )  --  (2^64 - 1)
```