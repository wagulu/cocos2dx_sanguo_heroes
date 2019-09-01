//
//  StateSoldierChangeToGhost.h
//  sanguoClient
//
//  Created by fuchenhao on 5/8/15.
//
//

#ifndef __sanguoClient__StateSoldierChangeToGhost__
#define __sanguoClient__StateSoldierChangeToGhost__

#include "IState.h"
#include "BattleConfig.h"
#include "SimpleTween.h"

class StateSoldierChangeToGhost : public IState
{
public:
    enum RESULT
    {
        WORKING,
        DONE,
        ENEMY_NOT_EXIST
    };
    RESULT m_result = WORKING;
    
    struct GhostData
    {
        int targetId;
        bool isFirstHit = false;
        bool isLastHit = false;
        float flyUpTime;
        float flyUpHeight;
        float flyUpStartSpeed;
        float flyUpEndSpeed;
        float flyUpFactorX;
        float scaleFactor;
        float scaleMax;
        float changeToGhostTime;
        float moveAwayTime;
        float moveAwayDistance;
        float moveCloseTime;
        float hitOffset;
        BattleConfig::EffectGroup* appearEffect = nullptr;
        BattleConfig::EffectGroup* loopEffect = nullptr;
        BattleConfig::EffectGroup* hitEffect = nullptr;
        BattleConfig::AttackData attackData;
        
        GhostData(const BattleConfig::AttackData& attackData) : attackData(attackData) {}
    };
    
    StateSoldierChangeToGhost(const GhostData& ghostData);
    virtual ~StateSoldierChangeToGhost();
    
    virtual int getStateCode()
    {
        return BattleConfig::SoldierState::STATE_CHANGE_TO_GHOST;
    }
    
    virtual void enter(entityx::Entity& entity);
    virtual void exit(entityx::Entity& entity);
    virtual int update(entityx::Entity& entity, double dt);
    virtual bool isDelegateState();
    virtual bool isDelegateStateWorking();
    
private:
    enum class SUB_STATE
    {
        FLY_UP,
        CHANGE_TO_GHOST,
        MOVE_AWAY_ENEMY,
        MOVE_CLOSE_ENEMY
    };
    SUB_STATE m_subState = SUB_STATE::FLY_UP;
    
    GhostData m_ghostData;
    entityx::Entity m_targetEntity;
    
    SimpleTween* m_moveTween = nullptr;
    cocos2d::Vec3 m_beginPos;
    cocos2d::Vec3 m_deltaPos;
    cocos2d::Vec3 m_endPos;
    float m_scale;
    
    void clearMoveTween();
    void prepareToFlyUp(entityx::Entity& entity);
    void prepareToChangeToGhost(entityx::Entity& entity);
    void prepareToMoveAwayEnemy(entityx::Entity& entity);
    void prepareToMoveCloseEnemy(entityx::Entity& entity);
    void moveSoldierFlyingUp(entityx::Entity& entity, float dt);
    void moveGhostAwayFromEnemy(entityx::Entity& entity, float dt);
    void moveGhostCloseToEnemy(entityx::Entity& entity, float dt);
    void doAttack(entityx::Entity& entity);
};

#endif /* defined(__sanguoClient__StateSoldierChangeToGhost__) */
