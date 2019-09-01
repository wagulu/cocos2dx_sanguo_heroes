//
//  SimpleMovement.cpp
//  Game
//
//  Created by fuchenhao on 3/13/15.
//
//

#include "SimpleMovement.h"

SimpleMovement::SimpleMovement()
{
    
}

SimpleMovement::~SimpleMovement()
{
}

void SimpleMovement::updateDirection(BattleComponent::Direction::Handle& direction, const BattleComponent::Position::Handle& currentPos, const BattleComponent::Position::Handle& targetPos)
{
   updateDirection(direction, currentPos, targetPos->x, targetPos->y, targetPos->z);
}

void SimpleMovement::updateDirection(BattleComponent::Direction::Handle& direction, const BattleComponent::Position::Handle& currentPos, const BattleComponent::Target::Handle& targetPos)
{
    updateDirection(direction, currentPos, targetPos->x, targetPos->y, targetPos->z);
}

void SimpleMovement::updateDirection(BattleComponent::Direction::Handle& direction, const BattleComponent::Position::Handle& currentPos, float targetX, float targetY, float targetZ)
{
    m_tempCurrentPos.set(currentPos->x, currentPos->y, currentPos->z);
    m_tempTargetPos.set(targetX, targetY, targetZ);
    m_tempDirection.set(m_tempCurrentPos, m_tempTargetPos);
    
    m_tempDirection.normalize();
    
    direction->x = m_tempDirection.x;
    direction->y = m_tempDirection.y;
    direction->z = m_tempDirection.z;
}

bool SimpleMovement::move(BattleComponent::Direction::Handle& direction, BattleComponent::Position::Handle& position, float dt, float speed)
{
    m_tempX = position->x + dt * direction->x * speed;
    m_tempY = position->y + dt * direction->y * speed;
    m_tempZ = position->z + dt * direction->z * speed;
    
    if (!MapHelper::outOfBounds(m_tempX, m_tempZ))
    {
        position->x = m_tempX;
        position->y = m_tempY;
        position->z = m_tempZ;
        return true;
    }
    return false;
}

bool SimpleMovement::move(BattleComponent::Direction::Handle& direction, BattleComponent::Position::Handle& position, float dt, float speed, float targetX, float targetY, float targetZ)
{
    m_tempCurrentPos.set(position->x, position->y, position->z);
    m_tempTargetPos.set(targetX, targetY, targetZ);
    float moveSquared = powf(speed * dt, 2);
    float distanceSquared = m_tempCurrentPos.distanceSquared(m_tempTargetPos);
    if (moveSquared >= distanceSquared)
    {
        position->x = targetX;
        position->y = targetY;
        position->z = targetZ;
        return true;
    }
    
    return move(direction, position, dt, speed);
}
