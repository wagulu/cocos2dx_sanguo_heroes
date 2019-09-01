//
//  SoldierCreator.h
//  Game
//
//  Created by fuchenhao on 3/9/15.
//
//

#ifndef __SOLDIER_CREATOR_H__
#define __SOLDIER_CREATOR_H__


#include "BattleEntity.h"
#include "MapHelper.h"
#include "BattleEvent.h"
#include "BattleStage.h"

class SoldierCreator
{
public:
    
    SoldierCreator(BattleEntity* entities, BattleStage* stage);
    ~SoldierCreator();

    int addSoldier(BattleConfig::Side side, BattleConfig::SoldierConfig& config, int tileZ, int tileX, float delayShow = 0.0f, bool turnFace = false, int initHp = -1);
    void addBackgroundSoldier(BattleConfig::Side side, BattleConfig::SoldierConfig& config, int tileZ, int tileX);

private:

    BattleEntity* m_entities;
    BattleStage* m_stage;
};


#endif /* defined(__SOLDIER_CREATOR_H__) */
