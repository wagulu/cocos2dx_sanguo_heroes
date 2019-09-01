
//
//  BattleEvent.h
//  Game
//
//  Created by fuchenhao on 2/11/15.
//
//

#ifndef __BATTLE_EVENT_H__
#define __BATTLE_EVENT_H__

#include "entityx.h"
#include "BattleConfig.h"
#include "ActorMode.h"
#include "BattleComponent.h"
#include "BattleAnimation.h"
#include "StateSoldierChangeToGhost.h"

#define EVENT_CLASS(NAME) struct NAME : public entityx::Event<NAME>

#define EVENT_BATTLE_END                "msgGameOver"
#define EVENT_POPUPCOMBO                "PopupCombo"
#define EVENT_START_DELEGATE            "startDelegate"
#define EVENT_STOP_DELEGATE             "stopDelegate"
#define EVENT_CLOSE_STRATEGY_PANEL      "CloseStrategyPanel"

class BattleEvent
{
public:
    
    // ================== skill - action system
    //=================================================================

#pragma mark -
#pragma mark Attack

    EVENT_CLASS(AddAttack)
    {
        AddAttack(const BattleConfig::AttackData& attackData) : attackData(attackData) {}
        
        const BattleConfig::AttackData& attackData;
    };
    
    EVENT_CLASS(HurtByAttack)
    {
        HurtByAttack(entityx::Entity& fromEntity, entityx::Entity& targetEntity, const BattleConfig::AttackData& attackData) : fromEntity(fromEntity), targetEntity(targetEntity), attackData(attackData) {};
        
        entityx::Entity& fromEntity;
        entityx::Entity& targetEntity;
        const BattleConfig::AttackData& attackData;
    };
    
    EVENT_CLASS(HurtByGhost)
    {
        HurtByGhost(int ghostSoldierId, int targetId) : ghostSoldierId(ghostSoldierId), targetId(targetId) {}
        
        int ghostSoldierId;
        int targetId;
    };
    
