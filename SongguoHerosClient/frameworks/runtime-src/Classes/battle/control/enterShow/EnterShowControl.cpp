//
//  EnterShowControl.cpp
//  sanguoClient
//
//  Created by Chance on 15-3-31.
//
//

#include "EnterShowControl.h"
#include "BattleWorld.h"
#include "AudioSystem.h"
#include "AudioManager.h"

using namespace cocos2d;

EnterShowControl::EnterShowControl()
{
    
}

EnterShowControl::~EnterShowControl()
{
    for(auto& sAction : m_enterShowAction)
    {
        delete sAction.second;
    }
    m_enterShowAction.clear();
}

void EnterShowControl::setPosForEnterShow(float distance)
{
    BattleComponent::Position::Handle  pos;
    BattleComponent::Animation::Handle ani;
    std::vector<entityx::Entity> &soldiers = _BATTLE_ENTITY.getLeftSoldiers();
    for(int i = 0; i < soldiers.size(); i++)
    {
        pos = (soldiers[i]).component<BattleComponent::Position>();
        ani = (soldiers[i]).component<BattleComponent::Animation>();
        ani->battleAnimation->setPositionX(pos->x - distance);
    }
    
    BattleComponent::Position::Handle  posL;
    BattleComponent::Animation::Handle aniL;
    std::vector<entityx::Entity> &heros = _BATTLE_ENTITY.getLeftHeros();
    for(int i = 0; i < heros.size(); i++)
    {
        posL = heros[i].component<BattleComponent::Position>();
        aniL = heros[i].component<BattleComponent::Animation>();
        aniL->battleAnimation->setPositionX(posL->x - distance);
    }
    
    if (distance <= 0)
    {
        m_enterShowGoDone = true;
        
        std::vector<entityx::Entity> &heros = _BATTLE_ENTITY.getLeftHeros();
        for(int i = 0; i < heros.size(); i++)
        {
            _ENTITY_EVENT.emit(BattleEvent::StrategyHeroPlayAnimation(heros[i], BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_IDLE), 0.05f));
            _STRATEGY_CONTROL.setHeroControl(StrategyControl::HERO_STOP, heros[i].component<BattleComponent::Identify>()->id);
        }
        
        std::vector<entityx::Entity> &soldiers = _BATTLE_ENTITY.getLeftSoldiers();
        for(int i = 0; i < soldiers.size(); i++)
        {
            _ENTITY_EVENT.emit(BattleEvent::StateSoldierPlayAnimation(soldiers[i], BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_IDLE), 0.05f));
            _ENTITY_EVENT.emit(BattleEvent::StrategyIdle(soldiers[i]));
        }
    }
}

void EnterShowControl::enterShowUpdateGo(float dt)
{
    if (!m_enterShowGoDone)
    {
        if (m_enterShowGoDelay > 0)
        {
            m_enterShowGoDelay -= dt;
            if (m_enterShowGoDelay <= 0)
            {
                float runTime = m_enterShowGoDistance / m_enterShowGoSpeed;
                
                std::vector<entityx::Entity> &heros = _BATTLE_ENTITY.getLeftHeros();
                for(int i = 0; i < heros.size(); i++)
                {
                    _ENTITY_EVENT.emit(BattleEvent::StrategyHeroPlayAnimation(heros[i], BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_RUN), runTime + 1));
                }
                
                std::vector<entityx::Entity> &soldiers = _BATTLE_ENTITY.getLeftSoldiers();
                for(int i = 0; i < soldiers.size(); i++)
                {
                    _ENTITY_EVENT.emit(BattleEvent::StateSoldierPlayAnimation(soldiers[i], BattleConfig::getInstance()->getAnimationName(BattleConfig::ANI_RUN), runTime + 1));
                }
                
                BattleConfig::CameraShakeData shakeData(runTime, 1.8f, 2, 10, 0.2f, false);
                _ENTITY_EVENT.emit<BattleEvent::CameraShakeEvent>(shakeData);
            }
        }
        else
        {
            m_enterShowGoDistance -= dt * m_enterShowGoSpeed;
            
            if(m_enterShowGoDistance <= 0)
            {
                m_enterShowGoDistance = 0;
            }
            
            setPosForEnterShow(m_enterShowGoDistance);
        }
    }
    
    bool allQueueComplete = true;
    BattleComponent::Camera::Handle camera;
    for (entityx::Entity entity : _ENTITY_MANAGER.entities_with_components(camera))
    {
        if (camera->cameraXQueue.size() > 0
            || camera->cameraYQueue.size() > 0
            || camera->cameraZQueue.size() > 0
            || camera->cameraR3dQueue.size() > 0
            || camera->cameraR2dQueue.size() > 0
            || camera->cameraRYQueue.size() > 0)
        {
            allQueueComplete = false;
            break;
        }
    }
    if (allQueueComplete)
    {
        setEnterShowDone();
    }
}

