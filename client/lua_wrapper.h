#ifndef LUA_WRAPPER_H
#define LUA_WRAPPER_H

#include <lua.h>
#include <lauxlib.h>

lua_State * init_lua(void) {
    lua_State * L = luaL_newstate();

    return L;
}

#endif
