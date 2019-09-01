//
//  HeroCreator.cpp
//  Game
//
//  Created by fuchenhao on 3/9/15.
//
//

#include "HeroCreator.h"
#include "BattleWorld.h"

using namespace std;

HeroCreator::HeroCreator(BattleEntity* entities, BattleStage* stage)
{
    m_entities = entities;
    m_stage = stage;
}

HeroCreator::~HeroCreator()
{
}

bool HeroCreator::addHero(int heroId, BattleConfig::HeroConfig& config, int tileZ, int tileX)
{
    if(m_entities->isEmpty(tileZ, tileX))
    {
        BattleConfig::Side side = MapHelper::getSide(heroId);
        BattleConfig::FaceTo faceTo = (side == BattleConfig::SIDE_LEFT ? BattleConfig::FACE_TO_RIGHT : BattleConfig::FACE_TO_LEFT);
        float x = tileX * GRID_WIDTH;
        float y = 0;
        float z = tileZ * GRID_HEIGHT + HALF_GRID_HEIGHT;
        
        entityx::Entity entity = _ENTITY_MANAGER.create();
        entity.assign<BattleComponent::Position>(x, y, z);
        entity.assign<BattleComponent::Direction>((side == BattleConfig::SIDE_LEFT ? 1 : -1), 0.0, 0.0);
        entity.assign<BattleComponent::Identify>(heroId, BattleConfig::ENTITY_HERO, side, faceTo);
        entity.assign<BattleComponent::ObjectStateMachine>(entity);
        entity.assign<BattleComponent::AI>();
        entity.assign<BattleComponent::ActorMode>(entity);
        entity.assign<BattleComponent::Combo>();
        entity.assign<BattleComponent::HeroStrategy>();

        cocos2d::Sprite* icon = cocos2d::Sprite::create();
        cocos2d::Sprite* sp1;
        cocos2d::Sprite* sp2;
        if(side == BattleConfig::SIDE_LEFT)
        {
            sp1 = cocos2d::Sprite::create("png/map.png", cocos2d::Rect(COLOR_GENERAL_1));
            sp2 = cocos2d::Sprite::create("png/map.png", cocos2d::Rect(COLOR_GENERAL_1));
        }
        else
        {
            sp1 = cocos2d::Sprite::create("png/map.png", cocos2d::Rect(COLOR_GENERAL_2));
            sp2 = cocos2d::Sprite::create("png/map.png", cocos2d::Rect(COLOR_GENERAL_2));
        }
        sp2->setPositionX(-4);
        icon->addChild(sp1);
        icon->addChild(sp2);
        icon->setTag(heroId);
        _BATTLE_STAGE.getMap()->addChild(icon);
        icon->setPositionX(tileX * 4);
        icon->setPositionY(tileZ * 4);
        entity.assign<BattleComponent::MiniIcon>(icon);
        
        auto frame = _BATTLE_STAGE.getMap()->getChildByTag(MAP_FRAME);
        _BATTLE_STAGE.getMap()->addChild(frame);
        
        auto animation = _BATTLE_ANIMATION_CREATOR.createSingle(config.getAssetName(), BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_IDLE), x, y, z, (side == BattleConfig::SIDE_LEFT));
        auto animationComp = entity.assign<BattleComponent::Animation>(animation);
        animationComp->assetName = config.getAssetName();
        
        auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
        auto property = entity.assign<BattleComponent::Property>(config.base, battleFieldConfig.propertyMin, battleFieldConfig.propertyMax);
        property->set(BattleConfig::Property::HP, config.heroConfigData.currentHP);
        property->set(BattleConfig::Property::MP, config.heroConfigData.currentMP);
        
        entity.assign<BattleComponent::General>(config.heroConfigData);
        
        auto gasGather = entity.assign<BattleComponent::GasGather>(config.skills);
    
        m_entities->addEntity(entity, tileZ, tileX);
        
        if (config.hasAI())
        {
             if(side == BattleConfig::SIDE_RIGHT || battleFieldConfig.forceAutoFight)
             {
                 _ENTITY_EVENT.emit<BattleEvent::AddAI>(config.heroConfigData.strategy, heroId);
             }
        }
        
        for (auto* pData : gasGather->skills)
        {
            if (pData->pConfig->type == BattleConfig::SkillType::NEGATIVE)
            {
                _ENTITY_EVENT.emit<BattleEvent::FireNegativeSkill>(pData, heroId);
            }
        }
        
        return true;
    }
    return false;
}

void HeroCreator::addBackupHero(BattleConfig::Side side, BattleConfig::HeroConfig& config, int tileZ, int tileX)
{
    const std::string assetName = config.getAssetName();
    const std::string& animationName = BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_IDLE);
    
    auto* pEffectGroup = _BATTLE_ANIMATION_CREATOR.convertAnimationToEffectGroup(assetName, animationName);
    bool reverse = (side == BattleConfig::SIDE_LEFT);
    float x = tileX * GRID_WIDTH;
    float y = 0;
    float z = tileZ * GRID_HEIGHT + HALF_GRID_HEIGHT;
    
    auto effectEntity = _ENTITY_MANAGER.create();
    
    BattleEvent::AddEffect effect(pEffectGroup, reverse, x, y, z, 0, -1);
    effect.effectEntity = effectEntity;
    _ENTITY_EVENT.emit(effect);
    
    CC_SAFE_DELETE(pEffectGroup);
    
    auto* pBackupHero = new BattleEntity::BackupHero(tileZ, tileX, config, effectEntity);
    m_entities->addBackupHero(pBackupHero, side);
}
