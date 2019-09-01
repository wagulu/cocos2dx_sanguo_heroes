//
//  BattleConfig.h
//  TestAnimation
//
//  Created by fuchenhao on 7/31/14.
//
//

#ifndef __BATTLE_CONFIG_H__
#define __BATTLE_CONFIG_H__

#include <string>
#include <array>
#include "cocostudio/CocoStudio.h"
#include "entityx.h"
#include "AICondition.h"
#include "AIAction.h"

using namespace std;

#define STAGE_WIDTH         90
#define STAGE_HEIGHT        20
#define HALF_STAGE_HEIGHT   10

#define STAGE_MIDDLE_X      20
#define STAGE_MIDDLE_Z      10
#define STAGE_UP_Z          16
#define STAGE_DOWN_Z        4
#define STAGE_MIDDLE_UP     14
#define STAGE_MIDDLE_DOWN   6
#define STAGE_DUEL_LEFT_X   40
#define STAGE_DUEL_RIGHT_X  50

#define GRID_WIDTH          80.0
#define GRID_HEIGHT         80.0
#define HALF_GRID_WIDTH     40.0
#define HALF_GRID_HEIGHT    40.0

#define ATTACK_DISTANCE_MELEE_MELEE    (GRID_WIDTH * 1.15)
#define ATTACK_DISTANCE_HERO_MELEE     (GRID_WIDTH * 1.65)
#define ATTACK_DISTANCE_HERO_HERO_NEAR (GRID_WIDTH * 2.15)
#define ATTACK_DISTANCE_HERO_HERO_FAR  (GRID_WIDTH * 3.15)

#define SOLDIER_MELEE_HEIGHT  80.0f
#define HERO_HEIGHT           160.0f

#define CHARACTER_SCALE     0.5

#define ARROW_BIAS          50.0
#define ARROW_ATTACK_DIS    30

#define MAP_FRAME           1

#define LEFT_SOLDIER        1
#define RIGHT_SOLDIER       1001
#define LEFT_HERO           2001
#define RIGHT_HERO          2101
#define ACTOR_ID_MAX        2200

#define COLOR_SOLDIER_1     0,0,3,3
#define COLOR_SOLDIER_2     3,0,3,3
#define COLOR_GENERAL_1     6,0,3,3
#define COLOR_GENERAL_2     0,3,3,3

// camera start
#define CAMERA_MIN_X        400.0
#define CAMERA_MAX_X        6800.0
#define CAMERA_MIN_Z        -120.0
#define CAMERA_MAX_Z        700.0

#define CONF_INIT_CAMERA_ROTATE     -14.0f
#define CONF_INIT_CAMERA_Y          370.0f
#define CONF_INIT_CAMERA_Z          700.0f

#define FOLLOW_DISTANCE     90.0f
#define NEAR_DISTANCE       30.0f
// camera end

//#define SHOW_DISTANCE       0
//#define SHOW_DISTANCE       800

//#define SHOW_SPEED          200

#define FREE_MODE           false

class BattleConfig
{
public:
    
    enum Side
    {
        SIDE_LEFT,
        SIDE_RIGHT,
        SIDE_ALL
    };
    
    enum FaceTo
    {
        FACE_TO_RIGHT,
        FACE_TO_LEFT
    };
    
    enum EntityType
    {
        ENTITY_SOLDIER,
        ENTITY_HERO
    };
    
    enum class BattleType
    {
        PVE,
        PVP,
        DUEL
    };
    
    enum class SoldierType
    {
        MELEE = 1,
        ARCHER,
        BLOCK,
        VIRUS
    };

