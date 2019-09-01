//
//  StateSoldierRun.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "StateSoldierIdle.h"
#include "SoldierSystem.h"
#include "../../AICommonHelper.h"
#include "StateSoldierRecenter.h"
#include "StateSoldierProtectHero.h"

StateSoldierIdle::StateSoldierIdle()
{
    
}

StateSoldierIdle::~StateSoldierIdle()
{
    
}

void StateSoldierIdle::enter(entityx::Entity& entity)
{
    BattleComponent::Direction::Handle direction = entity.component<BattleComponent::Direction>();
    
    direction->x = 0;
    direction->y = 0;
    direction->z = 0;

//    m_findTargetTicket = SoldierSystem::getFindTargetInIdleTicketRequest().queryTicket();
}

void StateSoldierIdle::exit(entityx::Entity& entity)
{
    
}

int StateSoldierIdle::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    auto soldier = entity.component<BattleComponent::Soldier>();
    auto animation = entity.component<BattleComponent::Animation>();
    auto identify = entity.component<BattleComponent::Identify>();
//    auto ai = entity.component<BattleComponent::AI>();
    
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_IDLE);
    
    StrategyControl::SOLDIER_CONTROL_TYPE type = _STRATEGY_CONTROL.getSoldierControl(identify->side == BattleConfig::SIDE_LEFT);
    switch(type)
    {
        case StrategyControl::SOLDIER_BACK:
            _RETURN_STATE_RESULT(MOVE_BACK);
            break;

        case StrategyControl::SOLDIER_ATTACK_SOLDIER:
            _RETURN_STATE_RESULT(ATTACK_ENEMY);
            break;

        case StrategyControl::SOLDIER_ATTACK_HERO:
            _RETURN_STATE_RESULT(ATTACK_ENEMY_HERO);
            break;
        
        case StrategyControl::SOLDIER_PROTECT_HERO:
            if(!StateSoldierProtectHero::isArriveHero(entity))
            {
                _RETURN_STATE_RESULT(PROTECT_HERO);
            }
            break;
        
        case StrategyControl::SOLDIER_RECENTER:
            if(!StateSoldierRecenter::isArriveCenter(entity))
            {
                _RETURN_STATE_RESULT(RECENTER);
            }
            break;
        
        case StrategyControl::SOLDIER_DEAPRT:
            _RETURN_STATE_RESULT(DEPART);
            break;
            
        default:

            break;
            
    }
    
    if(m_result == WORKING)
    {
        if(_BATTLE_TIME > 2)
        {
            if(shouldEnterBowManAttack(entity))
            {
                _RETURN_STATE_RESULT(BOWMAN_ATTACK);
            }
            
        }
        
        if(isTimeToResearchTarget(entity))
        {
            _TARGET_FINDER.findTargetNearby(entity);
            if(soldier->target != 0)
            {
                _RETURN_STATE_RESULT(ATTACK_NEARBY_ENEMY);
            }
        }
        
    }
    
    return m_result;
}
