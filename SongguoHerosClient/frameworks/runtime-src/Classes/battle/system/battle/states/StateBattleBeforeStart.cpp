//
//  StateBattleBeforeStart.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#include "StateBattleBeforeStart.h"
#include "BattleWorld.h"
#include "AudioSystem.h"
#include "AudioManager.h"

void StateBattleBeforeStart::enter(entityx::Entity& entity)
{
    _ENTITY_EVENT.subscribe<BattleEvent::DialogueStartComplete>(*this);
    
    _BATTLE_STAGE.getRole()->setVisible(false);
    
    _ENTITY_EVENT.emit<BattleEvent::ShowStartDialogue>();
}

void StateBattleBeforeStart::exit(entityx::Entity& entity)
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::DialogueStartComplete>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::DialogueStartComplete>(*this);
    }
    
    std::string eventName = AUDIOEVENTS_BATTLE_START;
    std::string triggerName = ""; //getHeroSoldierAudioTriggerName(entity);
    BattleEvent::BattleSound soundEvent(eventName, triggerName, entityx::Entity());
    _ENTITY_EVENT.emit(soundEvent);
}

int StateBattleBeforeStart::update(entityx::Entity& entity, double dt)
{
    if (m_result != WORKING)
    {
        return m_result;
    }
    
    if (m_subState == SubState::ENTER_SHOW)
    {
        if (_ENTER_SHOW_CONTROL.isInEnterShow())
        {
            if (!m_roleIsShown)
            {
                m_roleIsShown = true;
                _BATTLE_STAGE.getRole()->setVisible(true);
            }
            
            _ENTER_SHOW_CONTROL.enterShowUpdateGo(dt);
        }
        else
        {
            _RETURN_STATE_RESULT(COMPLETE);
        }
    }
    
    return m_result;
}

void StateBattleBeforeStart::changeSubState(SubState subState)
{
    if (m_subState != subState)
    {
        m_subState = subState;
        
        if (m_subState == SubState::ENTER_SHOW)
        {
            _ENTER_SHOW_CONTROL.triggerEnterShow(BattleConfig::getInstance()->getBattleFieldConfig().enterShow);
        }
    }
}

void StateBattleBeforeStart::receive(const BattleEvent::DialogueStartComplete& e)
{
    changeSubState(SubState::ENTER_SHOW);
}
