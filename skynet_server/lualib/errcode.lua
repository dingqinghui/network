
return {
    RT_OK = 1,
    MODULE_ERROR = 10,
    CONTROL_SERVER_ERROR = 11,
    GATE_SERVER_ERROR = 21,

    TOKEN_NO_EXIST = 1001,
    TOKEN_ERROR = 1002,
    ADD_ACOUNT_FAIL = 1003,
    ACCOUNT_EXIST = 1004,
    ACCOUNT_NOT_EXIST = 1005,
    PASSWORD_INCORRECT = 1006,
    DB_GET_PASSWORD_FAIL = 1007,

    TOKEN_EXPIRE = 1008,                -- token 过期
    NOT_AVAILD_GATE = 1009,             -- 没有可用的网关

    USER_NOT_EXIST = 10010,              -- user 对象不存在
    USER_UUID_ERROR = 10011,             -- 玩家UUID错误
    USER_LOADING_DATA = 10012            -- 角色数据加载中
}