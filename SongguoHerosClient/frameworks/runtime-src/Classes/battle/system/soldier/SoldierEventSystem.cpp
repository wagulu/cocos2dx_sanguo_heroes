//
//  EventSystem.cpp
//  Game
//
//  Created by fu.chenhao on 3/2/15.
//
//

#include "SoldierEventSystem.h"
#include "BattleWorld.h"
#include "SoldierSystem.h"
#include "StateSoldierMoveBack.h"
#include "StateSoldierMoveToEnemy.h"
#include "StateSoldierMoveToEnemyHero.h"
#include "StateSoldierProtectHero.h"
#include "StateSoldierRecenter.h"
#include "StateSoldierAttackEnemy.h"
#include "StateSoldierDead.h"
#include "StateSoldierDepart.h"
#include "StateSoldierIdle.h"
#include "StateSoldierMoveToTargetTile.h"
#include "StateSoldierDizzy.h"
#include "StateSoldierFadeIn.h"
#include "StateSoldierPlayAnimation.h"
#include "StateSoldierBattleEnd.h"
#include "StateSoldierChangeToBlock.h"
#include "StateSoldierChangeToGhost.h"
#include "StateSoldierHitBack.h"

void SoldierEventSystem::configure(entityx::EventManager& events)
{
//    events.subscribe<BattleEvent::ForceChangeState>(*this);

    events.subscribe<BattleEvent::BeAttacked>(*this);
    
    events.subscribe<BattleEvent::StrategyBattleEnd>(*this);
    events.subscribe<BattleEvent::StrategyIdle>(*this);
    events.subscribe<BattleEvent::StrategyAttack>(*this);
    events.subscribe<BattleEvent::StrategyAttackHero>(*this);
    events.subscribe<BattleEvent::StrategyProtectHero>(*this);
    events.subscribe<BattleEvent::StrategyRecenter>(*this);
    events.subscribe<BattleEvent::StrategyDepart>(*this);
    events.subscribe<BattleEvent::StrategyMoveBack>(*this);
    events.subscribe<BattleEvent::StrategySoldierDizzy>(*this);
    events.subscribe<BattleEvent::StrategySoldierFadeIn>(*this);
    events.subscribe<BattleEvent::StrategySoldierChangeToGhost>(*this);
    events.subscribe<BattleEvent::StrategySoldierChangeToBlock>(*this);
    events.subscribe<BattleEvent::StateSoldierHitBack>(*this);
    
    events.subscribe<BattleEvent::BattleActorDead>(*this);
    events.subscribe<BattleEvent::HeroPushBattleActor>(*this);

//    events.subscribe<BattleEvent::SwitchActorMode>(*this);

    events.subscribe<BattleEvent::KnockBackSoldier>(*this);
    events.subscribe<BattleEvent::StateSoldierPlayAnimation>(*this);
}

//void SoldierEventSystem::receive(const BattleEvent::ForceChangeState& e)
//{
//    /*
//     NOTE
//     we should avoid to send this event to ask entity to change state
//     it is ONLY a temparary solution to migrate old code to new system
//     */
//
//    return;
//    
//    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
//    
//    switch (e.targetState)
//    {
//        case BattleConfig::STATE_SOLDIER:
//            objectStateMachine->changeState(new StateSoldierMoveToEnemy());
//            break;
//            
//            //        case BattleConfig::STATE_BACK:
//            //            objectStateMachine->changeState(new StateSoldierMoveToEnemy());
//            //            break;
//            //
//            //        case BattleConfig::STATE_DEPART:
//            //            objectStateMachine->changeState(new StateSoldierMoveToEnemy());
//            //            break;
//            //
//        case BattleConfig::STATE_CENTER:
//            objectStateMachine->changeState(new StateSoldierRecenter());
//            break;
//            
//        case BattleConfig::STATE_PROTECT:
//            objectStateMachine->changeState(new StateSoldierProtectHero());
//            break;
//            
//        case BattleConfig::STATE_GENERAL:
//            objectStateMachine->changeState(new StateSoldierMoveToEnemy());
//            break;
//            
//        default:
//            CCLOG("unsupport change state = %i", e.targetState);
//            break;
//    }
//
//}


void SoldierEventSystem::receive(const BattleEvent::BeAttacked& e)
{
    BattleComponent::ObjectStateMachine::Handle sourceStateMachine = e.sourceEntity.component<BattleComponent::ObjectStateMachine>();
    BattleComponent::Identify::Handle sourceId = e.sourceEntity.component<BattleComponent::Identify>();

    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
//    BattleComponent::Soldier::Handle sourceSoldier = e.sourceEntity.component<BattleComponent::Soldier>();
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();

    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    
    int sourceStateCode = sourceStateMachine->getCurrentStateCode();
    int objectStateCode = objectStateMachine->getCurrentStateCode();

    if(sourceStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            soldier->target = sourceId->id;
            objectStateMachine->changeState(new StateSoldierAttackEnemy());
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategyAttack& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();

    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    

    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierMoveToEnemy());
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategyBattleEnd& e)
{
    auto soldier = e.entity.component<BattleComponent::Soldier>();
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierBattleEnd());
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategyIdle& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    
    
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierIdle());
            
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategyAttackHero& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    
    
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierMoveToEnemyHero());
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategyProtectHero& e)
{
    auto soldier = e.entity.component<BattleComponent::Soldier>();
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();

    if(!soldier.valid())
    {
        return; //return if not a soldier
    }

    int objectStateCode = objectStateMachine->getCurrentStateCode();
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            soldier->target = (e.entity.component<BattleComponent::Identify>()->side == BattleConfig::SIDE_LEFT) ? LEFT_HERO : RIGHT_HERO;
            
            objectStateMachine->changeState(new StateSoldierProtectHero());
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategyRecenter& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    

    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierRecenter());
            
        }
    }
    
}

