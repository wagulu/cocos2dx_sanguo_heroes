
#ifndef __LUA_EXTRA_H_
#define __LUA_EXTRA_H_

#include "ProjectConfig.h"

#if RELEASE_VERSION

#if defined(_USRDLL)
    #define LUA_EXTENSIONS_DLL     __declspec(dllexport)
#else         /* use a DLL library */
    #define LUA_EXTENSIONS_DLL
#endif

#if __cplusplus
extern "C" {
#endif

#include "lauxlib.h"

void LUA_EXTENSIONS_DLL luaopen_lua_extensions(lua_State *L);
    
#if __cplusplus
}
#endif

#endif//RELEASE_VERSION

#endif /* __LUA_EXTRA_H_ */
