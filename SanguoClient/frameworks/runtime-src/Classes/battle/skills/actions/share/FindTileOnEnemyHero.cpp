//
//  FindTileOnEnemyHero.cpp
//  sanguoGame
//
//  Created by fuchenhao on 8/3/15.
//
//

#include "FindTileOnEnemyHero.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void FindTileOnEnemyHero::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    int targetId = MapHelper::getSide(m_fromId) == BattleConfig::SIDE_LEFT ? RIGHT_HERO : LEFT_HERO;
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity)
    {
        auto position = pTargetEntity->component<BattleComponent::Position>();
        _ENTITY_MANAGER.get(m_entityId).assign<BattleComponent::SkillTileTarget>(MapHelper::getTileX(position->x), MapHelper::getTileZ(position->z));
    }
    
    remove();
}
