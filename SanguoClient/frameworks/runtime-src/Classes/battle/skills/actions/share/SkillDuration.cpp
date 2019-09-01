//
//  SkillDuration.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/16/15.
//
//

#include "SkillDuration.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void SkillDuration::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    float duration = caclFormulaResult(*m_skillActionData->config["duration"]->s);
    
    _ENTITY_MANAGER.get(m_entityId).assign<BattleComponent::SkillDuration>(duration);
    
    remove();
}