    enum SoldierState
    {
        STATE_ATTACK,
        STATE_IDLE,
        STATE_PLAY_ANIMATION,
        STATE_DIZZY,
        STATE_PROTECT,
//        STATE_PROTECT_WIND,
//        STATE_PROTECT_RANDOM_RUN,
        STATE_GENERAL,
        STATE_SOLDIER, //move
//        STATE_MOVE_TO_TARGET_WIND, //wind
        STATE_DEPART,
        STATE_CENTER,
//        STATE_CENTER_WIND,
//        STATE_CENTER_WAIT,
        STATE_BACK,
        STATE_TARGET,
        STATE_LOST,
        STATE_DEAD,
        STATE_BATTLE_END,
        STATE_CHANGE_TO_BLOCK,
        STATE_CHANGE_TO_GHOST,
        STATE_ANCHOR_ATTACK,
        STATE_FADEIN,
        STATE_HIT_BACK
    };
    
    enum GeneralState
    {
        GENERAL_IDLE,
        GENERAL_PLAY_ANIMATION,
        GENERAL_RUN,
        GENERAL_ATTACK,
        GENERAL_DIZZY,
        GENERAL_DEAD,
        GENERAL_CHANGE_LINE,
        GENERAL_MOVE_TO_HERO_BACK,
        GENERAL_MOVE_TO_TARGET_TILE,
        GENERAL_DASH,
        GENERAL_BATTLE_END
    };
    
	enum AnimationType
	{
		ANI_RUN,
		ANI_ATTACK1,
		ANI_ATTACK2,
		ANI_ATTACK3,
		ANI_IDLE,
		ANI_DEATH,
        ANI_DEATH1,
        ANI_DEATH2,
        ANI_DIZZY,
        ANI_DEFENCE,
        ANI_HIT
	};
    
    enum ChangeGasType
    {
        GAS_KILLSOLDIER,//杀死对方士兵
        GAS_SOLDIERKILLED,//己方士兵被击杀
        GAS_ATTACKBYSOLDIER//武将被士兵攻击
    };
    
    enum SkillArrowType
    {
        ARROW_SKILL_LINE,//default
        ARROW_SKILL_VERTICAL,//垂直方式
        ARROW_SKILL_CURVE,
        ARROW_SKILL_CIRCLE,//围绕持续
        ARROW_SKILL_LINE_TRACK
    };
    
    enum MoveArrowResult
    {
        ARROW_NO_HIT,
        ARROW_HIT,
        ARROW_OUT
    };
    
    enum MoveSoldierResult
    {
        MOVE_SOLDIER_ARRIVE,
        MOVE_SOLDIER_NEXT,
        MOVE_SOLDIER_BLOCK
    };
    
    enum TargetPosition
    {
        POSITION_LEFT,
        POSITION_RIGHT,
        POSITION_NONE
    };
    
    enum AIState
    {
        AI_WAITING,// wait condition ok
        AI_DELAY,// wait action delay
        AI_WORKING,// try to implement action
        AI_COMPLETE
    };
    
    enum class AttackType
    {
        NORMAL,  //普通攻击
        SKILL    //技能攻击
    };
    
    enum AttackTargetType
    {
        ATTACK_TARGET,  //基于目标的攻击
        ATTACK_POSITION //基于位置的攻击
    };
    
    enum class NormalDamageType
    {
        UNKNOWN,
        PHYSICAL = 1,   //普通物理攻击，基于attack属性计算伤害
        MAGIC           //普通法术攻击，基于power属性计算伤害
    };
    
    enum DamageType
    {
        DAMAGE_PHYSICAL,//基于物理伤害
        DAMAGE_MAGIC,   //基于法术伤害
        DAMAGE_HOLY,    //真实伤害
        DAMAGE_KILL,    //必死攻击
        DAMAGE_CURE,    //治疗
        DAMAGE_PERCENT, //基于百分比伤害
        DAMAGE_UNKNOWN  //未知伤害，不做任何处理
    };
    
    enum class DamageSide
    {
        ENEMY,
        SELF,
        BOTH
    };
    
    enum DamageEntityType
    {
        DAMAGE_ENTITY_SOLDIER,
        DAMAGE_ENTITY_HERO,
        DAMAGE_ENTITY_ALL
    };
    
    enum class SkillType
    {
        POSITIVE,
        NEGATIVE
    };
    
