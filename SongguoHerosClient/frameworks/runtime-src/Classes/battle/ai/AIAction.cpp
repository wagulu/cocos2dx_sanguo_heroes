//
//  AIAction.cpp
//  Game
//
//  Created by fuchenhao on 3/10/15.
//
//

#include "AIAction.h"
#include "MapHelper.h"

AIAction::AIAction(AIConfig::AIActionData* data, int heroId)
{
    m_data = data;
    m_heroId = heroId;
    m_side = MapHelper::getSide(heroId);
}

AIAction::~AIAction()
{
    
}
