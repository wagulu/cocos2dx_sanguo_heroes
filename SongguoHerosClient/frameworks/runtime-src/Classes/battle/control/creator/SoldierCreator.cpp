//
//  SoldierCreator.cpp
//  Game
//
//  Created by fuchenhao on 3/9/15.
//
//

#include "SoldierCreator.h"
#include "BattleWorld.h"


SoldierCreator::SoldierCreator(BattleEntity* entities, BattleStage* stage)
{
    m_entities = entities;
    m_stage = stage;
}

SoldierCreator::~SoldierCreator()
{
}

int SoldierCreator::addSoldier(BattleConfig::Side side, BattleConfig::SoldierConfig& config, int tileZ, int tileX, float delayShow, bool turnFace, int initHp)
{
    if(m_entities->isEmpty(tileZ, tileX))
    {
        int id = m_entities->getSoldierId(side);
        float x = tileX * GRID_WIDTH + HALF_GRID_WIDTH;
        float y = 0;
        float z = tileZ * GRID_HEIGHT + HALF_GRID_HEIGHT;
        
        entityx::Entity entity = _ENTITY_MANAGER.create();
        entity.assign<BattleComponent::Position>(x, y, z);
        entity.assign<BattleComponent::Direction>(0.0, 0.0, 0.0);
        entity.assign<BattleComponent::Target>(0.0, 0.0, 0.0);
     
        if (turnFace)
        {
            entity.assign<BattleComponent::Identify>(id, BattleConfig::ENTITY_SOLDIER, side, (side == BattleConfig::SIDE_RIGHT ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT));
        }
        else
        {
            entity.assign<BattleComponent::Identify>(id, BattleConfig::ENTITY_SOLDIER, side, (side == BattleConfig::SIDE_LEFT ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT));
        }
        
        entity.assign<BattleComponent::ObjectStateMachine>(entity);
        entity.assign<BattleComponent::AI>();
//        entity.assign<BattleComponent::ActorMode>(entity);
        entity.assign<BattleComponent::Combo>();
        
        cocos2d::Sprite* icon;
        if(side == BattleConfig::SIDE_LEFT)
        {
            icon = cocos2d::Sprite::create("png/map.png", cocos2d::Rect(COLOR_SOLDIER_1));
        }
        else
        {
            icon = cocos2d::Sprite::create("png/map.png", cocos2d::Rect(COLOR_SOLDIER_2));
        }
        icon->setTag(id);
        _BATTLE_STAGE.getMap()->addChild(icon);
        icon->setPositionX(tileX * 4);
        icon->setPositionY(tileZ * 4);
        entity.assign<BattleComponent::MiniIcon>(icon);

        auto frame = _BATTLE_STAGE.getMap()->getChildByTag(MAP_FRAME);
        _BATTLE_STAGE.getMap()->addChild(frame);
        
        const auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
        
        const std::string assetName = config.getAssetName(side == BattleConfig::Side::SIDE_LEFT ? battleFieldConfig.leftSoldierAssetStyle : battleFieldConfig.rightSoldierAssetStyle);
        const std::string& animationName = BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_IDLE);
        auto* pAnimation = _BATTLE_ANIMATION_CREATOR.createSingle(assetName, animationName, x, y, z, (side == BattleConfig::SIDE_LEFT));
        entity.assign<BattleComponent::Animation>(pAnimation);
        entity.assign<BattleComponent::Property>(config.base, battleFieldConfig.propertyMin, battleFieldConfig.propertyMax);
        entity.assign<BattleComponent::Soldier>(config.soldierConfigData);
        
        m_entities->addEntity(entity, tileZ, tileX);
        
        if (initHp != -1)
        {
            auto targetProperty = entity.component<BattleComponent::Property>();
            targetProperty->set(BattleConfig::Property::HP, initHp);
        }

        if (delayShow > 0)
        {
            _ENTITY_EVENT.emit(BattleEvent::StrategySoldierFadeIn(entity, delayShow));
        }
        return id;
    }
    
    return -1;
}

void SoldierCreator::addBackgroundSoldier(BattleConfig::Side side, BattleConfig::SoldierConfig& config, int tileZ, int tileX)
{
    const auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
    const std::string assetName = config.getAssetName(side == BattleConfig::Side::SIDE_LEFT ? battleFieldConfig.leftSoldierAssetStyle : battleFieldConfig.rightSoldierAssetStyle);
    const std::string& animationName = BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_IDLE);
    
    auto* pEffectGroup = _BATTLE_ANIMATION_CREATOR.convertAnimationToEffectGroup(assetName, animationName);
    bool reverse = (side == BattleConfig::SIDE_LEFT);
    float x = tileX * GRID_WIDTH + HALF_GRID_WIDTH;
    float y = 0;
    float z = tileZ * GRID_HEIGHT + HALF_GRID_HEIGHT;
    
    _ENTITY_EVENT.emit<BattleEvent::AddEffect>(pEffectGroup, reverse, x, y, z, 0, -1);
    
    CC_SAFE_DELETE(pEffectGroup);
}
