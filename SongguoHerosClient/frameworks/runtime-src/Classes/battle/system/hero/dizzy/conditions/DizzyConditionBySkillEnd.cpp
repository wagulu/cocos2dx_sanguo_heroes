//
//  DizzyConditionBySkillEnd.cpp
//  sanguoClient
//
//  Created by fuchenhao on 5/28/15.
//
//

#include "DizzyConditionBySkillEnd.h"
#include "BattleWorld.h"

DizzyConditionBySkillEnd::~DizzyConditionBySkillEnd()
{
}

void DizzyConditionBySkillEnd::init(entityx::Entity& entity)
{
}

void DizzyConditionBySkillEnd::update(entityx::Entity& entity, float dt)
{
    if (!_ENTITY_MANAGER.valid(m_pDizzyData->skillEntityId))
    {
        m_complete = true;
    }
}
