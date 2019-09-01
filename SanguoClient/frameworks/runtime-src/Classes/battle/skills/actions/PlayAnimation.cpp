//
//  PlayAnimation.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/12/15.
//
//

#include "PlayAnimation.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void PlayAnimation::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pTargetEntity != nullptr)
    {
        float duration = 0;
        if (m_skillActionData->hasMember("duration"))
        {
            duration = m_skillActionData->config["duration"]->d;
        }
        
        const string& animationName = *(m_skillActionData->config["animation"]->s);
        
        _ENTITY_EVENT.emit<BattleEvent::StrategyHeroPlayAnimation>(*pTargetEntity, animationName, duration);
    }
    
    remove();
}
