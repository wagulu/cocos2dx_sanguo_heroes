//
//  EnumParser.h
//  sanguoClient
//
//  Created by fuchenhao on 4/6/15.
//
//

#ifndef sanguoClient_EnumParser_h
#define sanguoClient_EnumParser_h

#include <string>
#include <map>
#include "BattleConfig.h"
#include "ShakeSystem.h"

using namespace std;

template <typename EnumType>
struct EnumParser
{
    EnumParser() {};
    
    EnumType getValue(const string &value)
    {
        auto iter = enumMap.find(value);
        if (iter == enumMap.end())
        {
            CCLOG("EnumParser fail, can not find %s", value.c_str());
            throw runtime_error(value);
        }
        return iter->second;
    }
    
    virtual void derived() = 0;

    std::map<std::string, EnumType> enumMap;
    
    static std::map<EnumType, std::map<std::string, EnumType>> map;
};

struct EnumParserPropertyName : EnumParser<BattleConfig::Property>
{
    void derived() {};
    
    EnumParserPropertyName()
    {
        enumMap["arrowSpeed"] = BattleConfig::Property::ARROW_SPEED;
        enumMap["speed"] = BattleConfig::Property::SPEED;
        enumMap["strikeCD"] = BattleConfig::Property::STRIKE_CD;
        enumMap["DR"] = BattleConfig::Property::DR;
        enumMap["HP"] = BattleConfig::Property::HP;
        enumMap["MP"] = BattleConfig::Property::MP;
        enumMap["attack"] = BattleConfig::Property::ATTACK;
        enumMap["power"] = BattleConfig::Property::POWER;
        enumMap["atkVamp"] = BattleConfig::Property::ATK_VAMP;
        enumMap["abilityVamp"] = BattleConfig::Property::ABILITY_VAMP;
        enumMap["holyDamage"] = BattleConfig::Property::HOLY_DAMAGE;
        enumMap["atkHitRate"] = BattleConfig::Property::ATK_HIT_RATE;
        enumMap["abilityHitRate"] = BattleConfig::Property::ABILITY_HIT_RATE;
        enumMap["atkDodgeRate"] = BattleConfig::Property::ATK_DODGE_RATE;
        enumMap["abilityDodgeRate"] = BattleConfig::Property::ABILITY_DODGE_RATE;
        enumMap["atkCSR"] = BattleConfig::Property::ATK_CSR;
        enumMap["abilityCSR"] = BattleConfig::Property::ABILITY_CSR;
        enumMap["atkAntiCSR"] = BattleConfig::Property::ATK_ANTI_CSR;
        enumMap["abilityAntiCSR"] = BattleConfig::Property::ABILITY_ANTI_CSR;
        enumMap["physicalCSDR"] = BattleConfig::Property::PHYSICAL_CSDR;
        enumMap["spellCSDR"] = BattleConfig::Property::SPELL_CSDR;
        enumMap["physicalResilience"] = BattleConfig::Property::PHYSICAL_RESILIENCE;
        enumMap["spellResilience"] = BattleConfig::Property::SPELL_RESILIENCE;
        enumMap["ArPR"] = BattleConfig::Property::ARPR;
        enumMap["SRPR"] = BattleConfig::Property::SRPR;
        enumMap["physicalDR"] = BattleConfig::Property::PHYSICAL_DR;
        enumMap["spellDR"] = BattleConfig::Property::SPELL_DR;
        enumMap["atkVampRate"] = BattleConfig::Property::ATK_VAMP_RATE;
        enumMap["abilityVampRate"] = BattleConfig::Property::ABILITY_VAMP_RATE;
        enumMap["healingBonus"] = BattleConfig::Property::HEALING_BONUS;
        enumMap["physicalShield"] = BattleConfig::Property::PHYSICAL_SHIELD;
        enumMap["spellShield"] = BattleConfig::Property::SPELL_SHIELD;
        enumMap["shield"] = BattleConfig::Property::SHIELD;
        enumMap["atkHolyDamage"] = BattleConfig::Property::ATK_HOLY_DAMAGE;
        enumMap["abilityHolyDamage"] = BattleConfig::Property::ABILITY_HOLY_DAMAGE;
    };
};

