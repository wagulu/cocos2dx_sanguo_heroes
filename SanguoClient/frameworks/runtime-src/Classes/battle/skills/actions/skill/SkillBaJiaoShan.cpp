//
//  SkillBaJiaoShan.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/29/15.
//
//

#include "SkillBaJiaoShan.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void SkillBaJiaoShan::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int width = (int)m_skillActionData->config["width"]->d;
    int height = (int)m_skillActionData->config["height"]->d;
    
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
    
    float duration = m_skillActionData->config["duration"]->d;
    
    for (int targetId : targets)
    {
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if (pTargetEntity == nullptr) continue;
        
        BattleConfig::TileResults results;
        _TARGET_FINDER.findRectAreaTilesOnHero(width, height, *pTargetEntity, results);
        
        auto side = (*pTargetEntity).component<BattleComponent::Identify>()->side;
        
        for (auto* pTile : results.tiles)
        {
            auto targetId = _BATTLE_ENTITY.getTarget(pTile->tileZ, pTile->tileX);
            if(targetId > 0 && MapHelper::getSide(targetId) != side)
            {
                if(MapHelper::isSoldier(targetId))
                {
                    _ENTITY_EVENT.emit(BattleEvent::StateSoldierHitBack((*_BATTLE_ENTITY.getEntity(targetId)), duration));
                }
                else
                {
                    _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "knockBackHero");
                }
            }
        }
    }
    
    remove();
}
