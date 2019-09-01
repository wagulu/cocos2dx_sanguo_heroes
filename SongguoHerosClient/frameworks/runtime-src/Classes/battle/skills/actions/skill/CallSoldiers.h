//
//  CallSoldiers.h
//  Game
//
//  Created by fuchenhao on 15-4-15.
//
//

#ifndef __Game__CallSoldiers__
#define __Game__CallSoldiers__

#include "SkillAction.h"
#include "BattleEvent.h"

class CallSoldiers : public SkillAction, public entityx::Receiver<CallSoldiers>
{
public:
    enum CallSoldiersMode
    {
        UP_AND_DOWN,
        BEHIND_HERO,
        BASE_POS,
        TURN_ENEMY
    };
    
    ~CallSoldiers();
    
    void receive(const BattleEvent::HurtByAttack& e);
    void receive(const BattleEvent::BattleActorRebirth& e);
    
    void fireAction(entityx::EntityManager &es) override;
    
    void update(entityx::EntityManager &es, float elapse, float dt) override;
    
private:
    int getTargetHeroID(int toSide);
    
    void subscribeEvent();
    
    void killSoldierAtOnce(entityx::Entity entity, bool needRebirth = true, float delay = 0.0);
    
    BattleConfig::EffectGroup* m_showSoliderEffect;
    BattleConfig::EffectGroup* m_loopSoliderEffect;
    
    void changeSoldierToBlockState(int targetId);
    
    BattleConfig::Side changeSoldierSide(BattleConfig::Side curSide);
    BattleConfig::Side m_soldierSide;
    float m_elapse = 0.0f;
    float m_holdTime = -1.0f;
    vector<int> m_createSoldiers;
    
    float m_delayElapse = 0.0f;
    float m_delayKill = 0.0f;
    vector<entityx::Entity> m_killSoldiers;
    
    void changeRoleColor(int changedID, bool effect = false, bool opp = false, bool colorChange = true);
    
    bool m_virus = false;
    
    void sendAudio_recruit();
};


#endif /* defined(__Game__CallSoldiers__) */
