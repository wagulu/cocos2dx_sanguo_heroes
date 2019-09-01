//
//  convertLua.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 4/28/15.
//
//

#include "convertJsonContent.h"

#include "cocostudio/CocoStudio.h"
#include "json/writer.h"
#include "json/stringbuffer.h"

extern "C"
{
    
#include "CCLuaEngine.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
    
}

int convertJsonContent(lua_State *L)
{
    const char* content = lua_tostring(L, 1);
    const char* path = lua_tostring(L, 2);
    std::string json(content);
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    doc.Accept(writer);
    std::string strJson(buffer.GetString(), buffer.Size());
    
    //
    lua_pushstring(L, strJson.c_str());
    
    return 1;
    
    

}