    enum class SkillTarget
    {
        SELF_HERO,
        SELF_HERO_ALL,
        SELF_SOLDIER,
        SELF_ALL,
        OPP_HERO,
        OPP_HERO_ALL,
        OPP_SOLDIER,
        OPP_ALL,
        FRONT_NEARBY_ENEMY
    };
    
    enum Property
    {
        ARROW_SPEED,              //普攻波动速度	   arrowSpeed
        SPEED,                    //移动速度值	   speed
        STRIKE_CD,                //攻击速度值	   strikeCD
        DR,                       //伤害减免率	   DR
        HP,                       //HP值     	   HP
        MP,                       //MP值     	   MP
        ATTACK,                   //物理攻击值	   attack
        POWER,                    //法术攻击值	   power
        ATK_VAMP,                 //普攻生命回复	   atkVamp
        ABILITY_VAMP,             //技能生命回复	   abilityVamp
        HOLY_DAMAGE,              //真实伤害值	   holyDamage
        ATK_HIT_RATE,             //普攻命中率	   atkHitRate
        ABILITY_HIT_RATE,         //技能命中率	   abilityHitRate
        ATK_DODGE_RATE,           //普攻闪避率	   atkDodgeRate
        ABILITY_DODGE_RATE,       //技能闪避率	   abilityDodgeRate
        ATK_CSR,                  //普攻暴击率	   atkCSR
        ABILITY_CSR,              //技能暴击率	   abilityCSR
        ATK_ANTI_CSR,             //普攻爆抗率	   atkAntiCSR
        ABILITY_ANTI_CSR,         //技能爆抗率	   abilityAntiCSR
        PHYSICAL_CSDR,            //物理爆伤率	   physicalCSDR
        SPELL_CSDR,               //法术爆伤率	   spellCSDR
        PHYSICAL_RESILIENCE,      //物理韧性率	   physicalResilience
        SPELL_RESILIENCE,         //法术韧性率	   spellResilience
        ARPR,                     //物理护甲穿透率	   ArPR
        SRPR,                     //法术防御穿透率	   SRPR
        PHYSICAL_DR,              //物理伤害减免率	   physicalDR
        SPELL_DR,                 //法术伤害减免率	   spellDR
        ATK_VAMP_RATE,            //普攻吸血率	   atkVampRate
        ABILITY_VAMP_RATE,        //技能吸血率	   abilityVampRate
        HEALING_BONUS,            //治疗效果提高率   healingBonus
        PHYSICAL_SHIELD,          //物理护盾        physicalShield
        SPELL_SHIELD,             //法术护盾        spellShield
        SHIELD,                   //物理和法术护盾   shield
        ATK_HOLY_DAMAGE,          //普攻附加真实伤害 atkHolyDamage
        ABILITY_HOLY_DAMAGE,      //技能附加真实伤害 abilityHolyDamage
        SIZE_OF_PROPERTY_NAME,
        ALL_PROPERTIES
    };
    
    struct HeroConfigData
    {
        int id;                                          //武将id
        int maxSoldier;                                  //最大带兵数
        int standOrder;                                  //武将站位顺序，越大表示站位越靠前
        int level;                                       //武将等级
        int ERHPBase;                                    //己方武将被攻击回气值
        int ERKillBase;                                  //杀敌回气值
        int ERLoseBase;                                  //损失回气值
        float abilityCD;                                 //按百分比降低技能冷却时间
        float spellCostReduction;                        //按百分比降低释放技能的消耗MP
        float currentHP;                                 //开战时的初始HP
        float currentMP;                                 //开战时的初始MP
        BattleConfig::NormalDamageType normalDamageType; //普攻伤害类型
        int star;                                        //星级
        int maxStar;                                     //最大星级
        int heroType;                                    //力 智 统
        std::string asset;
        std::string pos;                                 //f1,b1
        std::string strategy;                            //ai
    };
    
    struct SoldierConfigData
    {
        int id;                                          //兵种id
        BattleConfig::SoldierType soldierType;           //兵种类型，melee or archer
        float orderSpeed;                                //兵种指令移动速度
        BattleConfig::NormalDamageType normalDamageType; //普攻伤害类型
    };
    
