//
//  SkillTiaoJi.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/16/15.
//
//

#include "SkillTiaoJi.h"
#include "BattleWorld.h"
#include "StateSoldierChangeToGhost.h"

using namespace entityx;

void SkillTiaoJi::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);

    //find solider
    auto identify = _BATTLE_ENTITY.getEntity(m_fromId)->component<BattleComponent::Identify>();
    auto position = _BATTLE_ENTITY.getEntity(m_fromId)->component<BattleComponent::Position>();
    int attackDistance = (int)m_skillActionData->config["attackDistance"]->d;
    bool finded = false;
    
    int step = identify->side == BattleConfig::SIDE_LEFT ? 1 : -1;
    int startTileX = MapHelper::getTileX(position->x);
    int startTileZ = MapHelper::getTileZ(position->z);
    int tileTargetId;
    
    for(int i = 1; i <= attackDistance; i++)
    {
        tileTargetId = _BATTLE_ENTITY.getTarget(startTileZ, startTileX + i * step);
        
        if(tileTargetId && tileTargetId != identify->id && MapHelper::isHero(tileTargetId))
        {
            finded = true;
            
            BattleConfig::HeroDizzyConfig dizzyConfig = *m_skillData->pConfig->dizzy->getConfig(*m_skillActionData->config["dizzyConfig"]->s);
            
            auto* pDizzyActionSkillTiaoJi = dizzyConfig.findAction("skill_tiao_ji");
            if (pDizzyActionSkillTiaoJi != nullptr)
            {
                auto* targetEntity = _BATTLE_ENTITY.getEntity(tileTargetId);
                
                pDizzyActionSkillTiaoJi->config["targetX"] = BattleConfig::ParamData((double)(position->x + (float)(4 * step * GRID_WIDTH)));
                BattleConfig::HeroDizzyData dizzyData;
                dizzyData.skillEntityId = m_entityId;

                _ENTITY_EVENT.emit(BattleEvent::StrategyHeroDizzy(*targetEntity, dizzyData, dizzyConfig));
            }
            
            _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, "hitHero");
            
            break;
        }
    }
    
    //TO-DO if not find, try to find a soldier
    
    //kill soldier
    
    remove();
}
