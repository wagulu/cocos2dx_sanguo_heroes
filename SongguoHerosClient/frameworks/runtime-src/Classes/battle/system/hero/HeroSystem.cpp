//
//  HeroSystem.cpp
//  Game
//
//  Created by fu.chenhao on 2/26/15.
//
//

#include "HeroSystem.h"
#include "AICommonHelper.h"

#include "StateHeroIdle.h"
#include "StateHeroRun.h"
#include "StateHeroAttack.h"
#include "StateHeroDead.h"
#include "StateHeroIdle.h"
#include "StateHeroPlayAnimation.h"
#include "StateHeroDizzy.h"
#include "StateHeroChangeLine.h"
#include "StateHeroMoveToHeroBack.h"
#include "StateHeroMoveToTargetTile.h"
#include "StateHeroDash.h"
#include "StateHeroBattleEnd.h"

void HeroSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::BattlePause>(*this);
    events.subscribe<BattleEvent::BattleResume>(*this);
}

void HeroSystem::receive(const BattleEvent::BattlePause& e)
{
    m_isPausing = true;
}

void HeroSystem::receive(const BattleEvent::BattleResume& e)
{
    m_isPausing = false;
}

void HeroSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{    
    BattleComponent::General::Handle general;
    BattleComponent::Position::Handle position;
    BattleComponent::Property::Handle property;
    BattleComponent::ObjectStateMachine::Handle objectStateMachine;

    for (entityx::Entity entity : es.entities_with_components(general, position, objectStateMachine))
    {
        if((!m_isPausing) || entity.has_component<BattleComponent::ActiveEntity>())
        {
            if(objectStateMachine->getCurrentState() == NULL)
            {
                objectStateMachine->changeState(new StateHeroIdle());
            }
            
//            IState* pCurrentState = objectStateMachine->getCurrentState();
            int currentStateCode = objectStateMachine->getCurrentStateCode();
            int result = objectStateMachine->update(dt);

            switch(currentStateCode)
            {
                case BattleConfig::GENERAL_IDLE:
                    if(result == StateHeroIdle::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroIdle::RUSH_OUT)
                    {
                        objectStateMachine->changeState(new StateHeroRun());
                        
                    }
                    else if(result == StateHeroIdle::ATTACK_ENEMY)
                    {
                        objectStateMachine->changeState(new StateHeroAttack(currentStateCode));
                    }
                    break;
                    
                case BattleConfig::GENERAL_RUN:
                    if(result == StateHeroRun::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroRun::ATTACK_ENEMY)
                    {
                        objectStateMachine->changeState(new StateHeroAttack(currentStateCode));
                    }
                    else if(result == StateHeroRun::NEED_CHANGE_LINE)
                    {
                        objectStateMachine->changeState(new StateHeroChangeLine());
                    }
                    break;
                    
                case BattleConfig::GENERAL_ATTACK:
                    if(result == StateHeroAttack::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroAttack::ENEMY_DEAD)
                    {
//                        int lastState = ((StateHeroAttack*)pCurrentState)->getLastStateCode();
//                        if(lastState == BattleConfig::GENERAL_RUN)
//                        {
//                            objectStateMachine->changeState(new StateHeroRun());
//                        }
//                        else if(lastState == BattleConfig::GENERAL_IDLE)
//                        {
//                            objectStateMachine->changeState(new StateHeroIdle());
//                        }
//                        else
//                        {
//                            objectStateMachine->changeState(new StateHeroIdle());
//                        }
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    else if(result == StateHeroAttack::ENEMY_OUT_OF_RANGE)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    break;
                    
                case BattleConfig::GENERAL_DEAD:
                    break;
                    
                case BattleConfig::GENERAL_PLAY_ANIMATION:
                    if(result == StateHeroPlayAnimation::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroPlayAnimation::DONE)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    break;
                    
                case BattleConfig::GENERAL_DIZZY:
                    if(result == StateHeroDizzy::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroDizzy::OVER)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    break;
                    
                case BattleConfig::GENERAL_DASH:
                    if(result == StateHeroDash::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroDash::OVER)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    break;
                    
                case BattleConfig::GENERAL_CHANGE_LINE:
                    if(result == StateHeroChangeLine::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroChangeLine::MOVE_TO_HERO_BACK)
                    {
                        objectStateMachine->changeState(new StateHeroMoveToHeroBack());
                    }
                    else if(result == StateHeroChangeLine::FAIL)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    break;
                    
                case BattleConfig::GENERAL_MOVE_TO_HERO_BACK:
                    if(result == StateHeroMoveToHeroBack::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroMoveToHeroBack::ARRIVE)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    else if(result == StateHeroMoveToHeroBack::TARGET_HERO_DEAD)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    else if(result == StateHeroMoveToHeroBack::ATTACK_ENEMY)
                    {
                        objectStateMachine->changeState(new StateHeroAttack(currentStateCode));
                    }
                    break;
                    
                case BattleConfig::GENERAL_MOVE_TO_TARGET_TILE:
                    if(result == StateHeroMoveToTargetTile::WORKING)
                    {
                        
                    }
                    else if(result == StateHeroMoveToTargetTile::ARRIVE)
                    {
                        objectStateMachine->changeState(new StateHeroIdle());
                    }
                    break;
                
                case BattleConfig::GeneralState::GENERAL_BATTLE_END:
                    break;
            }
        }
    }
};
