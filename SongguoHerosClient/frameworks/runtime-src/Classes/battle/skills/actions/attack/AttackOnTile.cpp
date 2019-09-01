//
//  AttackOnTile.cpp
//  SGGame
//
//  Created by fuchenhao on 4/27/15.
//
//

#include "AttackOnTile.h"
#include "BattleWorld.h"

using namespace entityx;

AttackOnTile::~AttackOnTile()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::SkillAttackOnTileEvent>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::SkillAttackOnTileEvent>(*this);
    }
}

void AttackOnTile::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    _ENTITY_EVENT.subscribe<BattleEvent::SkillAttackOnTileEvent>(*this);
}

void AttackOnTile::receive(const BattleEvent::SkillAttackOnTileEvent& e)
{
    BattleConfig::AttackData attackData(m_fromId, 0);
    attackData.setTargetPos(e.tileZ, e.tileX);
    attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
    attackData.damagePosition.x = GRID_WIDTH * e.tileX;
    attackData.damagePosition.y = 0;
    attackData.damagePosition.z = 780;
    
    attackData.damageData.damageType = e.damageType;
    attackData.damageData.damageEntityType = e.damageEntityType;
    
    if (m_skillActionData->damageData.deadAnimation)
    {
        attackData.damagePosition.group = m_effect->getEffect(*m_skillActionData->damageData.deadAnimation);
    }
    
    if (e.damageEntityType == BattleConfig::DAMAGE_ENTITY_HERO)
    {
        if (m_skillActionData->config["hit_hero"])
        {
            auto* pEffectGroup = m_effect->getEffect(*m_skillActionData->config["hit_hero"]->s);
            if (pEffectGroup != nullptr) {
                bool reserse = m_fromId == LEFT_HERO ? false : true;
                float effectX = m_fromId == LEFT_HERO ? 140 : -140;
                _ENTITY_EVENT.emit<BattleEvent::AddEffect>(pEffectGroup, reserse, attackData.damagePosition.x + effectX, attackData.damagePosition.y, GRID_HEIGHT * e.tileZ, 0.0f, 0.0f);
                
    //            BattleConfig::CameraShakeData shakeData(0.5f, 10.0f, 5, 2, 0.0f);
    //            _ENTITY_EVENT.emit<BattleEvent::CameraShakeEvent>(shakeData);
            }
        }
    }
    
    attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH;
    attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV;
    attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;

    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
}
