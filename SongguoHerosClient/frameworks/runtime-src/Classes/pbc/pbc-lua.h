#ifndef __LUA_PBC_EXTRA_H_
#define __LUA_PBC_EXTRA_H_

#if __cplusplus
extern "C"
{
#endif
    
#include "lauxlib.h"
#include "tolua++.h"
    
TOLUA_API int luaopen_protobuf_c(lua_State *L);
TOLUA_API int register_protobuf_module(lua_State *L);
    
#if __cplusplus
}
#endif
#endif