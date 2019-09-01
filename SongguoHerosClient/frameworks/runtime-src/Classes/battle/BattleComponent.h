//
//  BattleComponent
//
//  Created by fuchenhao on 3/1/13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//


#ifndef __BATTLE_COMPONENT_H__
#define __BATTLE_COMPONENT_H__

#include <vector>

#include "../entityx/entityx.h"
#include "../asset/CCAsset.h"
#include "math.h"
#include "BattleConfig.h"
#include "animation/BattleAnimation.h"
#include "animation/BattleAnimationGroup.h"
#include "ModeGroup.h"
#include "StrategyControl.h"
#include "StateMachine.h"
#include "TweenQueue.h"
#include "cocos2d.h"

#define _COMPONENT(COMPONENT) struct COMPONENT : entityx::Component<COMPONENT>

class SkillObject;

class BattleComponent
{
public:
    
    struct Battle : entityx::Component<Battle>
    {
        float totalTime;
        
        Battle(float totalTime) : totalTime(totalTime) {}
    };
    
    struct Animation : entityx::Component<Animation>
    {
        BattleAnimation* battleAnimation;
        TweenQueue m_tweenQueue;
        std::string assetName;
        
        Animation(cocos2d::AdvancedAnimation *animation)
        {
            battleAnimation = new BattleAnimation(animation);
        }
        
        ~Animation()
        {
            delete battleAnimation;
        }
    };
    
    struct AnimationGroup : entityx::Component<AnimationGroup>
    {
        BattleAnimationGroup* battleAnimation;
        TweenQueue m_tweenQueue;
        bool randomStart;
        
        AnimationGroup(std::vector<BattleAnimation*> *animations, bool randomStart = true) : randomStart(randomStart)
        {
            battleAnimation = new BattleAnimationGroup(animations);
        }
        
        ~AnimationGroup()
        {
            delete battleAnimation;
        }
    };
    
    struct EntityAnimation : entityx::Component<EntityAnimation>
    {
        BattleAnimationGroup* battleAnimation;
        TweenQueue m_tweenQueue;
        bool loop;
        float lifeTime;
        string tag;
        
        EntityAnimation(std::vector<BattleAnimation*> *animations, bool loop = true, float lifeTime = -1, string* pTag = nullptr) : loop(loop), lifeTime(lifeTime)
        {
            battleAnimation = new BattleAnimationGroup(animations);
            battleAnimation->playCurrent(loop);
            if (pTag != nullptr) tag = *pTag;
        }
        
        ~EntityAnimation()
        {
            delete battleAnimation;
        }
    };
    
    struct BuffAnimation : entityx::Component<BuffAnimation>
    {
        BattleAnimationGroup* battleAnimation;
        bool loop;
        float lifeTime;
        
        BuffAnimation(std::vector<BattleAnimation*> *animations, bool loop = true, float lifeTime = -1) : loop(loop), lifeTime(lifeTime)
        {
            battleAnimation = new BattleAnimationGroup(animations);
            battleAnimation->playCurrent(loop);
        }
        
        ~BuffAnimation()
        {
            delete battleAnimation;
        }
    };
    
    struct ActiveEntity : entityx::Component<ActiveEntity>
    {
    };
    
	struct Position : entityx::Component<Position>
	{
	    Position(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
	    float x, y, z;
        
        void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
        void add(float x, float y, float z) { this->x += x; this->y += y; this->z += z; }
        void sub(float x, float y, float z) { this->x -= x; this->y -= y; this->z -= z; }
	};

	struct Direction : entityx::Component<Direction>
	{
	    Direction(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
	    float x, y, z;
        
        void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	};
    
    struct Target : entityx::Component<Target>
	{
	    Target(float x = 0.0f, float y = 0.0f, float z = 0.0f) : x(x), y(y), z(z) {}
	    float x, y, z;
        
        void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	};
    
    struct TargetEntity : entityx::Component<TargetEntity>
    {
        TargetEntity(const entityx::Entity& targetEntity) : targetEntity(targetEntity) {}
        entityx::Entity targetEntity;
    };
    
    struct Identify : entityx::Component<Identify>
	{
        int id;
        BattleConfig::EntityType type;
        BattleConfig::Side side;
        BattleConfig::FaceTo faceTo;
        
        Identify(int id, BattleConfig::EntityType type, BattleConfig::Side side, BattleConfig::FaceTo faceTo) : id(id), type(type), side(side), faceTo(faceTo) {}
    };
    
    struct Property : entityx::Component<Property>
    {
    private:
        std::array<bool, BattleConfig::SIZE_OF_PROPERTY_NAME> m_lock;
        std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME> m_current;
        std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME> m_original;
        const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& m_min;
        const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& m_max;
        
