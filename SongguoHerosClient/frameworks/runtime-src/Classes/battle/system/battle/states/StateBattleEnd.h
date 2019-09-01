//
//  StateBattleEnd.h
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#ifndef __sanguoGame__StateBattleEnd__
#define __sanguoGame__StateBattleEnd__

#include "IState.h"
#include "entityx.h"
#include "BattleSystem.h"
#include "BattleEvent.h"

class StateBattleEnd : public IState, public entityx::Receiver<StateBattleEnd>
{
public:
    enum RESULT
    {
        WORKING,
        COMPLETE
    };
    RESULT m_result = WORKING;
    
    enum class SubState
    {
        WAID_HERO_DEAD,
        PLAY_BATTLE_RESULT_EFFECT,
        SHOW_DIALOGUE
    };
    
    virtual int getStateCode()
    {
        return BattleSystem::BattleState::BATTLE_END;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    void receive(const BattleEvent::StateHeroDeadDone& e);
    void receive(const BattleEvent::PlayBattleResultEffectComplete& e);
    void receive(const BattleEvent::DialogueResultComplete& e);
    
private:
    void changeSubState(SubState subState);
    void backToLuaScene(); 
    
    SubState m_subState;
};

#endif /* defined(__sanguoGame__StateBattleEnd__) */
