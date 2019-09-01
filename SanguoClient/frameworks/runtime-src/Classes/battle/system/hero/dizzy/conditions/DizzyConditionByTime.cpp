//
//  DizzyConditionByTime.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/27/15.
//
//

#include "DizzyConditionByTime.h"

DizzyConditionByTime::~DizzyConditionByTime()
{
}

void DizzyConditionByTime::init(entityx::Entity& entity)
{
    m_dizzyTime = m_pDizzyData->dizzyTime;
}

void DizzyConditionByTime::update(entityx::Entity& entity, float dt)
{
    m_dizzyTime -= dt;
    if (m_dizzyTime <= 0)
    {
        m_complete = true;
    }
}
