return {

    
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

}