    public:
        Property(const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& data,
                 const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& dataMin,
                 const std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& dataMax);
        float get(BattleConfig::Property name);
        void set(BattleConfig::Property name, float value);
        float getOriginal(BattleConfig::Property name);
        bool isLocked(BattleConfig::Property name);
        void setLock(BattleConfig::Property name, bool lock);

    };

#pragma mark -
#pragma mark Attack
    
    struct Attack : entityx::Component<Attack>
    {
        Attack(const BattleConfig::AttackData& attackData) : attackData(attackData) {}
        
        BattleConfig::AttackData attackData;
    };
    
#pragma mark -
#pragma mark Gas
    
    struct GasGather:entityx::Component<GasGather>
    {
        std::vector<BattleConfig::SkillData*>& skills;
        bool lock = false;
        
        GasGather(std::vector<BattleConfig::SkillData*>& skills) : skills(skills) {}
    };
    
    struct Buff : entityx::Component<Buff>
    {
        std::vector<BattleConfig::BuffData*> buffList;
    
        Buff() {}
        
        ~Buff()
        {
            for(BattleConfig::BuffData *buffData : buffList)
            {
                if (buffData != NULL) delete buffData;
            }
            buffList.clear();
        }
    };
    
    struct General : entityx::Component<General>
	{
        int target;
        BattleConfig::HeroConfigData config;

        General(const BattleConfig::HeroConfigData& config) : config(config), target(0) {}
        ~General() { CCLOG("general clear"); }
    };
    
	struct Soldier : entityx::Component<Soldier>
	{
        int target;
        BattleConfig::SoldierConfigData config;
        
        Soldier(const BattleConfig::SoldierConfigData& config) : config(config), target(0) {}
        void cleanTarget() { target = 0; }
	};

    _COMPONENT(AnchorArrowData)
    {
        enum STATE
        {
            WORK,
            DEAD,
        };
        
        entityx::Entity m_firer;
        STATE state = WORK;
        
        AnchorArrowData(entityx::Entity firer): m_firer(firer) {}
    };
    
    _COMPONENT(MiniIcon)
    {
        cocos2d::Node* m_pIcon;
        MiniIcon(cocos2d::Node* m_pIcon): m_pIcon(m_pIcon) {}
    };
    
    struct AI : entityx::Component<AI>
    {
        float lastAttackTime = 0;
        int lastResearchTargetTicket = -1;
        int lastResearchBowManTargetTicket = -1;

        std::string strategy;
        std::vector<BattleConfig::AIData*> ai;
        
        bool isTileLosted = false;
        
        ~AI()
        {
            clearAllAI();
        }
        
        void clearAllAI()
        {
            for (auto* pData : ai)
            {
                delete pData;
            }
            ai.clear();
        }
        
        void clearCompletedAI();
        
    };
    
    struct ObjectStateMachine : entityx::Component<ObjectStateMachine>
    {
        bool isDead = false;
        StateMachine stateMachine;

        ObjectStateMachine(entityx::Entity& entity):
        stateMachine(entity)
        {
        }
        virtual ~ObjectStateMachine()
        {
        }
        
        void changeState(IState* pState, bool acceptDelegate = true) { stateMachine.changeState(pState, acceptDelegate); }
        IState* getCurrentState() { return stateMachine.getCurrentState(); }
        int getCurrentStateCode() { return stateMachine.getCurrentStateCode(); }
        int update(float dt) { return stateMachine.update(dt); }
    };
    
#pragma mark -
#pragma mark Effect
    
    //TO-DO check play current
    struct Effect : entityx::Component<Effect>
	{
        float delay, duration;
        bool follow;
        int fromId;
        
        float elapse = 0;
        bool playing = false;
        
        entityx::Entity followTarget; //follow mode
        
        Effect(float delay = 0, float duration = 0, bool follow = false, int fromId = 0) :
               delay(delay), duration(duration), follow(follow), fromId(fromId) {}
    };
    
    enum SkillArrowTrackState
    {
        ARROW_FLYING,
        ARROW_TRACKING
    };
    
    struct SkillArrow : entityx::Component<SkillArrow>
    {
        std::vector<int> targets;//移动过程中遇到的敌人对象
        BattleConfig::SkillArrowData arrowData;
        int startX;//init when add
        int startZ;//init when add
        SkillArrowTrackState track = ARROW_FLYING;
        bool hitHero;
        bool destroyAfterHitHero;
        
        float phase;
        
        int m_audioId;
        
