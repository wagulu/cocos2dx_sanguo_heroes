//
//  MoveEnemyToHero.cpp
//  Game
//
//  Created by zhaikaiyu on 3/11/15.
//
//

#include "SkillGuiJiShiHun.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"

void SkillGuiJiShiHun::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    float blackHoleRepeat = caclFormulaResult("black_hole_repeat");
    
    m_blackHoleTime = blackHoleRepeat * m_skillActionData->config["blackHoleRepeatDuration"]->d;
    m_finalHitAnimationTime = m_blackHoleTime + m_skillActionData->config["finalHitAnimationDelay"]->d;
    m_absorbFastTime = m_blackHoleTime + m_skillActionData->config["absorbFastDelay"]->d;
    m_knockBackTime = m_blackHoleTime + m_skillActionData->config["knockBackDelay"]->d;
    m_finalHitAttackTime = m_blackHoleTime + m_skillActionData->config["finalHitAttackDelay"]->d;

    m_preHitTime = 0;
    m_radius = m_skillActionData->config["radius"]->d;
    m_attackInterval = m_skillActionData->config["attackInterval"]->d;
    
    m_dizzyData.skillEntityId = m_entityId;
    m_dizzyConfig = *m_skillData->pConfig->dizzy->getConfig(*m_skillActionData->config["heroDizzyConfig"]->s);
    m_pDizzyActionSkillGuiJi = m_dizzyConfig.findAction("skill_gui_ji_shi_hun");
    
    if (m_pDizzyActionSkillGuiJi != nullptr)
    {
        m_pDizzyActionSkillGuiJi->config.insert(std::pair<std::string, BattleConfig::ParamData>("absorbFromId", BattleConfig::ParamData((double)m_fromId)));
    }
    
    // add_entity_effect, loopSkill使用
    _ENTITY_MANAGER.get(m_entityId).assign<BattleComponent::SkillDuration>(m_blackHoleTime);
    
    _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "blackHoleStart");
}

void SkillGuiJiShiHun::update(entityx::EntityManager &es, float elapse, float dt)
{
    bool dizzyRequired = false;
    if (m_absorbFastTime > 0)
    {
        dizzyRequired = true;
        m_absorbFastTime -= dt;
    }
    
    if (m_knockBackTime > 0)
    {
        m_knockBackTime -= dt;
    }
    
    if (m_finalHitAnimationTime > 0)
    {
        m_finalHitAnimationTime -= dt;
        if (m_finalHitAnimationTime <= 0)
        {
            _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "finalHitAnimation");
        }
    }
    
    bool damageRequired = false;
    if (m_blackHoleTime > 0)
    {
        damageRequired = true;
        
        m_blackHoleTime -= dt;
        if (m_blackHoleTime <= 0)
        {
            _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "finalHitEffect");
        }
    }
    
    if (m_finalHitAttackTime > 0)
    {
        m_finalHitAttackTime -= dt;
        if (m_finalHitAttackTime <= 0)
        {
            _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "finalHitAttack");
            remove();
            return;
        }
    }
    
    auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pFromEntity == nullptr)
    {
        remove();
        return;
    }
    
    auto fromPos = pFromEntity->component<BattleComponent::Position>();
    int centerZ = MapHelper::getTileZ(fromPos->z);
    int centerX = MapHelper::getTileX(fromPos->x);
    int centerXLeft = centerX - 1;
    int centerXRight = centerX;
    
    if (dizzyRequired)
    {
        for (int x = 1; x <= m_radius; x++)
        {
            dizzyEnemyHero(centerZ, centerXLeft - x);
            dizzyEnemyHero(centerZ, centerXRight + x);
        }
    }
    
    if (damageRequired && elapse - m_preHitTime >= m_attackInterval)
    {
       m_preHitTime = elapse;
        
       hitDamage(centerZ, centerXLeft - 1);
       hitDamage(centerZ, centerXRight + 1);
            
       _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "attackEnemySoldier");
    }
}

void SkillGuiJiShiHun::dizzyEnemyHero(int tileZ, int tileX)
{
    if (MapHelper::validTile(tileZ, tileX))
    {
        int targetId = _BATTLE_ENTITY.getTarget(tileZ, tileX);
        if (targetId != 0 && MapHelper::getSide(targetId) != m_side && MapHelper::isHero(targetId))
        {
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity != nullptr && m_pDizzyActionSkillGuiJi != nullptr)
            {
                m_pDizzyActionSkillGuiJi->config["knockBackTime"] = BattleConfig::ParamData((double)m_knockBackTime);
                m_pDizzyActionSkillGuiJi->config["absorbFastTime"] = BattleConfig::ParamData((double)m_absorbFastTime);
                
                _ENTITY_EVENT.emit(BattleEvent::StrategyHeroDizzy(*pTargetEntity, m_dizzyData, m_dizzyConfig));
            }
        }
    }
}

void SkillGuiJiShiHun::hitDamage(int titleZ, int titleX)
{
    BattleConfig::AttackData attackData(m_fromId);
    attackData.setTargetPos(titleZ, titleX);
    attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);

    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
}