    struct Tile
    {
        int tileZ;
        int tileX;
        
        Tile(int tileZ, int tileX) : tileZ(tileZ), tileX(tileX) {}
    };
    
    struct TileResults
    {
        std::vector<Tile*> tiles;
        
        void add(int tileZ, int tileX)
        {
            tiles.push_back(new Tile(tileZ, tileX));
        }
        
        ~TileResults()
        {
            for (auto* pTile : tiles)
            {
                delete pTile;
            }
            tiles.clear();
        };
    };
    
    // for skills
    struct ParamData
    {
        std::string* s = nullptr;
        double d;
        bool b;
        
        ParamData() {}
        ParamData(std::string* s) : s(s) {}
        ParamData(double d) : d(d) {}
        ParamData(bool b) : b(b) {}
        ParamData(const ParamData& source)
        {
            if (source.s != nullptr) s = new std::string(*source.s);
            d = source.d;
            b = source.b;
        }
        
        ~ParamData()
        {
            CC_SAFE_DELETE(s);
        }
    };
    
    struct EffectLayer
    {
        std::string layerName;
        int layerType;
        int layerZOrder;
    };
    
    struct EffectGroup
    {
        std::string asset;
        std::vector<EffectLayer*> effect;
        
        ~EffectGroup()
        {
            for(auto& i : effect)
            {
                delete i;
            }
            effect.clear();
        };
    };
    
    struct EffectMatrix
    {
        std::vector<EffectGroup*> effect;
        
        EffectGroup* getEffect()
        {
            return effect[cocos2d::random() % effect.size()];
        };
        
        ~EffectMatrix()
        {
            for(auto& i : effect)
            {
                delete i;
            }
            effect.clear();
        };
    };
    
    struct SkillEffect
    {
        std::string asset;
        std::map<std::string, EffectMatrix*> effect;
        
        SkillEffect()
        {
            asset = "";
        }
        
        ~SkillEffect()
        {
            for(auto& i : effect)
            {
                delete i.second;
            }
            effect.clear();
        }
        
        EffectGroup* getEffect(const std::string& name)
        {
            auto result = effect.find(name);
            if(result != effect.end())
            {
                return result->second->getEffect();
            }
            return nullptr;
        }
    };
    
    struct DamageData
    {
        DamageType damageType = DamageType::DAMAGE_UNKNOWN;
        DamageSide damageSide = DamageSide::ENEMY;
        DamageEntityType damageEntityType = DamageEntityType::DAMAGE_ENTITY_ALL;
        float damageDelay = 0;
        float damageBase = 0;
        float damageStep = 0; //increase some damage per level
        float damageFactor = 0;
        float damagePercent = 0;
        float damageHoly = 0;
        float deadSpeedH;
        float deadSpeddV;
        float deadSpeedHDecay;
        float deadSpeedVDecay;
        float gravity;
        bool showHpFlyText = true;
        bool showDefaultSkillHitEffect = false;
        
        std::string *triggerSkillActionOnAttack = nullptr;
        std::string *deadAnimation = nullptr;
    };
    
    struct DamagePosition
    {
        float x, y, z;
        float speedH;
        float speedV;
        float gravity;
        EffectGroup* group = nullptr;
        
        DamagePosition()
        {
            x = y = z = 0;
            speedH = 0;
            speedV = 0;
            gravity = 1000;
        }
        
        bool isZero()
        {
            return x == 0 && y == 0 && z == 0;
        }
    };
    
    struct CameraShakeData
    {
        float delay, duration, amplitude;
        int mode, interval;
        bool damping;
        BattleConfig::Side side;
        
        CameraShakeData(float duration = 0.5f, float amplitude = 10.0f, int mode = 2, int interval = 2, float delay = 0.0f, bool damping = true, BattleConfig::Side side = BattleConfig::SIDE_LEFT)
            : duration(duration), amplitude(amplitude), mode(mode), interval(interval), delay(delay), damping(damping), side(side) {}
    };
    
