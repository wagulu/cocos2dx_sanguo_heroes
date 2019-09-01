//
//  Skill.h
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//


#ifndef __GAME_SKILL_H__
#define __GAME_SKILL_H__

#include "../BattleConfig.h"
#include "../BattleEvent.h"
#include "../common/ActiveObject.h"
#include "SkillAction.h"
#include "../../entityx/entityx.h"

class SkillObject : public ActiveObject
{
public:
    SkillObject(BattleConfig::SkillData* skillData, int m_fromId, const entityx::Entity::Id& entityId);
    virtual ~SkillObject();
    
    void addAction(SkillAction* pAction, BattleConfig::SkillActionData* pActionData);
    void triggerAction(const std::string& trigger);
    void update(entityx::EntityManager &es, float elapse, float dt);
    
    inline BattleConfig::SkillData* getSkillData()
    {
        return m_skillData;
    }
    
private:
    int m_fromId;
    entityx::Entity::Id m_entityId;
    BattleConfig::Side m_side;
    BattleConfig::SkillData* m_skillData;
    std::vector<SkillAction*> m_skillActions;
    std::vector<std::string*> m_removeTriggers;
    std::vector<std::string> m_pendingTriggers;
};

#endif /* defined(__GAME_COMMON_SKILL_H__) */
