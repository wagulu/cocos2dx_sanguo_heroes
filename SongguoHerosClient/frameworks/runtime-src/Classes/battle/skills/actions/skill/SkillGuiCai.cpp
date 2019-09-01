//
//  SkillGuiCai.cpp
//  SGGame
//
//  Created by fuchenhao on 4/27/15.
//
//

#include "SkillGuiCai.h"
#include "BattleWorld.h"

#include "StateSoldierDead.h"
#include "StateSoldierLost.h"

using namespace entityx;

SkillGuiCai::~SkillGuiCai()
{
    if (_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
}

void SkillGuiCai::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    m_damageRange = m_skillActionData->config["range"]->d;
    
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}

void SkillGuiCai::killSoldierAtOnce(entityx::Entity entity)
{
    auto objectStateMachine = entity.component<BattleComponent::ObjectStateMachine>();
    if (objectStateMachine->getCurrentStateCode() != BattleConfig::SoldierState::STATE_DEAD)
    {
        objectStateMachine->changeState(new StateSoldierDead());
    }
}

void SkillGuiCai::receive(const BattleEvent::HurtByAttack& e)
{
    auto identify = e.targetEntity.component<BattleComponent::Identify>();
    if (identify->side != MapHelper::getSide(m_fromId)) return;
    if (identify->type != BattleConfig::ENTITY_SOLDIER) return;
    
    auto property = e.targetEntity.component<BattleComponent::Property>();
    if (property->get(BattleConfig::Property::HP) > 0) return;
    
    float ratio = caclFormulaResult(*m_skillActionData->config["ratio"]->s) * 100;
    float diceValue = cocos2d::random<int>(1, 100);
    if (diceValue > ratio) return;
    
    killSoldierAtOnce(e.targetEntity);
        
    auto position = e.targetEntity.component<BattleComponent::Position>();
    int centerZ = MapHelper::getTileZ(position->z);
    int centerX = MapHelper::getTileX(position->x);

    BattleConfig::TileResults results;
    _TARGET_FINDER.findRectAreaTilesCenterOnTile(m_damageRange, centerZ, centerX, results);

    auto* group = m_effect->getEffect(*m_skillActionData->config["effect"]->s);
    bool reserse = m_side != BattleConfig::SIDE_LEFT;
    _ENTITY_EVENT.emit<BattleEvent::AddEffect>(group, reserse, position->x, position->y, position->z, 0.0f, 0.0f);
    
    int deltaX;
    int deltaZ;
    int distance;
    
    float damage = caclFormulaResult(*m_skillActionData->config["hurt"]->s);
    
    for (auto* pTile : results.tiles)
    {
        deltaX = abs(pTile->tileX - centerX);
        deltaZ = abs(pTile->tileZ - centerZ);
        distance = max(deltaX, deltaZ) - 1;
        
        BattleConfig::AttackData attackData(m_fromId, 0.4);
        attackData.setTargetPos(pTile->tileZ, pTile->tileX);
        attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
        attackData.damageData.damageHoly = damage;
        attackData.damagePosition.x = position->x;
        attackData.damagePosition.y = 0;
        attackData.damagePosition.z = position->z;
        attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH - distance * m_skillActionData->damageData.deadSpeedHDecay;
        attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV - distance * m_skillActionData->damageData.deadSpeedVDecay;
        attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
        
        _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    }
}
