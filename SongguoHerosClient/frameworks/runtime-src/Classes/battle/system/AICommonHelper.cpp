//
//  SystemCommonHelper.cpp
//  Game
//
//  Created by fu.chenhao on 2/26/15.
//
//

#include "AICommonHelper.h"
#include "SoldierSystem.h"
#include "HeroSystem.h"
#include "StateHeroDizzy.h"

void ensureAnimation(BattleAnimation* animation, int id, bool loop)
{
    if(animation->getCurrent() != id) {
        animation->playAnimation(id, loop);
    }
}


bool isFreeToSendSkill(entityx::Entity& e)
{
    auto identify = e.component<BattleComponent::Identify>();
    
    if (MapHelper::isHero(identify->id))
    {
        auto objectStateMachine = e.component<BattleComponent::ObjectStateMachine>();
        int stateCode = objectStateMachine->getCurrentStateCode();
        
        bool validState = (isHeroStrategyState(stateCode)
                           || stateCode == BattleConfig::GENERAL_ATTACK);
        if(!validState)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    
    auto actorMode = e.component<BattleComponent::ActorMode>();
    if (actorMode->m_modeGroup.isModeActive(ActorMode::MODE_TYPE::DISABLE_SKILL))
    {
        return false;
    }
    
    return true;
}


bool canEnterDizzyMode(entityx::Entity& e)
{
    auto objectStateMachine = e.component<BattleComponent::ObjectStateMachine>();
    
    int code = objectStateMachine->getCurrentStateCode();
    if (code == BattleConfig::GENERAL_DIZZY)
    {
        auto* pDizzyState = (StateHeroDizzy*)objectStateMachine->getCurrentState();
        if (!pDizzyState->allowBreakByAnotherDizzy())
        {
            return false;
        }
    }
    else
    {
        auto actorMode = e.component<BattleComponent::ActorMode>();
        if (actorMode->m_modeGroup.isModeActive(ActorMode::MODE_TYPE::NO_DIZZY))
        {
            return false;
        }
    }
    
    return true;
}


void updateMiniMapIcon(BattleStage* m_stage, entityx::Entity& entity, int m_currentTileX, int m_currentTileZ)
{
    auto miniIcon = entity.component<BattleComponent::MiniIcon>();
    if (miniIcon)
    {
        miniIcon->m_pIcon->setPositionX(m_currentTileX * 4);
        miniIcon->m_pIcon->setPositionY(m_currentTileZ * 4);
    }
}


bool shouldEnterBowManAttack(entityx::Entity& e)
{
//    const float BOWMAN_AIM_INTERVAL = 0.5;

    auto soldier = e.component<BattleComponent::Soldier>();
    if(!soldier.valid())
    {
        return false;
    }
    if(soldier->config.soldierType != BattleConfig::SoldierType::ARCHER)
    {
        return false;
    }

    if(isTimeToBowManResearchTarget(e))
    {
        if(_TARGET_FINDER.findBowmanTarget(e))
        {
            return true;
        }
    }

    return false;
}

bool isTimeToBowManResearchTarget(entityx::Entity& e)
{
    auto ai = e.component<BattleComponent::AI>();
    
    bool r = false;
    
    if(ai->lastResearchBowManTargetTicket < 0)
    {
        ai->lastResearchBowManTargetTicket = SoldierSystem::getFindTargetInIdleTicketRequest().queryTicket();
    }
    if(!SoldierSystem::getFindTargetInIdleTicketRequest().isWaiting(ai->lastResearchBowManTargetTicket))
    {
        r = true;
        ai->lastResearchBowManTargetTicket = SoldierSystem::getFindTargetInIdleTicketRequest().queryTicket();
    }
    
    return r;
}

bool isTimeToResearchTarget(entityx::Entity& e)
{
    auto ai = e.component<BattleComponent::AI>();

    bool r = false;
    
    if(ai->lastResearchTargetTicket < 0)
    {
        ai->lastResearchTargetTicket = SoldierSystem::getFindTargetInIdleTicketRequest().queryTicket();
    }
    if(!SoldierSystem::getFindTargetInIdleTicketRequest().isWaiting(ai->lastResearchTargetTicket))
    {
        r = true;
        ai->lastResearchTargetTicket = SoldierSystem::getFindTargetInIdleTicketRequest().queryTicket();
    }
    
    return r;
}

bool isTimeToFire(entityx::Entity& entity, float durationScale, bool rewriteTimeMark)
{
    auto property = entity.component<BattleComponent::Property>();
    auto ai = entity.component<BattleComponent::AI>();
    float CD = property->get(BattleConfig::Property::STRIKE_CD) * durationScale;
    float battleTime = _BATTLE_STAGE.getBattleTime();
    if (battleTime - ai->lastAttackTime >= CD)
    {
        if(rewriteTimeMark)
        {
            ai->lastAttackTime = battleTime;
        }
        return true;
    }
    return false;
}

