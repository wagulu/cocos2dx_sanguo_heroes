//
//  DizzyConditionBySkillEnd.h
//  sanguoClient
//
//  Created by fuchenhao on 5/28/15.
//
//

#ifndef __sanguoClient__DizzyConditionBySkillEnd__
#define __sanguoClient__DizzyConditionBySkillEnd__

#include "DizzyCondition.h"

class DizzyConditionBySkillEnd : public DizzyCondition
{
public:
    virtual ~DizzyConditionBySkillEnd();
    
    virtual void init(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
};

#endif /* defined(__sanguoClient__DizzyConditionBySkillEnd__) */
