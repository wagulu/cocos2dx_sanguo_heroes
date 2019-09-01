//
//  DebugConfig.h
//  Game
//
//  Created by fuchenhao on 3/26/15.
//
//

#ifndef Game_DebugHelper_h
#define Game_DebugHelper_h

#include "ProjectConfig.h"

#include <cocos2d.h>

class DebugTools
{
public:
    static void showHeroTile();
};



extern "C" int isReleaseVersion(lua_State *L);

#endif
