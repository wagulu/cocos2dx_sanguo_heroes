//
//  RemoveShieldAnimationOnUseUp.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/22/15.
//
//

#include "RemoveShieldAnimationOnUseUp.h"
#include "EnumParser.h"

void RemoveShieldAnimationOnUseUp::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    m_shieldProperty = EnumParserPropertyName().getValue(*m_skillActionData->config["property"]->s);
    m_tag = *m_skillActionData->config["tag"]->s;
    
    auto skillTargetResults = es.get(m_entityId).component<BattleComponent::SkillTargetResults>();
    if (!skillTargetResults.valid() || skillTargetResults->targets.size() == 0)
    {
        remove();
    }
    else
    {
        m_targets = skillTargetResults->targets;
    }
}

void RemoveShieldAnimationOnUseUp::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (m_targets.size() == 0)
    {
        remove();
    }
    else
    {
        int targetId;
        for (auto iter = m_targets.begin(); iter != m_targets.end(); )
        {
            targetId = *iter;
            
            auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if (pTargetEntity != nullptr)
            {
                auto property = pTargetEntity->component<BattleComponent::Property>();
                if (property->get(m_shieldProperty) <= 0)
                {
                    iter = m_targets.erase(iter);
                    _ENTITY_EVENT.emit<BattleEvent::RemoveEntityAnimation>(*pTargetEntity, &m_tag);
                    continue;
                }
            }
            
            iter++;
        }
    }
}
