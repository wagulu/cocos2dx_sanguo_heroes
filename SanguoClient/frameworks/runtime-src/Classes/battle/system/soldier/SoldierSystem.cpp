//
//  SoldierSystem.cpp
//  Game
//
//  Created by fu.chenhao on 2/25/15.
//
//

#include "SoldierSystem.h"
#include "BattleWorld.h"
#include "AICommonHelper.h"
#include "StateSoldierIdle.h"
#include "StateSoldierMoveToEnemy.h"
#include "StateSoldierAttackEnemy.h"
#include "StateSoldierProtectHero.h"
#include "StateSoldierRecenter.h"
#include "StateSoldierDepart.h"
#include "StateSoldierMoveToEnemyHero.h"
#include "StateSoldierMoveToTargetTile.h"
#include "StateSoldierLost.h"
#include "StateSoldierMoveBack.h"
#include "StateSoldierDizzy.h"
#include "StateSoldierFadeIn.h"
#include "StateSoldierPlayAnimation.h"
#include "StateSoldierArchorAttack.h"
#include "StateSoldierChangeToBlock.h"
#include "StateSoldierChangeToGhost.h"
#include "StateSoldierDead.h"
#include "StateSoldierHitBack.h"

float SoldierSystem::s_maxPosX = -1.0f;

TicketRequest SoldierSystem::m_findTargetInIdleTicketRequest(200, 0.02);
//TicketRequest SoldierSystem::m_checkBowManAttackTicketRequest(200, 0.1);

void SoldierSystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    m_findTargetInIdleTicketRequest.update(dt);
//    m_checkBowManAttackTicketRequest.update(dt);
//    _TARGET_FINDER.cleanCheckTargetInLine();
    
    BattleComponent::Identify::Handle identify;
    BattleComponent::Soldier::Handle soldier;
    BattleComponent::Position::Handle position;
    BattleComponent::ObjectStateMachine::Handle objectStateMachine;

    for (entityx::Entity entity : es.entities_with_components(identify, soldier, objectStateMachine, position))
    {
        if(objectStateMachine->getCurrentState() == NULL)
        {
            objectStateMachine->changeState(new StateSoldierIdle());
        }
        
        int currentStateCode = objectStateMachine->getCurrentStateCode();
        if (_SOLDIER_MOVEMENT.isSoldierPositionLostInTileMap(identify->id, position->z, position->x)
            && currentStateCode != BattleConfig::STATE_LOST
            && currentStateCode != BattleConfig::STATE_DEAD)
        {
            objectStateMachine->changeState(new StateSoldierLost());
        }
        
        IState* pCurrentState = objectStateMachine->getCurrentState();
        currentStateCode = objectStateMachine->getCurrentStateCode();
        int result = objectStateMachine->update(dt);
        
        switch (currentStateCode)
        {
            case BattleConfig::STATE_IDLE:
                if(result == StateSoldierIdle::WORKING)
                {
                    
                }
                else if(result == StateSoldierIdle::ATTACK_ENEMY)
                {
                    objectStateMachine->changeState(new StateSoldierMoveToEnemy());
                }
                else if(result == StateSoldierIdle::ATTACK_ENEMY_HERO)
                {
                    objectStateMachine->changeState(new StateSoldierMoveToEnemyHero());
                }
                else if(result == StateSoldierIdle::PROTECT_HERO)
                {
                    objectStateMachine->changeState(new StateSoldierProtectHero());
                }
                else if(result == StateSoldierIdle::DEPART)
                {
                    objectStateMachine->changeState(new StateSoldierDepart());
                }
                else if(result == StateSoldierIdle::RECENTER)
                {
                    objectStateMachine->changeState(new StateSoldierRecenter());
                }
                else if(result == StateSoldierIdle::MOVE_BACK)
                {
                    objectStateMachine->changeState(new StateSoldierMoveBack());
                }
                //
                else if(result == StateSoldierIdle::ATTACK_NEARBY_ENEMY)
                {
                    objectStateMachine->changeState(new StateSoldierAttackEnemy());
                }
                else if(result == StateSoldierIdle::BOWMAN_ATTACK)
                {
                    objectStateMachine->changeState(new StateSoldierArchorAttack());
                }
                break;

            case BattleConfig::STATE_PLAY_ANIMATION:
                if(result == StateSoldierPlayAnimation::WORKING)
                {
                    
                }
                else if(result == StateSoldierPlayAnimation::DONE)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                break;
                
            case BattleConfig::STATE_SOLDIER:
                if(result == StateSoldierMoveToEnemy::WORKING)
                {
                    
                }
                else if(result == StateSoldierMoveToEnemy::ATTACK_ENEMY)
                {
                    objectStateMachine->changeState(new StateSoldierAttackEnemy());
                }
                else if(result == StateSoldierMoveToEnemy::AIMING_ENEMY_HERO)
                {
                    objectStateMachine->changeState(new StateSoldierMoveToEnemyHero());
                }
                else if(result == StateSoldierMoveToEnemy::BOWMAN_ATTACK)
                {
                    objectStateMachine->changeState(new StateSoldierArchorAttack());
                }
                break;
                
            case BattleConfig::STATE_ATTACK:
                if(result == StateSoldierAttackEnemy::WORKING)
                {
                    
                }
                else if(result == StateSoldierAttackEnemy::ENEMY_INVALID)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                break;
                
            case BattleConfig::STATE_PROTECT:
                if(result == StateSoldierProtectHero::WORKING)
                {
                    
                }
                else if(result == StateSoldierProtectHero::ARRIVE)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                else if(result == StateSoldierProtectHero::HERO_NOT_EXIST)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                else if(result == StateSoldierProtectHero::BOWMAN_ATTACK)
                {
                    objectStateMachine->changeState(new StateSoldierArchorAttack());
                }
                break;
                
                
            case BattleConfig::STATE_CENTER:
                if(result == StateSoldierRecenter::WORKING)
                {
                    
                }
                else if(result == StateSoldierRecenter::ARRIVE)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                else if(result == StateSoldierRecenter::BOWMAN_ATTACK)
                {
                    objectStateMachine->changeState(new StateSoldierArchorAttack());
                }
                break;
                
            case BattleConfig::STATE_DEPART:
                if(result == StateSoldierDepart::WORKING)
                {
                    
                }
                else if(result == StateSoldierDepart::BOWMAN_ATTACK)
                {
                    objectStateMachine->changeState(new StateSoldierArchorAttack());
                }
                break;

                
            case BattleConfig::STATE_GENERAL:
                if(result == StateSoldierMoveToEnemyHero::WORKING)
                {
                    
                }
                else if(result == StateSoldierMoveToEnemyHero::ATTACK_ENEMY)
                {
                    objectStateMachine->changeState(new StateSoldierAttackEnemy());
                }
                else if(result == StateSoldierMoveToEnemyHero::FIND_NEARBY_ENEMY_SOLDIER)
                {
                    objectStateMachine->changeState(new StateSoldierMoveToEnemy());
                }
                else if(result == StateSoldierMoveToEnemyHero::BOWMAN_ATTACK)
                {
                    objectStateMachine->changeState(new StateSoldierArchorAttack());
                }
                break;
                
            case BattleConfig::STATE_TARGET:
                if(result == StateSoldierMoveToTargetTile::WORKING)
                {
                    
                }
                else if(result == StateSoldierMoveToTargetTile::ARRIVE)
                {
                    int lastState = ((StateSoldierMoveToTargetTile*)pCurrentState)->getLastStateCode();
                    if(lastState == BattleConfig::STATE_CENTER)
                    {
                        objectStateMachine->changeState(new StateSoldierRecenter());
                    }
                    else if(lastState == BattleConfig::STATE_DEPART)
                    {
                        objectStateMachine->changeState(new StateSoldierDepart());
                    }
                    else if(lastState == BattleConfig::STATE_BACK)
                    {
                        objectStateMachine->changeState(new StateSoldierMoveBack());
                    }
                    else if(lastState == BattleConfig::STATE_PROTECT)
                    {
                        objectStateMachine->changeState(new StateSoldierProtectHero());
                    }
                    else if(lastState == BattleConfig::STATE_SOLDIER)
                    {
                        objectStateMachine->changeState(new StateSoldierMoveToEnemy());
                    }
                    else if(lastState == BattleConfig::STATE_GENERAL)
                    {
                        objectStateMachine->changeState(new StateSoldierMoveToEnemyHero());
                    }
                    else if(lastState == BattleConfig::STATE_IDLE)
                    {
                        objectStateMachine->changeState(new StateSoldierIdle());
                    }
                    else
                    {
                        objectStateMachine->changeState(new StateSoldierIdle());
                    }

                }
                break;
            
            case BattleConfig::STATE_LOST:
                if(result == StateSoldierLost::WORKING)
                {
                    
                }
                else if(result == StateSoldierLost::MOVED_TO_NEW_TILE)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                else if(result == StateSoldierLost::NOT_LOST)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                else if(result == StateSoldierLost::RESTORE)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                break;
                
            case BattleConfig::STATE_BACK:
                if(result == StateSoldierMoveBack::WORKING)
                {
                    
                }
                else if(result == StateSoldierMoveBack::BOWMAN_ATTACK)
                {
                    objectStateMachine->changeState(new StateSoldierArchorAttack());
                }
                break;
                
            case BattleConfig::STATE_HIT_BACK:
                if(result == StateSoldierHitBack::WORKING)
                {
                    
                }
                else if(result == StateSoldierHitBack::DONE)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                break;
                
            case BattleConfig::STATE_DIZZY:
                if(result == StateSoldierDizzy::WORKING)
                {
                    
                }
                else if(result == StateSoldierDizzy::OVER)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                break;
                
            case BattleConfig::STATE_FADEIN:
                if(result == StateSoldierFadeIn::WORKING)
                {
                    
                }
                else if(result == StateSoldierFadeIn::OVER)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                break;
                
            case BattleConfig::STATE_ANCHOR_ATTACK:
                if(result == StateSoldierArchorAttack::WORKING)
                {
                    
                }
                else if(result == StateSoldierArchorAttack::ENEMY_INVALID)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                else if(result == StateSoldierArchorAttack::ATTACK_DONE)
                {
                    objectStateMachine->changeState(new StateSoldierIdle());
                }
                break;
            
            case BattleConfig::SoldierState::STATE_CHANGE_TO_GHOST:
                if(result == StateSoldierChangeToGhost::WORKING)
                {
                    
                }
                else if (result == StateSoldierChangeToGhost::ENEMY_NOT_EXIST)
                {
                    objectStateMachine->changeState(new StateSoldierDead());
                }
                else if (result == StateSoldierChangeToGhost::DONE)
                {
                    objectStateMachine->changeState(new StateSoldierDead());
                }
                break;
                
            case BattleConfig::SoldierState::STATE_CHANGE_TO_BLOCK:
                if(result == StateSoldierChangeToBlock::WORKING)
                {
                    
                }
                else if (result == StateSoldierChangeToBlock::DEAD)
                {
                    objectStateMachine->changeState(new StateSoldierDead());
                }
                break;
            
            case BattleConfig::SoldierState::STATE_BATTLE_END:
                break;
            
            default:
                break;
        }
    }
};
