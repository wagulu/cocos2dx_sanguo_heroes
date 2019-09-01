//
//  AssignEnemyHeroCenter.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/14/15.
//
//

#include "AssignEnemyHeroCenter.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void AssignEnemyHeroCenter::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* entity = _BATTLE_ENTITY.getEntity(m_fromId);
    if(entity)
    {
        auto identify = (*entity).component<BattleComponent::Identify>();
        int targetId = identify->side == BattleConfig::SIDE_LEFT ? RIGHT_HERO : LEFT_HERO;
        auto targetEntity = _BATTLE_ENTITY.getEntity(targetId);
        if(targetEntity)
        {
            auto position = (*targetEntity).component<BattleComponent::Position>();
            _ENTITY_MANAGER.get(m_entityId).assign<BattleComponent::SkillTileCenter>(MapHelper::getTileX(position->x), MapHelper::getTileZ(position->z));
        }
        else
        {
            remove();
        }
    }
    else
    {
        remove();
    }
    
    remove();
}
