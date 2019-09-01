//
//  TriStageTrigger.h
//  sanguoGame
//
//  Created by fuchenhao on 8/17/15.
//
//

#ifndef __sanguoGame__TriStageTrigger__
#define __sanguoGame__TriStageTrigger__

#include "SkillAction.h"

class TriStageTrigger : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
private:
    void addAnimation(int stage);
    float m_elapse = 0;
    float m_stage2Duration;
    std::array<float, 3> m_stageTime;
    std::array<bool, 3> m_stageFired;
};



#endif /* defined(__sanguoGame__TriStageTrigger__) */
