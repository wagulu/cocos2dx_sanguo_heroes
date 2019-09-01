//
//  StateSoldierRun.h
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#ifndef __Game__StateSoldierMoveToTargetTile__
#define __Game__StateSoldierMoveToTargetTile__

#include <stdio.h>
#include "../../stateMachine/IState.h"

class StateSoldierMoveToTargetTile : public IState
{
public:
    enum RESULT
    {
        WORKING,
        ARRIVE,
        //        ATTACK_ENEMY,
        //        WIND,
    };
    RESULT m_result = WORKING;
    
    StateSoldierMoveToTargetTile(int m_lastStateCode);
    virtual ~StateSoldierMoveToTargetTile();
    
    virtual int getStateCode()
    {
        return BattleConfig::STATE_TARGET;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    
    int getLastStateCode() { return m_lastStateCode; }
    
private:
//    enum SUB_STATE
//    {
//        NORMAL,
//        WIND,
//        WAIT,
//    };
//    SUB_STATE m_subState = NORMAL;
    
    int m_lastStateCode;
    
};

#endif /* defined(__Game__StateSoldierRun__) */
