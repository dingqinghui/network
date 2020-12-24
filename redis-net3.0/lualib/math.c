// my_math.c
  
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
  
#include <math.h>
#include <stdio.h>
  
  
static int my_math_sin (lua_State *L) {
    lua_pushnumber(L, sin(luaL_checknumber(L, 1)));
    return 1;
}
  
static int my_math_cos (lua_State *L) {
    lua_pushnumber(L, cos(luaL_checknumber(L, 1)));
    return 1;
}
  
  
static const luaL_Reg mathlib[] = {
    {"my_cos",   my_math_cos},
    {"my_sin",   my_math_sin},
    {NULL, NULL}
};
  
/*
** Open my_math library
*/
LUALIB_API int luaopen_my_math (lua_State *L) {
  luaL_newlib(L, mathlib);
  return 1;
}