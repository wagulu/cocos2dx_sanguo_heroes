//
//  SkillConfig.cpp
//  Game
//
//  Created by fuchenhao on 2/26/15.
//
//

#include "SkillConfig.h"
#include "EnumParser.h"
#include "EnterShowControl.h"

SkillConfig::SkillConfig()
{
    m_path = "config/skill/";
}

SkillConfig::~SkillConfig()
{
    for(auto& skill : m_skills)
    {
        delete skill.second;
    }
    m_skills.clear();
}

void SkillConfig::initBuffEffect(const std::string& path)
{
    if(m_buffEffectLoaded) return;
    
    std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile(path);
    if (json.length() == 0) return;
    
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());

    std::string asset = doc["asset"].GetString();
    
    if(doc.HasMember("effect") && doc["effect"].IsObject())
    {
        auto& effect = doc["effect"];
        for(auto it = effect.MemberonBegin(); it != effect.MemberonEnd(); it++)
        {
            if(it->value.IsArray())
            {
                auto group = new BattleConfig::EffectGroup();
                group->asset = asset;
                
                for(int i = 0; i < it->value.Size(); i++)
                {
                    auto layer = new BattleConfig::EffectLayer();
                    group->effect.push_back(layer);
                    
                    auto& current = it->value[i];
                    
                    layer->layerName = std::string(current["name"].GetString());
                    layer->layerType = current["type"].GetInt();
                    layer->layerZOrder = current["z-order"].GetInt();
                }
                m_buffEffect.insert(std::pair<std::string, BattleConfig::EffectGroup*>(std::string(it->name.GetString()), group));
            }
        }
    }
    
    m_buffEffectLoaded = true;
}

