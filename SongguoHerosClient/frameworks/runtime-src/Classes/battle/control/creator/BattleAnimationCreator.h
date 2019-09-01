//
//  BattleAnimationCreator.h
//  Game
//
//  Created by fuchenhao on 3/17/15.
//
//

#ifndef __Game__BattleAnimationCreator__
#define __Game__BattleAnimationCreator__

#include "BattleAnimation.h"
#include "BattleConfig.h"

using namespace std;

class BattleAnimationCreator
{
public:
    
    BattleAnimationCreator();
    ~BattleAnimationCreator();
    
    BattleConfig::EffectGroup* convertAnimationToEffectGroup(const string& assetName, const string& animationName, cocos2d::AdvancedAnimation::BlendMode blendMode = cocos2d::AdvancedAnimation::BlendMode::BLEND_NORMAL, int zOrder = 0);
    
    std::vector<BattleAnimation*>* create(BattleConfig::EffectGroup* effectGroup, float x, float y, float z, bool reverse = false, bool rotate = false);
    std::vector<BattleAnimation*>* create(const string& assetName, const string& animationName, float x, float y, float z, bool reverse = false, const cocos2d::Vec3 &rotation = cocos2d::Vec3(0, 0, 0), cocos2d::AdvancedAnimation::BlendMode blendMode = cocos2d::AdvancedAnimation::BLEND_NORMAL);
    std::vector<BattleAnimation*>* create(const string& assetName, const string& animationName, map<string, cocos2d::Node*> replaces, float x, float y, float z, bool reverse = false, const cocos2d::Vec3 &rotation = cocos2d::Vec3(0, 0, 0), cocos2d::AdvancedAnimation::BlendMode blendMode = cocos2d::AdvancedAnimation::BLEND_NORMAL);
    cocos2d::AdvancedAnimation* createSingle(const string& assetName, const string& animationName, float x, float y, float z, bool reverse = false, float delayShow = 0.0f);
};

#endif /* defined(__Game__BattleAnimationCreator__) */
