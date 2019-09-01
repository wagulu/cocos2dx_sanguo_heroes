//
//  DisableBreak.cpp
//  sanguoClient
//
//  Created by fuchenhao on 6/2/15.
//
//

#include "DisableBreak.h"

void DisableBreak::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    this->m_skillData->canBreak = false;
    
    remove();
}
