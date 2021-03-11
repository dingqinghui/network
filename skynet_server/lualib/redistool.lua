
local redisop = require "redisop"
local usertime = require "usertime"
local skynet = require "skynet"

local redistool = {}


local lock_code = [[
    --  如果获取锁成功，则返回 1
    local key     = KEYS[1]
    local content = ARGV[1]
    local ttl     = tonumber(ARGV[2])
    local lockSet = redis.call('setnx', key, content)
    if lockSet == 1 then
        redis.call('PEXPIRE', key, ttl)
    else
        -- 重入锁
        local value = redis.call('get', key)
        if(value == content) then
            lockSet = 1;
            redis.call('PEXPIRE', key, ttl)
        end
    end
    return lockSet
]]

local unlock_code = [[
    local key     = KEYS[1]
    local content = ARGV[1]
    local value = redis.call('get', key)
    if value == content then
        return redis.call('del', key)
    else
        return 0
    end
]]

-- 分布式锁
function redistool.try_distribute_lock(lock_key,request_id,lock_expire_time,wait_time)
    local tm = usertime.getmilliseconds()
    local ret = nil
    while true do 
        ret = redisop.eval(lock_code,1,lock_key,request_id,lock_expire_time)
        if ret == 1 then 
            break
        else
            if wait_time == 0 then 
                break
            elseif wait_time < 0 then 
                skynet.sleep(10)
            else 
                if usertime.getmilliseconds() - tm >= wait_time then 
                    break
                end
            end 
        end 
    end
    return ret == 1
end
 

function redistool.distribute_unlock(lock_key,request_id)
    return redisop.eval(unlock_code,1,lock_key,request_id) == 1
end




return redistool