struct EnumParserSkillTarget : EnumParser<BattleConfig::SkillTarget>
{
    void derived() {};
    
    EnumParserSkillTarget()
    {
        enumMap["SELF_HERO"] = BattleConfig::SkillTarget::SELF_HERO;
        enumMap["SELF_HERO_ALL"] = BattleConfig::SkillTarget::SELF_HERO_ALL;
        enumMap["SELF_SOLDIER"] = BattleConfig::SkillTarget::SELF_SOLDIER;
        enumMap["SELF_ALL"] = BattleConfig::SkillTarget::SELF_ALL;
        enumMap["OPP_HERO"] = BattleConfig::SkillTarget::OPP_HERO;
        enumMap["OPP_HERO_ALL"] = BattleConfig::SkillTarget::OPP_HERO_ALL;
        enumMap["OPP_SOLDIER"] = BattleConfig::SkillTarget::OPP_SOLDIER;
        enumMap["OPP_ALL"] = BattleConfig::SkillTarget::OPP_ALL;
        enumMap["FRONT_NEARBY_ENEMY"] = BattleConfig::SkillTarget::FRONT_NEARBY_ENEMY;
    };
};

struct EnumParserSkillArrowType : EnumParser<BattleConfig::SkillArrowType>
{
    void derived() {};
    
    EnumParserSkillArrowType()
    {
        enumMap["skill_arrow_line"] = BattleConfig::SkillArrowType::ARROW_SKILL_LINE;
        enumMap["skill_arrow_vertical"] = BattleConfig::SkillArrowType::ARROW_SKILL_VERTICAL;
        enumMap["skill_arrow_curve"] = BattleConfig::SkillArrowType::ARROW_SKILL_CURVE;
        enumMap["skill_arrow_circle"] = BattleConfig::SkillArrowType::ARROW_SKILL_CIRCLE;
        enumMap["skill_arrow_line_track"] = BattleConfig::SkillArrowType::ARROW_SKILL_LINE_TRACK;
    };
};

struct EnumParserShakeMode : EnumParser<ShakeSystem::ShakeMode>
{
    void derived() {};
    
    EnumParserShakeMode()
    {
        enumMap["SHAKE_NONE"] = ShakeSystem::ShakeMode::SHAKE_NONE;
        enumMap["SHAKE_X"] = ShakeSystem::ShakeMode::SHAKE_X;
        enumMap["SHAKE_Y"] = ShakeSystem::ShakeMode::SHAKE_Y;
        enumMap["SHAKE_Z"] = ShakeSystem::ShakeMode::SHAKE_Z;
        enumMap["SHAKE_BOTH_XY"] = ShakeSystem::ShakeMode::SHAKE_BOTH_XY;
        enumMap["SHAKE_BOTH_XYZ"] = ShakeSystem::ShakeMode::SHAKE_BOTH_XYZ;
    };
};

struct EnumParserDamageSide : EnumParser<BattleConfig::DamageSide>
{
    void derived() {};
    
    EnumParserDamageSide()
    {
        enumMap["ENEMY"] = BattleConfig::DamageSide::ENEMY;
        enumMap["SELF"] = BattleConfig::DamageSide::SELF;
        enumMap["BOTH"] = BattleConfig::DamageSide::BOTH;
    };
};

struct EnumParserBattleType : EnumParser<BattleConfig::BattleType>
{
    void derived() {};
    
    EnumParserBattleType()
    {
        enumMap["pve"] = BattleConfig::BattleType::PVE;
        enumMap["pvp"] = BattleConfig::BattleType::PVP;
        enumMap["duel"] = BattleConfig::BattleType::DUEL;
    };
};

#endif