void SoldierEventSystem::receive(const BattleEvent::BattleActorDead& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    

    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if (soldier->config.soldierType == BattleConfig::SoldierType::BLOCK)
        {
            objectStateMachine->changeState(new StateSoldierDead(e.attackData, true));
        }
        else
        {
            objectStateMachine->changeState(new StateSoldierDead(e.attackData));
        }
    }
    
}

void SoldierEventSystem::receive(const BattleEvent::HeroPushBattleActor& e)
{
    auto identify = e.targetEntity.component<BattleComponent::Identify>();
    if (identify->type != BattleConfig::ENTITY_SOLDIER)
    {
        return;//return if not a soldier
    }
    
    auto heroPos = e.fromEntity.component<BattleComponent::Position>();
    auto soldierPos = e.targetEntity.component<BattleComponent::Position>();
    auto target = e.targetEntity.component<BattleComponent::Target>();
    if (heroPos->z <= soldierPos->z)
    {
        target->set(soldierPos->x, soldierPos->y, soldierPos->z + GRID_HEIGHT);
    }
    else
    {
        target->set(soldierPos->x, soldierPos->y, soldierPos->z - GRID_HEIGHT);
    }

    auto objectStateMachine = e.targetEntity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierMoveToTargetTile(objectStateCode));
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategyDepart& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    
    
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierDepart());
        }
    }
    
}

void SoldierEventSystem::receive(const BattleEvent::StrategyMoveBack& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    
    
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierMoveBack());
            
        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StateSoldierPlayAnimation& e)
{
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    if(!soldier.valid())
    {
        CCLOG("======= !soldier.valid()");
        return; //return if not a hero
    }
    
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        
        objectStateMachine->changeState(new StateSoldierPlayAnimation(e.animation, e.duration));
    }
}

void SoldierEventSystem::receive(const BattleEvent::StateSoldierHitBack& e)
{
    if(!e.entity.valid())
    {
        return; //return if not a hero
    }
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    if(!soldier.valid())
    {
        return; //return if not a hero
    }
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        
        objectStateMachine->changeState(new StateSoldierHitBack(e.duration));
    }
}

/*
void SoldierEventSystem::receive(const BattleEvent::SwitchActorMode& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    
    
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierModeState(objectStateCode))
        {
            objectStateMachine->changeState(new StateSoldierDizzy());
            
        }
    }
    
}
 */

void SoldierEventSystem::receive(const BattleEvent::KnockBackSoldier& e)
{
    BattleComponent::Soldier::Handle soldier = e.entity.component<BattleComponent::Soldier>();
    BattleComponent::Position::Handle position = e.entity.component<BattleComponent::Position>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();

//    BattleComponent::Soldier::Handle sourceSoldier = e.sourceEntity.component<BattleComponent::Soldier>();
    BattleComponent::Position::Handle sourcePosition = e.sourceEntity.component<BattleComponent::Position>();

    if(!soldier.valid())
    {
        return; //return if not a soldier
    }
    

    
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(objectStateCode != BattleConfig::STATE_DEAD)
    {
        if(isSoldierStrategyState(objectStateCode)
           || objectStateCode == BattleConfig::STATE_ATTACK
           )
        {
            float directX = 1;
            if(sourcePosition->x > position->x)
            {
                directX = -1;
            }

            _SOLDIER_MOVEMENT.knockBack(e.entity, directX, e.expectDist);
            
        }
    }
    
}

void SoldierEventSystem::receive(const BattleEvent::StrategySoldierDizzy& e)
{
    if (!e.entity.valid()
        || e.entity.component<BattleComponent::Identify>()->type != BattleConfig::ENTITY_SOLDIER)
    {
        return;
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if (objectStateCode != BattleConfig::STATE_DEAD
        && objectStateCode != BattleConfig::SoldierState::STATE_CHANGE_TO_BLOCK)
    {
//        if (isSoldierStrategyState(objectStateCode))
//        {
            objectStateMachine->changeState(new StateSoldierDizzy(e.dizzyTime));
//        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategySoldierFadeIn& e)
{
    if (!e.entity.valid() || e.entity.component<BattleComponent::Identify>()->type != BattleConfig::ENTITY_SOLDIER)
    {
        return;
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if (objectStateCode != BattleConfig::STATE_DEAD)
    {
        //        if (isSoldierStrategyState(objectStateCode))
        //        {
        objectStateMachine->changeState(new StateSoldierFadeIn(e.dizzyTime));
        //        }
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategySoldierChangeToGhost& e)
{
    if (!e.entity.valid() || e.entity.component<BattleComponent::Identify>()->type != BattleConfig::ENTITY_SOLDIER)
    {
        return;
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    if (objectStateCode != BattleConfig::STATE_DEAD)
    {
        objectStateMachine->changeState(new StateSoldierChangeToGhost(e.ghostData));
    }
}

void SoldierEventSystem::receive(const BattleEvent::StrategySoldierChangeToBlock& e)
{
    auto targetEntity = e.entity;
    if (!targetEntity.valid() || targetEntity.component<BattleComponent::Identify>()->type != BattleConfig::ENTITY_SOLDIER)
    {
        return;
    }
    
    auto objectStateMachine = targetEntity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    if (objectStateCode != BattleConfig::STATE_DEAD)
    {
        objectStateMachine->changeState(new StateSoldierChangeToBlock(e));
    }
}

void SoldierEventSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
    
}
