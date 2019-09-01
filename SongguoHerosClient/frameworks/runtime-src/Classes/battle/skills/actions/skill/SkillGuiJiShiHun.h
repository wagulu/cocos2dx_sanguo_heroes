//
//  SkillGuiJiShiHun.h
//  Game
//
//  Created by zhaikaiyu on 3/11/15.
//
//

#ifndef __SKILL_GUI_JI_SHI_HUN__
#define __SKILL_GUI_JI_SHI_HUN__

#include "SkillAction.h"
#include "entityx.h"
#include "BattleComponent.h"

class SkillGuiJiShiHun : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    void dizzyEnemyHero(int tileZ, int tileX);
    void hitDamage(int titleZ, int titleX);
    
private:
    float m_blackHoleTime;
    float m_finalHitAnimationTime;
    float m_finalHitAttackTime;
    float m_absorbFastTime;
    float m_knockBackTime;
    float m_preHitTime;
    int m_radius;
    float m_attackInterval;
    
    BattleConfig::HeroDizzyData m_dizzyData;
    BattleConfig::HeroDizzyConfig m_dizzyConfig;
    BattleConfig::HeroDizzyActionConfig* m_pDizzyActionSkillGuiJi = nullptr;
};


#endif /* defined(__SKILL_GUI_JI_SHI_HUN__) */
