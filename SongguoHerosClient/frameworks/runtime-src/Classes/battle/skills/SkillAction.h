//
//  SkillAction.h
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//

#ifndef __SKILL_ACTION_H__
#define __SKILL_ACTION_H__

#include "../common/ActiveObject.h"
#include "../../entityx/entityx.h"
#include "../BattleConfig.h"

class SkillAction : public ActiveObject
{
public:
    friend class SkillObject;
    
    SkillAction();
    ~SkillAction();
    
    virtual void update(entityx::EntityManager &es, float elapse, float dt);
    virtual void fireAction(entityx::EntityManager &es);
    virtual void onTriggerFired(const std::string& trigger);
    inline float hasFired() { return m_fired; };
    
    inline void setData(int fromId, BattleConfig::Side side, const entityx::Entity::Id& entityId, BattleConfig::SkillData* skillData, BattleConfig::SkillActionData* skillActionData)
    {
        m_fromId = fromId;
        m_side = side;
        m_entityId = entityId;
        m_skillData = skillData;
        m_skillActionData = skillActionData;
        m_skillId = skillData->id;
        m_effect = skillData->pConfig->effect;
        m_formula = skillData->pConfig->formula;
    };
    
    float caclFormulaResult(const std::string name);
    bool caclFormulaEnable(const std::string name, int targetId);
    
protected:
    int m_fromId;
    int m_skillId;
    float m_endTime;
    entityx::Entity::Id m_entityId;
    BattleConfig::Side m_side;
    BattleConfig::SkillData* m_skillData;
    BattleConfig::SkillActionData* m_skillActionData;
    BattleConfig::SkillEffect* m_effect;
    BattleConfig::SkillFormula* m_formula;
    
private:
    bool m_fired;
};

#endif /* defined(__SKILL_ACTION_H__) */



