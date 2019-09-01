//
//  StateSoldierHitBack.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/29/15.
//
//

#include "StateSoldierHitBack.h"
#include "../../AICommonHelper.h"

StateSoldierHitBack::StateSoldierHitBack(float duration)
{
    m_totalTime = duration;
}

StateSoldierHitBack::~StateSoldierHitBack()
{
    
}

void StateSoldierHitBack::enter(entityx::Entity& entity)
{
    auto identify = entity.component<BattleComponent::Identify>();
    auto direction = entity.component<BattleComponent::Direction>();
    
    if (identify->side == BattleConfig::SIDE_LEFT)
    {
        direction->x = -1;
        direction->z = 0;
        identify->faceTo = BattleConfig::FACE_TO_RIGHT;
    }
    else
    {
        direction->x = 1;
        direction->z = 0;
        identify->faceTo = BattleConfig::FACE_TO_LEFT;
    }
    
    BattleComponent::Animation::Handle animation = entity.component<BattleComponent::Animation>();
    ensureAnimation(animation->battleAnimation, BattleConfig::ANI_DEFENCE);
}

void StateSoldierHitBack::exit(entityx::Entity& entity)
{
    
}

int StateSoldierHitBack::update(entityx::Entity& entity, double dt)
{
    if(m_result != WORKING)
    {
        return m_result;
    }
    
    m_elapsedTime += dt;
    if(m_elapsedTime >= m_totalTime) m_result = DONE;
    
    _SOLDIER_MOVEMENT.move2(dt, entity);

    return m_result;
}