        SkillArrow(const BattleConfig::SkillArrowData& arrowData, int startX, int startZ, bool destroyAfterHitHero = true);
        ~SkillArrow();
    };
    
    _COMPONENT(FlyTextEffect)
    {
        std::vector<std::vector<BattleAnimation*>*> animations;
        float delay;
        float interval;
        
        FlyTextEffect(float interval, float delay = 0) : interval(interval), delay(delay) {}
        
        ~FlyTextEffect();
    };
    
#pragma mark -
#pragma mark Camera
    
    _COMPONENT(ShakeCom)
    {
        float delay, duration, elapse, amplitude;
        int mode, direction, interval, index;
        //float orgX, orgY, orgZ;
        bool damping;
        
        ShakeCom(float duration = 0.0f, float amplitude = 10.0f, int mode = 0, int interval = 2, float delay = 0.0f, bool damping = true) :
                 duration(duration), amplitude(amplitude), mode(mode), interval(interval), delay(delay)
        {
            elapse = 0.0f;
            direction = 1;
            index = 0;
        }
        
        ~ShakeCom()
        {
        }
    };
    
    struct CameraFollowData
    {
        bool following;
        float toY;
        float toZ;
        float r3D;
        float r2D;
        entityx::Entity followTarget; //follow mode
    };

    _COMPONENT(Camera)
	{
//        int cameraMode;
        int cameraLastMode;
        CameraFollowData followData;
        std::vector<BattleConfig::CameraAction*> cameraQueue;
        
        int cameraModeX;
        std::vector<BattleConfig::CameraActionData*> cameraXQueue;
        int cameraModeY;
        std::vector<BattleConfig::CameraActionData*> cameraYQueue;
        float backYDelay;
        int cameraModeZ;
        std::vector<BattleConfig::CameraActionData*> cameraZQueue;
        float backZDelay;
        int cameraModeR3d;
        std::vector<BattleConfig::CameraActionData*> cameraR3dQueue;
        float backR3dDelay;
        int cameraModeR2d;
        std::vector<BattleConfig::CameraActionData*> cameraR2dQueue;
        float backR2dDelay;
        int cameraModeRY;
        std::vector<BattleConfig::CameraActionData*> cameraRYQueue;
        float backRYDelay;
        
        int64_t causeId;
	};
    
	struct Skill : entityx::Component<Skill>
	{
        SkillObject* skill = nullptr;
        float elapse;
        int fromId;
        std::vector<int> m_playingSoundPool;
	    
        Skill(SkillObject* skill, int fromId) : skill(skill), elapse(0), fromId(fromId) {}
        ~Skill();
	};
    
    _COMPONENT(HeroStrategy)
    {
        StrategyControl::HERO_CONTROL_TYPE type;
        
        HeroStrategy(StrategyControl::HERO_CONTROL_TYPE type = StrategyControl::HERO_STOP) : type(type) {}
    };

    _COMPONENT(ActorMode)
    {
        ModeGroup m_modeGroup;
        
        ActorMode(entityx::Entity& entity):
        m_modeGroup(entity)
        {

        }
        ~ActorMode()
        {

        }
    };
    
    _COMPONENT(Combo)
    {
        int m_comboTimes;
        float m_lastComboTime;

        Combo()
        {
        }
        ~Combo()
        {
        }
        
    };
    
#pragma mark -
#pragma mark Skill Component
    
    _COMPONENT(SkillTileCenter)
    {
        int tileZ, tileX;
        
        SkillTileCenter(int tileX, int tileZ) : tileX(tileX), tileZ(tileZ) {}
    };
    
    _COMPONENT(SkillTileTarget)
    {
        int tileZ, tileX;
        
        SkillTileTarget(int tileX, int tileZ) : tileX(tileX), tileZ(tileZ) {}
    };
    
    _COMPONENT(SkillTileResults)
    {
        BattleConfig::TileResults results;
        
        SkillTileResults(const BattleConfig::TileResults& fromResults)
        {
            for (auto* pTile : fromResults.tiles)
            {
                results.add(pTile->tileZ, pTile->tileX);
            }
        }
    };
    
    _COMPONENT(SkillTargetResults)
    {
        std::vector<int> targets;
        
        SkillTargetResults(const std::vector<int>& targets) : targets(targets) {}
    };
    
    _COMPONENT(SkillDuration)
    {
        float duration;
        
        SkillDuration(float duration) : duration(duration) {}
    };
    
    _COMPONENT(SkillReduceEnemyMp)
    {
        float actualReduceValue;
        
        SkillReduceEnemyMp(float actualReduceValue) : actualReduceValue(actualReduceValue) {}
    };
};

#endif
