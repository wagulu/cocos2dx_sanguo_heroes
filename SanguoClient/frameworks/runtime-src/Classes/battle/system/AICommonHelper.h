//
//  SystemCommonHelper.h
//  Game
//
//  Created by fu.chenhao on 2/26/15.
//
//

#ifndef __SYSTEM_COMMON_HELPER__
#define __SYSTEM_COMMON_HELPER__

#include <stdio.h>
#include "BattleConfig.h"
#include "BattleWorld.h"
#include "cocos2d.h"
#include "HeroSystem.h"

static bool isDead(entityx::Entity& e)
{
    if (!e.valid()) return true;
    
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.component<BattleComponent::ObjectStateMachine>();
//    BattleComponent::Identify::Handle identify = e.component<BattleComponent::Identify>();
    
    return objectStateMachine->isDead;
//    if(MapHelper::isHero(id))
//    {
//        return objectStateMachine->getCurrentStateCode() == BattleConfig::GENERAL_DEAD;
//    }
//    else if(MapHelper::isSoldier(id))
//    {
//        return objectStateMachine->getCurrentStateCode() == BattleConfig::STATE_DEAD;
//    }
//    
//    return false;
}


static int getActorStateCode(entityx::Entity& e)
{
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.component<BattleComponent::ObjectStateMachine>();
    return objectStateMachine->getCurrentStateCode();
}

extern "C" void ensureAnimation(BattleAnimation* animation, int id, bool loop = true);

extern "C" bool isFreeToSendSkill(entityx::Entity& e);

extern "C" bool canEnterDizzyMode(entityx::Entity& e);

extern "C" void updateMiniMapIcon(BattleStage* m_stage, entityx::Entity& entity, int m_currentTileX, int m_currentTileZ);

extern "C" bool shouldEnterBowManAttack(entityx::Entity& e);


extern "C" bool isTimeToBowManResearchTarget(entityx::Entity& e);
extern "C" bool isTimeToResearchTarget(entityx::Entity& e);

extern "C" bool isTimeToFire(entityx::Entity& e, float durationScale = 1, bool rewriteTimeMark = true);


#endif /* defined(__SYSTEM_COMMON_HELPER__) */
