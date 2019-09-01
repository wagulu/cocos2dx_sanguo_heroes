//
//  LockSkill.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/7/15.
//
//

#include "LockOppSkill.h"
#include "BattleWorld.h"
#include "EnumParser.h"

LockOppSkill::~LockOppSkill()
{
    setLockState(false);
}

void LockOppSkill::fireAction(entityx::EntityManager &es)
{
    if(m_hasLocked) return;
    
    SkillAction::fireAction(es);
    
    int targetId = (m_fromId == LEFT_HERO) ? RIGHT_HERO : LEFT_HERO;
    bool enable = false;
    if(m_skillActionData->hasMember("enable"))
    {
        enable = caclFormulaEnable(*m_skillActionData->config["enable"]->s, targetId);
    }
    else if(m_skillActionData->hasMember("ratio"))
    {
        float ratio = caclFormulaResult(*m_skillActionData->config["ratio"]->s);
        enable = cocos2d::random<float>(0.0f, 1.0f) <= ratio;
    }
    
    if(enable)
    {
        m_lifeTime = caclFormulaResult(*m_skillActionData->config["duration"]->s);
    
        setLockState(true);
        
        m_hasLocked = true;
    }
    else
    {
        remove();
    }
}

void LockOppSkill::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (elapse >= m_lifeTime)
    {
        setLockState(false);
        
        remove();
    }
}

void LockOppSkill::setLockState(bool lock)
{
    int targetId = (m_fromId == LEFT_HERO) ? RIGHT_HERO : LEFT_HERO;
    auto* entity = _BATTLE_ENTITY.getEntity(targetId);
    if(entity)
    {
        auto gasGather = (*entity).component<BattleComponent::GasGather>();
        gasGather->lock = true;
    }
}
