//
//  ComboSystem.cpp
//  Game
//
//  Created by fu.chenhao on 3/24/15.
//
//

#include "ComboSystem.h"
#include "BattleWorld.h"
#include "AttackSystem.h"
#include "BattleCocosEvent.h"

#define COMBO_INTERVAL  (1)
#define COMBO_MIN_INTERVAL  (1.0 / 60)
#define COMBO_BEGINS     (2)

void ComboSystem::configure(entityx::EventManager& events)
{
    //    events.subscribe<BattleEvent::ForceChangeState>(*this);
    
    events.subscribe<BattleEvent::HurtByAttack>(*this);
    
    
}

void ComboSystem::receive(const BattleEvent::HurtByAttack& e)
{
    if(e.attackData.getAttackType() != BattleConfig::AttackType::SKILL)
    {
        return;
    }
    
    if(e.attackData.fromId != LEFT_HERO)
    {
        return;
    }
    
    if(e.targetEntity.component<BattleComponent::Identify>()->id != RIGHT_HERO)
    {
        return;
    }
    
    BattleComponent::Combo::Handle combo = e.targetEntity.component<BattleComponent::Combo>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = e.targetEntity.component<BattleComponent::ObjectStateMachine>();
    
    //    BattleComponent::Soldier::Handle sourceSoldier = e.sourceEntity.component<BattleComponent::Soldier>();
    BattleComponent::General::Handle hero = e.targetEntity.component<BattleComponent::General>();
    if(!hero.valid())
    {
        return; //return if not a hero
    }
    
    //    int sourceStateCode = sourceStateMachine->getCurrentStateCode();
//    int objectStateCode = objectStateMachine->getCurrentStateCode();
//    if(objectStateCode != BattleConfig::GENERAL_DEAD)
    if(!objectStateMachine->isDead)
    {
        float currentTime = _BATTLE_STAGE.getBattleTime();
        float elapsedTime = currentTime - combo->m_lastComboTime;
        if(elapsedTime >= COMBO_MIN_INTERVAL && elapsedTime <= COMBO_INTERVAL)
        {
            combo->m_comboTimes++;
            combo->m_lastComboTime = currentTime;

            if(combo->m_comboTimes >= COMBO_BEGINS)
            {
                //create combo
                BattleCocosEvent::PopupCombo event(combo->m_comboTimes);
                cocos2d::Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
            }
        }
        else
        {
            combo->m_comboTimes = 1;
            combo->m_lastComboTime = currentTime;
            
        }
    }
    else
    {
        
    }
    
}

void ComboSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
    
}
