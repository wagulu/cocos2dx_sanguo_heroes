//
//  DizzyAction.h
//  sanguoClient
//
//  Created by fuchenhao on 5/26/15.
//
//

#ifndef __sanguoClient__DizzyAction__
#define __sanguoClient__DizzyAction__

#include "entityx.h"
#include "BattleConfig.h"

class DizzyAction
{
public:    
    virtual ~DizzyAction() {};
    
    virtual void enter(entityx::Entity& entity) = 0;
    virtual void exit(entityx::Entity& entity) = 0;
    virtual void update(entityx::Entity& entity, float dt) = 0;
    
    virtual bool isComplete()
    {
        return true;
    }
    
    void setData(BattleConfig::HeroDizzyData* pDizzyData, BattleConfig::HeroDizzyActionConfig* pActionConfig)
    {
        m_pDizzyData = pDizzyData;
        m_pActionConfig = pActionConfig;
    };
    
protected:
    BattleConfig::HeroDizzyData* m_pDizzyData = nullptr;
    BattleConfig::HeroDizzyActionConfig* m_pActionConfig = nullptr;
};

#endif /* defined(__sanguoClient__DizzyAction__) */
