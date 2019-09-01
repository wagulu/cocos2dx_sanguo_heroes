//
//  AICondition.cpp
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#include "AICondition.h"
#include "MapHelper.h"

AICondition::AICondition(AIConfig::AIConditionData* data, int heroId)
{
    m_data = data;
    m_heroId = heroId;
    m_side = MapHelper::getSide(heroId);
    
    m_count = cocos2d::random<int>(0, MAX_COUNT);
}

AICondition::~AICondition()
{
    
}

bool AICondition::shouldCheck()
{
    m_count++;
    if(m_count >= MAX_COUNT)
    {
        m_count -= MAX_COUNT;
        return true;
    }
    return false;
}
