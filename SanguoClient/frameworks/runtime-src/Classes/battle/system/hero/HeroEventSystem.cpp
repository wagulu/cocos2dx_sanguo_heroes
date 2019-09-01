//
//  HeroEventSystem.cpp
//  Game
//
//  Created by fu.chenhao on 3/2/15.
//
//

#include "HeroEventSystem.h"
#include "HeroSystem.h"

#include "StateHeroRun.h"
#include "StateHeroIdle.h"
#include "StateHeroAttack.h"
#include "StateHeroDead.h"
#include "StateHeroPlayAnimation.h"
#include "StateHeroDizzy.h"
#include "StateHeroDash.h"
#include "StateHeroBattleEnd.h"
#include "StateHeroMoveToTargetTile.h"

#include "BattleWorld.h"
#include "AttackSystem.h"
#include "AICommonHelper.h"

void HeroEventSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::BeAttacked>(*this);
    events.subscribe<BattleEvent::BattleActorDead>(*this);

    events.subscribe<BattleEvent::StrategyBattleEnd>(*this);
    events.subscribe<BattleEvent::StrategyHeroRush>(*this);
    events.subscribe<BattleEvent::StrategyHeroStop>(*this);
    events.subscribe<BattleEvent::StrategyHeroDizzy>(*this);
    events.subscribe<BattleEvent::StrategyHeroPlayAnimation>(*this);
    events.subscribe<BattleEvent::StrategyHeroDash>(*this);
    events.subscribe<BattleEvent::StrategyHeroMoveToTargetTile>(*this);
}

void HeroEventSystem::receive(const BattleEvent::BeAttacked& e)
{
    BattleComponent::ObjectStateMachine::Handle sourceStateMachine = e.sourceEntity.component<BattleComponent::ObjectStateMachine>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    
    //    BattleComponent::Soldier::Handle sourceSoldier = e.sourceEntity.component<BattleComponent::Soldier>();
    BattleComponent::General::Handle hero = e.entity.component<BattleComponent::General>();
    if(!hero.valid())
    {
        return; //return if not a hero
    }
    
    int sourceStateCode = sourceStateMachine->getCurrentStateCode();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    
    if(sourceStateCode != BattleConfig::STATE_DEAD)
    {
        if(isHeroStrategyState(objectStateCode))
        {
            BattleComponent::Identify::Handle sourceId = e.sourceEntity.component<BattleComponent::Identify>();
            hero->target = sourceId->id;
            objectStateMachine->changeState(new StateHeroAttack(objectStateCode));
        }
        else
        {
            
        }
    }

}

void HeroEventSystem::receive(const BattleEvent::BattleActorDead& e)
{
    auto hero = e.entity.component<BattleComponent::General>();
    if (!hero.valid())
    {
        return; //return if not a hero
    }

    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
    if (objectStateCode != BattleConfig::STATE_DEAD)
    {
        objectStateMachine->changeState(new StateHeroDead(e.attackData));
    }
}

void HeroEventSystem::receive(const BattleEvent::StrategyHeroPlayAnimation& e)
{
    auto hero = e.entity.component<BattleComponent::General>();
    if (!hero.valid())
    {
        return; //return if not a hero
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    if (!objectStateMachine->isDead)
    {
        objectStateMachine->changeState(new StateHeroPlayAnimation(e.animation, e.duration));
    }
}

void HeroEventSystem::receive(const BattleEvent::StrategyBattleEnd& e)
{
    auto hero = e.entity.component<BattleComponent::General>();
    if(!hero.valid())
    {
        return; //return if not a hero
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    if(!objectStateMachine->isDead)
    {
        if(isHeroStrategyState(objectStateMachine->getCurrentStateCode()))
        {
            objectStateMachine->changeState(new StateHeroBattleEnd());
        }
    }
}

void HeroEventSystem::receive(const BattleEvent::StrategyHeroDash& e)
{
    auto hero = e.entity.component<BattleComponent::General>();
    if (!hero.valid())
    {
        return; //return if not a hero
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    if (!objectStateMachine->isDead)
    {
        objectStateMachine->changeState(new StateHeroDash(e.animation, e.duration, e.topSpeed));
    }
}

void HeroEventSystem::receive(const BattleEvent::StrategyHeroRush& e)
{
    if (!e.entity.valid() || e.entity.component<BattleComponent::Identify>()->type != BattleConfig::ENTITY_HERO)
    {
        return;
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
//    if(objectStateCode != BattleConfig::GENERAL_DEAD)
    if(!objectStateMachine->isDead)
    {
        if(isHeroStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateHeroRun());
        }
    }
}

void HeroEventSystem::receive(const BattleEvent::StrategyHeroStop& e)
{
    if (!e.entity.valid() || e.entity.component<BattleComponent::Identify>()->type != BattleConfig::ENTITY_HERO)
    {
        return;
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    int objectStateCode = objectStateMachine->getCurrentStateCode();
//    if (objectStateCode != BattleConfig::GENERAL_DEAD)
    if(!objectStateMachine->isDead)
    {
        if (isHeroStrategyState(objectStateCode))
        {
            objectStateMachine->changeState(new StateHeroIdle());
        }
    }
}

void HeroEventSystem::receive(const BattleEvent::StrategyHeroDizzy& e)
{
    if (!e.entity.valid() || e.entity.component<BattleComponent::Identify>()->type != BattleConfig::ENTITY_HERO)
    {
        return;
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    if(!objectStateMachine->isDead
       && canEnterDizzyMode(e.entity))
    {
        bool acceptDelegate = false;
        objectStateMachine->changeState(new StateHeroDizzy(e.dizzyData, e.dizzyConfig), acceptDelegate);
    }
}

void HeroEventSystem::receive(const BattleEvent::StrategyHeroMoveToTargetTile& e)
{
    auto hero = e.entity.component<BattleComponent::General>();
    if (!hero.valid())
    {
        return; //return if not a hero
    }
    
    auto objectStateMachine = e.entity.component<BattleComponent::ObjectStateMachine>();
    if(!objectStateMachine->isDead)
    {
        if(isHeroStrategyState(objectStateMachine->getCurrentStateCode()))
        {
            objectStateMachine->changeState(new StateHeroMoveToTargetTile(e.targetTile));
        }
    }
}

void HeroEventSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
    
}
