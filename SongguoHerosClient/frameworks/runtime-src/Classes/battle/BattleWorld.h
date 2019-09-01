//
//  BattleWorld.h
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#ifndef __BATTLE_WORLD_H__
#define __BATTLE_WORLD_H__

#include "entityx.h"

#include "BattleStage.h"
#include "BattleConfig.h"
#include "BattleComponent.h"
#include "BattleEvent.h"
#include "BattleResult.h"

#include "control/map/PathFinding.h"
#include "control/map/TargetFinder.h"
#include "control/map/BattleEntity.h"
#include "control/map/SoldierMovement.h"
#include "control/map/HeroMovement.h"
#include "control/map/SimpleMovement.h"
#include "control/map/ArrowMovement.h"
#include "control/map/FormationManager.h"
#include "control/map/MapHelper.h"
#include "control/creator/SoldierCreator.h"
#include "control/creator/HeroCreator.h"
#include "control/creator/BackgroundCreator.h"
#include "control/creator/EffectCreator.h"
#include "control/creator/BattleAnimationCreator.h"
#include "control/creator/AnchorArrowCreator.h"
#include "control/camera/CameraManager.h"
#include "control/enterShow/EnterShowControl.h"
#include "common/StrategyControl.h"

#define _ENTITY_EVENT (BattleWorld::getInstance()->getEntityX().events)
#define _ENTITY_MANAGER (BattleWorld::getInstance()->getEntityX().entities)

#define _BATTLE_STAGE BattleWorld::getInstance()->getBattleStage()

#define _BATTLE_ENTITY BattleWorld::getInstance()->getBattleEntity()
#define _PATH_FINDING BattleWorld::getInstance()->getPathFinding()
#define _TARGET_FINDER BattleWorld::getInstance()->getTargetFinder()
#define _SOLDIER_MOVEMENT BattleWorld::getInstance()->getSoldierMovement()
#define _HERO_MOVEMENT BattleWorld::getInstance()->getHeroMovement()
#define _SIMPLE_MOVEMENT (BattleWorld::getInstance()->getSimpleMovement())
#define _ARROW_MOVEMENT (BattleWorld::getInstance()->getArrowMovement())

#define _SOLDIER_CREATOR BattleWorld::getInstance()->getSoldierCreator()
#define _HERO_CREATOR BattleWorld::getInstance()->getHeroCreator()
#define _BACKGROUND_CREATOR BattleWorld::getInstance()->getBackgroundCreator()
#define _EFFECT_CREATOR (BattleWorld::getInstance()->getEffectCreator())
#define _BATTLE_ANIMATION_CREATOR (BattleWorld::getInstance()->getBattleAnimationCreator())
#define _ANCHOR_ARROW_CREATOR (BattleWorld::getInstance()->getAnchorArrowCreator())

#define _STRATEGY_CONTROL BattleWorld::getInstance()->getStrategyControl()

#define _CAMERA_MANAGER BattleWorld::getInstance()->getCameraManager()
#define _ENTER_SHOW_CONTROL BattleWorld::getInstance()->getEnterShowControl()
#define _BATTLE_TIME BattleWorld::getInstance()->getBattleStage().getBattleTime()

class BattleWorld
{
public:
    
    static BattleWorld* getInstance();
    
    BattleWorld();
    ~BattleWorld();
    
    void create();
    void destory();
    
    inline entityx::EntityX& getEntityX() { assert(m_created); return *m_entityX; };
    
    inline BattleStage& getBattleStage() { assert(m_created); return *m_battleStage; };
    inline BattleEntity& getBattleEntity() { assert(m_created); return *m_battleEntity; };
    inline PathFinding& getPathFinding() { assert(m_created); return *m_pathFinding; };
    inline TargetFinder& getTargetFinder() { assert(m_created); return *m_targetFinder; };
    inline SoldierMovement& getSoldierMovement() { assert(m_created); return *m_soldierMovement; };
    inline HeroMovement& getHeroMovement() { assert(m_created); return *m_heroMovement; };
    inline SimpleMovement& getSimpleMovement() { assert(m_created); return *m_simpleMovement; };
    inline ArrowMovement& getArrowMovement() { assert(m_created); return *m_arrowMovement; };

    inline SoldierCreator& getSoldierCreator() { assert(m_created); return *m_soldierCreator; };
    inline HeroCreator& getHeroCreator() { assert(m_created); return *m_heroCreator; };
    inline BackgroundCreator& getBackgroundCreator() { assert(m_created); return *m_backgroundCreator; };
    inline EffectCreator& getEffectCreator() { assert(m_created); return *m_effectCreator; };
    inline BattleAnimationCreator& getBattleAnimationCreator() { assert(m_created); return *m_battleAnimationCreator; };
    inline AnchorArrowCreator& getAnchorArrowCreator() { assert(m_created); return *m_anchorArrowCreator; };
    
    inline BattleResult& getBattleResult() { assert(m_created); return *m_battleResult; };
    
    inline StrategyControl& getStrategyControl() { assert(m_created); return *m_strategyControl; };
    
    inline CameraManager& getCameraManager() { assert(m_created); return *m_cameraManager; };
    inline EnterShowControl& getEnterShowControl() { assert(m_created); return *m_enterShowControl; };

    
private:
    entityx::EntityX* m_entityX = nullptr;
    
    BattleStage* m_battleStage = nullptr;
    BattleEntity* m_battleEntity = nullptr;
    PathFinding* m_pathFinding = nullptr;
    TargetFinder* m_targetFinder = nullptr;
    SoldierMovement* m_soldierMovement = nullptr;
    HeroMovement* m_heroMovement = nullptr;
    SimpleMovement* m_simpleMovement = nullptr;
    ArrowMovement* m_arrowMovement = nullptr;
    
    BattleResult* m_battleResult = nullptr;
    
    SoldierCreator* m_soldierCreator = nullptr;
    HeroCreator* m_heroCreator = nullptr;
    BackgroundCreator* m_backgroundCreator = nullptr;
    EffectCreator* m_effectCreator = nullptr;
    BattleAnimationCreator* m_battleAnimationCreator = nullptr;
    AnchorArrowCreator* m_anchorArrowCreator = nullptr;
    
    StrategyControl* m_strategyControl = nullptr;
    
    CameraManager* m_cameraManager = nullptr;
    EnterShowControl* m_enterShowControl = nullptr;

    bool m_created = false;
    
    static BattleWorld* s_sharedBattleWorld;
    
    
public:
    inline void setAutoQuickBattleMode(bool mode) { m_isAutoQuickBattleMode = mode; }
    inline bool isAutoQuickBattleMode() { return m_isAutoQuickBattleMode; }
    inline int getBattleSpeedTimes() { return m_isAutoQuickBattleMode? 10: 1; }
private:
    bool m_isAutoQuickBattleMode = false;
    
};

#endif /* defined(__BATTLE_WORLD_H__) */
