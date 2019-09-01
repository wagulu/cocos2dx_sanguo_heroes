//
//  AnchorArrowSystem.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 4/16/15.
//
//

#include "AnchorArrowSystem.h"
#include "StateSoldierAttackEnemy.h"
#include "BattleComponent.h"
#include "ActorMode.h"
#include "AICommonHelper.h"
#include "BattleWorld.h"
#include "BattleEntity.h"
#include "MapHelper.h"

void AnchorArrowSystem::configure(entityx::EventManager& events)
{
    
}

void AnchorArrowSystem::update(entityx::EntityManager& es, entityx::EventManager& events, double dt)
{
    BattleComponent::Position::Handle position;
    BattleComponent::AnchorArrowData::Handle data;
    BattleComponent::Direction::Handle direction;
    BattleComponent::Animation::Handle animations;

    for (entityx::Entity entity : es.entities_with_components(data, position, direction, animations))
    {
        switch (data->state)
        {
            case BattleComponent::AnchorArrowData::STATE::WORK:
            {
                auto firer = data->m_firer;
                if(!firer.valid())
                {
                    data->state = BattleComponent::AnchorArrowData::STATE::DEAD;
                    continue;
                }

                float _x = position->x;
                float dx = direction->x * dt;
                float nx = _x + dx;
                position->x = nx;
                animations->battleAnimation->setPositionX(nx);
                
                entityx::Entity* pEnemy = checkHurtEnemy(firer, _x, position->x, position->z);
                if(pEnemy)
                {
                    doAttack(firer, *pEnemy);
                    
                    data->state = BattleComponent::AnchorArrowData::STATE::DEAD;
                    continue;
                }

                int tileX = MapHelper::getTileX(position->x);
                int tileZ = MapHelper::getTileX(position->z);
                if(!MapHelper::validTile(tileZ, tileX))
                {
                    data->state = BattleComponent::AnchorArrowData::STATE::DEAD;
                    continue;
                }
                
            }
                break;
                
            case BattleComponent::AnchorArrowData::STATE::DEAD:
            {
                entity.destroy();
            }
                break;
                
            default:
                break;
        }
    }
    
}


entityx::Entity* AnchorArrowSystem::checkHurtEnemy(entityx::Entity firer, float ox, float cx, float z)
{
    BattleComponent::Identify::Handle identify = firer.component<BattleComponent::Identify>();

    int oTileX = MapHelper::getTileX(ox);
    int cTileX = MapHelper::getTileX(cx);
    int tileZ = MapHelper::getTileZ(z);
    int stepX = (cx > ox)? 1: -1;

    bool loop = true;
    while (loop)
    {
        if(MapHelper::validTile(tileZ, oTileX))
        {
            auto targetId = _BATTLE_ENTITY.getTarget(tileZ, oTileX);
            auto targetEntity = _BATTLE_ENTITY.getEntity(targetId);
            if(targetEntity != NULL)
            {
                auto targetIdentify = targetEntity->component<BattleComponent::Identify>();
                if(targetIdentify->side != identify->side)
                {
                    return targetEntity;
                }
            }
            
        }

        if(oTileX == cTileX)
        {
            loop = false;
        }
        else
        {
            oTileX += stepX;
        }
    }

    return NULL;
}

void AnchorArrowSystem::doAttack(entityx::Entity& firer, entityx::Entity& target)
{
    auto identify = firer.component<BattleComponent::Identify>();
//    auto soldier = entity.component<BattleComponent::Soldier>();
    auto position = firer.component<BattleComponent::Position>();

    auto targetIdentify = target.component<BattleComponent::Identify>();

    BattleConfig::AttackData attackData(identify->id, 0);
    attackData.setTargetId(targetIdentify->id);
    attackData.damagePosition.x = position->x;
    attackData.damagePosition.y = position->y;
    attackData.damagePosition.z = position->z;
    attackData.damagePosition.speedH = SOLDIER_ATTACK_SPEED_H;
    attackData.damagePosition.speedV = SOLDIER_ATTACK_SPEED_V;
    attackData.damagePosition.gravity = SOLDIER_ATTACK_GRAVITY;
    
    _ENTITY_EVENT.emit<BattleEvent::AddAttack>(attackData);
    
}

