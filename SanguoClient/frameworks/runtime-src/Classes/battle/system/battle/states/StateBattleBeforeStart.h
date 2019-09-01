//
//  StateBattleBeforeStart.h
//  sanguoGame
//
//  Created by fuchenhao on 7/21/15.
//
//

#ifndef __sanguoGame__StateBattleBeforeStart__
#define __sanguoGame__StateBattleBeforeStart__

#include "IState.h"
#include "entityx.h"
#include "BattleSystem.h"
#include "BattleEvent.h"

class StateBattleBeforeStart : public IState, public entityx::Receiver<StateBattleBeforeStart>
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
        SHOW_START_DIALOGUE,
        ENTER_SHOW,
    };
    
    virtual int getStateCode()
    {
        return BattleSystem::BattleState::BEFORE_START;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    void receive(const BattleEvent::DialogueStartComplete& e);
    
private:
    SubState m_subState = SubState::SHOW_START_DIALOGUE;
    
    bool m_roleIsShown = false;
    
    void changeSubState(SubState subState);
};

#endif /* defined(__sanguoGame__StateBattleBeforeStart__) */
