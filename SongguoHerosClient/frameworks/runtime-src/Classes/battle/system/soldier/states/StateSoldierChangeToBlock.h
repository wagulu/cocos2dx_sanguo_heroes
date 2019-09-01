//
//  StateSoldierChangeToBlock.h
//  sanguoClient
//
//  Created by fuchenhao on 6/26/15.
//
//

#ifndef __sanguoClient__StateSoldierChangeToBlock__
#define __sanguoClient__StateSoldierChangeToBlock__

#include "IState.h"
#include "BattleConfig.h"
#include "BattleEvent.h"

class StateSoldierChangeToBlock : public IState, public entityx::Receiver<StateSoldierChangeToBlock>
{
public:
    enum RESULT
    {
        WORKING,
        DEAD
    };
    RESULT m_result = WORKING;
    
    StateSoldierChangeToBlock(const BattleEvent::StrategySoldierChangeToBlock& eventBlock);
    virtual ~StateSoldierChangeToBlock();
    
    virtual int getStateCode()
    {
        return BattleConfig::SoldierState::STATE_CHANGE_TO_BLOCK;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    
    void receive(const BattleEvent::HurtByAttack& e);
    
private:
    BattleEvent::StrategySoldierChangeToBlock m_blockData;
    
    bool m_reverse = false;
    
    entityx::Entity m_appearEffectEntity;
    entityx::Entity m_loopEffectEntity;
};

#endif /* defined(__sanguoClient__StateSoldierChangeToBlock__) */