    struct SkillArrowData
    {
        SkillArrowData(BattleConfig::SkillArrowType type,
                       BattleConfig::Side side,
                       BattleConfig::FaceTo face,
                       float speed,
                       entityx::Entity::Id id,
                       int skillId,
                       int radius = 0,
                       int range = -1,
                       int hp = -1,
                       const std::string trigger = ""):
        type(type),
        side(side),
        face(face),
        speed(speed),
        id(id),
        skillId(skillId),
        radius(radius),
        range(range),
        hp(hp),
        trigger(trigger)
        {
        }
        BattleConfig::SkillArrowType type;
        float speed;
        BattleConfig::Side side;
        BattleConfig::FaceTo face;
        entityx::Entity::Id id;
        int skillId;
        int radius;
        int range;
        int hp;
        float breakDistance;
        float targetZ;
        std::string trigger;
        
        float phase;
        float amplitude;
        float frequency;
    };
    
    struct CameraActionConfig
    {
        std::string name;
        std::map<std::string, ParamData*> config;
        
        ~CameraActionConfig()
        {
            for(auto& it : config)
            {
                delete it.second;
            }
            config.clear();
        }
        
        bool hasMember(const std::string& key)
        {
            return config.find(key) != config.end();
        }
    };
    
    struct CameraActions
    {
        std::string file;
        std::vector<CameraActionConfig*> actionsX;
        std::vector<CameraActionConfig*> actionsY;
        std::vector<CameraActionConfig*> actionsZ;
        std::vector<CameraActionConfig*> actionsR3d;
        std::vector<CameraActionConfig*> actionsR2d;
        
        ~CameraActions()
        {
            for(auto& action : actionsX)
            {
                delete action;
            }
            actionsX.clear();
            
            for(auto& action : actionsY)
            {
                delete action;
            }
            actionsY.clear();
            
            for(auto& action : actionsZ)
            {
                delete action;
            }
            actionsZ.clear();
            
            for(auto& action : actionsR3d)
            {
                delete action;
            }
            actionsR3d.clear();
            
            for(auto& action : actionsR2d)
            {
                delete action;
            }
            actionsR2d.clear();
        }
    };
    
    struct SkillActionData
    {
        std::string name;
        std::string trigger;
        float startTime;     // -1:不按时间点自动触发，>=0:按时间点自动触发
        float lifeTime;      // -1:不手动调用remove则永久存在，>=0:触发后持续一段时间自动销毁
        DamageData damageData;
        std::string* pTriggerOnRemove = nullptr;
        
        std::map<std::string, ParamData*> config;
        
        ~SkillActionData()
        {
            for(auto& it : config)
            {
                delete it.second;
            }
            config.clear();
            
            CC_SAFE_DELETE(damageData.triggerSkillActionOnAttack);
            CC_SAFE_DELETE(damageData.deadAnimation);
            CC_SAFE_DELETE(pTriggerOnRemove);
            CCLOG("skill action data remove");
        }
        
        bool hasMember(const std::string& key)
        {
            return config.find(key) != config.end();
        }
    };
    
    struct Formula
    {
        std::string formula;
        std::string property;
        float max, min, divide, step, base, factor;
    };
    
    struct SkillFormula
    {
        std::map<std::string, Formula*> formulas;

        ~SkillFormula()
        {
            for(auto& iter : formulas)
            {
                delete iter.second;
            }
            formulas.clear();
        }
        
        Formula* getFormula(const std::string& name)
        {
            auto result = formulas.find(name);
            if(result != formulas.end())
            {
                return result->second;
            }
            return nullptr;
        }
    };
    
    struct SkillCameraActions
    {
        std::map<std::string, CameraActions*> actions;
        
        ~SkillCameraActions()
        {
            for(auto& iter : actions)
            {
                delete iter.second;
            }
            actions.clear();
        }
        
