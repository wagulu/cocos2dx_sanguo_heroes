//
//  AttackOnFindTarget.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#include "AttackOnFindTarget.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AttackOnFindTarget::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* pFromEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pFromEntity != nullptr)
    {
        TargetFinder::TargetResults targetResults;
        vector<int>& targets = targetResults.targets;
        
        if (m_skillActionData->hasMember("SkillTarget"))
        {
            auto skillTarget = EnumParserSkillTarget().getValue(*m_skillActionData->config["SkillTarget"]->s);
            _TARGET_FINDER.findSkillTargets(m_fromId, skillTarget, targetResults);
        }
        else
        {
            auto skillTargetResults = es.get(m_entityId).component<BattleComponent::SkillTargetResults>();
            if (skillTargetResults.valid()) targets = skillTargetResults->targets;
        }
        
        if (targets.size() > 0)
        {
            int repeat = (m_skillActionData->hasMember("repeat") ? m_skillActionData->config["repeat"]->d : 1);
            float interval = (m_skillActionData->hasMember("interval") ? m_skillActionData->config["interval"]->d : 0);
            auto position = pFromEntity->component<BattleComponent::Position>();
            
            for (int targetId : targets)
            {
                BattleConfig::AttackData attackData(m_fromId, 0, repeat, interval);
                attackData.setTargetId(targetId);
                attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
                attackData.damagePosition.x = position->x;
                attackData.damagePosition.y = 0;
                attackData.damagePosition.z = position->z;
                attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH;
                attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV;
                attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
                _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
            }
        }
    }

    remove();
}
