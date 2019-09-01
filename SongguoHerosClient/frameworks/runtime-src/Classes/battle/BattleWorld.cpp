//
//  BattleWorld.cpp
//  Game
//
//  Created by fuchenhao on 3/4/15.
//
//

#include "BattleWorld.h"
#include "control/PrepairBattle.h"
#include "CCAssetDefineCache.h"

BattleWorld* BattleWorld::s_sharedBattleWorld = nullptr;

BattleWorld* BattleWorld::getInstance()
{
    if(!s_sharedBattleWorld)
    {
        s_sharedBattleWorld = new BattleWorld();
    }
    return s_sharedBattleWorld;
}

BattleWorld::BattleWorld()
{
}

BattleWorld::~BattleWorld()
{
}

void BattleWorld::create()
{
    assert(!m_created);
    m_created = true;
    
    m_entityX = new entityx::EntityX();
    
    m_cameraManager = new CameraManager();
    m_enterShowControl = new EnterShowControl();
    m_battleEntity = new BattleEntity();
    m_battleStage = new BattleStage(*m_entityX);
    m_battleResult = new BattleResult();
    m_pathFinding = new PathFinding(m_battleEntity);
    m_targetFinder = new TargetFinder(m_battleEntity);
    m_soldierMovement = new SoldierMovement(m_battleEntity, m_battleStage);
    m_heroMovement = new HeroMovement(m_battleEntity, m_battleStage);
    m_simpleMovement = new SimpleMovement();
    m_arrowMovement = new ArrowMovement();
    m_strategyControl = new StrategyControl();
    
    m_soldierCreator = new SoldierCreator(m_battleEntity, m_battleStage);
    m_heroCreator = new HeroCreator(m_battleEntity, m_battleStage);
    m_backgroundCreator = new BackgroundCreator(m_battleStage);
    m_effectCreator = new EffectCreator();
    m_battleAnimationCreator = new BattleAnimationCreator();
    m_anchorArrowCreator = new AnchorArrowCreator(m_battleEntity, m_battleStage);
    
    PrepairBattle::prepair();
}

void BattleWorld::destory()
{
    assert(m_created);

    CC_SAFE_DELETE(m_battleStage);
    CC_SAFE_DELETE(m_strategyControl);
    CC_SAFE_DELETE(m_pathFinding);
    CC_SAFE_DELETE(m_soldierMovement);
    CC_SAFE_DELETE(m_heroMovement);
    CC_SAFE_DELETE(m_simpleMovement);
    CC_SAFE_DELETE(m_arrowMovement);
    
    CC_SAFE_DELETE(m_battleEntity);
    CC_SAFE_DELETE(m_soldierCreator);
    CC_SAFE_DELETE(m_heroCreator);
    CC_SAFE_DELETE(m_backgroundCreator);
    CC_SAFE_DELETE(m_effectCreator);
    CC_SAFE_DELETE(m_battleAnimationCreator);
    CC_SAFE_DELETE(m_anchorArrowCreator);
    
    CC_SAFE_DELETE(m_cameraManager);
    CC_SAFE_DELETE(m_enterShowControl);
    
    CC_SAFE_DELETE(m_entityX);
    
    cocos2d::AssetDefineCache::getInstance()->removeAllAsset();
    
    m_created = false;
}