        CameraActions* getCameraActions(const std::string& name)
        {
            auto result = actions.find(name);
            if(result != actions.end())
            {
                return result->second;
            }
            return nullptr;
        }
    };
    
    struct HeroDizzyConfig;
    struct SkillDizzy
    {
        std::map<std::string, HeroDizzyConfig*> config;
        
        ~SkillDizzy()
        {
            for (auto& iter : config)
            {
                delete iter.second;
            }
            config.clear();
        }
        
        HeroDizzyConfig* getConfig(const std::string& name)
        {
            auto iter = config.find(name);
            if (iter != config.end())
            {
                return iter->second;
            }
            return nullptr;
        }
    };
    
    struct SkillConfigData
    {
        SkillType type;
        std::vector<SkillActionData*> actions;
        SkillEffect* effect;
        SkillFormula* formula;
        SkillDizzy* dizzy;
        SkillCameraActions* cameraActions;
        
        SkillConfigData()
        {
            effect = new SkillEffect();
            formula = new SkillFormula();
            dizzy = new SkillDizzy();
            cameraActions = new SkillCameraActions();
            type = SkillType::POSITIVE;
        }
        
        ~SkillConfigData()
        {
            for(auto* action : actions)
            {
                delete action;
            }
            actions.clear();
            
            delete effect;
            delete formula;
            delete dizzy;
            delete cameraActions;
        }
    };
    
    struct BuffData
    {
        int targetId;
        BattleConfig::Property targetProperty;
        float changeAbsolute;
        float changePercent;
        float changeFinal;
        float lifeTime;
        bool lock;
        std::string buffIcon;
        
        BuffData(int targetId, BattleConfig::Property targetProperty, float changeAbsolute = 0, float changePercent = 0, float lifeTime = 0, bool lock = false) : targetId(targetId), targetProperty(targetProperty), changeAbsolute(changeAbsolute), changePercent(changePercent), lifeTime(lifeTime), lock(lock) { buffIcon = ""; }
    };
    
    struct AttackData
    {
    public:
        //发起者Id
        int fromId;
        //施放前等待
        float delay;
        //施放间隔
        float interval;
        //施放次数
        int loop;
        //是否是第一下伤害
        bool isFirstHit;
        //技能Id
        int skillId;
        //用于计算伤害的数据
        DamageData damageData;
        //命中时击飞的数据
        DamagePosition damagePosition;
        //技能EntityId
        entityx::Entity::Id skillEntityId;
        //最终伤害值
        float finalHpChanged = 0;
        
        AttackData(int fromId, float delay = 0, int loop = 1, float interval = 0) : m_type(AttackType::NORMAL), fromId(fromId), delay(delay), loop(loop), interval(interval), isFirstHit(true) {}
        
        void setTargetId(int targetId)
        {
            m_targetId = targetId;
            m_targetType = ATTACK_TARGET;
        }
    
        void setTargetPos(int tileZ, int tileX)
        {
            m_targetTileZ = tileZ;
            m_targetTileX = tileX;
            m_targetType = ATTACK_POSITION;
        }
        
        void setSkillData(int sId, const DamageData& damage, const entityx::Entity::Id& entityId)
        {
            skillId = sId;
            damageData = damage;
            skillEntityId = entityId;
            m_type = AttackType::SKILL;
            
            if (damageData.damageDelay > 0) delay = damageData.damageDelay;
        }
        
        inline AttackType getAttackType() const
        {
            return m_type;
        }
        inline AttackTargetType getAttackTargetType() const
        {
            return m_targetType;
        }
        inline int getTargetId() const
        {
            return m_targetId;
        }
        inline int getTargetTileZ() const
        {
            return m_targetTileZ;
        }
        inline int getTargetTileX() const
        {
            return m_targetTileX;
        }
        
    private:
        AttackType m_type;
        AttackTargetType m_targetType;
        //目标Id
        int m_targetId;
        //目标位置
        int m_targetTileZ;
        int m_targetTileX;
    };
    
