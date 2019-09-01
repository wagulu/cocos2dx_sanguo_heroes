//
//  AnchorArrowCreator.cpp
//  sanguoClient
//
//  Created by fu.chenhao on 4/16/15.
//
//

#include "AnchorArrowCreator.h"
#include "BattleWorld.h"


#define ARROW_SPEED (GRID_WIDTH * 20)

AnchorArrowCreator::AnchorArrowCreator(BattleEntity* entities, BattleStage* stage)
{
    m_entities = entities;
    m_stage = stage;
}

AnchorArrowCreator::~AnchorArrowCreator()
{
}

bool AnchorArrowCreator::addArrow(entityx::Entity firer, int direction)
{
    BattleComponent::Position::Handle position = firer.component<BattleComponent::Position>();
    BattleComponent::Identify::Handle identify = firer.component<BattleComponent::Identify>();

    entityx::Entity entity = _ENTITY_MANAGER.create();
    entity.assign<BattleComponent::Position>(position->x, position->y, position->z);
    entity.assign<BattleComponent::Direction>(direction * ARROW_SPEED, 0.0, 0.0);
    entity.assign<BattleComponent::AnchorArrowData>(firer);
    

    auto animations = _BATTLE_ANIMATION_CREATOR.createSingle("attack", "animation_arrow", position->x, position->y, position->z, identify->side == BattleConfig::SIDE_LEFT);
    entity.assign<BattleComponent::Animation>(animations);
    
    return true;
}


