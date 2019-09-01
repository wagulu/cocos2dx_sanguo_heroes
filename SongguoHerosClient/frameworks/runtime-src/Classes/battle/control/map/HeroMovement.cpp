//
//  HeroMovement.cpp
//  Game
//
//  Created by fuchenhao on 3/5/15.
//
//

#include "HeroMovement.h"
#include "AICommonHelper.h"

HeroMovement::HeroMovement(BattleEntity* entities, BattleStage* stage)
{
    m_entities = entities;
    m_stage = stage;
}

HeroMovement::~HeroMovement()
{
}

bool HeroMovement::move(float dt, entityx::Entity& entity, MoveIgnoreType ignoreType, float speed)
{
    if (speed == 0) speed = entity.component<BattleComponent::Property>()->get(BattleConfig::Property::SPEED);
    
    auto position = entity.component<BattleComponent::Position>();
    auto direction = entity.component<BattleComponent::Direction>();
    
    float x = position->x + dt * direction->x * speed;
    float y = position->y + dt * direction->y * speed;
    float z = position->z + dt * direction->z * speed;
    
    return moveTo(x, y, z, entity, ignoreType);
}

bool HeroMovement::move(float dt, float targetX, float targetY, float targetZ, entityx::Entity& entity, MoveIgnoreType ignoreType, float speed)
{
    static cocos2d::Vec3 m_tempCurrentPos;
    static cocos2d::Vec3 m_tempTargetPos;
    
    auto position = entity.component<BattleComponent::Position>();
    if (speed == 0) speed = entity.component<BattleComponent::Property>()->get(BattleConfig::Property::SPEED);
    
    m_tempCurrentPos.set(position->x, position->y, position->z);
    m_tempTargetPos.set(targetX, targetY, targetZ);
    float moveSquared = powf(speed * dt, 2);
    float distanceSquared = m_tempCurrentPos.distanceSquared(m_tempTargetPos);
    if (moveSquared >= distanceSquared)
    {
        return moveTo(targetX, targetY, targetZ, entity, ignoreType);
    }
    
    auto direction = entity.component<BattleComponent::Direction>();
    float x = position->x + dt * direction->x * speed;
    float y = position->y + dt * direction->y * speed;
    float z = position->z + dt * direction->z * speed;
    
    return moveTo(x, y, z, entity, ignoreType);
}

