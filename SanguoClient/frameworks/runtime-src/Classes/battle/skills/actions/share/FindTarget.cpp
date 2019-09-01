//
//  FindTarget.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#include "FindTarget.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void FindTarget::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto skillTarget = EnumParserSkillTarget().getValue(*m_skillActionData->config["SkillTarget"]->s);
    
    TargetFinder::TargetResults targetResults;
    
    _TARGET_FINDER.findSkillTargets(m_fromId, skillTarget, targetResults);
    
    auto skillEntity = _ENTITY_MANAGER.get(m_entityId);
    auto skillTargetResult = skillEntity.component<BattleComponent::SkillTargetResults>();
    if (skillTargetResult.valid()) skillTargetResult.remove();
    
    skillEntity.assign<BattleComponent::SkillTargetResults>(targetResults.targets);
    
    remove();
}
