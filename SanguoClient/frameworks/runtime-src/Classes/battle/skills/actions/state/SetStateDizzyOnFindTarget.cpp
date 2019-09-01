//
//  SetStateDizzyOnFindTarget.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/15/15.
//
//

#include "SetStateDizzyOnFindTarget.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void SetStateDizzyOnFindTarget::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);

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
        if (skillTargetResults.valid())
        {
            targets = skillTargetResults->targets;
        }
    }
    
    for (int targetId : targets)
    {
        setTargetDizzy(targetId);
    }
    
    remove();
}
