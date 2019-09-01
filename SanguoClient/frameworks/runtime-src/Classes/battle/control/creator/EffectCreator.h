//
//  EffectCreator.h
//  Game
//
//  Created by fuchenhao on 3/19/15.
//
//

#ifndef __Game__EffectCreator__
#define __Game__EffectCreator__

#include "entityx.h"
#include "BattleConfig.h"

class EffectCreator
{
public:
    
    EffectCreator();
    ~EffectCreator();
    
    void createNormalAttackEffect(entityx::Entity& fromEntity, const std::string& animationName);
    void createHitEffect(entityx::Entity& fromEntity, entityx::Entity& targetEntity, const BattleConfig::AttackData& attackData);
    void createTurnRedEffect(entityx::Entity& targetEntity);
    void createMissEffect(entityx::Entity& targetEntity);
    void createHPDecreaseEffect(entityx::Entity& fromEntity, entityx::Entity& targetEntity, int value, bool isCritical);
    void createHPIncreaseEffect(entityx::Entity& fromEntity, entityx::Entity& targetEntity, int value, bool fireNow);
    void createMPChangeEffect(entityx::Entity& fromEntity, entityx::Entity& targetEntity, int value, bool increase, bool fireNow);

private:
    const cocos2d::Vec3& getFlyTextAppearPosition(entityx::Entity& targetEntity);
    
    cocos2d::Vec3 m_tempPosition;
};

#endif /* defined(__Game__EffectCreator__) */
