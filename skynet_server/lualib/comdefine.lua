return {
    FRAME_LOOP = 10,

    USER_SURVIVAL_MAX_TM  = 10 ,
    
    LOGIN_TOKEN_EXPIRE = 10,        -- 登陆TOKEN过期时间

    LOG_LEVEL = {
        DEBUG = 1,
        INFO = 2,
        WARNNING = 3,
        ERROR = 4,
        FATAL = 5,
    },


    USER_STATUS = {
        LOADING =        1,      -- 开始加载数据 
        LOADED =         2,      -- 数据加载完成
        WAIT_RECONNECT = 3,      -- 网络断开 数据加载完成
        NOUSER =         4,      -- 玩家不存在   
    },


    EVENT_NAME = {
        CLUSTER_CHANGE = "CLUSTER_CHANGE",      -- 集群节点变化
    }
}