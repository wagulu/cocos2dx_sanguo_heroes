//
//  SimpleMovement.h
//  Game
//
//  Created by fuchenhao on 3/13/15.
//
//

#ifndef __Game__SimpleMovement__
#define __Game__SimpleMovement__

#include "BattleEntity.h"
#include "../../BattleStage.h"
#include "MapHelper.h"

class SimpleMovement
{
public:
    
    SimpleMovement();
    ~SimpleMovement();
    
    void updateDirection(BattleComponent::Direction::Handle& direction, const BattleComponent::Position::Handle& currentPos, const BattleComponent::Position::Handle& targetPos);
    void updateDirection(BattleComponent::Direction::Handle& direction, const BattleComponent::Position::Handle& currentPos, const BattleComponent::Target::Handle& targetPos);
    void updateDirection(BattleComponent::Direction::Handle& direction, const BattleComponent::Position::Handle& currentPos, float targetX, float targetY, float targetZ);
    bool move(BattleComponent::Direction::Handle& direction, BattleComponent::Position::Handle& position, float dt, float speed);
    bool move(BattleComponent::Direction::Handle& direction, BattleComponent::Position::Handle& position, float dt, float speed, float targetX, float targetY, float targetZ);
    
private:
    cocos2d::Vec3 m_tempDirection;
    cocos2d::Vec3 m_tempCurrentPos;
    cocos2d::Vec3 m_tempTargetPos;
    float m_tempX;
    float m_tempY;
    float m_tempZ;
};

#endif /* defined(__Game__SimpleMovement__) */
