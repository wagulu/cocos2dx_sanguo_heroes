//
//  StrategyControl.cpp
//  Game
//
//  Created by fu.chenhao on 3/6/15.
//
//

#include "StrategyControl.h"
#include "BattleWorld.h"
#include "cocos2d.h"


StrategyControl::StrategyControl()
{
    
}


StrategyControl::~StrategyControl()
{
    
}


void StrategyControl::setSoldierControl(SOLDIER_CONTROL_TYPE control, bool left)
{
    if(left)
        m_soldierControlLeft = control;
    else
        m_soldierControlRight = control;
    
    auto& entities = (left) ? _BATTLE_ENTITY.getLeftSoldiers() : _BATTLE_ENTITY.getRightSoldiers();
    
    for(auto& entity : entities)
    {
        switch (control)
        {
            case SOLDIER_ATTACK_SOLDIER:
                _ENTITY_EVENT.emit(BattleEvent::StrategyAttack(entity));
                break;
                
            case SOLDIER_ATTACK_HERO:
                _ENTITY_EVENT.emit(BattleEvent::StrategyAttackHero(entity));
                break;
                
            case SOLDIER_DEAPRT:
                _ENTITY_EVENT.emit(BattleEvent::StrategyDepart(entity));
                break;
                
            case SOLDIER_RECENTER:
                _ENTITY_EVENT.emit(BattleEvent::StrategyRecenter(entity));
                break;
                
            case SOLDIER_BACK:
                _ENTITY_EVENT.emit(BattleEvent::StrategyMoveBack(entity));
                break;
                
            case SOLDIER_PROTECT_HERO:
                _ENTITY_EVENT.emit(BattleEvent::StrategyProtectHero(entity));
                break;
                
            case SOLDIER_STOP:
                _ENTITY_EVENT.emit(BattleEvent::StrategyIdle(entity));
                break;
                
            case SOLDIER_BATTLE_END:
                _ENTITY_EVENT.emit(BattleEvent::StrategyBattleEnd(entity));
                break;
                
            default:
                break;
        }
    }
}

void StrategyControl::setHeroControl(HERO_CONTROL_TYPE control, int heroId)
{
    auto pEntity = _BATTLE_ENTITY.getEntity(heroId);
    if (pEntity == nullptr)
    {
        return;
    }
    
    switch (control)
    {
        case HERO_STOP:
            pEntity->component<BattleComponent::HeroStrategy>()->type = StrategyControl::HERO_STOP;
            _ENTITY_EVENT.emit(BattleEvent::StrategyHeroStop(*pEntity));
            break;
            
        case HERO_RUSH_OUT:
            pEntity->component<BattleComponent::HeroStrategy>()->type = StrategyControl::HERO_RUSH_OUT;
            _ENTITY_EVENT.emit(BattleEvent::StrategyHeroRush(*pEntity));
            break;
            
        case HERO_BATTLE_END:
            pEntity->component<BattleComponent::HeroStrategy>()->type = StrategyControl::HERO_BATTLE_END;
            _ENTITY_EVENT.emit(BattleEvent::StrategyBattleEnd(*pEntity));
            break;
            
        default:
            break;
    }
}

void StrategyControl::setBattleEndForAll()
{
    auto& leftHeros = _BATTLE_ENTITY.getLeftHeros();;
    for (int i = 0; i < leftHeros.size(); i++)
    {
        setHeroControl(StrategyControl::HERO_BATTLE_END, leftHeros[i].component<BattleComponent::Identify>()->id);
    }
    auto& rightHeros = _BATTLE_ENTITY.getRightHeros();
    for (int i = 0; i < rightHeros.size(); i++)
    {
        setHeroControl(StrategyControl::HERO_BATTLE_END, rightHeros[i].component<BattleComponent::Identify>()->id);
    }
    setSoldierControl(StrategyControl::SOLDIER_BATTLE_END, true);
    setSoldierControl(StrategyControl::SOLDIER_BATTLE_END, false);
}
