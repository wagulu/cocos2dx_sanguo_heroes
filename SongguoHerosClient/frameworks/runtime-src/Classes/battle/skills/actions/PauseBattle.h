//
//  PauseBattle.h
//  Game
//
//  Created by fuchenhao on 3/2/15.
//
//

#ifndef __PAUSE_BATTLE_H__
#define __PAUSE_BATTLE_H__

#include "SkillAction.h"

class PauseBattle : public SkillAction
{
public:
    ~PauseBattle();
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
private:
    void resumeBattle();
    
    float m_duration = 0;
    bool m_resumeRequired = false;
};

#endif /* defined(__PAUSE_BATTLE_H__) */
