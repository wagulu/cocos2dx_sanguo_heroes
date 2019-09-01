//
//  EnterShowControl.h
//  sanguoClient
//
//  Created by fuchenhao on 15-3-31.
//
//

#ifndef __sanguoClient__EnterShowControl__
#define __sanguoClient__EnterShowControl__

#include <stdio.h>

#include <map>
#include "BattleConfig.h"
#include "cocos2d.h"

class EnterShowControl
{
public:
    EnterShowControl();
    ~EnterShowControl();
    
    void triggerEnterShow(const std::string& configName);
    
    void triggerCameraActions(BattleConfig::CameraActions* actions, uint64_t causeId = 0);
    static BattleConfig::CameraActions* parseCameraActions(rapidjson::Value& value);
    
    void setEnterShowDone();
    void clearEnterShow();
    
    void enterShowUpdateGo(float dt);

    inline bool isInEnterShow() {return m_enterShow;};
    
private:
    float m_enterShowGoDistance = 0.0f;
    float m_enterShowGoDelay;
    float m_enterShowGoSpeed = 0.0f;
    bool m_enterShowGoDone = false;
    
    bool m_enterShow = false;
    bool m_userCancelled = false;
    
    std::map<std::string, BattleConfig::CameraActions*> m_enterShowAction;
    
    BattleConfig::CameraActions* loadEnterShowConfig(const std::string name);
    
    void setPosForEnterShow(float distance);
    
    void enterShowStartGo(float delay);
};

#endif /* defined(__sanguoClient__EnterShowControl__) */
