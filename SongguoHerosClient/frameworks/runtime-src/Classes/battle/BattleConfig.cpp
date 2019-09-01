//
//  BattleConfig.cpp
//  TestAnimation
//
//  Created by fuchenhao on 8/1/14.
//
//

#include "BattleConfig.h"
#include "SkillManager.h"
#include "EnumParser.h"

BattleConfig* BattleConfig::s_sharedBattleConfig = nullptr;

BattleConfig* BattleConfig::getInstance()
{
	if(!s_sharedBattleConfig)
	{
		s_sharedBattleConfig = new BattleConfig();
	}
	return s_sharedBattleConfig;
}

BattleConfig::BattleConfig()
{
	m_animationNames[ANI_RUN] = std::string("animation_run");
	m_animationNames[ANI_ATTACK1] = std::string("animation_attack1");
	m_animationNames[ANI_ATTACK2] = std::string("animation_attack2");
	m_animationNames[ANI_ATTACK3] = std::string("animation_attack3");
	m_animationNames[ANI_IDLE] = std::string("animation_idle");
	m_animationNames[ANI_DEATH] = std::string("animation_death");
	m_animationNames[ANI_DEATH1] = std::string("animation_death1");
	m_animationNames[ANI_DEATH2] = std::string("animation_death2");
    m_animationNames[ANI_HIT] = std::string("animation_hit");
    m_animationNames[ANI_DIZZY] = std::string("animation_dizzy");
    m_animationNames[ANI_DEFENCE] = std::string("animation_defence");
}

BattleConfig::~BattleConfig()
{
    clearBattleFieldConfig();
}

void BattleConfig::clearBattleFieldConfig()
{
    if (m_pBattleFieldConfig != nullptr)
    {
        CC_SAFE_DELETE(m_pBattleFieldConfig);
    }
}

void BattleConfig::initBattleFieldConfig(const std::string& config)
{
    rapidjson::Document data;
    data.Parse<rapidjson::kParseDefaultFlags>(config.c_str());
    
    clearBattleFieldConfig();
    m_pBattleFieldConfig = new BattleConfig::BattleFieldConfig();
    
    auto& battleFieldConfig = *m_pBattleFieldConfig;
    
    //{"side":"left", "role":"1", "content":"本官姓吕名布字奉先，谁能谁上别BB"},
    if(data.HasMember("dialogue") && data["dialogue"].IsObject())
    {
        if(data["dialogue"].HasMember("enter"))
        {
            for(int i = 0; i < data["dialogue"]["enter"].Size(); i++)
            {
                auto* dialogue = new Dialogue();
                parseDialogueConfig(dialogue, data["dialogue"]["enter"][i]);
                battleFieldConfig.enter.push_back(dialogue);
            }
        }
        if(data["dialogue"].HasMember("win"))
        {
            for(int i = 0; i < data["dialogue"]["win"].Size(); i++)
            {
                auto* dialogue = new Dialogue();
                parseDialogueConfig(dialogue, data["dialogue"]["win"][i]);
                battleFieldConfig.win.push_back(dialogue);
            }
        }
        if(data["dialogue"].HasMember("lose"))
        {
            for(int i = 0; i < data["dialogue"]["lose"].Size(); i++)
            {
                auto* dialogue = new Dialogue();
                parseDialogueConfig(dialogue, data["dialogue"]["lose"][i]);
                battleFieldConfig.lose.push_back(dialogue);
            }
        }
    }
    
    parseTeamConfig(battleFieldConfig.myTeam, data["myTeam"]);
    parseTeamConfig(battleFieldConfig.enemyTeam, data["enemyTeam"]);
    
    parseBaseConfig(battleFieldConfig.propertyMin, data["propertyMin"]);
    parseBaseConfig(battleFieldConfig.propertyMax, data["propertyMax"]);
    
    if(data.HasMember("weather"))
    {
        battleFieldConfig.weather = data["weather"].GetString();
    }
    
    if(data.HasMember("slowTime"))
    {
        battleFieldConfig.slowTime = data["slowTime"].GetDouble();
    }
    else
    {
        battleFieldConfig.slowTime = 2.0;
    }

    battleFieldConfig.totalTime = data["totalTime"].GetInt();
    battleFieldConfig.xOffset = data["xOffset"].GetInt();
    battleFieldConfig.CSDRFloorCoefficient = data["CSDRFloorCoefficient"].GetDouble();
    battleFieldConfig.background = data["background"].GetString();
    battleFieldConfig.leftSoldierAssetStyle = data["leftSoldierAssetStyle"].GetString();
    battleFieldConfig.rightSoldierAssetStyle = data["rightSoldierAssetStyle"].GetString();
    battleFieldConfig.leftSoldierIcon = data["leftSoldierIcon"].GetString();
    battleFieldConfig.rightSoldierIcon = data["rightSoldierIcon"].GetString();
    battleFieldConfig.battleType = EnumParserBattleType().getValue(data["battleType"].GetString());
    battleFieldConfig.enterShow = data["enterShow"].GetString();
    battleFieldConfig.forceAutoFight = data.HasMember("foreAutoFight") && data["foreAutoFight"].GetBool();
}

