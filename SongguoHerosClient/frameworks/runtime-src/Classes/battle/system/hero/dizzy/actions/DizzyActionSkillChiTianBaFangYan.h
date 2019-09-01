//
//  DizzyActionSkillChiTianBaFangYan.h
//  sanguoClient
//
//  Created by fuchenhao on 6/5/15.
//
//

#ifndef __sanguoClient__DizzyActionSkillChiTianBaFangYan__
#define __sanguoClient__DizzyActionSkillChiTianBaFangYan__

#include "DizzyAction.h"
#include "SimpleTween.h"
#include "BattleEvent.h"

class DizzyActionSkillChiTianBaFangYan : public DizzyAction, public entityx::Receiver<DizzyActionSkillChiTianBaFangYan>
{
public:
    struct HeroFlyData
    {
        float flyUpTime;
        float flyUpHeight;
        float dropDownDelay;//-1:不会自动掉落，等待最后一击才会掉落
        float dropDownTime;
        float knockBackTime;
        int knockBackTile;
        float knockBackDizzyTime;
        int lastHitSoldierId = 0;
        int firstHitSoldierId = 0;
        std::string* pLastHitTrigger = nullptr;
        std::string* pHeroDropTrigger = nullptr;
    };
    
    virtual ~DizzyActionSkillChiTianBaFangYan();
    
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
        KNOCK_DOWN,
        KNOCK_DOWN_COMPLETE,
        KNOCK_BACK_PREPARE,
        KNOCK_BACK,
        KNOCK_BACK_DIZZY,
        DONE
    };
    FlyState m_subState = FlyState::FLY_UP;
    
    HeroFlyData m_flyData;
    float m_elapse = 0;
    SimpleTween* m_moveTween = nullptr;
    
    cocos2d::Vec3 m_beginPos;
    cocos2d::Vec3 m_deltaPos;
    cocos2d::Vec3 m_endPos;
    
    void clearMoveTween();
    void fireSkillTrigger(std::string* pTrigger);
};

#endif /* defined(__sanguoClient__DizzyActionSkillChiTianBaFangYan__) */