    EVENT_CLASS(SkillArrowHitEvent)
    {
        SkillArrowHitEvent(int skillId, const entityx::Entity::Id& id, const std::vector<int>& targets, entityx::Entity& entity)
                            : skillId(skillId), id(id), targets(targets), entity(entity) {}

        int skillId;
        entityx::Entity::Id id;
        const std::vector<int>& targets;
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(SkillArrowDestroyEvent)
    {
        SkillArrowDestroyEvent(int skillId, const entityx::Entity::Id& id) : skillId(skillId), id(id) {}
        
        int skillId;
        entityx::Entity::Id id;
    };
    
    EVENT_CLASS(SkillArrowOutOfHpEvent)
    {
        SkillArrowOutOfHpEvent(int skillId, const entityx::Entity::Id& id, float x, float z) : skillId(skillId), id(id), x(x), z(z) {}
        
        float x, z;
        int skillId;
        entityx::Entity::Id id;
    };
    
    EVENT_CLASS(SkillArrowOutEvent)
    {
        SkillArrowOutEvent(int skillId, const entityx::Entity::Id& id, float x, float z) : skillId(skillId), id(id), x(x), z(z) {}
        
        float x, z;
        int skillId;
        entityx::Entity::Id id;
    };
    
    EVENT_CLASS(SkillAttackOnTileEvent)
    {
        SkillAttackOnTileEvent(int tileX, int tileZ,
                               BattleConfig::DamageType damageType = BattleConfig::DAMAGE_UNKNOWN,
                               BattleConfig::DamageEntityType damageEntityType = BattleConfig::DAMAGE_ENTITY_ALL)
                              :tileX(tileX), tileZ(tileZ), damageType(damageType), damageEntityType(damageEntityType) {}
        int tileX, tileZ;
        BattleConfig::DamageType damageType;
        BattleConfig::DamageEntityType damageEntityType;
    };
    
#pragma mark -
#pragma mark Skill
    
    EVENT_CLASS(FireSkill)
    {
        FireSkill(BattleConfig::SkillData* pSkillData, int fromId) : pSkillData(pSkillData), fromId(fromId) {};
        
        BattleConfig::SkillData* pSkillData;
        int fromId;
    };
    
    EVENT_CLASS(FireNegativeSkill)
    {
        FireNegativeSkill(BattleConfig::SkillData* pSkillData, int fromId) : pSkillData(pSkillData), fromId(fromId) {};
        
        BattleConfig::SkillData* pSkillData;
        int fromId;
    };
    
    EVENT_CLASS(PreFireSkill)
    {
        PreFireSkill(int skillId, int fromId) : skillId(skillId), fromId(fromId) {};
        
        int skillId;
        int fromId;
    };
    
    EVENT_CLASS(TriggerSkillAction)
    {
        TriggerSkillAction(const entityx::Entity::Id& id, const std::string& trigger) : id(id), trigger(trigger) {}
        
        const entityx::Entity::Id& id;
        const std::string& trigger;
    };
    
    EVENT_CLASS(CancelSkill)
    {
        CancelSkill(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
        bool forceCancel = false;
    };
    
    EVENT_CLASS(CompleteSkill)
    {
        CompleteSkill(int fromId) : fromId(fromId) {};
        
        int fromId;
    };
    
#pragma mark -
#pragma mark AI
    
    EVENT_CLASS(AddAI)
    {
        AddAI(std::string name, int heroId) : name(name), heroId(heroId) {};
        
        std::string name;
        int heroId;
    };
    
    
#pragma mark -
#pragma mark Effect
    
    struct AddEffectSimple: public entityx::Event<AddEffectSimple>
    {
        AddEffectSimple(std::vector<BattleAnimation*>* animations, float x, float y, float z, float delay = 0, float duration = 0) :
        animations(animations), x(x), y(y), z(z), delay(delay), duration(duration) {};
        
        std::vector<BattleAnimation*>* animations;
        float x, y, z, delay, duration;
    };

    struct AddEffect: public entityx::Event<AddEffect>
    {
        AddEffect(BattleConfig::EffectGroup* group, bool reverse, float x, float y, float z, float delay, float duration, int fromId = 0, bool rotate = false) :
        group(group), reverse(reverse), x(x), y(y), z(z), delay(delay), duration(duration), fromId(fromId), rotate(rotate) {};
        
        BattleConfig::EffectGroup* group;
        float x, y, z, delay, duration;
        bool reverse, rotate;
        int fromId;
        
        // you can choose to create effect entity by yourself, so you can also destory it by yourself
        entityx::Entity effectEntity;
    };
    
    struct AddEffectToEntity: public entityx::Event<AddEffectToEntity>
    {
        AddEffectToEntity(BattleConfig::EffectGroup* group, bool reverse, const entityx::Entity& entity, float delay, float duration, int fromId = 0) :
        group(group), reverse(reverse), entity(entity), delay(delay), duration(duration), fromId(fromId) {};
        
        BattleConfig::EffectGroup* group;
        entityx::Entity entity;
        float delay, duration;
        bool reverse;
        int fromId;
        
        // you can choose to create effect entity by yourself, so you can also destory it by yourself
        entityx::Entity effectEntity;
    };
    
    struct AddEntityAnimation : public entityx::Event<AddEntityAnimation>
    {
        AddEntityAnimation(BattleConfig::EffectGroup* group, const entityx::Entity& entity, bool reverse, bool loop = true, float lifeTime = -1, std::string* pTag = nullptr) :
        group(group), entity(entity), reverse(reverse), loop(loop), lifeTime(lifeTime), pTag(pTag) {};
        
        BattleConfig::EffectGroup* group;
        entityx::Entity entity;
        bool reverse;
        bool loop;
        float lifeTime;
        std::string* pTag;
    };
    
    struct RemoveEntityAnimation : public entityx::Event<RemoveEntityAnimation>
    {
        RemoveEntityAnimation(const entityx::Entity& entity, std::string* pTag = nullptr) : entity(entity), pTag(pTag) {}
        
        entityx::Entity entity;
        std::string* pTag;
    };
    
    struct AddBuffAnimation : public entityx::Event<AddBuffAnimation>
    {
        AddBuffAnimation(BattleConfig::EffectGroup* group, const entityx::Entity& entity, bool reverse, bool loop = true, float lifeTime = -1) :
        group(group), entity(entity), reverse(reverse), loop(loop), lifeTime(lifeTime) {};
        
        BattleConfig::EffectGroup* group;
        entityx::Entity entity;
        bool reverse;
        bool loop;
        float lifeTime;
    };
    
    struct AddSkillArrow: public entityx::Event<AddSkillArrow>
    {
        AddSkillArrow(BattleConfig::EffectGroup* group, float x, float y, float z, BattleConfig::SkillArrowData data, bool follow = true, bool destroyAfterHitHero = true) :
        group(group), x(x), y(y), z(z), data(data), follow(follow), destroyAfterHitHero(destroyAfterHitHero) {};
        
        BattleConfig::EffectGroup* group;
        float x, y, z;
        bool follow;
        bool destroyAfterHitHero;
        BattleConfig::SkillArrowData data;
    };
    
    EVENT_CLASS(AddEffectFlyText)
    {
        AddEffectFlyText(entityx::Entity& targetEntity, std::vector<BattleAnimation*>* animations, bool fireNow = false) : targetEntity(targetEntity), animations(animations), fireNow(fireNow) {}
        
        entityx::Entity& targetEntity;
        std::vector<BattleAnimation*>* animations;
        bool fireNow;
    };

#pragma mark -
#pragma mark Buff
    
    struct AddBuff : public entityx::Event<AddBuff>
    {
        AddBuff(const BattleConfig::BuffData& buffData) : buffData(buffData) {}
        
        const BattleConfig::BuffData& buffData;
    };
    
    struct RemoveBuff : public entityx::Event<RemoveBuff>
    {
        RemoveBuff(int targetId, BattleConfig::Property targetProperty) : targetId(targetId), targetProperty(targetProperty) {}
        
        int targetId;
        BattleConfig::Property targetProperty;
    };
    
    struct ClearAllBuff : public entityx::Event<ClearAllBuff>
    {
        ClearAllBuff(int targetId) : targetId(targetId) {}
        int targetId;
    };
    
    struct ChangeGasEvent:public entityx::Event<ChangeGasEvent>
    {
        ChangeGasEvent(entityx::Entity::Id entityId,int damage, int type) : entityId(entityId), damage(damage),type(type) {};
        entityx::Entity::Id entityId;
        int damage;//伤害
        int type;//类型 目前有三种 己方武将被攻击回气值 杀敌回气值 损失回气值
    };
    

    // ================== AI system
    //=================================================================

    /*
    struct ForceChangeState: public entityx::Event<ForceChangeState>
    {
        ForceChangeState(entityx::Entity& entity, int targetState) : entity(entity), targetState(targetState) {};
        
        entityx::Entity& entity;
        int targetState;
    };
     */
    
    struct BeAttacked: public entityx::Event<BeAttacked>
    {
        BeAttacked(entityx::Entity& entity, entityx::Entity& sourceEntity) : entity(entity), sourceEntity(sourceEntity) {};
        
        entityx::Entity& entity;
        entityx::Entity& sourceEntity;
    };
    
    struct StrategyRecenter: public entityx::Event<StrategyRecenter>
    {
        StrategyRecenter(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    struct StrategyAttack: public entityx::Event<StrategyAttack>
    {
        StrategyAttack(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyIdle)
    {
        StrategyIdle(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    struct StrategyProtectHero: public entityx::Event<StrategyProtectHero>
    {
        StrategyProtectHero(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyDepart)
    {
        StrategyDepart(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyAttackHero)
    {
        StrategyAttackHero(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyMoveBack)
    {
        StrategyMoveBack(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyBattleEnd)
    {
        StrategyBattleEnd(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyHeroRush)
    {
        StrategyHeroRush(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyHeroStop)
    {
        StrategyHeroStop(entityx::Entity& entity) : entity(entity) {};
        
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyHeroDizzy)
    {
        StrategyHeroDizzy(entityx::Entity& entity, const BattleConfig::HeroDizzyData& dizzyData, const BattleConfig::HeroDizzyConfig& dizzyConfig) : entity(entity), dizzyData(dizzyData), dizzyConfig(dizzyConfig) {};
        
        entityx::Entity& entity;
        const BattleConfig::HeroDizzyData& dizzyData;
        const BattleConfig::HeroDizzyConfig& dizzyConfig;
    };
    
    EVENT_CLASS(StrategyHeroMoveToTargetTile)
    {
        StrategyHeroMoveToTargetTile(entityx::Entity& entity, const BattleConfig::Tile& targetTile) : entity(entity), targetTile(targetTile) {}
        
        entityx::Entity& entity;
        const BattleConfig::Tile& targetTile;
    };
    
    EVENT_CLASS(StrategySoldierFadeIn)
    {
        StrategySoldierFadeIn(entityx::Entity& entity, float dizzyTime) : entity(entity), dizzyTime(dizzyTime) {};
        
        entityx::Entity& entity;
        float dizzyTime;
    };
    
    EVENT_CLASS(StrategySoldierDizzy)
    {
        StrategySoldierDizzy(entityx::Entity& entity, float dizzyTime) : entity(entity), dizzyTime(dizzyTime) {};
        
        entityx::Entity& entity;
        float dizzyTime;
    };
    
    EVENT_CLASS(StrategySoldierChangeToGhost)
    {
        StrategySoldierChangeToGhost(entityx::Entity& entity, const StateSoldierChangeToGhost::GhostData& ghostData) : entity(entity), ghostData(ghostData) {};
      
        entityx::Entity& entity;
        const StateSoldierChangeToGhost::GhostData& ghostData;
    };
    
    EVENT_CLASS(StrategySoldierChangeToBlock)
    {
        StrategySoldierChangeToBlock(entityx::Entity& entity, float lifeTime) : entity(entity), lifeTime(lifeTime) {};
        
        entityx::Entity entity;
        float lifeTime = 0;
        BattleConfig::EffectGroup* appearEffect = nullptr;
        BattleConfig::EffectGroup* loopEffect = nullptr;
        BattleConfig::EffectGroup* deadEffect = nullptr;
    };
    
    EVENT_CLASS(StateHeroDeadDone)
    {
        StateHeroDeadDone(entityx::Entity& entity) : entity(entity) {}
        
        entityx::Entity& entity;
    };

    EVENT_CLASS(BattleActorDead)
    {
        BattleActorDead(entityx::Entity& entity, const BattleConfig::AttackData& attackData, bool atOnce = false) : entity(entity), attackData(attackData), atOnce(atOnce) {};
        
        entityx::Entity& entity;
        const BattleConfig::AttackData& attackData;
        bool atOnce;
    };
    
    EVENT_CLASS(BattleActorRebirth)
    {
        BattleActorRebirth(entityx::Entity& entity, int tileZ, int tileX, BattleConfig::Side orgSide, uint64_t skillUniID) : entity(entity), tileZ(tileZ), tileX(tileX), orgSide(orgSide), skillUniID(skillUniID) {};
        
        entityx::Entity& entity;
        int tileZ, tileX;
        BattleConfig::Side orgSide;
        uint64_t skillUniID;
    };
    
    EVENT_CLASS(StateSoldierPlayAnimation)
    {
        StateSoldierPlayAnimation(entityx::Entity& entity, std::string animation, float duration = 0) : entity(entity), animation(animation), duration(duration) {};
        
        std::string animation;
        float duration;
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StateSoldierHitBack)
    {
        StateSoldierHitBack(entityx::Entity& entity, float duration = 2.0) : entity(entity), duration(duration) {};
        
        float duration;
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyHeroPlayAnimation)
    {
        StrategyHeroPlayAnimation(entityx::Entity& entity, std::string animation, float duration = 0) : entity(entity), animation(animation), duration(duration) {};
        
        std::string animation;
        float duration;
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(StrategyHeroDash)
    {
        StrategyHeroDash(entityx::Entity& entity, std::string animation, float duration = 0.0f, float topSpeed = 8.0f)
                        : entity(entity), animation(animation), duration(duration), topSpeed(topSpeed) {};
        
        std::string animation;
        float duration;
        float topSpeed;
        entityx::Entity& entity;
    };
    
    EVENT_CLASS(HeroPushBattleActor)
    {
        HeroPushBattleActor(entityx::Entity& fromEntity, entityx::Entity& targetEntity) : fromEntity(fromEntity), targetEntity(targetEntity) {};
        
        entityx::Entity& fromEntity;
        entityx::Entity& targetEntity;
    };
    
    EVENT_CLASS(KnockBackSoldier)
    {
        KnockBackSoldier(entityx::Entity& entity, entityx::Entity& sourceEntity, float expectDist) : entity(entity), sourceEntity(sourceEntity), expectDist(expectDist) {};
        
        entityx::Entity& entity;
        entityx::Entity& sourceEntity;
        float expectDist;
    };

    
#pragma mark -
#pragma mark Camera
    // ================== camera
    //=================================================================

    EVENT_CLASS(CameraMoveToEvent)
    {
        CameraMoveToEvent(float toX = 0, float toZ = 0, float toY = 0)
                          : toX(toX), toZ(toZ), toY(toY) {};
        float toX, toY, toZ;
    };
    
    EVENT_CLASS(CameraMoveToWithAniEvent)
    {
        CameraMoveToWithAniEvent(float toX = 0, float toZ = 0, float toY = 0,
                                 float time = 0, float delay = 0,
                                 int curCameraMode = 0, int nextCameraMode = 0,
                                 float rotate = CONF_INIT_CAMERA_ROTATE, int tweenMode = 0,
                                 uint64_t causeId = 0, bool force = true, float backDelay = 1.0f)
                                 : toX(toX), toZ(toZ), toY(toY), time(time), delay(delay), curCameraMode(curCameraMode), nextCameraMode(nextCameraMode), rotate(rotate), tweenMode(tweenMode), causeId(causeId), force(force), backDelay(backDelay) {};
        float toX, toY, toZ;
        float time, delay;
        float rotate;
        int curCameraMode;
        int nextCameraMode;
        int tweenMode;
        bool force;
        int64_t causeId;
        float backDelay;
    };
    
    
    EVENT_CLASS(CameraMoveToWithAniXEvent)
    {
        CameraMoveToWithAniXEvent(float toX = 0,
                                 float time = 0, float delay = 0,
                                 int curCameraMode = 0, int nextCameraMode = 0,
                                 int tweenMode = 0,
                                 uint64_t causeId = 0, bool force = true)
        : toX(toX), time(time), delay(delay), curCameraMode(curCameraMode), nextCameraMode(nextCameraMode), tweenMode(tweenMode), causeId(causeId), force(force) {};
        float toX;
        float time, delay;
        int curCameraMode;
        int nextCameraMode;
        int tweenMode;
        bool force;
        int64_t causeId;
    };
    
    EVENT_CLASS(CameraMoveToWithAniYEvent)
    {
        CameraMoveToWithAniYEvent(float toY = 0,
                                 float time = 0, float delay = 0,
                                 int curCameraMode = 0, int nextCameraMode = 0,
                                 int tweenMode = 0,
                                 uint64_t causeId = 0, bool force = true)
        : toY(toY), time(time), delay(delay), curCameraMode(curCameraMode), nextCameraMode(nextCameraMode), tweenMode(tweenMode), causeId(causeId), force(force) {};
        float toY;
        float time, delay;
        int curCameraMode;
        int nextCameraMode;
        int tweenMode;
        bool force;
        int64_t causeId;
    };
    
    EVENT_CLASS(CameraMoveToWithAniZEvent)
    {
        CameraMoveToWithAniZEvent(float toZ = 0,
                                 float time = 0, float delay = 0,
                                 int curCameraMode = 0, int nextCameraMode = 0,
                                 int tweenMode = 0,
                                 uint64_t causeId = 0, bool force = true)
        : toZ(toZ), time(time), delay(delay), curCameraMode(curCameraMode), nextCameraMode(nextCameraMode), tweenMode(tweenMode), causeId(causeId), force(force) {};
        float toZ;
        float time, delay;
        int curCameraMode;
        int nextCameraMode;
        int tweenMode;
        bool force;
        int64_t causeId;
    };
    
    EVENT_CLASS(CameraMoveToWithAniR3dEvent)
    {
        CameraMoveToWithAniR3dEvent(float toR3d = 0,
                                 float time = 0, float delay = 0,
                                 int curCameraMode = 0, int nextCameraMode = 0,
                                 int tweenMode = 0,
                                 uint64_t causeId = 0, bool force = true)
        : toR3d(toR3d), time(time), delay(delay), curCameraMode(curCameraMode), nextCameraMode(nextCameraMode), tweenMode(tweenMode), causeId(causeId), force(force) {};
        float toR3d;
        float time, delay;
        int curCameraMode;
        int nextCameraMode;
        int tweenMode;
        bool force;
        int64_t causeId;
    };
    
    EVENT_CLASS(CameraMoveToWithAniR2dEvent)
    {
        CameraMoveToWithAniR2dEvent(float toR2d = 0,
                                 float time = 0, float delay = 0,
                                 int curCameraMode = 0, int nextCameraMode = 0,
                                 int tweenMode = 0,
                                 uint64_t causeId = 0, bool force = true)
        : toR2d(toR2d), time(time), delay(delay), curCameraMode(curCameraMode), nextCameraMode(nextCameraMode), tweenMode(tweenMode), causeId(causeId), force(force) {};
        float toR2d;
        float time, delay;
        int curCameraMode;
        int nextCameraMode;
        int tweenMode;
        bool force;
        int64_t causeId;
    };
    
    EVENT_CLASS(CameraMoveToWithAniRYEvent)
    {
        CameraMoveToWithAniRYEvent(float toRY = 0,
                                    float time = 0, float delay = 0,
                                    int curCameraMode = 0, int nextCameraMode = 0,
                                    int tweenMode = 0,
                                    uint64_t causeId = 0, bool force = true)
        : toRY(toRY), time(time), delay(delay), curCameraMode(curCameraMode), nextCameraMode(nextCameraMode), tweenMode(tweenMode), causeId(causeId), force(force) {};
        float toRY;
        float time, delay;
        int curCameraMode;
        int nextCameraMode;
        int tweenMode;
        bool force;
        int64_t causeId;
    };
    
    EVENT_CLASS(CameraQueueClearEvent)
    {
        CameraQueueClearEvent() {};
    };
    
    EVENT_CLASS(CameraFollowEntity)
    {
        CameraFollowEntity(entityx::Entity& tarEntity, bool canBreak = false, bool setState = true) : tarEntity(tarEntity), canBreak(canBreak), setState(setState) {};
        entityx::Entity& tarEntity;
        bool canBreak;
        bool setState;
    };
    
    EVENT_CLASS(CameraTriggerFollowEntity)
    {
        CameraTriggerFollowEntity(bool following = true) : following(following) {};
        bool following;
    };
    
    EVENT_CLASS(CameraShakeEvent)
    {
        CameraShakeEvent(const BattleConfig::CameraShakeData& cameraShakeData) : cameraShakeData(cameraShakeData) {};
        const BattleConfig::CameraShakeData& cameraShakeData;
    };
    
    EVENT_CLASS(CameraDisableShakeEvent)
    {
        CameraDisableShakeEvent(bool diable = false) : diable(diable) {};
        bool diable;
    };
    
    EVENT_CLASS(CameraClearShakeEvent)
    {
        
    };
    
    EVENT_CLASS(SetCameraModeForLockEvent)
    {
        SetCameraModeForLockEvent(bool sinus = false, int roleID = -1) : sinus(sinus), roleID(roleID) {};
        bool sinus;
        int roleID;
    };
    

#pragma mark -
#pragma mark actor
    // ================== actor mode
    //=================================================================

    EVENT_CLASS(SetActorMode)
    {
        SetActorMode(entityx::Entity& tarEntity, ActorMode::MODE_TYPE m_type, float m_duration):
        tarEntity(tarEntity),
        m_type(m_type),
        m_duration(m_duration)
        {
            
        }
        ~SetActorMode()
        {
            
        }
        
        ActorMode::MODE_TYPE m_type;
        float m_duration;
        entityx::Entity& tarEntity;
        
    };

    EVENT_CLASS(PlayActorModeAnim)
    {
        PlayActorModeAnim(entityx::Entity& tarEntity, ActorMode::MODE_TYPE mode, bool show = true):
        tarEntity(tarEntity),
        mode(mode),
        show(show)
        {
            
        }
        ~PlayActorModeAnim()
        {
            
        }
        
        entityx::Entity& tarEntity;
        ActorMode::MODE_TYPE mode;
        bool show;
        
    };

    /*
    EVENT_CLASS(SwitchActorMode)
    {
        SwitchActorMode(entityx::Entity& tarEntity, ActorMode::MODE_TYPE mode):
        entity(tarEntity),
        mode(mode)
        {
            
        }
        ~SwitchActorMode()
        {
            
        }
        
        entityx::Entity& entity;
        ActorMode::MODE_TYPE mode;
//        float m_duration;
        
    };
     */
    
    // ================== battle
    //=================================================================
    
    EVENT_CLASS(DialogueStartComplete)
    {
        
    };
    
    EVENT_CLASS(DialogueResultComplete)
    {
        
    };
    
    EVENT_CLASS(ShowStartDialogue)
    {
        
    };
    
    EVENT_CLASS(ShowWinDialogue)
    {
        
    };
    
    EVENT_CLASS(ShowLoseDialogue)
    {
        
    };
    
    EVENT_CLASS(ShowBattleSceneUI)
    {
        
    };
    
    EVENT_CLASS(ShowBattleSceneUIComplete)
    {
        
    };
    
    EVENT_CLASS(HideBattleSceneUI)
    {
        
    };
    
    EVENT_CLASS(PlayHeroDeadEffect)
    {
        
    };
    
    EVENT_CLASS(PlayBattleResultEffect)
    {
        
    };
    
    EVENT_CLASS(PlayBattleResultEffectComplete)
    {
        
    };
    
    EVENT_CLASS(BattlePause)
    {
        
    };
    
    EVENT_CLASS(BattleResume)
    {
        
    };
    
    EVENT_CLASS(ExitBattleBeforeEnd)
    {
        
    };
    
    EVENT_CLASS(LockSkill)
    {
        LockSkill(BattleConfig::Side side) : side(side) {}
        
        BattleConfig::Side side;
    };
    
    EVENT_CLASS(UnlockSkill)
    {
        UnlockSkill(BattleConfig::Side side) : side(side) {}
        
        BattleConfig::Side side;
    };
    
    EVENT_CLASS(LockStatusChange)
    {
        LockStatusChange(bool lock) : lock(lock) {}
        
        bool lock;
    };
    
    EVENT_CLASS(StartBattleProgress)
    {
        StartBattleProgress(float percent) :percent(percent) {}
        float percent;
    };
    
    EVENT_CLASS(StartBattleComplete)
    {
        
    };
    
    EVENT_CLASS(UpdateBuffIcon)
    {
        UpdateBuffIcon() {};
    };
    
    EVENT_CLASS(AddBuffIcon)
    {
        std::string path;
        BattleConfig::Side side;
        
        AddBuffIcon(std::string path, BattleConfig::Side side) : path(path), side(side) {};
    };
    
    EVENT_CLASS(RemoveBuffIcon)
    {
        std::string path;
        BattleConfig::Side side;
        
        RemoveBuffIcon(std::string path, BattleConfig::Side side) : path(path), side(side) {};
    };
    
    // ================== audio
    //=================================================================
    
    EVENT_CLASS(BattleSound)
    {
        std::string name;
        std::string triggerName;
        int* pSoundId;
        entityx::Entity fireEntity;
        
        BattleSound(std::string name, std::string triggerName, entityx::Entity fireEntity) :
            name(name), triggerName(triggerName), fireEntity(fireEntity)
        {
            pSoundId = new int();
        };
        ~BattleSound()
        {
            delete pSoundId;
            pSoundId = NULL;
        };
    };
    
};

#endif
