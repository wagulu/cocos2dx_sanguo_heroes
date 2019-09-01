//
//  IntervalTrigger.h
//  sanguoClient
//
//  Created by fuchenhao on 4/13/15.
//
//

#ifndef __sanguoClient__IntervalTrigger__
#define __sanguoClient__IntervalTrigger__



#include "../SkillAction.h"
#include "../../BattleWorld.h"
#include "../../BattleConfig.h"

class IntervalTrigger : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
private:
    float m_interval;
    int m_times;
    float m_elapse = 0;
    std::string* m_trigger;
};


#endif /* defined(__sanguoClient__IntervalTrigger__) */