BattleConfig::SkillConfigData* SkillConfig::getSkillConfig(const std::string& configName)
{
    auto result = m_skills.find(configName);
    if (result != m_skills.end()) return result->second;
    
    std::string json = cocos2d::FileUtils::getInstance()->getStringFromFile(m_path + configName + ".json");
    if (json.length() == 0) return nullptr;
    
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
    
    int size;
    
    auto skill = new BattleConfig::SkillConfigData();
    m_skills[configName] = skill;

    if(doc.HasMember("negative") && doc["negative"].GetBool())
    {
        skill->type = BattleConfig::SkillType::NEGATIVE;
    }
    if(doc.HasMember("actions") && doc["actions"].IsArray())
    {
        size = doc["actions"].Size();
        for(int i = 0; i < size; i++)
        {
            auto& value = doc["actions"][i];
            auto action = new BattleConfig::SkillActionData();
            skill->actions.push_back(action);
            
            if(value.HasMember("name"))
            {
                action->name = std::string(value["name"].GetString());
            }
            
            if(value.HasMember("start"))
            {
                action->startTime = value["start"].GetDouble();
            }
            else
            {
                action->startTime = -1;
            }
            
            if(value.HasMember("lifeTime"))
            {
                action->lifeTime = value["lifeTime"].GetDouble();
            }
            else
            {
                action->lifeTime = -1;
            }
            
            if(value.HasMember("trigger"))
            {
                action->trigger = value["trigger"].GetString();
            }
            
            if(value.HasMember("triggerOnRemove"))
            {
                action->pTriggerOnRemove = new std::string(value["triggerOnRemove"].GetString());
            }
            
            if(value.HasMember("damage") && value["damage"].IsObject())
            {
                auto& damage = value["damage"];
                
                if(damage.HasMember("type"))
                {
                    action->damageData.damageType = (BattleConfig::DamageType)damage["type"].GetInt();
                }
                if(damage.HasMember("side"))
                {
                    action->damageData.damageSide = (BattleConfig::DamageSide)damage["side"].GetInt();
                }
                if(damage.HasMember("entityType"))
                {
                    action->damageData.damageEntityType = (BattleConfig::DamageEntityType)damage["entityType"].GetInt();
                }
                if(damage.HasMember("delay"))
                {
                    action->damageData.damageDelay = damage["delay"].GetDouble();
                }
                if(damage.HasMember("base"))
                {
                    action->damageData.damageBase = damage["base"].GetInt();
                }
                if(damage.HasMember("step"))
                {
                    action->damageData.damageStep = damage["step"].GetInt();
                }
                if(damage.HasMember("factor"))
                {
                    action->damageData.damageFactor = damage["factor"].GetDouble();
                }
                if(damage.HasMember("percent"))
                {
                    action->damageData.damagePercent = damage["percent"].GetDouble();
                }
                if(damage.HasMember("holy"))
                {
                    action->damageData.damageHoly = damage["holy"].GetDouble();
                }
                if(damage.HasMember("speed_h"))
                {
                    action->damageData.deadSpeedH = damage["speed_h"].GetDouble();
                }
                if(damage.HasMember("speed_v"))
                {
                    action->damageData.deadSpeddV = damage["speed_v"].GetDouble();
                }
                if(damage.HasMember("speed_h_decay"))
                {
                    action->damageData.deadSpeedHDecay = damage["speed_h_decay"].GetDouble();
                }
                if(damage.HasMember("speed_v_decay"))
                {
                    action->damageData.deadSpeedVDecay = damage["speed_v_decay"].GetDouble();
                }
                if(damage.HasMember("gravity"))
                {
                    action->damageData.gravity = damage["gravity"].GetDouble();
                }
                if(damage.HasMember("dead_animation"))
                {
                    action->damageData.deadAnimation = new std::string(damage["dead_animation"].GetString());
                }
                if(damage.HasMember("trigger"))
                {
                    action->damageData.triggerSkillActionOnAttack = new std::string(damage["trigger"].GetString());
                }
                if(damage.HasMember("showHpFlyText"))
                {
                    action->damageData.showHpFlyText = damage["showHpFlyText"].GetBool();
                }
                if(damage.HasMember("showDefaultSkillHitEffect"))
                {
                    action->damageData.showDefaultSkillHitEffect = damage["showDefaultSkillHitEffect"].GetBool();
                }
            }
            
            if(value.HasMember("params") && value["params"].IsObject())
            {
                auto& params = value["params"];
                for(auto it = params.MemberonBegin(); it != params.MemberonEnd(); it++)
                {
                    auto param = new BattleConfig::ParamData();
                    if(it->value.IsString())
                    {
                        param->s = new std::string(it->value.GetString());
                    }
                    else if(it->value.IsNumber())
                    {
                        param->d = it->value.GetDouble();
                    }
                    else if(it->value.IsBool())
                    {
                        param->b = it->value.GetBool();
                    }
                    action->config.insert(std::pair<std::string, BattleConfig::ParamData*>(std::string(it->name.GetString()), param));
                }
            }
        }
    }

    if(doc.HasMember("formula") && doc["formula"].IsObject())
    {
        //max,min,divide,step,base,factor,propertyFactor
        //formula,property
        
        auto& formulas = doc["formula"];
        for(auto it = formulas.MemberonBegin(); it != formulas.MemberonEnd(); it++)
        {
            if(it->value.IsObject())
            {
                auto formula = new BattleConfig::Formula();
                skill->formula->formulas.insert(std::pair<std::string, BattleConfig::Formula*>(std::string(it->name.GetString()), formula));
                
                if(it->value.HasMember("max")) formula->max = it->value["max"].GetDouble();
                if(it->value.HasMember("min")) formula->min = it->value["min"].GetDouble();
                if(it->value.HasMember("divide")) formula->divide = it->value["divide"].GetDouble();
                if(it->value.HasMember("step")) formula->step = it->value["step"].GetDouble();
                if(it->value.HasMember("base")) formula->base = it->value["base"].GetDouble();
                if(it->value.HasMember("factor")) formula->factor = it->value["factor"].GetDouble();
                
                if(it->value.HasMember("formula")) formula->formula = std::string(it->value["formula"].GetString());
                if(it->value.HasMember("property")) formula->property = std::string(it->value["property"].GetString());
            }
        }
    }
    
    if(doc.HasMember("cameraActions") && doc["cameraActions"].IsObject())
    {
        auto& cameraActions = doc["cameraActions"];
        for(auto it = cameraActions.MemberonBegin(); it != cameraActions.MemberonEnd(); it++)
        {
            if(it->value.IsObject())
            {
                auto* actions = EnterShowControl::parseCameraActions(it->value);
                skill->cameraActions->actions.insert(std::pair<std::string, BattleConfig::CameraActions*>(std::string(it->name.GetString()), actions));
            }
        }
    }
    
    if(doc.HasMember("dizzy") && doc["dizzy"].IsObject())
    {
        const auto& dizzyJsonMap = doc["dizzy"];
        for(auto it = dizzyJsonMap.MemberonBegin(); it != dizzyJsonMap.MemberonEnd(); it++)
        {
            if(it->value.IsObject())
            {
                auto* pDizzyConfig = new BattleConfig::HeroDizzyConfig();
                skill->dizzy->config.insert(std::pair<std::string, BattleConfig::HeroDizzyConfig*>(std::string(it->name.GetString()), pDizzyConfig));
                
                const auto& dizzyJson = it->value;
                
                if (dizzyJson.HasMember("allowBreakByAnotherDizzy"))
                {
                    pDizzyConfig->allowBreakByAnotherDizzy = dizzyJson["allowBreakByAnotherDizzy"].GetBool();
                }
                
                if (dizzyJson.HasMember("action") && dizzyJson["action"].IsArray())
                {
                    const auto& dizzyActionJsonArray = dizzyJson["action"];
                    size = dizzyActionJsonArray.Size();
                    for (int i = 0; i < size; i++)
                    {
                        const auto& dizzyActionJson = dizzyActionJsonArray[i];
                        if (dizzyActionJson.IsObject())
                        {
                            BattleConfig::HeroDizzyActionConfig dizzyActionConfig;
                            
                            for (auto iterAction = dizzyActionJson.MemberonBegin(); iterAction != dizzyActionJson.MemberonEnd(); iterAction++)
                            {
                                const std::string& key = iterAction->name.GetString();
                                if (key == "name")
                                {
                                    dizzyActionConfig.name = iterAction->value.GetString();
                                }
                                else
                                {
                                    BattleConfig::ParamData param;
                                    if(iterAction->value.IsString())
                                    {
                                        param.s = new std::string(iterAction->value.GetString());
                                    }
                                    else if(iterAction->value.IsNumber())
                                    {
                                        param.d = iterAction->value.GetDouble();
                                    }
                                    else if(iterAction->value.IsBool())
                                    {
                                        param.b = iterAction->value.GetBool();
                                    }
                                    dizzyActionConfig.config.insert(std::pair<std::string, BattleConfig::ParamData>(key, param));
                                }
                            }
                            
                            pDizzyConfig->actions.push_back(dizzyActionConfig);
                        }
                    }
                }
                if (dizzyJson.HasMember("condition") && dizzyJson["condition"].IsArray())
                {
                    const auto& dizzyConditionJsonArray = dizzyJson["condition"];
                    size = dizzyConditionJsonArray.Size();
                    for (int i = 0; i < size; i++)
                    {
                        const auto& dizzyConditionJson = dizzyConditionJsonArray[i];
                        if (dizzyConditionJson.IsObject())
                        {
                            BattleConfig::HeroDizzyConditionConfig dizzyConditionConfig;
                            
                            for (auto iterCondition = dizzyConditionJson.MemberonBegin(); iterCondition != dizzyConditionJson.MemberonEnd(); iterCondition++)
                            {
                                const std::string& key = iterCondition->name.GetString();
                                if (key == "name")
                                {
                                    dizzyConditionConfig.name = iterCondition->value.GetString();
                                }
                                else if (key == "required")
                                {
                                    dizzyConditionConfig.required = iterCondition->value.GetBool();
                                }
                                else
                                {
                                    BattleConfig::ParamData param;
                                    if(iterCondition->value.IsString())
                                    {
                                        param.s = new std::string(iterCondition->value.GetString());
                                    }
                                    else if(iterCondition->value.IsNumber())
                                    {
                                        param.d = iterCondition->value.GetDouble();
                                    }
                                    else if(iterCondition->value.IsBool())
                                    {
                                        param.b = iterCondition->value.GetBool();
                                    }
                                    dizzyConditionConfig.config.insert(std::pair<std::string, BattleConfig::ParamData>(key, param));
                                }
                            }
                            
                            pDizzyConfig->conditions.push_back(dizzyConditionConfig);
                        }
                    }
                }
            }
        }
    }
    
    if(doc.HasMember("asset"))
    {
        skill->effect->asset = doc["asset"].GetString();
    }
    
    if(doc.HasMember("effect") && doc["effect"].IsObject())
    {
        auto& effect = doc["effect"];
        for(auto it = effect.MemberonBegin(); it != effect.MemberonEnd(); it++)
        {
            if(it->value.IsArray())
            {
                auto matrix = new BattleConfig::EffectMatrix();
                skill->effect->effect.insert(std::pair<std::string, BattleConfig::EffectMatrix*>(std::string(it->name.GetString()), matrix));
                
                size = it->value.Size();
                for(int i = 0; i < size; i++)
                {
                    if(it->value[i].IsArray())
                    {
                        auto group = new BattleConfig::EffectGroup();
                        group->asset = skill->effect->asset;
                        
                        matrix->effect.push_back(group);
                    
                        for(int j = 0; j < it->value[i].Size(); j++)
                        {
                            auto layer = new BattleConfig::EffectLayer();
                            group->effect.push_back(layer);
                            
                            auto& current = it->value[i][j];
                            
                            layer->layerName = std::string(current["name"].GetString());
                            layer->layerType = current["type"].GetInt();
                            layer->layerZOrder = current["z-order"].GetInt();
                        }
                    }
                }
            }
        }
    }

    return skill;
}
