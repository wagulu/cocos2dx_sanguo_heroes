//
//  BattleSystem.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/13/15.
//
//

#include "BattleSystem.h"
#include "BattleWorld.h"
#include "StateBattleBeforeStart.h"
#include "StateBattleStartFight.h"
#include "StateBattleEnd.h"
#include "StateBattleChangeBackupHero.h"

BattleSystem::~BattleSystem()
{
}

void BattleSystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::StartBattleComplete>(*this);
    events.subscribe<BattleEvent::BattleActorDead>(*this);
    events.subscribe<BattleEvent::ExitBattleBeforeEnd>(*this);
}

void BattleSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::Battle::Handle battle;
    BattleComponent::ObjectStateMachine::Handle objectStateMachine;
    
    for (entityx::Entity entity : es.entities_with_components(battle, objectStateMachine))
    {
        if (objectStateMachine->getCurrentState() != NULL)
        {
            int currentStateCode = objectStateMachine->getCurrentStateCode();
            int result = objectStateMachine->update(dt);
            
            switch(currentStateCode)
            {
                case BattleState::BEFORE_START:
                    if (result == StateBattleBeforeStart::WORKING)
                    {
                        
                    }
                    else if (result == StateBattleBeforeStart::COMPLETE)
                    {
                        objectStateMachine->changeState(new StateBattleStartFight());
                    }
                    break;
                    
                case BattleState::START_FIGHT:
                    if (result == StateBattleStartFight::WORKING)
                    {
                        
                    }
                    else if (result == StateBattleStartFight::TIME_UP)
                    {
                        objectStateMachine->changeState(new StateBattleEnd());
                    }
                    break;
                    
                case BattleState::CHANGE_BACKUP_HERO:
                    if (result == StateBattleChangeBackupHero::WORKING)
                    {
                        
                    }
                    else if (result == StateBattleChangeBackupHero::COMPLETE)
                    {
                        objectStateMachine->changeState(new StateBattleStartFight());
                    }
                    else if (result == StateBattleChangeBackupHero::CREATE_BACKUP_HERO_FAIL)
                    {
                        objectStateMachine->changeState(new StateBattleEnd());
                    }
                    break;
                    
                case BattleState::BATTLE_END:
                    break;
            }
        }
    }
    
    if (m_backToDefaultTimeRateDelay > 0)
    {
        m_backToDefaultTimeRateDelay -= dt;
        if (m_backToDefaultTimeRateDelay <= 0)
        {
            _BATTLE_STAGE.changeTimeRate(1.0f);
        }
    }
}

void BattleSystem::receive(const BattleEvent::StartBattleComplete &e)
{
    auto& battleEntity = _BATTLE_ENTITY.getBattle();
    auto objectStateMachine = battleEntity.component<BattleComponent::ObjectStateMachine>();
    if (objectStateMachine->getCurrentState() == NULL)
    {
        objectStateMachine->changeState(new StateBattleBeforeStart());
    }
}

void BattleSystem::receive(const BattleEvent::BattleActorDead& e)
{
    auto& battleEntity = _BATTLE_ENTITY.getBattle();
    auto objectStateMachine = battleEntity.component<BattleComponent::ObjectStateMachine>();
    if (objectStateMachine->getCurrentStateCode() == BattleState::START_FIGHT)
    {
        int targetId = e.entity.component<BattleComponent::Identify>()->id;
        if (MapHelper::isMainHero(targetId))
        {
            BattleWorld::getInstance()->getBattleResult().setHeroDead(targetId);
            
            float timeRate = 3.0f;
            m_backToDefaultTimeRateDelay = BattleConfig::getInstance()->getBattleFieldConfig().slowTime / timeRate;
            _BATTLE_STAGE.changeTimeRate(timeRate);
            
            if (_BATTLE_ENTITY.hasBackupHero(MapHelper::getSide(targetId)))
            {
                objectStateMachine->changeState(new StateBattleChangeBackupHero());
            }
            else
            {
                objectStateMachine->changeState(new StateBattleEnd());
            }
        }
    }
}

void BattleSystem::receive(const BattleEvent::ExitBattleBeforeEnd& e)
{
    auto& battleEntity = _BATTLE_ENTITY.getBattle();
    auto objectStateMachine = battleEntity.component<BattleComponent::ObjectStateMachine>();
    if (objectStateMachine->getCurrentStateCode() != BattleState::BATTLE_END)
    {
        BattleWorld::getInstance()->getBattleResult().setExitBeforeEnd();
        
        objectStateMachine->changeState(new StateBattleEnd());
    }
}

