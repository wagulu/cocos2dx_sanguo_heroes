//
//  DizzyActionSkillTiaoJi.h
//  sanguoGame
//
//  Created by fuchenhao on 7/16/15.
//
//

#ifndef __sanguoGame__DizzyActionSkillTiaoJi__
#define __sanguoGame__DizzyActionSkillTiaoJi__

#include "DizzyAction.h"
#include "SimpleTween.h"
#include "BattleEvent.h"

class DizzyActionSkillTiaoJi : public DizzyAction, public entityx::Receiver<DizzyActionSkillTiaoJi>
{
public:
    
    virtual ~DizzyActionSkillTiaoJi();
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual void update(entityx::Entity& entity, float dt);
    virtual bool isComplete();
    
    void receive(const BattleEvent::HurtByGhost& e);
    
private:
    
    enum class FlyState
    {
        FLY_UP,
        FLY_TOP,
        DROP_DOWN,
        DONE
    };
    
    FlyState m_subState = FlyState::FLY_UP;
    
    float m_flyHeight;
    float m_flyDistance;
    float m_flyUpTime;
    float m_delay;
    float m_dropTime;
    float m_targetX;
    float m_elapse = 0;
    SimpleTween* m_moveTweenY = nullptr;
    SimpleTween* m_moveTweenX = nullptr;
    
    cocos2d::Vec3 m_beginPos;
    cocos2d::Vec3 m_endPos;
    
    void clearMoveTween();
};

#endif /* defined(__sanguoGame__DizzyActionSkillTiaoJi__) */