BattleConfig::CameraActions* EnterShowControl::loadEnterShowConfig(const std::string name)
{
    auto result = m_enterShowAction.find(name);
    if(result != m_enterShowAction.end()) return result->second;
    
    std::string jsonFile = cocos2d::FileUtils::getInstance()->getStringFromFile("config/enterShow/" + name + ".json");
    
    rapidjson::Document doc;
    doc.Parse<rapidjson::kParseDefaultFlags>(jsonFile.c_str());
    
    auto* data = EnterShowControl::parseCameraActions(doc);
    data->file = name;
    m_enterShowAction.insert(std::pair<std::string, BattleConfig::CameraActions*>(name, data));
    
    return data;
}

BattleConfig::CameraActions* EnterShowControl::parseCameraActions(rapidjson::Value& doc)
{
    int size;
    auto enterShowData = new BattleConfig::CameraActions();
    
    if(doc.HasMember("actionsX") && doc["actionsX"].IsArray())
    {
        size = doc["actionsX"].Size();
        for(int i = 0; i < size; i++)
        {
            auto& value = doc["actionsX"][i];
            auto action = new BattleConfig::CameraActionConfig();
            enterShowData->actionsX.push_back(action);
            
            if(value.HasMember("name"))
            {
                action->name = std::string(value["name"].GetString());
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
                    action->config.insert(std::pair<std::string, BattleConfig::ParamData*>(std::string(it->name.GetString()), param));
                }
            }
        }
    }
    
    if(doc.HasMember("actionsY") && doc["actionsY"].IsArray())
    {
        size = doc["actionsY"].Size();
        for(int i = 0; i < size; i++)
        {
            auto& value = doc["actionsY"][i];
            auto action = new BattleConfig::CameraActionConfig();
            enterShowData->actionsY.push_back(action);
            
            if(value.HasMember("name"))
            {
                action->name = std::string(value["name"].GetString());
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
                    action->config.insert(std::pair<std::string, BattleConfig::ParamData*>(std::string(it->name.GetString()), param));
                }
            }
        }
    }
    
    if(doc.HasMember("actionsZ") && doc["actionsZ"].IsArray())
    {
        size = doc["actionsZ"].Size();
        for(int i = 0; i < size; i++)
        {
            auto& value = doc["actionsZ"][i];
            auto action = new BattleConfig::CameraActionConfig();
            enterShowData->actionsZ.push_back(action);
            
            if(value.HasMember("name"))
            {
                action->name = std::string(value["name"].GetString());
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
                    action->config.insert(std::pair<std::string, BattleConfig::ParamData*>(std::string(it->name.GetString()), param));
                }
            }
        }
    }
    
    if(doc.HasMember("actionsR3d") && doc["actionsR3d"].IsArray())
    {
        size = doc["actionsR3d"].Size();
        for(int i = 0; i < size; i++)
        {
            auto& value = doc["actionsR3d"][i];
            auto action = new BattleConfig::CameraActionConfig();
            enterShowData->actionsR3d.push_back(action);
            
            if(value.HasMember("name"))
            {
                action->name = std::string(value["name"].GetString());
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
                    action->config.insert(std::pair<std::string, BattleConfig::ParamData*>(std::string(it->name.GetString()), param));
                }
            }
        }
    }
    
    if(doc.HasMember("actionsR2d") && doc["actionsR2d"].IsArray())
    {
        size = doc["actionsR2d"].Size();
        for(int i = 0; i < size; i++)
        {
            auto& value = doc["actionsR2d"][i];
            auto action = new BattleConfig::CameraActionConfig();
            enterShowData->actionsR2d.push_back(action);
            
            if(value.HasMember("name"))
            {
                action->name = std::string(value["name"].GetString());
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
                    action->config.insert(std::pair<std::string, BattleConfig::ParamData*>(std::string(it->name.GetString()), param));
                }
            }
        }
    }
    
    return enterShowData;
}

void EnterShowControl::triggerEnterShow(const std::string& configName)
{
    m_enterShow = true;
    BattleConfig::CameraActions* enterData = loadEnterShowConfig(configName);
    triggerCameraActions(enterData);
}


