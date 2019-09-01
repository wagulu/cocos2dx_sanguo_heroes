//
//  BattleAnimationGroup.h
//  Game
//
//  Created by fuchenhao on 12/23/14.
//
//

#ifndef __BATTLE_ANIMATION_GROUP_H__
#define __BATTLE_ANIMATION_GROUP_H__

#include "BattleAnimation.h"
#include <vector>

class BattleAnimationGroup
{
    
public:
    
    BattleAnimationGroup(std::vector<BattleAnimation*> *animations);
    ~BattleAnimationGroup();
    void playCurrent(bool loop, bool randomStart = true);
    void hideEffect();
    void setPositionX(float x);
    void setPosition(float x, float y, float z);
    void setScale(float scale);
    void setVisible(bool visible);
    bool isComplete();
    void update(float dt);
    
    std::vector<BattleAnimation*> * getAnimations() { return animations; }
    
private:
    
    std::vector<BattleAnimation*> *animations;
    
};

#endif /* defined(__Game__BattleAnimationGroup__) */

