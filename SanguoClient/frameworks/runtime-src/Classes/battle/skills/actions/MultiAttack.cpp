//
//  MultiAttack.cpp
//  Game
//
//  Created by zhaikaiyu on 3/5/15.
//
//

#include "MultiAttack.h"
#include "BattleWorld.h"

void MultiAttack::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    entityx::Entity* e = _BATTLE_ENTITY.getEntity(m_fromId);
    if (e)
    {
        BattleComponent::General::Handle hero = e->component<BattleComponent::General>();
        BattleComponent::Position::Handle position = e->component<BattleComponent::Position>();
        BattleComponent::Identify::Handle identify = e->component<BattleComponent::Identify>();
        
        int times=m_skillActionData->config["times"]->d;//to do 根据技能判断
        float time =m_skillActionData->config["time"]->d;//to do 根据技能判断
        int type =m_skillActionData->config["type"]->d;//to do 根据技能判断
        
        BattleConfig::AttackData attackData(m_fromId, 0, times, time);
        attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
        
        attackData.damagePosition.x = position->x;
        attackData.damagePosition.y = 0;
        attackData.damagePosition.z = position->z;
        
        attackData.damagePosition.speedH = m_skillActionData->damageData.deadSpeedH;
        attackData.damagePosition.speedV = m_skillActionData->damageData.deadSpeddV;
        attackData.damagePosition.gravity = m_skillActionData->damageData.gravity;
        
        if (type == MultiAttackType::MULTI_ATTACK_OPP_HERO)
        {
            //TODO, lrui, multi general
            if (identify->side == BattleConfig::SIDE_LEFT)
            {
                attackData.setTargetId(RIGHT_HERO);
            }
            else
            {
                attackData.setTargetId(LEFT_HERO);
            }
            _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
        }
        else if (type == MultiAttackType::MULTI_ATTACK_SELF_HERO)
        {
            //TODO, lrui, multi general
            if (identify->side == BattleConfig::SIDE_LEFT)
            {
                attackData.setTargetId(LEFT_HERO);
            }
            else
            {
                attackData.setTargetId(RIGHT_HERO);
            }
            _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
        }
        else
        {
            if (_TARGET_FINDER.findHeroFrontNearbyTarget(*e) > 0)
            {
                attackData.setTargetId(hero->target);
                attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
                _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
            }
        }
    }
    
    remove();
}
