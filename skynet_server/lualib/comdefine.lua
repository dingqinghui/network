return {
    FRAME_LOOP = 10,

    USER_SURVIVAL_MAX_TM  = 10 ,
    
    LOGIN_TOKEN_EXPIRE = 10,        -- 登陆TOKEN过期时间

    USER_OFFLINE_EXPIRE = 60 * 3,   -- 玩家离线重连过期时间 

    LOG_LEVEL = {
        DEBUG = 1,
        INFO = 2,
        WARNNING = 3,
        ERROR = 4,
        FATAL = 5,
    },


    -- 事件
    EVENT_NAME = {
        CLUSTER_CHANGE = "CLUSTER_CHANGE",      -- 集群节点变化
    }

    -- 服务名
    SERVICE_NAME = {
        CTR_CONTROL     = ".controld",
        GATE_HUB        = ".hub",
        GATE_USERMGR    = ".usermgr",
    }

}