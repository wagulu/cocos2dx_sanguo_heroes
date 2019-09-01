//
//  TriggerCameraActions.cpp
//  sanguoGame
//
//  Created by fuchenhao on 8/6/15.
//
//

#include "TriggerCameraActions.h"
#include "BattleWorld.h"


void TriggerCameraActions::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    _ENTER_SHOW_CONTROL.triggerCameraActions(this->m_skillData->pConfig->cameraActions->getCameraActions(*m_skillActionData->config["action"]->s), m_entityId.id());
    
    remove();
}
