//
//  DizzyConditionByTime.h
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#ifndef __sanguoClient__DizzyConditionByTime__
#define __sanguoClient__DizzyConditionByTime__

#include "DizzyCondition.h"

class DizzyConditionByTime : public DizzyCondition
{
public:
    virtual ~DizzyConditionByTime();
    
    virtual void init(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
    
private:
    float m_dizzyTime = 0;
};

#endif /* defined(__sanguoClient__DizzyConditionByTime__) */
