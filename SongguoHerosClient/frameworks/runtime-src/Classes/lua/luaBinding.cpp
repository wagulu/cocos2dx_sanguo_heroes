//
//  luaBinding.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 3/31/15.
//
//

#include "luaBinding.h"

#include "DebugHelper.h"

extern "C"
{
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
    
}

#include <stdio.h>
#include "HttpAgent.h"
#include "BattleScene.h"
#include "convertJsonContent.h"
#include "AudioManager.h"
#include "CCAdvancedAnimation.h"
#include "UpdateEngine.h"

static const luaL_Reg LIBRARYS[] =
{
    {"sendHttp", sendHttp_lua},
//    {"getHttpResponseData", getHttpResponseData},
    {"createBattle", createBattle},
    {"convertJsonContent", convertJsonContent},
    
    
    {NULL, NULL}
};

static const luaL_Reg SOUND_LIBRARYS[] =
{
    
    {"preloadSoundEffect", preloadSoundEffect},
    {"playSoundEffect", playSoundEffect},
    {"playSoundEffectWithLowMusicVolume", playSoundEffectWithLowMusicVolume},
    {"playMusic", playMusic},
    {"stopMusic", stopMusic},
    {"pauseMusic", pauseMusic},
    {"resumeMusic", resumeMusic},
    {"enableMusic", enableMusic},
    {"disableMusic", disableMusic},
    {"enableSoundEffect", enableSoundEffect},
    {"disableSoundEffect", disableSoundEffect},
    {"unloadAllSoundEffects", unloadAllSoundEffects},
    
    {NULL, NULL}
};

static const luaL_Reg ANIMATION_LIBRARYS[] =
{
    
    {"freeAllAdvancedAnimation", freeAllAdvancedAnimation},
    {"freeAllAdvancedAnimationAsset", freeAllAdvancedAnimationAsset},
    {"createAdvancedAnimation", createAdvancedAnimation},
    {"setCurrentAnimation", setCurrentAnimation},
    {"play", play},
    {"stop", stop},
    {"playOnce", playOnce},
    {"setColor", setColor},
    
    {NULL, NULL}
};

static const luaL_Reg CONFIG_LIBRARYS[] =
{
    {"isReleaseVersion", isReleaseVersion},
    {NULL, NULL}
};

static const luaL_Reg UPDATE_LIBRARYS[] =
{
    {"setLuaCallbacks", setLuaCallbacks},
    {"loadVersion", loadVersion},
    {"loadPackages", loadPackages},
    {NULL, NULL}
};

void bindingAll(lua_State *L)
{
    luaL_register(L, "SanGuoLib", LIBRARYS);
    luaL_register(L, "SanGuoSoundLib", SOUND_LIBRARYS);
    luaL_register(L, "SanGuoAnimationLib", ANIMATION_LIBRARYS);
    luaL_register(L, "SanGuoConfigLib", CONFIG_LIBRARYS);
    luaL_register(L, "SanGuoUpdateLib", UPDATE_LIBRARYS);
}