void BattleConfig::parseTeamConfig(BattleConfig::TeamConfig& config, rapidjson::Value& data)
{
    config.formation = data["formation"].GetString();
    
    parseSoldierConfig(config.soldier, data["soldier"]);
    
    // Compatible with old hero config format
    if (data.HasMember("hero") && data["hero"].IsObject())
    {
        auto* pHeroConfig = new HeroConfig();
        parseHeroConfig(*pHeroConfig, data["hero"]);
        config.heros.push_back(pHeroConfig);
    }
    
    if (data.HasMember("heros") && data["heros"].IsArray())
    {
        auto& heros = data["heros"];
        for (int i = 0; i < heros.Size(); i++)
        {
            auto* pHeroConfig = new HeroConfig();
            parseHeroConfig(*pHeroConfig, heros[i]);
            config.heros.push_back(pHeroConfig);
        }
    }
}

void BattleConfig::parseDialogueConfig(BattleConfig::Dialogue* dialogue, rapidjson::Value& data)
{
    //{"side":"left", "role":"1", "content":"本官姓吕名布字奉先，谁能谁上别BB"},
    dialogue->side = (std::string(data["side"].GetString()) == "left") ? SIDE_LEFT : SIDE_RIGHT;
    dialogue->content = std::string(data["content"].GetString());
    dialogue->role = std::string(data["role"].GetString());
    dialogue->name = std::string(data["name"].GetString());
}


void BattleConfig::parseSoldierConfig(BattleConfig::SoldierConfig& config, rapidjson::Value& data)
{
    config.num = data["num"].GetInt();
    config.asset = data["asset"].GetString();
    config.soldierConfigData.id = data["id"].GetInt();
    config.soldierConfigData.soldierType = static_cast<BattleConfig::SoldierType>(data["soldierType"].GetInt());
    config.soldierConfigData.orderSpeed = data["orderSpeed"].GetInt();
    config.soldierConfigData.normalDamageType = static_cast<BattleConfig::NormalDamageType>(data["damageType"].GetInt());
    parseBaseConfig(config.base, data["base"]);
}

void BattleConfig::parseHeroConfig(BattleConfig::HeroConfig& config, rapidjson::Value& data)
{
    parseBaseConfig(config.base, data["base"]);
    
    config.heroConfigData.pos = data["pos"].GetString();
    config.heroConfigData.asset = data["asset"].GetString();
    config.heroConfigData.strategy = data["strategy"].GetString();
    config.heroConfigData.id = data["id"].GetInt();
    config.heroConfigData.star = data["star"].GetInt();
    config.heroConfigData.maxStar = data["maxStar"].GetInt();
    config.heroConfigData.maxSoldier = data["maxSoldier"].GetInt();
    config.heroConfigData.standOrder = data["standOrder"].GetInt();
    config.heroConfigData.level = data["level"].GetInt();
    config.heroConfigData.ERHPBase = data["ERHPBase"].GetInt();
    config.heroConfigData.ERKillBase = data["ERKillBase"].GetInt();
    config.heroConfigData.ERLoseBase = data["ERLoseBase"].GetInt();
    config.heroConfigData.abilityCD = data["abilityCD"].GetDouble();
    config.heroConfigData.spellCostReduction = data["spellCostReduction"].GetDouble();
    config.heroConfigData.normalDamageType = static_cast<BattleConfig::NormalDamageType>(data["damageType"].GetInt());
    config.heroConfigData.currentHP = (data.HasMember("currentHP") ? data["currentHP"].GetDouble() : config.base[BattleConfig::Property::HP]);
    config.heroConfigData.currentMP = (data.HasMember("currentMP") ? data["currentMP"].GetDouble() : config.base[BattleConfig::Property::MP]);
    
    config.clearSkills();
    
    if (data.HasMember("skills") && data["skills"].IsArray())
    {
        auto& skills = data["skills"];
        for (int i = 0; i < skills.Size(); i++)
        {
            parseSkillConfig(config, skills[i]);
        }
    }
}

void BattleConfig::parseSkillConfig(BattleConfig::HeroConfig& config, rapidjson::Value& data)
{
    CCLOG("init skill %s", data["config"].GetString());
    
    auto* pConfig = SkillManager::getInstance()->getSkillConfig(data["config"].GetString());
    if (pConfig != nullptr)
    {
        int level = data["level"].GetInt();
        if (level > 0)
        {
            auto* skillData = new SkillData();
            skillData->id = data["id"].GetInt();
            skillData->name = data["name"].GetString();
            skillData->pConfig = pConfig;
            skillData->level = level;
            skillData->mpCost = data["mpCost"].GetInt();
            skillData->energyCost = data["energyCost"].GetDouble();
            skillData->energySpeed = data["energySpeed"].GetDouble();
            skillData->energyStart = data["energyStart"].GetDouble();
            
            skillData->energy = skillData->energyStart;
            
            if (data.HasMember("soldier"))
            {
                skillData->pSoldier = new SoldierConfig();
                parseSoldierConfig(*skillData->pSoldier, data["soldier"]);
            }
            
            config.skills.push_back(skillData);
        }
    }
}

void BattleConfig::parseBaseConfig(std::array<float, BattleConfig::SIZE_OF_PROPERTY_NAME>& base, rapidjson::Value& data)
{
    base.fill(0);
    
    EnumParserPropertyName parser;
    auto &map = parser.enumMap;
    for (auto iter = map.begin(); iter != map.end(); iter++)
    {
        if (data.HasMember(iter->first.c_str()))
        {
            base[iter->second] = data[iter->first.c_str()].GetDouble();
        }
    }
}