bool HeroMovement::moveTo(float x, float y, float z, entityx::Entity& entity, MoveIgnoreType ignoreType)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto position = entity.component<BattleComponent::Position>();
    
    bool block = false;
    
    if (MapHelper::outOfBounds(x, z))
    {
        block = true;
        return block;
    }
    
    int prevTileX = MapHelper::getTileX(position->x);
    int prevTileZ = MapHelper::getTileZ(position->z);
    int newTileX = MapHelper::getTileX(x);
    int newTileZ = MapHelper::getTileZ(z);
    
    if (prevTileX != newTileX || prevTileZ != newTileZ)
    {
        int newTileIdRR = m_entities->getTarget(newTileZ, newTileX + 1);
        int newTileIdR = m_entities->getTarget(newTileZ, newTileX);
        int newTileIdL = m_entities->getTarget(newTileZ, newTileX - 1);
        int newTileIdLL = m_entities->getTarget(newTileZ, newTileX - 2);
        
        bool isRSoldierR = MapHelper::getEntityType(newTileIdRR) == BattleConfig::ENTITY_SOLDIER;
        bool isLSoldierL = MapHelper::getEntityType(newTileIdLL) == BattleConfig::ENTITY_SOLDIER;
        
        bool isRSoldier = MapHelper::getEntityType(newTileIdR) == BattleConfig::ENTITY_SOLDIER;
        bool isLSoldier = MapHelper::getEntityType(newTileIdL) == BattleConfig::ENTITY_SOLDIER;
        bool isRSelf = MapHelper::getSide(newTileIdR) == identify->side;
        bool isLSelf = MapHelper::getSide(newTileIdL) == identify->side;
        bool ignoreR = ((ignoreType == MoveIgnoreType::IGNORE_ALL_SOLDIER && isRSoldier)
                        || (ignoreType == MoveIgnoreType::IGNORE_SELF_SOLDIER && isRSoldier && isRSelf)
                        || (ignoreType == MoveIgnoreType::IGNORE_ENEMY_SOLDIER && isRSoldier && !isRSelf));
        bool ignoreL = ((ignoreType == MoveIgnoreType::IGNORE_ALL_SOLDIER && isLSoldier)
                        || (ignoreType == MoveIgnoreType::IGNORE_SELF_SOLDIER && isLSoldier && isLSelf)
                        || (ignoreType == MoveIgnoreType::IGNORE_ENEMY_SOLDIER && isLSoldier && !isLSelf));
        
// ==== force BLOCK
        bool typeBlock = false;
        if (isRSoldierR && MapHelper::getSide(newTileIdRR) == identify->side)
        {
            auto* pTargetEntityR2 = _BATTLE_ENTITY.getEntity(newTileIdRR);
            if (pTargetEntityR2 != NULL)
            {
                BattleComponent::Soldier::Handle soldierR2 = pTargetEntityR2->component<BattleComponent::Soldier>();
                typeBlock = (soldierR2->config.soldierType == BattleConfig::SoldierType::BLOCK);
            }
        }
        
        if (isLSoldierL && !typeBlock && MapHelper::getSide(newTileIdLL) == identify->side)
        {
            auto* pTargetEntityL2 = _BATTLE_ENTITY.getEntity(newTileIdLL);
            if (pTargetEntityL2 != NULL)
            {
                BattleComponent::Soldier::Handle soldierL2 = pTargetEntityL2->component<BattleComponent::Soldier>();
                typeBlock = (soldierL2->config.soldierType == BattleConfig::SoldierType::BLOCK);
            }
        }

        if (isRSoldier && !typeBlock)
        {
            auto* pTargetEntityR = _BATTLE_ENTITY.getEntity(newTileIdR);
            if (pTargetEntityR != NULL)
            {
                BattleComponent::Soldier::Handle soldierR = pTargetEntityR->component<BattleComponent::Soldier>();
                typeBlock = (soldierR->config.soldierType == BattleConfig::SoldierType::BLOCK);
            }
        }
        if (isLSoldier && !typeBlock)
        {
            auto* pTargetEntityL = _BATTLE_ENTITY.getEntity(newTileIdL);
            if (pTargetEntityL != NULL)
            {
                BattleComponent::Soldier::Handle soldierL = pTargetEntityL->component<BattleComponent::Soldier>();
                typeBlock = (soldierL->config.soldierType == BattleConfig::SoldierType::BLOCK);
            }
        }
        
        if (m_stage->getBattleType() == BattleConfig::BattleType::DUEL)
        {
            if (prevTileX < newTileX)
            {
                float rightBoundX = (identify->side == BattleConfig::SIDE_LEFT ? STAGE_DUEL_RIGHT_X - 2 : STAGE_DUEL_RIGHT_X);
                if (newTileX > rightBoundX) block = true;
            }
            else if (prevTileX > newTileX)
            {
                float leftBoundX = (identify->side == BattleConfig::SIDE_LEFT ? STAGE_DUEL_LEFT_X : STAGE_DUEL_LEFT_X + 2);
                if (newTileX < leftBoundX) block = true;
            }
        }
        
        if (!block
            && !typeBlock
            && (newTileIdR == 0 || newTileIdR == identify->id || ignoreR) && (newTileIdL == 0 || newTileIdL == identify->id || ignoreL))
        {
            m_entities->changeHeroTarget(prevTileZ, prevTileX, newTileZ, newTileX, identify->id);
            
            updateMiniMapIcon(m_stage, entity, newTileX, newTileZ);
        }
        else
        {
            block = true;
        }
    }
    
    if (!block)
    {
        position->set(x, y, z);
        entity.component<BattleComponent::Animation>()->battleAnimation->setPosition(x, y, z);
    }
    
    return block;
}

bool HeroMovement::canAttack(entityx::Entity& entity, entityx::Entity& targetEntity)
{
    auto targetIdentify = targetEntity.component<BattleComponent::Identify>();
    auto targetPos = targetEntity.component<BattleComponent::Position>();
    auto myPos = entity.component<BattleComponent::Position>();
    auto myIdentify = entity.component<BattleComponent::Identify>();
    
    int heroTileZ = MapHelper::getTileZ(myPos->z);
    int heroTileX = MapHelper::getTileX(myPos->x);
    int targetTileZ = MapHelper::getTileZ(targetPos->z);
    int targetTileX = MapHelper::getTileX(targetPos->x);
    
    // hero attack soldier range, 0 = empty, . = soldier, * = hero
    // 0..0
    // .**.
    // 0..0
    //
    // hero attack hero range, 0 = empty, * = hero, . = enemy
    //            0000..              ..0000
    // faceRight  00**.. or faceLeft  ..**00
    //            0000..              ..0000
    if (targetIdentify->type == BattleConfig::ENTITY_SOLDIER)
    {
        if (heroTileZ == targetTileZ && (targetTileX == heroTileX + 1 || targetTileX == heroTileX - 2)) return true;
        if(abs(heroTileZ - targetTileZ) == 1 && (targetTileX == heroTileX || targetTileX == heroTileX - 1)) return true;
    }
    else if (targetIdentify->type == BattleConfig::ENTITY_HERO)
    {
        if (abs(heroTileZ - targetTileZ) <= 1)
        {
            if (myIdentify->faceTo == BattleConfig::FACE_TO_RIGHT
                && (targetTileX == heroTileX + 2 /*|| targetTileX == heroTileX + 3*/)) return true;
            if (myIdentify->faceTo == BattleConfig::FACE_TO_LEFT
                && (targetTileX == heroTileX - 2 /*|| targetTileX == heroTileX - 3*/)) return true;
        }
    }
    return false;
}

