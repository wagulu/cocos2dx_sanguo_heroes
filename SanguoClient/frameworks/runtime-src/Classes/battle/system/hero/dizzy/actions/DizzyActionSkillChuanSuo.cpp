//
//  DizzyActionSkillChuanSuo.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/22/15.
//
//

#include "DizzyActionSkillChuanSuo.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "SkillObject.h"

void DizzyActionSkillChuanSuo::enter(entityx::Entity& entity)
{
    auto direction = entity.component<BattleComponent::Direction>();
    auto identify = entity.component<BattleComponent::Identify>();
    auto animation = entity.component<BattleComponent::Animation>();

    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_HIT, false);
    direction->x = identify->faceTo == BattleConfig::FACE_TO_LEFT ? -1 : 1;
}

void DizzyActionSkillChuanSuo::exit(entityx::Entity& entity)
{
}

bool DizzyActionSkillChuanSuo::isComplete()
{
    return m_subState == SUB_STATE::DONE;
}

void DizzyActionSkillChuanSuo::update(entityx::Entity& entity, float dt)
{
    if (m_subState == SUB_STATE::DONE)
    {
        return;
    }

    if (m_subState == SUB_STATE::RUN)
    {
        bool result = _HERO_MOVEMENT.move(dt, entity, HeroMovement::IGNORE_ALL_SOLDIER, 1300);
        if(result)
        {
            m_subState = SUB_STATE::WAIT;
            
            auto skillEntity = _ENTITY_MANAGER.get(m_pDizzyData->skillEntityId);
            auto skillComp = skillEntity.component<BattleComponent::Skill>();
            
            if (skillComp->skill)
            {
                skillComp->skill->triggerAction("playAnimation2");
            }
        }
    }
    else if (m_subState == SUB_STATE::WAIT)
    {
        m_waitTime += dt;
        if(m_waitTime >= 0.7)
        {
            m_subState = SUB_STATE::DONE;
        
            auto skillEntity = _ENTITY_MANAGER.get(m_pDizzyData->skillEntityId);
            auto skillComp = skillEntity.component<BattleComponent::Skill>();
        
            if (skillComp->skill)
            {
                skillComp->skill->triggerAction("lastHit");
            }
        }
    }
}
