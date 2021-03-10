return {
    LOAD_USER = "select * from t_user where uuid =  \'%d\' ",
    
    UPDATE_USER = "update into t_user ( `uuid`, `reg_time`,`name`) values ( \'%d\', \'%d\', \'%s\')",
    
}