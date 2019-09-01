//
//  FindRandomTargetAroundTile.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/14/15.
//
//

#include "FindRandomTargetAroundTile.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void FindRandomTargetAroundTile::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int biasX = (int)(cocos2d::rand_minus1_1() * m_skillActionData->config["width"]->d * 0.5f);
    int biasZ = (int)(cocos2d::rand_minus1_1() * m_skillActionData->config["height"]->d * 0.5f);
    
    auto skillEntity = _ENTITY_MANAGER.get(m_entityId);
    auto center = skillEntity.component<BattleComponent::SkillTileCenter>();
    
    if(!skillEntity.has_component<BattleComponent::SkillTileTarget>())
    {
        skillEntity.assign<BattleComponent::SkillTileTarget>(center->tileX + biasX, center->tileZ + biasZ);
    }
    else
    {
        auto target = skillEntity.component<BattleComponent::SkillTileTarget>();
        target->tileX = center->tileX + biasX;
        target->tileZ = center->tileZ + biasZ;
    }
    
    remove();
}
