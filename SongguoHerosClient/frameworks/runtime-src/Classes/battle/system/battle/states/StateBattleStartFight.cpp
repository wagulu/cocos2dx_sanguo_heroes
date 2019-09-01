//
//  StateBattleStartFight.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#include "StateBattleStartFight.h"
#include "BattleWorld.h"

void StateBattleStartFight::enter(entityx::Entity& entity)
{
    _ENTITY_EVENT.subscribe<BattleEvent::ShowBattleSceneUIComplete>(*this);
    
    _ENTITY_EVENT.emit<BattleEvent::ShowBattleSceneUI>();
}

void StateBattleStartFight::exit(entityx::Entity& entity)
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::ShowBattleSceneUIComplete>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::ShowBattleSceneUIComplete>(*this);
    }
    
    _ENTITY_EVENT.emit<BattleEvent::HideBattleSceneUI>();
    
    _BATTLE_STAGE.stopFight();
}

int StateBattleStartFight::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    if (m_subState == SubState::FIGHTING)
    {
        _BATTLE_STAGE.updateBattleTime(dt);
        
        auto battle = entity.component<BattleComponent::Battle>();
        
        if (_BATTLE_STAGE.getBattleTime() >= battle->totalTime)
        {
            BattleWorld::getInstance()->getBattleResult().setTimeUp();
            
            _RETURN_STATE_RESULT(TIME_UP);
        }
    }
    
    return m_result;
}

void StateBattleStartFight::receive(const BattleEvent::ShowBattleSceneUIComplete& e)
{
    m_subState = SubState::FIGHTING;
    
    _BATTLE_STAGE.startFight();
    
    if (_BATTLE_STAGE.getBattleType() == BattleConfig::BattleType::DUEL)
    {
        _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_CONTROL_TYPE::HERO_RUSH_OUT, LEFT_HERO);
        _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_CONTROL_TYPE::HERO_RUSH_OUT, RIGHT_HERO);
    }
    
    if (BattleWorld::getInstance()->isAutoQuickBattleMode())
    {
        cocos2d::Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(EVENT_START_DELEGATE);
    }
    
    auto* pEntity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
    if (pEntity)
    {
        _ENTITY_EVENT.emit<BattleEvent::CameraFollowEntity>(*pEntity);
    }
}
