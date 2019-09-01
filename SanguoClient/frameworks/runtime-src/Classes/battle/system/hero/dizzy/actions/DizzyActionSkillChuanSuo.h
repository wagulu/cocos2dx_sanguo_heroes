//
//  DizzyActionSkillChuanSuo.h
//  sanguoGame
//
//  Created by fuchenhao on 7/22/15.
//
//

#ifndef __sanguoGame__DizzyActionSkillChuanSuo__
#define __sanguoGame__DizzyActionSkillChuanSuo__

#include "DizzyAction.h"
#include "entityx.h"
#include "SimpleTween.h"

class DizzyActionSkillChuanSuo : public DizzyAction
{
public:
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
    virtual bool isComplete();
    
private:
    enum class SUB_STATE
    {
        RUN,
        WAIT,
        DONE
    };
    
    float m_waitTime = 0;
    
    SUB_STATE m_subState = SUB_STATE::RUN;
};

#endif /* defined(__sanguoGame__DizzyActionSkillChuanSuo__) */
