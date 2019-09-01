//
//  DizzyConditionByActionComplete.h
//  sanguoClient
//
//  Created by fuchenhao on 6/7/15.
//
//

#ifndef __sanguoClient__DizzyConditionByActionComplete__
#define __sanguoClient__DizzyConditionByActionComplete__

#include "DizzyCondition.h"
#include "DizzyAction.h"

class DizzyConditionByActionComplete : public DizzyCondition
{
public:
    virtual ~DizzyConditionByActionComplete();
    
    virtual void init(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
    
private:
    std::vector<DizzyAction*> m_dizzyActions;
};

#endif /* defined(__sanguoClient__DizzyConditionByActionComplete__) */
