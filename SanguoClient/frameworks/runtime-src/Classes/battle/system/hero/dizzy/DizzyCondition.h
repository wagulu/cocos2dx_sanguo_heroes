//
//  DizzyCondition.h
//  sanguoClient
//
//  Created by fuchenhao on 5/26/15.
//
//

#ifndef __sanguoClient__DizzyCondition__
#define __sanguoClient__DizzyCondition__

#include "entityx.h"
#include "BattleConfig.h"

class DizzyCondition
{
public:    
    virtual ~DizzyCondition() {};
    
    virtual void init(entityx::Entity& entity) = 0;
    virtual void update(entityx::Entity& entity, float dt) = 0;
    
    inline bool isComplete()
    {
        return m_complete;
    };
    
    inline bool isRequired()
    {
        return (m_pConditionConfig && m_pConditionConfig->required);
    };
    
    void setData(BattleConfig::HeroDizzyData* pDizzyData, BattleConfig::HeroDizzyConditionConfig* pConditionConfig)
    {
        m_pDizzyData = pDizzyData;
        m_pConditionConfig = pConditionConfig;
    };

protected:
    bool m_complete = false;
    BattleConfig::HeroDizzyData* m_pDizzyData = nullptr;
    BattleConfig::HeroDizzyConditionConfig* m_pConditionConfig = nullptr;
};

#endif /* defined(__sanguoClient__DizzyCondition__) */
