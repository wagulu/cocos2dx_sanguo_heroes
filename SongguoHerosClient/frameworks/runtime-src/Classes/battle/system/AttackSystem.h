//
//  AttackSystem.h
//  Game
//
//  Created by fuchenhao on 2/26/15.
//
//

#ifndef __ATTACK_SYSTEM_H__
#define __ATTACK_SYSTEM_H__

#include "../../entityx/entityx.h"
#include "BattleEvent.h"

class AttackSystem : public entityx::System<AttackSystem>, public entityx::Receiver<AttackSystem>
{
public:
    
    AttackSystem();
    
    void configure(entityx::EventManager& events) override;
    
    void receive(const BattleEvent::AddAttack& e);
    void receive(const BattleEvent::BattlePause& e);
    void receive(const BattleEvent::BattleResume& e);
    
    void update(entityx::EntityManager& es, entityx::EventManager& events, double dt) override;
    
    bool canNormalAttackTarget(entityx::Entity& fromEntity, entityx::Entity& targetEntity, const BattleConfig::AttackData& attackData);
    bool canSkillAttackTarget(entityx::Entity& fromEntity, entityx::Entity& targetEntity, const BattleConfig::AttackData& attackData);

    void doHurt(entityx::Entity& fromEntity, entityx::Entity& targetEntity, BattleConfig::AttackData& attackData);
    
    int getSkillLevel(entityx::Entity& fromEntity, int skillId);
    BattleConfig::NormalDamageType getNormalDamageType(entityx::Entity& fromEntity);
    
    float changeHP(entityx::Entity& entity, float delta);
    void changeGas(entityx::Entity& attackEntity, entityx::Entity& deadEntity);
    
    bool isNormalHurtDodge(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty);
    bool isSkillHurtDodge(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty);
    bool isNormalHurtCritical(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty);
    bool isSkillHurtCritical(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty);
    
    float computeNormalPhysicalHurt(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty, bool isCritical);
    float computeNormalMagicHurt(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty, bool isCritical);
    float computeSkillPhysicalHurt(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty, const BattleConfig::DamageData& damageData, int skillLevel, bool isCritical);
    float computeSkillMagicHurt(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty, const BattleConfig::DamageData& damageData, int skillLevel, bool isCritical);
    float computeShieldAbsorb(float hurt, BattleComponent::Property::Handle& targetProperty, BattleConfig::Property shieldType);
    float computeCureHurt(BattleComponent::Property::Handle& fromProperty, BattleComponent::Property::Handle& targetProperty, const BattleConfig::DamageData& damageData, int skillLevel, bool isCritical);
    float computeSkillKillHurt(BattleComponent::Property::Handle& targetProperty);
    float computeSkillPercentHurt(BattleComponent::Property::Handle& targetProperty, const BattleConfig::DamageData& damageData);
    float computeSkillHolyHurt(const BattleConfig::DamageData& damageData);
    float computeNormalVamp(BattleComponent::Property::Handle& fromProperty, float hurt);
    float randomizeHurtNumber(float hurt);
    
private:
    float m_CSDRFloorCoefficient;//爆伤下限系数
    
    bool m_isPausing = false;
};

#endif /* defined(__ATTACK_SYSTEM_H__) */
