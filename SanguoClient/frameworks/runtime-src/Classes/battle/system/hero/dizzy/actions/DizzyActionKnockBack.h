//
//  DizzyActionKnockBack.h
//  sanguoClient
//
//  Created by fuchenhao on 5/28/15.
//
//

#ifndef __sanguoClient__DizzyActionKnockBack__
#define __sanguoClient__DizzyActionKnockBack__

#include "DizzyAction.h"
#include "entityx.h"
#include "BattleEvent.h"

class DizzyActionKnockBack : public DizzyAction, public entityx::Receiver<DizzyActionKnockBack>
{
public:
    virtual ~DizzyActionKnockBack();
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
    
    void receive(const BattleEvent::HurtByAttack& e);

private:
    void knockBackHero(entityx::Entity& entity);
    
    entityx::Entity m_entity;
    int m_heroId = 0;
    int m_knockBackTile = 0;
    std::string m_knockBackTrigger;
};

#endif /* defined(__sanguoClient__DizzyActionKnockBack__) */
