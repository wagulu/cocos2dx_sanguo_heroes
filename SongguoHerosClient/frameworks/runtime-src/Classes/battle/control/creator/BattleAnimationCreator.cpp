//
//  BattleAnimationCreator.cpp
//  Game
//
//  Created by fuchenhao on 3/17/15.
//
//

#include "BattleAnimationCreator.h"
#include "BattleWorld.h"

BattleAnimationCreator::BattleAnimationCreator()
{
}

BattleAnimationCreator::~BattleAnimationCreator()
{
}

BattleConfig::EffectGroup* BattleAnimationCreator::convertAnimationToEffectGroup(const string& assetName, const string& animationName, cocos2d::AdvancedAnimation::BlendMode blendMode , int zOrder)
{
    auto* pGroup = new BattleConfig::EffectGroup();
    pGroup->asset = assetName;
    
    auto* pLayer = new BattleConfig::EffectLayer();
    pLayer->layerName = animationName;
    pLayer->layerType = blendMode;
    pLayer->layerZOrder = zOrder;
    pGroup->effect.push_back(pLayer);
    
    return pGroup;
}

std::vector<BattleAnimation*>* BattleAnimationCreator::create(BattleConfig::EffectGroup* effectGroup, float x, float y, float z, bool reverse, bool rotate)
{
    auto define = cocos2d::AssetDefineCache::getInstance()->getAsset(effectGroup->asset);
    auto animations = new std::vector<BattleAnimation*>();
    auto& layers = effectGroup->effect;
    
    for(int i = 0; i < effectGroup->effect.size(); i++)
    {
        auto animation = cocos2d::AdvancedAnimation::create(define, layers[i]->layerName.c_str());
        animation->setPositionX(x);
        animation->setPositionY(y);
        animation->setPositionZ(-z);
        animation->setBlendMode(layers[i]->layerType);
        animation->setScale(CHARACTER_SCALE);
        animation->stop();
        
        if(reverse) animation->setScaleX(-CHARACTER_SCALE);
        if(rotate) animation->setRotation3D(cocos2d::Vec3(90, 0, 0));
        
        _BATTLE_STAGE.getRole()->addChild(animation, -(z - layers[i]->layerZOrder));
        animation->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
        auto battleAnimation = new BattleAnimation(animation);
        battleAnimation->setBiasZ(layers[i]->layerZOrder);
        animations->push_back(battleAnimation);
    }
    return animations;
}

std::vector<BattleAnimation*>* BattleAnimationCreator::create(const string& assetName, const string& animationName, float x, float y, float z, bool reverse, const cocos2d::Vec3 &rotation, cocos2d::AdvancedAnimation::BlendMode blendMode)
{
    auto pDefine = cocos2d::AssetDefineCache::getInstance()->getAsset(assetName);
    auto pAnimations = new std::vector<BattleAnimation*>();
    
    auto pAni = cocos2d::AdvancedAnimation::create(pDefine, animationName);
    pAni->setPositionX(x);
    pAni->setPositionY(y);
    pAni->setPositionZ(-z);
    pAni->setRotation3D(rotation);
    pAni->setBlendMode(blendMode);
    pAni->setScale(CHARACTER_SCALE);
    pAni->stop();
    if(reverse) pAni->setScaleX(-CHARACTER_SCALE);
    
    _BATTLE_STAGE.getRole()->addChild(pAni, -(z - 1));
    pAni->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
    pAnimations->push_back(new BattleAnimation(pAni));
    
    return pAnimations;
}

std::vector<BattleAnimation*>* BattleAnimationCreator::create(const string& assetName, const string& animationName, map<string, cocos2d::Node*> replaces, float x, float y, float z, bool reverse, const cocos2d::Vec3 &rotation, cocos2d::AdvancedAnimation::BlendMode blendMode)
{
    auto pDefine = cocos2d::AssetDefineCache::getInstance()->getAsset(assetName);
    auto pAnimations = new std::vector<BattleAnimation*>();
    
    auto pAni = cocos2d::AdvancedAnimation::create(pDefine, animationName);
    pAni->setPositionX(x);
    pAni->setPositionY(y);
    pAni->setPositionZ(-z);
    pAni->setRotation3D(rotation);
    pAni->setBlendMode(blendMode);
    pAni->setScale(CHARACTER_SCALE);
    pAni->stop();
    pAni->setVisible(false);
    if(reverse) pAni->setScaleX(-CHARACTER_SCALE);
    
    for (auto iter = replaces.begin(); iter != replaces.end(); ++iter)
    {
        cocos2d::Node* pAsset = iter->second;
        pAsset->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
        
        pAni->addReplace(iter->first, pAsset);
    }
    
    _BATTLE_STAGE.getRole()->addChild(pAni, -(z - 1));
    pAni->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
    pAnimations->push_back(new BattleAnimation(pAni));
    
    return pAnimations;
}

cocos2d::AdvancedAnimation* BattleAnimationCreator::createSingle(const string& assetName, const string& animationName, float x, float y, float z, bool reverse, float delayShow)
{
    auto pDefine = cocos2d::AssetDefineCache::getInstance()->getAsset(assetName);
    
    auto pAni = cocos2d::AdvancedAnimation::create(pDefine, animationName, delayShow);
    pAni->setPositionX(x);
    pAni->setPositionY(y);
    pAni->setPositionZ(-z);
    pAni->setScale(CHARACTER_SCALE);
    pAni->stop();
    if(reverse) pAni->setScaleX(-CHARACTER_SCALE);
    
    _BATTLE_STAGE.getRole()->addChild(pAni, -z);
    pAni->setCameraMask((unsigned short)cocos2d::CameraFlag::USER1);
    
    return pAni;
}