    struct AIData
    {
        BattleConfig::AIState state;
        std::vector<AICondition*> *conditions;
        AIAction* action;
        bool repeated;
        float elapse;
        
        AIData(std::vector<AICondition*> *conditions, AIAction* action, bool repeated) : conditions(conditions), action(action), repeated(repeated)
        {
            state = BattleConfig::AI_WAITING;
            elapse = 0;
            for(auto condition : *conditions)
            {
                condition->configure();
            }
            action->configure();
        }
        
        ~AIData()
        {
            for(auto condition : *conditions)
            {
                delete condition;
            }
            conditions->clear();
            delete conditions;
            delete action;
        }
    };
    
    struct CameraAction
    {
        float fromX;
        float toX;
        float fromY;
        float toY;
        float fromZ;
        float toZ;
        float fromRotate;
        float toRotate;
        
        float time;
        float delay;
        float elapse;
        
        int tweenMode;
        int nextCameraState;
    };
    
    struct CameraActionData
    {
        float toData;
        
        float time;
        float delay;
        float elapse;
        
        int tweenMode;
        int nextCameraState;
    };
    
    struct HeroDizzyActionConfig
    {
        std::string name;
        std::map<std::string, ParamData> config;
        
        bool hasMember(const std::string& key)
        {
            return config.find(key) != config.end();
        }
    };
    
    struct HeroDizzyConditionConfig
    {
        std::string name;
        bool required = true;
        std::map<std::string, ParamData> config;
        
        bool hasMember(const std::string& key)
        {
            return config.find(key) != config.end();
        }
    };
    
    struct HeroDizzyConfig
    {
        bool allowBreakByAnotherDizzy = true;
        std::vector<HeroDizzyActionConfig> actions;
        std::vector<HeroDizzyConditionConfig> conditions;
        
        HeroDizzyActionConfig* findAction(const std::string& name)
        {
            for (auto& config : actions)
            {
                if (config.name == name) return &config;
            }
            return nullptr;
        }
        
        HeroDizzyConditionConfig* findCondition(const std::string& name)
        {
            for (auto& config : conditions)
            {
                if (config.name == name) return &config;
            }
            return nullptr;
        }
    };
    
    struct HeroDizzyData
    {
        entityx::Entity::Id skillEntityId;
        float dizzyTime;
    };
    
    struct SoldierConfig
    {
        std::string asset;
        int num;
        BattleConfig::SoldierConfigData soldierConfigData;
        std::array<float, SIZE_OF_PROPERTY_NAME> base;
        
        const std::string getAssetName(const std::string& style) const
        {
            return "soldier_" + asset + "_" + style;
        }
    };
    
    struct SkillData
    {
        int id;
        std::string name;
        int level = 1;
        float energyCost = 0;
        float energySpeed = 0;
        float energyStart = 0;
        int mpCost = 0;
        SkillConfigData* pConfig = nullptr;
        SoldierConfig* pSoldier = nullptr;
        
        float energy = 0;
        bool isReady = false;
        bool isActive = false;
        bool canBreak = true;
        
        SkillData() {}
        ~SkillData() { CC_SAFE_DELETE(pSoldier); }
        
        inline void changeEnergy(float delta)
        {
            energy += delta;
            if (energy > energyCost) energy = energyCost;
            if (energy < 0) energy = 0;
        }
    };
    
    struct HeroConfig
    {
        BattleConfig::HeroConfigData heroConfigData;
        std::vector<BattleConfig::SkillData*> skills;
        std::array<float, SIZE_OF_PROPERTY_NAME> base;
        
        bool hasAI() { return heroConfigData.strategy.length() > 0; };
        
        void clearSkills()
        {
            for (auto* s : skills)
            {
                delete s;
            }
            skills.clear();
        }
        
        const std::string getAssetName() const
        {
            return "hero_" + heroConfigData.asset;
        }
        
        ~HeroConfig()
        {
            clearSkills();
        }
    };
    
    struct TeamConfig
    {
        SoldierConfig soldier;
        std::vector<HeroConfig*> heros;
        std::string formation;
        
