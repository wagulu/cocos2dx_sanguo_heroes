//
//  StateBattleEnd.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#include "StateBattleEnd.h"
#include "BattleWorld.h"
#include "BattleEvent.h"
#include "ProjectConfig.h"
#include "base/CCScriptSupport.h"
#include "CCLuaEngine.h"

void StateBattleEnd::enter(entityx::Entity& entity)
{
    _STRATEGY_CONTROL.setBattleEndForAll();
    
    BattleWorld::getInstance()->getBattleResult().prepare();
    
    _ENTITY_EVENT.subscribe<BattleEvent::PlayBattleResultEffectComplete>(*this);
    _ENTITY_EVENT.subscribe<BattleEvent::DialogueResultComplete>(*this);
    
    if (BattleWorld::getInstance()->getBattleResult().getReason() == BattleResult::Reason::HERO_DEAD)
    {
        changeSubState(SubState::WAID_HERO_DEAD);
    }
    else
    {
        changeSubState(SubState::PLAY_BATTLE_RESULT_EFFECT);
    }
}

void StateBattleEnd::exit(entityx::Entity& entity)
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::StateHeroDeadDone>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::StateHeroDeadDone>(*this);
    }
    if (_ENTITY_EVENT.hasReceive<BattleEvent::PlayBattleResultEffectComplete>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::PlayBattleResultEffectComplete>(*this);
    }
    if (_ENTITY_EVENT.hasReceive<BattleEvent::DialogueResultComplete>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::DialogueResultComplete>(*this);
    }
}

int StateBattleEnd::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
        
    return m_result;
}

void StateBattleEnd::changeSubState(SubState subState)
{
    m_subState = subState;
    
    if (m_subState == SubState::WAID_HERO_DEAD)
    {
        _ENTITY_EVENT.subscribe<BattleEvent::StateHeroDeadDone>(*this);
    }
    else if (m_subState == SubState::PLAY_BATTLE_RESULT_EFFECT)
    {
        _ENTITY_EVENT.emit<BattleEvent::PlayBattleResultEffect>();
    }
    else if (m_subState == SubState::SHOW_DIALOGUE)
    {
        if (BattleWorld::getInstance()->getBattleResult().isWin())
        {
            _ENTITY_EVENT.emit<BattleEvent::ShowWinDialogue>();
        }
        else
        {
            _ENTITY_EVENT.emit<BattleEvent::ShowLoseDialogue>();
        }
    }
}

void StateBattleEnd::receive(const BattleEvent::StateHeroDeadDone& e)
{
    changeSubState(SubState::PLAY_BATTLE_RESULT_EFFECT);
}

void StateBattleEnd::receive(const BattleEvent::PlayBattleResultEffectComplete& e)
{
    changeSubState(SubState::SHOW_DIALOGUE);
}

void StateBattleEnd::receive(const BattleEvent::DialogueResultComplete& e)
{
    m_result = COMPLETE;
    
    backToLuaScene();
}

void StateBattleEnd::backToLuaScene()
{
#if (USE_LUA_ENGINE)
    lua_State *L = cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState();
    lua_getglobal(L, "onMessageResponse");
    lua_pushstring(L, EVENT_BATTLE_END);
    lua_pushstring(L, BattleWorld::getInstance()->getBattleResult().getResult().c_str());
    lua_call(L, 2, 0);
#endif
}
