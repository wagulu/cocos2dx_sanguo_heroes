//
//  DebugConfig.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/8/15.
//
//

#include "DebugHelper.h"
#include "BattleWorld.h"

extern "C"
{
#include "CCLuaEngine.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
    
}

using namespace std;

void DebugTools::showHeroTile()
{
#if (SHOW_HERO_TILE)
    static BattleWorld* pBattleWorld = nullptr;
    static BattleEntity* pBattleEntity = nullptr;
    static vector<cocos2d::Node*> labels;
    static int timer = 0;
   
    if (BattleWorld::getInstance() != pBattleWorld)
    {
        pBattleWorld = BattleWorld::getInstance();
        
        if (pBattleWorld->getBattleStage().getBackground()->getChildrenCount() > 0)
        {
            pBattleEntity = &pBattleWorld->getBattleEntity();
            labels.clear();
            timer = 0;
            for (int i=0; i< STAGE_WIDTH ; i++)
            {
                for (int j=0; j<STAGE_HEIGHT; j++)
                {
                    labels.push_back(pBattleWorld->getBattleStage().getBackground()->getChildByTag(i * 100 + j));
                }
            }
        }
        else
        {
            pBattleWorld = nullptr;
        }
    }
    
    timer++;
    if (timer % 60 == 0) timer = 0;
    else return;
    
    int targetId = 0;
    int idx = 0;
    for (int i=0; i< STAGE_WIDTH ; i++)
    {
        for (int j=0; j<STAGE_HEIGHT; j++)
        {
            auto* pLabel = labels[idx++];
            if (pLabel == nullptr) continue;
            targetId = pBattleEntity->getTarget(j, i);
            if (targetId > 0 && MapHelper::isHero(targetId)) pLabel->setVisible(false);
            else pLabel->setVisible(true);
        }
    }
    
#endif
}


int isReleaseVersion(lua_State *L)
{
    lua_pushboolean(L, RELEASE_VERSION);
    return 1;
}
