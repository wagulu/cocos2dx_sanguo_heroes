//
//  RemoveShieldAnimationOnUseUp.h
//  sanguoClient
//
//  Created by fuchenhao on 4/22/15.
//
//

#ifndef __sanguoClient__RemoveShieldAnimationOnUseUp__
#define __sanguoClient__RemoveShieldAnimationOnUseUp__

#include "SkillAction.h"

class RemoveShieldAnimationOnUseUp : public SkillAction
{
public:
    void fireAction(entityx::EntityManager &es) override;
    void update(entityx::EntityManager &es, float elapse, float dt) override;
private:
    BattleConfig::Property m_shieldProperty;
    string m_tag;
    vector<int> m_targets;
};

#endif /* defined(__sanguoClient__RemoveShieldAnimationOnUseUp__) */
