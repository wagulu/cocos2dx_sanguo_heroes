//
//  FindRandomTargetAroundHero.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/13/15.
//
//

#include "FindRandomTargetAroundHero.h"
#include "BattleWorld.h"
#include "EnumParser.h"

void FindRandomTargetAroundHero::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    BattleConfig::DamageSide damageSide = EnumParserDamageSide().getValue(*m_skillActionData->config["DamageSide"]->s);
    BattleConfig::Side side(BattleConfig::SIDE_ALL);
    if (damageSide == BattleConfig::DamageSide::ENEMY)
    {
        side = (m_side == BattleConfig::SIDE_LEFT ? BattleConfig::SIDE_RIGHT : BattleConfig::SIDE_LEFT);
    }
    else if (damageSide == BattleConfig::DamageSide::SELF)
    {
        side = m_side;
    }
    
    auto skillEntity = _ENTITY_MANAGER.get(m_entityId);
    bool find = false;
    
    if (side != BattleConfig::SIDE_ALL)
    {
        BattleConfig::Tile center(-1, -1);
        int squareSize = m_skillActionData->config["squareSize"]->d;
        _TARGET_FINDER.findCenter(side, squareSize, center);
        
        if (MapHelper::validTile(center.tileZ, center.tileX))
        {
            skillEntity.assign<BattleComponent::SkillTileTarget>(center.tileX, center.tileZ);
            find = true;
        }
    }
    
    if (!find && skillEntity.has_component<BattleComponent::SkillTileTarget>())
    {
        skillEntity.component<BattleComponent::SkillTileTarget>().remove();
    }
    
    remove();
}
