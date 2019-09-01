//
//  ShowBackground.h
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//

#ifndef __SHOW_BACKGROUND_H__
#define __SHOW_BACKGROUND_H__

#include "SkillAction.h"

class ShowBackground : public SkillAction
{
public:
    ~ShowBackground();
    
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    void onTriggerFired(const std::string& trigger);
    
private:
    void hideBackground();
    
    float m_showDuration = 0;
    float m_showColor = 45;
    float m_hideDuration = 0;
    float m_hideDelay = 0;
    std::string* m_pHideTrigger = nullptr;
    bool m_hideRequired = false;
};
#endif /* defined(__SHOW_BACKGROUND_H__) */