void EnterShowControl::triggerCameraActions(BattleConfig::CameraActions* enterData, uint64_t causeId)
{
    BattleComponent::Position::Handle positionL = _BATTLE_ENTITY.getEntity(LEFT_HERO)->component<BattleComponent::Position>();
    BattleComponent::Position::Handle positionR = _BATTLE_ENTITY.getEntity(RIGHT_HERO)->component<BattleComponent::Position>();

    for(int i = 0; i < enterData->actionsR3d.size(); )
    {
        float dur   = 0.0f;
        float delay = 0.0f;
        int   mode  = 0;
        int   a     = CameraManager::LINEAR;
        
        if ((enterData->actionsR3d)[i]->name == "hold_camera")
        {
            delay = ((enterData->actionsR3d)[i]->config)["dur"]->d;
            if(++i >= enterData->actionsR3d.size()) break;
        }
        
        if ((enterData->actionsR3d)[i]->name == "move_camera")
        {
            dur = ((enterData->actionsR3d)[i]->config)["dur"]->d;
            mode = ((enterData->actionsR3d)[i]->config)["zoom"]->d;
            
            if (((enterData->actionsR3d)[i]->config)["A"] != NULL)
            {
                a = ((enterData->actionsR3d)[i]->config)["A"]->d;
            }
        }
        
        float tarR = CameraManager::INIT_CAMERA_ROTATE;
        if (mode == 1)
        {
            if (((enterData->actionsR3d)[i]->config)["r"] != NULL)
            {
                tarR = ((enterData->actionsR3d)[i]->config)["r"]->d;
            }
        }
        
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR3dEvent>(tarR,
                                                                   dur, delay,
                                                                   CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                   a, causeId, false);
        
        ++i;
    }

    for(int i = 0; i < enterData->actionsR2d.size(); )
    {
        float dur   = 0.0f;
        float delay = 0.0f;
        int   mode  = 0;
        int   a     = CameraManager::LINEAR;
        
        if ((enterData->actionsR2d)[i]->name == "hold_camera")
        {
            delay = ((enterData->actionsR2d)[i]->config)["dur"]->d;
            if(++i >= enterData->actionsR2d.size()) break;
        }
        
        if ((enterData->actionsR2d)[i]->name == "move_camera")
        {
            dur = ((enterData->actionsR2d)[i]->config)["dur"]->d;
            mode = ((enterData->actionsR2d)[i]->config)["zoom"]->d;
            
            if (((enterData->actionsR2d)[i]->config)["A"] != NULL)
            {
                a = ((enterData->actionsR2d)[i]->config)["A"]->d;
            }
        }
        
        float tarR = CameraManager::INIT_CAMERA_ROTATE_2D;
        if (mode == 1)
        {
            if (((enterData->actionsR2d)[i]->config)["r"] != NULL)
            {
                tarR = ((enterData->actionsR2d)[i]->config)["r"]->d;
            }
        }
        
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(tarR,
                                                                     dur, delay,
                                                                     CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                     a, causeId, false);
        
        ++i;
    }
    
    
    // Z
    for(int i = 0; i < enterData->actionsZ.size(); )
    {
        float dur   = 0.0f;
        float delay = 0.0f;
        int   mode  = 0;
        int   a     = CameraManager::LINEAR;
        
        if ((enterData->actionsZ)[i]->name == "hold_camera")
        {
            delay = ((enterData->actionsZ)[i]->config)["dur"]->d;
            if(++i >= enterData->actionsZ.size()) break;
        }
        
        if ((enterData->actionsZ)[i]->name == "move_camera")
        {
            dur = ((enterData->actionsZ)[i]->config)["dur"]->d;
            mode = ((enterData->actionsZ)[i]->config)["zoom"]->d;
            
            if (((enterData->actionsZ)[i]->config)["A"] != NULL)
            {
                a = ((enterData->actionsZ)[i]->config)["A"]->d;
            }
        }
        
        float tarZ = CameraManager::INIT_CAMERA_Z;
        if (mode == 1)
        {
            if (((enterData->actionsZ)[i]->config)["z"] != NULL)
            {
                tarZ = ((enterData->actionsZ)[i]->config)["z"]->d;
            }
            else
            {
                tarZ = -410;
            }
        }
        
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniZEvent>(tarZ,
                                                                   dur, delay,
                                                                   CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                   a, causeId, false);
        
        ++i;
    }

    
    // Y
    for(int i = 0; i < enterData->actionsY.size(); )
    {
        float dur   = 0.0f;
        float delay = 0.0f;
        int   mode  = 0;
        int   a     = CameraManager::LINEAR;
        
        if ((enterData->actionsY)[i]->name == "hold_camera")
        {
            delay = ((enterData->actionsY)[i]->config)["dur"]->d;
            if(++i >= enterData->actionsY.size()) break;
        }
        
        if ((enterData->actionsY)[i]->name == "move_camera")
        {
            dur = ((enterData->actionsY)[i]->config)["dur"]->d;
            mode = ((enterData->actionsY)[i]->config)["zoom"]->d;
            
            if (((enterData->actionsY)[i]->config)["A"] != NULL)
            {
                a = ((enterData->actionsY)[i]->config)["A"]->d;
            }
        }
        
        float tarY = CameraManager::INIT_CAMERA_Y;
        
        if (mode == 1)
        {
            if (((enterData->actionsY)[i]->config)["y"] != NULL)
            {
                tarY = ((enterData->actionsY)[i]->config)["y"]->d;
            }
            else
            {
                tarY = 60;
            }
        }
        
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniYEvent>(tarY,
                                                                   dur, delay,
                                                                   CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                   a, causeId, false);
        
        ++i;
    }
    
    
    // X
    float noMoveTime   = 0.0f;
    float enterRunTime = 0.0f;
    float totalTime    = 0.0f;

    for(int i = 0; i < enterData->actionsX.size(); )
    {
        float dur   = 0.0f;
        float delay = 0.0f;
        int   tar   = 0;
        
        float dtX   = 0.0f;
        
        int   a     = CameraManager::LINEAR;
        
        if ((enterData->actionsX)[i]->name == "hold_camera")
        {
            delay = ((enterData->actionsX)[i]->config)["dur"]->d;
            totalTime += delay;

            if(++i >= enterData->actionsX.size()) break;
        }
        
        if ((enterData->actionsX)[i]->name == "move_camera")
        {
            dur = ((enterData->actionsX)[i]->config)["dur"]->d;
            tar = ((enterData->actionsX)[i]->config)["tar"]->d;

            if (((enterData->actionsX)[i]->config)["A"] != NULL)
            {
                a = ((enterData->actionsX)[i]->config)["A"]->d;
            }

            totalTime += dur;
        }
        
        float tarX = positionL->x;
        if (tar == 1)
        {
            tarX = positionR->x;
        }
        
        if (((enterData->actionsX)[i]->config)["dtx"] != NULL)
        {
            dtX = ((enterData->actionsX)[i]->config)["dtx"]->d;
        }
        
        tarX += dtX;
        //entity id
        if(++i >= enterData->actionsX.size()) {
            _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarX,
                                                                       dur, delay,
                                                                       CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                       a, causeId, false);
            break;
        }
        
        enterRunTime = 0;
        if ((enterData->actionsX)[i]->name == "parade_go")
        {
            m_enterShowGoDistance = ((enterData->actionsX)[i]->config)["showDistance"]->d;
            m_enterShowGoSpeed = ((enterData->actionsX)[i]->config)["showSpeed"]->d;
            
            enterRunTime = m_enterShowGoDistance / m_enterShowGoSpeed;
            
            if (enterRunTime > 0)
            {
                noMoveTime = totalTime;
                tarX += FOLLOW_DISTANCE;
            }
        }
        
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniXEvent>(tarX,
                                                                   dur, delay,
                                                                   CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                   a, causeId, false);

        if (enterRunTime > 0)
        {
            m_enterShowGoDone = false;
            setPosForEnterShow(m_enterShowGoDistance);
            m_enterShowGoDelay = noMoveTime;
            
            if(++i >= enterData->actionsX.size()) break;
        }
    }
}

