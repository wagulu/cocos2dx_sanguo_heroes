//
//  HeroCreator.h
//  Game
//
//  Created by fuchenhao on 3/9/15.
//
//

#ifndef __HERO_CREATOR_H__
#define __HERO_CREATOR_H__

#include "BattleEntity.h"
#include "BattleStage.h"

class HeroCreator
{
public:
    HeroCreator(BattleEntity* entities, BattleStage* stage);
    ~HeroCreator();
    
    bool addHero(int heroId, BattleConfig::HeroConfig& config, int tileZ, int tileX);
    void addBackupHero(BattleConfig::Side side, BattleConfig::HeroConfig& config, int tileZ, int tileX);
    
private:
    BattleEntity* m_entities;
    BattleStage* m_stage;
};

#endif /* defined(__HERO_CREATOR_H__) */