        ~TeamConfig()
        {
            for (auto* pHeroConfig : heros)
            {
                delete pHeroConfig;
            }
            heros.clear();
        }
    };
    
    //{"side":"left", "role":"1", "content":"本官姓吕名布字奉先，谁能谁上别BB"},
    
    struct Dialogue
    {
        Side side;
        std::string role;
        std::string name;
        std::string content;
    };
    
    struct BattleFieldConfig
    {
        TeamConfig myTeam;
        TeamConfig enemyTeam;
        
        std::vector<Dialogue*> enter;
        std::vector<Dialogue*> win;
        std::vector<Dialogue*> lose;
        
        std::array<float, SIZE_OF_PROPERTY_NAME> propertyMin;
        std::array<float, SIZE_OF_PROPERTY_NAME> propertyMax;
        
        float slowTime;
        int totalTime;
        int xOffset;
        float CSDRFloorCoefficient;
        std::string enterShow;
        std::string background;
        std::string leftSoldierAssetStyle;
        std::string rightSoldierAssetStyle;
        std::string leftSoldierIcon = "";
        std::string rightSoldierIcon = "";
        std::string weather;
        BattleType battleType;
        bool forceAutoFight;
        
        TeamConfig& getTeam(Side side)
        {
            return (side == Side::SIDE_LEFT ? myTeam : enemyTeam);
        }
        
        BattleFieldConfig()
        {
            weather = "";
        }
        
        ~BattleFieldConfig()
        {
            for(auto dialogue : enter)
            {
                delete dialogue;
            }
            for(auto dialogue : win)
            {
                delete dialogue;
            }
            for(auto dialogue : lose)
            {
                delete dialogue;
            }
            enter.clear();
            win.clear();
            lose.clear();
        }
    };
    
    struct FormationConfig
    {
        std::vector<int> soldiers;
        std::map<std::string, int> heros;
        int validSoldiersNum;
        
        FormationConfig()
        {
            soldiers.assign(100, 0);
        }
        
        cocos2d::Vec3& getSoldierPos(int idx)
        {
            if (idx >= 0 && idx < soldiers.size()) return getPos(soldiers.at(idx));
            return getPos(-1);
        }
        
        cocos2d::Vec3& getHeroPos(const std::string& pos)
        {
            auto iter = heros.find(pos);
            if(iter != heros.end()) return getPos(iter->second);
            return getPos(-1);
        }
        
        cocos2d::Vec3& getPos(int pos)
        {
            static cocos2d::Vec3 tempPos;
            if (pos == -1) tempPos.set(-1, -1, -1);
            else tempPos.set(pos & 0xFF, 0, (pos >> 8) & 0xFF);
            return tempPos;
        }
    };

	static BattleConfig* getInstance();
    
	inline std::string& getAnimationName(int id) {return m_animationNames[id];};
    
    BattleConfig::BattleFieldConfig& getBattleFieldConfig() { return *m_pBattleFieldConfig; };
    
    void clearBattleFieldConfig();
    void initBattleFieldConfig(const std::string& config);
    void parseTeamConfig(BattleConfig::TeamConfig& config, rapidjson::Value& data);
    void parseSoldierConfig(BattleConfig::SoldierConfig& config, rapidjson::Value& data);
    void parseHeroConfig(BattleConfig::HeroConfig& config, rapidjson::Value& data);
    void parseSkillConfig(BattleConfig::HeroConfig& config, rapidjson::Value& data);
    void parseDialogueConfig(BattleConfig::Dialogue* dialogue, rapidjson::Value& data);
    void parseBaseConfig(std::array<float, SIZE_OF_PROPERTY_NAME>& base, rapidjson::Value& data);
    
	BattleConfig();
	~BattleConfig();

private:
    std::array<std::string, 11> m_animationNames;
    BattleConfig::BattleFieldConfig* m_pBattleFieldConfig = nullptr;
    
    static BattleConfig *s_sharedBattleConfig;
};

#endif
