//
//  CancelSkill.cpp
//  sanguoGame
//
//  Created by fuchenhao on 7/28/15.
//
//

#include "CancelSkill.h"
#include "BattleWorld.h"

void CancelSkill::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    
    auto* pEntity = _BATTLE_ENTITY.getEntity(m_fromId);
    if (pEntity)
    {
        BattleEvent::CancelSkill event(*pEntity);
        event.forceCancel = true;
        _ENTITY_EVENT.emit(event);
    }
    
    remove();
}
