#ifndef LUA_WRAPPER_H
#define LUA_WRAPPER_H

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

int wrapper_roll(lua_State * L) {
    const char * r = lua_tostring(L, -1);
    
    return 1;
}

#endif