void EnterShowControl::setEnterShowDone()
{
    if (m_enterShow)
    {
        m_enterShow = false;
        
        _CAMERA_MANAGER.setCameraModeX(CameraManager::CAMERA_FOLLOW_CAN_BREAK);
    }
}

void EnterShowControl::clearEnterShow()
{
    if (!m_userCancelled)
    {
        m_userCancelled = true;
        
        _ENTITY_EVENT.emit<BattleEvent::CameraQueueClearEvent>();
        _ENTITY_EVENT.emit<BattleEvent::CameraClearShakeEvent>();
        
        setPosForEnterShow(0.0f);
        
        float defaultMoveTime = 0.1f;
        BattleComponent::Position::Handle positionL = _BATTLE_ENTITY.getEntity(LEFT_HERO)->component<BattleComponent::Position>();
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniEvent>(positionL->x + FOLLOW_DISTANCE, CameraManager::INIT_CAMERA_Z, CameraManager::INIT_CAMERA_Y,
                                                                  defaultMoveTime, 0,
                                                                  CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_FOLLOW_CAN_BREAK,
                                                                  CameraManager::INIT_CAMERA_ROTATE, CameraManager::EASE_OUT);
        
        _ENTITY_EVENT.emit<BattleEvent::CameraMoveToWithAniR2dEvent>(0,
                                                                     defaultMoveTime, 0,
                                                                     CameraManager::CAMERA_ZOOMING, CameraManager::CAMERA_ZOOMING,
                                                                     CameraManager::EASE_OUT, 0, false);
    }
}
