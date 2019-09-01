//
//  StateHeroDash.cpp
//  SGGame
//
//  Created by fuchenhao on 15-4-28.
//
//

#include "StateHeroDash.h"
#include "HeroSystem.h"
#include "../../AICommonHelper.h"


StateHeroDash::StateHeroDash(const std::string& animation, float duration, float speed) : m_animation(animation), m_duration(duration), m_speed(speed)
{

}

StateHeroDash::~StateHeroDash()
{
    if (m_moveTween != nullptr)
    {
        CC_SAFE_DELETE(m_moveTween);
    }
}

void StateHeroDash::enter(entityx::Entity& entity)
{
    m_pushHero = false;
    
    auto position = entity.component<BattleComponent::Position>();
    
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->playAnimation(m_animation, false, false);
    animation->battleAnimation->setBiasZ(1);
    animation->battleAnimation->setPositionZ(position->z);
    
    m_moveTween = new SimpleTween_linear(m_duration, m_speed, 0.0f);
}

void StateHeroDash::exit(entityx::Entity& entity)
{
    auto position = entity.component<BattleComponent::Position>();
    
    auto animation = entity.component<BattleComponent::Animation>();
    animation->battleAnimation->setBiasZ(0);
    animation->battleAnimation->setPositionZ(position->z);
    
    _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(position->x,
                                                               CameraManager::INIT_CAMERA_Z, CameraManager::INIT_CAMERA_Y,
                                                               CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FREE,
                                                               CameraManager::INIT_CAMERA_ROTATE, 0, false);
}

int StateHeroDash::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    m_elapse += dt;
    
    if (m_pushHero)
    {
        // my hero movement
//        CCLOG("============pushHero my hero movement");
        auto identify = entity.component<BattleComponent::Identify>();
        auto direction = entity.component<BattleComponent::Direction>();
        direction->x = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -1);
        
        direction->y = 0;
        direction->z = 0;
        
        float ratio = m_moveTween->out(dt);
        if (ratio <= 0.0001) ratio = 1;
        direction->x *= ratio;
        _HERO_MOVEMENT.move(dt, entity, HeroMovement::IGNORE_ALL_SOLDIER);
        
        // enemy hero movement
//        CCLOG("============pushHero enemy hero movement");
        auto* pTargetEntity = _BATTLE_ENTITY.getEntity(identify->id == RIGHT_HERO ? LEFT_HERO : RIGHT_HERO);
        if (pTargetEntity != nullptr)
        {
            auto positionT = pTargetEntity->component<BattleComponent::Position>();
            auto directionT = pTargetEntity->component<BattleComponent::Direction>();
            directionT->x = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -1);
            
            directionT->y = 0;
            directionT->z = 0;
            if (ratio <= 0.0001) ratio = 1;
            directionT->x *= ratio;
            _HERO_MOVEMENT.move(dt, *pTargetEntity);
        
            // attack enemy soldier behind hero
            int tileXE = MapHelper::getTileX(positionT->x);
            int tileZE = MapHelper::getTileX(positionT->z);
            
            int targetId = _BATTLE_ENTITY.getTarget(tileZE, tileXE + (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -2));
            
            if (targetId != 0 && !MapHelper::isHero(targetId))
            {
//                CCLOG("============= AttackData AttackData");
                _ENTITY_EVENT.emit<BattleEvent::SkillAttackOnTileEvent>(tileXE + 1, tileZE, BattleConfig::DAMAGE_KILL, BattleConfig::DAMAGE_ENTITY_SOLDIER);
            }
        }
    }
    
    else
    {
        if (_TARGET_FINDER.findHeroFrontNearbyTarget(entity) > 0)
        {
            auto identify = entity.component<BattleComponent::Identify>();
            auto position = entity.component<BattleComponent::Position>();
            int tileX = MapHelper::getTileX(position->x);
            int tileZ = MapHelper::getTileX(position->z);
            
            for (int i = 1; i < 3; i++)
            {
                int directionX = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -1);
                int tileXBias = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 0 : -1);
                int targetId = _BATTLE_ENTITY.getTarget(tileZ, tileX + tileXBias + i * directionX);
                
                if(MapHelper::getEntityType(targetId) == BattleConfig::ENTITY_HERO)
                {
                    if (targetId != 0)
                    {
                        _ENTITY_EVENT.emit<BattleEvent::SkillAttackOnTileEvent>(tileX + tileXBias + i * directionX, tileZ, BattleConfig::DAMAGE_PHYSICAL, BattleConfig::DAMAGE_ENTITY_HERO);

                        m_pushHero = true;
                        break;
                    }
                }
                else
                {
                    _ENTITY_EVENT.emit<BattleEvent::SkillAttackOnTileEvent>(tileX + tileXBias + 1 * directionX, tileZ, BattleConfig::DAMAGE_KILL, BattleConfig::DAMAGE_ENTITY_SOLDIER);
                    break;
                }
            }
        }
        else
        {
            auto identify = entity.component<BattleComponent::Identify>();
            auto direction = entity.component<BattleComponent::Direction>();
            direction->x = (identify->faceTo == BattleConfig::FACE_TO_RIGHT ? 1 : -1);
            
            direction->y = 0;
            direction->z = 0;
            
            float ratio = m_moveTween->out(dt);
            if (ratio <= 0.0001) ratio = 1;
            direction->x *= ratio;
            
            _HERO_MOVEMENT.move(dt, entity);
        }
    }
    
    if (m_elapse >= m_duration)
    {
        m_result = OVER;
    }

    return m_result;
}

