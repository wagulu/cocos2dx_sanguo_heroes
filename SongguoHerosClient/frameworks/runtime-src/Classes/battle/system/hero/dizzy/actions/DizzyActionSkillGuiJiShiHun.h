//
//  DizzyActionSkillGuiJiShiHun.h
//  sanguoClient
//
//  Created by fuchenhao on 6/4/15.
//
//

#ifndef __sanguoClient__DizzyActionSkillGuiJiShiHun__
#define __sanguoClient__DizzyActionSkillGuiJiShiHun__

#include "DizzyAction.h"
#include "entityx.h"
#include "SimpleTween.h"

class DizzyActionSkillGuiJiShiHun : public DizzyAction
{
public:
    struct DizzyBehaviorConfig
    {
        int absorbFromId;
        float absorbSlowSpeed;
        float absorbFastTile;
        float absorbFastTime;
        float knockBackTime;
        int knockBackValidDistance;
        float knockBackDuration;
        int knockBackTile;
        float knockBackDizzyDuration;
    };
    
    virtual ~DizzyActionSkillGuiJiShiHun();
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
    virtual bool isComplete();
    
private:
    enum class SUB_STATE
    {
        ABSORB_SLOW,
        ABSORB_FAST,
        KNOCK_BACK,
        KNOCK_BACK_DIZZY,
        DONE
    };
    SUB_STATE m_subState = SUB_STATE::ABSORB_SLOW;

    float m_moveFastToX = 0;
    float m_knockBackToX = 0;
    
    DizzyBehaviorConfig m_config;
    
    SimpleTween_quad* m_moveFastTween = nullptr;
    SimpleTween_cubic* m_knockBackTween = nullptr;
};

#endif /* defined(__sanguoClient__DizzyActionSkillGuiJiShiHun__) */