bool HeroMovement::moveApproachAttackRange(float dt, entityx::Entity& entity, entityx::Entity& targetEntity)
{
    auto myPos = entity.component<BattleComponent::Position>();
    auto myDir = entity.component<BattleComponent::Direction>();
    auto targetPos = targetEntity.component<BattleComponent::Position>();
    auto targetIdentify = targetEntity.component<BattleComponent::Identify>();
    
    int myTileZ = MapHelper::getTileZ(myPos->z);
    int targetTileZ = MapHelper::getTileZ(targetPos->z);
    
    float x;
    float y = 0;
    float z = myPos->z;//武将不主动移动z轴
    
    bool arrive = false;
    auto moveIgnoreType = MoveIgnoreType::IGNORE_SELF_SOLDIER;
    
    if (targetIdentify->type == BattleConfig::ENTITY_SOLDIER)
    {
        auto myIdentify = entity.component<BattleComponent::Identify>();
        // 武将不会靠近或远离身后的士兵，也不会靠近或远离侧面的士兵
        if ((myIdentify->faceTo == BattleConfig::FaceTo::FACE_TO_LEFT && myPos->x < targetPos->x)
            || (myIdentify->faceTo == BattleConfig::FaceTo::FACE_TO_RIGHT && myPos->x > targetPos->x)
            || (myTileZ != targetTileZ))
        {
            arrive = true;
        }
        else
        {
            if (myPos->x < targetPos->x)
            {
                x = targetPos->x - ATTACK_DISTANCE_HERO_MELEE;
                int tileX = MapHelper::getTileX(x);
                int tileXLeft = MapHelper::getTileX(targetPos->x) - 1;
                if (tileX < tileXLeft) x = tileXLeft * GRID_WIDTH;
                else if (tileX > tileXLeft) x = (tileXLeft + 1) * GRID_WIDTH - 1;
            }
            else
            {
                x = targetPos->x + ATTACK_DISTANCE_HERO_MELEE;
                int tileX = MapHelper::getTileX(x);
                int tileXRight = MapHelper::getTileX(targetPos->x) + 2;
                if (tileX < tileXRight) x = tileXRight * GRID_WIDTH;
                else if (tileX > tileXRight) x = (tileXRight + 1) * GRID_WIDTH - 1;
            }
        }
    }
    else
    {
        moveIgnoreType = MoveIgnoreType::IGNORE_ALL_SOLDIER;
        
        auto targetHero = targetEntity.component<BattleComponent::General>();
        if (targetHero->target == 0 || MapHelper::isSoldier(targetHero->target))
        {
            arrive = true;
        }
        else
        {
            float distance = 0;
            int tileDistance = 0;
            // 如果敌将当前的目标是我，则主动上前
            if (targetHero->target == entity.component<BattleComponent::Identify>()->id)
            {
                distance = ATTACK_DISTANCE_HERO_HERO_NEAR;
                tileDistance = 2;
            }
            else
            {
                auto* pFriendEntity = _BATTLE_ENTITY.getEntity(targetHero->target);
                // 如果友方目标死亡，则主动上前
                if (pFriendEntity == nullptr)
                {
                    distance = ATTACK_DISTANCE_HERO_HERO_NEAR;
                    tileDistance = 2;
                }
                else
                {
                    float friendStandOrder = pFriendEntity->component<BattleComponent::General>()->config.standOrder;
                    float myStandOrder = entity.component<BattleComponent::General>()->config.standOrder;
                    // 如果我的站位排序更大，则主动上前
                    if (myStandOrder > friendStandOrder)
                    {
                        distance = ATTACK_DISTANCE_HERO_HERO_NEAR;
                        tileDistance = 2;
                    }
                    // 如果我的站位排序较小，则主动退后，让友方将领承担伤害
                    else
                    {
                        distance = ATTACK_DISTANCE_HERO_HERO_FAR;
                        tileDistance = 3;
                    }
                }
            }
            
            if (myPos->x < targetPos->x)
            {
                x = targetPos->x - distance;
                int tileX = MapHelper::getTileX(x);
                int tileXLeft = MapHelper::getTileX(targetPos->x) - tileDistance;
                if (tileX < tileXLeft) x = tileXLeft * GRID_WIDTH;
                else if (tileX > tileXLeft) x = (tileXLeft + 1) * GRID_WIDTH - 1;
            }
            else
            {
                x = targetPos->x + distance;
                int tileX = MapHelper::getTileX(x);
                int tileXRight = MapHelper::getTileX(targetPos->x) + tileDistance;
                if (tileX < tileXRight) x = tileXRight * GRID_WIDTH;
                else if (tileX > tileXRight) x = (tileXRight + 1) * GRID_WIDTH - 1;
            }
        }
    }
    
    if (!arrive)
    {
        _SIMPLE_MOVEMENT.updateDirection(myDir, myPos, x, y, z);
        
        bool block = move(dt, x, y, z, entity, moveIgnoreType);
        if (block || myPos->x == x)
        {
            arrive = true;
        }
    }
    
    return arrive;
}
