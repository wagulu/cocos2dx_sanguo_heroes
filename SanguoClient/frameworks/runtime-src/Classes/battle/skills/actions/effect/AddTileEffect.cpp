//
//  AddTileEffect.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/24/15.
//
//

#include "AddTileEffect.h"
#include "BattleWorld.h"

void AddTileEffect::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    EffectConfigData config;
    if (parseConfig(config))
    {
        auto* effectGroup = m_effect->getEffect(*config.pEffect);
        
        auto& tiles = config.tileResults.tiles;
        for (auto* pTile : tiles)
        {
            float offsetX = (config.randomOffset > 0 ? (cocos2d::random() % 2 == 0 ? -1 : 1) * config.randomOffset : 0);
            float offsetZ = (config.randomOffset > 0 ? (cocos2d::random() % 2 == 0 ? -1 : 1) * config.randomOffset : 0);
            
            float x = MapHelper::getCenterX(pTile->tileX, false) + offsetX;
            float y = 0;
            float z = MapHelper::getCenterZ(pTile->tileZ) + offsetZ;
            
            _ENTITY_EVENT.emit<BattleEvent::AddEffect>(effectGroup, config.reverse, x, y, z, config.delay, config.duration);
        }
    }
    
    remove();
}

bool AddTileEffect::parseConfig(EffectConfigData& config)
{
    auto skillEntity = _ENTITY_MANAGER.get(m_entityId);
    
    config.pEffect = m_skillActionData->config["effect"]->s;
    config.reverse = (m_side == BattleConfig::SIDE_LEFT);
    
    if (m_skillActionData->hasMember("reverse"))
    {
        config.reverse = (m_side != BattleConfig::SIDE_LEFT);
    }
    
    if (m_skillActionData->hasMember("randomOffset"))
    {
        config.randomOffset = m_skillActionData->config["randomOffset"]->d;
    }
    
    if (m_skillActionData->hasMember("delay"))
    {
        config.delay = m_skillActionData->config["delay"]->d;
    }
    
    if (m_skillActionData->hasMember("duration"))
    {
        config.duration = m_skillActionData->config["duration"]->d;
    }
    else if (skillEntity.has_component<BattleComponent::SkillDuration>())
    {
        config.duration = skillEntity.component<BattleComponent::SkillDuration>()->duration;
    }
    else
    {
        return false;
    }
    
    if (skillEntity.has_component<BattleComponent::SkillTileTarget>())
    {
        auto skillTileTarget = skillEntity.component<BattleComponent::SkillTileTarget>();
        config.tileResults.add(skillTileTarget->tileZ, skillTileTarget->tileX);
    }
    else if (skillEntity.has_component<BattleComponent::SkillTileResults>())
    {
        auto& tiles = skillEntity.component<BattleComponent::SkillTileResults>()->results.tiles;
        for (auto* pTile : tiles)
        {
            config.tileResults.add(pTile->tileZ, pTile->tileX);
        }
    }
    else
    {
        return false;
    }
    
    return true;
}
