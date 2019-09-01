//
//  CallSoldiers.cpp
//  Game
//
//  Created by fuchenhao on 15-4-15.
//
//

#include "CallSoldiers.h"
#include "BattleWorld.h"
#include "StateSoldierDead.h"
#include "AudioManager.h"
#include "AudioSystem.h"

CallSoldiers::~CallSoldiers()
{
    if(_ENTITY_EVENT.hasReceive<BattleEvent::BattleActorRebirth>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::BattleActorRebirth>(*this);
    }
    
    if(_ENTITY_EVENT.hasReceive<BattleEvent::HurtByAttack>(*this))
    {
        _ENTITY_EVENT.unsubscribe<BattleEvent::HurtByAttack>(*this);
    }
    
    m_killSoldiers.clear();
}


void CallSoldiers::subscribeEvent()
{
    _ENTITY_EVENT.subscribe<BattleEvent::HurtByAttack>(*this);
}

void CallSoldiers::receive(const BattleEvent::HurtByAttack& e)
{
    // virus
    if (m_virus) {
        BattleComponent::Soldier::Handle fSoldier = e.fromEntity.component<BattleComponent::Soldier>();
        if(fSoldier.valid() && fSoldier->config.soldierType == BattleConfig::SoldierType::VIRUS)
        {
            bool turnEnable = false;
            if(m_skillActionData->hasMember("turnRatio"))
            {
                float turnRatio = caclFormulaResult(*m_skillActionData->config["turnRatio"]->s);
//                CCLOG("======== turnRatio:%f", turnRatio);
                turnEnable = cocos2d::rand_0_1() < turnRatio;
            }
            
//            if (cocos2d::rand_0_1() < 0.5)
            if (turnEnable)
            {
                killSoldierAtOnce(e.targetEntity, true, 0.2);
            }
        }
    }
}

void CallSoldiers::receive(const BattleEvent::BattleActorRebirth& e)
{
    if (e.skillUniID != m_entityId.id())
    {
        return;
    }
    
    auto& soldierConfig = m_skillData->pSoldier;
    
    auto targetProperty = e.entity.component<BattleComponent::Property>();
    int curHp = targetProperty->get(BattleConfig::Property::HP);
    
    BattleConfig::Side nextSide = changeSoldierSide(e.orgSide);
    
    int changedID = -1;
    if (nextSide == BattleConfig::SIDE_RIGHT)
    {
        // right soldier
        auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
        auto& enemySoldierConfig = battleFieldConfig.enemyTeam.soldier;
//        auto& soldierRightConfig = FormationManager::getInstance()->getFormation(BattleConfig::getInstance()->battleFieldConfig.enemyTeam.formation);
        changedID = _SOLDIER_CREATOR.addSoldier(nextSide, enemySoldierConfig, e.tileZ, e.tileX, 0, false, curHp);
    }
    else
    {
//        auto& battleFieldConfig = BattleConfig::getInstance()->getBattleFieldConfig();
//        auto& enemySoldierConfig = battleFieldConfig.enemyTeam.soldier;
//        changedID = _SOLDIER_CREATOR.addSoldier(nextSide, enemySoldierConfig, e.tileZ, e.tileX, 0, false, curHp);
        
        changedID = _SOLDIER_CREATOR.addSoldier(nextSide, *soldierConfig, e.tileZ, e.tileX, 0, false, curHp);
        changeRoleColor(changedID, true, false, false);
        
        auto* pTheEntity = _BATTLE_ENTITY.getEntity(_BATTLE_ENTITY.getTarget(e.tileZ, e.tileX));
        if (m_loopSoliderEffect != NULL)
        {
            _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(m_loopSoliderEffect, false, *pTheEntity, 0.0, m_holdTime - 1.1);
        }
    }

    if(changedID != -1)
    {
//        _ENTITY_EVENT.emit<BattleEvent::AddEffect>(m_showSoliderEffect, false, (e.tileX + 0.5) * GRID_WIDTH, 0.0, (e.tileZ + 0.5) * GRID_HEIGHT, 0.0f, 0.0f);
        m_createSoldiers.push_back(changedID);
    }
}


void CallSoldiers::changeRoleColor(int changedID, bool effect, bool opp, bool colorChange)
{
    auto* theNewEntity = _BATTLE_ENTITY.getEntity(changedID);
    
    if (theNewEntity == nullptr) return;
    
    BattleComponent::Soldier::Handle newSoldier = theNewEntity->component<BattleComponent::Soldier>();
    if(newSoldier.valid() && newSoldier->config.soldierType == BattleConfig::SoldierType::VIRUS)
    {
        m_virus = true;
        
        // use art color change
//        auto animation = theNewEntity->component<BattleComponent::Animation>();
//        
//        if (colorChange)
//        {
//            if (opp)
//            {
//                animation->battleAnimation->setColorR(0);
//                animation->battleAnimation->setColorB(0);
//            }
//            else
//            {
//                animation->battleAnimation->setColorG(0);
//                animation->battleAnimation->setColorB(0);
//            }
//        }
    }
    
    if (effect)
    {
        std::string effectName = "turn_solider";
        BattleConfig::EffectGroup* group = m_effect->getEffect(effectName);
        
        if(group != nullptr)
        {
            auto position = theNewEntity->component<BattleComponent::Position>();
            bool reserse = m_side != BattleConfig::SIDE_LEFT;
            
            _ENTITY_EVENT.emit<BattleEvent::AddEffect>(group, reserse, position->x, position->y, position->z, 0.0f, 0.0f);
        }
    }
}


int CallSoldiers::getTargetHeroID(int toSide)
{
    int targetId;
    if(toSide == BattleConfig::SIDE_RIGHT)
    {
        if(m_side == BattleConfig::SIDE_LEFT)
        {
            targetId = RIGHT_HERO;
        }
        else
        {
            targetId = LEFT_HERO;
        }
    }
    else
    {
        if(m_side == BattleConfig::SIDE_LEFT)
        {
            targetId = LEFT_HERO;
        }
        else
        {
            targetId = RIGHT_HERO;
        }
    }
    
    return targetId;
}


BattleConfig::Side CallSoldiers::changeSoldierSide(BattleConfig::Side curSide)
{
    if (curSide == BattleConfig::SIDE_LEFT)
    {
        return BattleConfig::SIDE_RIGHT;
    }
    else if (curSide == BattleConfig::SIDE_RIGHT)
    {
        return BattleConfig::SIDE_LEFT;
    }
    else
    {
        return BattleConfig::SIDE_ALL;
    }
}


void CallSoldiers::killSoldierAtOnce(entityx::Entity entity, bool needRebirth, float delay)
{
    if (!entity.valid()) return;
    
    auto identify = entity.component<BattleComponent::Identify>();
    BattleConfig::AttackData attackData(m_fromId, 0);
    attackData.setTargetId(identify->id);
    attackData.setSkillData(m_skillId, m_skillActionData->damageData, m_entityId);
    
    BattleComponent::Soldier::Handle soldier = entity.component<BattleComponent::Soldier>();
    BattleComponent::ObjectStateMachine::Handle objectStateMachine = entity.component<BattleComponent::ObjectStateMachine>();
    
    if(soldier.valid())
    {
        int objectStateCode = objectStateMachine->getCurrentStateCode();
        
        if(objectStateCode != BattleConfig::STATE_DEAD)
        {
            objectStateMachine->changeState(new StateSoldierDead(attackData, true, needRebirth, m_entityId.id(), delay));
        }
    }
}


void CallSoldiers::changeSoldierToBlockState(int targetId)
{
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity)
    {
        BattleEvent::StrategySoldierChangeToBlock eventBlock(*pTargetEntity, m_holdTime);
        eventBlock.appearEffect = m_effect->getEffect("show_solider");
        eventBlock.loopEffect = m_effect->getEffect("loop_solider");
        eventBlock.deadEffect = m_effect->getEffect("hide_solider");
        _ENTITY_EVENT.emit(eventBlock);
    }
}

void CallSoldiers::update(entityx::EntityManager &es, float elapse, float dt)
{
    if (m_delayKill > 0)
    {
        m_delayElapse += dt;
    
        if(m_delayElapse >= m_delayKill)
        {
            for(auto& entity : m_killSoldiers)
            {
                killSoldierAtOnce(entity);
            }
            
            m_delayKill = -1;
        }
    }
    
    if (m_holdTime > 0)
    {
        m_elapse += dt;
        
        if(m_elapse >= m_holdTime)
        {
//            CCLOG("============ time up! after  %d", m_soldierSide);
            if(m_createSoldiers.size() > 0)
            {
                for(auto it = m_createSoldiers.begin(); it != m_createSoldiers.end();)
                {
                    auto sId = *it;
                    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(sId);
                    it = m_createSoldiers.erase(it);
                    
                    if(pTargetEntity != NULL)
                    {
                        killSoldierAtOnce(*pTargetEntity, true);
                        
                        std::string killEffectName = "hit_solider2";
                        BattleConfig::EffectGroup* killGroup = m_effect->getEffect(killEffectName);
                        
                        if(killGroup != nullptr)
                        {
                            bool reserse = m_side != BattleConfig::SIDE_LEFT;
                            auto position = pTargetEntity->component<BattleComponent::Position>();
                            _ENTITY_EVENT.emit<BattleEvent::AddEffect>(killGroup, reserse, position->x, position->y, position->z, 0.0f, 0.0f);
                        }
                    }
                }
            }
            
            m_createSoldiers.clear();
            m_holdTime = -1;
        }
    }
}


void CallSoldiers::fireAction(entityx::EntityManager &es)
{
    SkillAction::fireAction(es);
    subscribeEvent();
 
    m_soldierSide = m_side;
    
    int toSide = BattleConfig::SIDE_LEFT;
    if (m_skillActionData->config["tar"])
    {
        toSide = m_skillActionData->config["tar"]->d;
    }
    int targetId = getTargetHeroID(toSide);
    
    auto* pTargetEntity = _BATTLE_ENTITY.getEntity(targetId);
    if (pTargetEntity == nullptr)
    {
        remove();
        return;
    }
    
    std::string formation = "c2";
    if (m_skillActionData->config["formation"])
    {
        formation = *m_skillActionData->config["formation"]->s;
    }

    BattleConfig::FormationConfig& formationConfig = FormationManager::getInstance()->getFormation(formation);
    
    std::string effectName = "hit_solider";
    if (m_skillActionData->config["effect"])
    {
        effectName = *m_skillActionData->config["effect"]->s;
    }
    m_showSoliderEffect = m_effect->getEffect(effectName);
    bool reserse = m_side != BattleConfig::SIDE_LEFT;

    std::string effectLoopName = "loop_solider";
    if (m_skillActionData->config["loopEffect"])
    {
        effectLoopName = *m_skillActionData->config["loopEffect"]->s;
    }
    m_loopSoliderEffect = m_effect->getEffect(effectLoopName);

    int mode = UP_AND_DOWN;
    if (m_skillActionData->config["mode"])
    {
        mode = m_skillActionData->config["mode"]->d;
    }
    
    auto& soldierConfig = m_skillData->pSoldier;
    
    int tileX = MapHelper::getTileX(pTargetEntity->component<BattleComponent::Position>()->x);
    int xPosDelta = 0;
    
    int xPosBaseDelta = 8;
    if (m_skillActionData->config["dtX"])
    {
        xPosBaseDelta = m_skillActionData->config["dtX"]->d;
    }
    
    bool turn = false;
    if (m_skillActionData->config["turn"])
    {
        turn = true;
    }
    
    m_holdTime = -1;
    if (m_skillActionData->config["dur"])
    {
        m_holdTime = m_skillActionData->config["dur"]->d;
    } else if (m_skillActionData->config["durFormula"])
    {
        m_holdTime = caclFormulaResult(*m_skillActionData->config["durFormula"]->s);
//        CCLOG("=========== m_holdTime： %f", m_holdTime);
    }

    float timeDelay = 0.08f;
    switch (mode) {
        case UP_AND_DOWN:
            xPosDelta = formationConfig.validSoldiersNum / 7 + 1;
            tileX -= xPosDelta;
            timeDelay = 0.08;
            break;
        
        case BEHIND_HERO:
            xPosDelta = xPosBaseDelta;
            if(reserse)
            {
                xPosDelta = -xPosDelta;
            }
        
            tileX -= xPosDelta;
            if (m_side == BattleConfig::SIDE_RIGHT)
            {
                tileX -= 1;
            }
 
            timeDelay = 0.02;
            break;
        
        default:
            break;
    }
    
    bool turnFace = false;
    if((MapHelper::getSide(targetId) != m_side) && ((toSide == BattleConfig::SIDE_LEFT && xPosBaseDelta > 0) ||
       (toSide == BattleConfig::SIDE_RIGHT && xPosBaseDelta < 0)))
    {
        turnFace = true;
    }
    
    int callNum = formationConfig.validSoldiersNum;
//    if (m_skillActionData->config["num"] != NULL)
//    {
//        callNum = m_skillActionData->config["num"]->d;
//    }
    
    // TODO chance
    if (mode == TURN_ENEMY || turn)
    {
        _ENTITY_EVENT.subscribe<BattleEvent::BattleActorRebirth>(*this);
    }
    
    m_createSoldiers.clear();
    
    bool hasSendAudio = false;
    
    if (mode != TURN_ENEMY)
    {
        int soldierNumber = (int)caclFormulaResult(*m_skillActionData->config["num"]->s);

        for(int i= 0; i < soldierNumber && i < callNum; i++)
        {
            auto& pos = formationConfig.getSoldierPos(i);
            if(pos.x >= 0 && pos.z >= 0)
            {
                int deltaX = 0;
                
                float randomT = cocos2d::rand_0_1() / 5.0f;
//            CCLOG("========= random: %f", randomT);
                
                if (m_side == BattleConfig::SIDE_LEFT)
                {
                    while (tileX + pos.x - deltaX < STAGE_WIDTH && tileX + pos.x - deltaX > 0)
                    {
                        int createdID = _SOLDIER_CREATOR.addSoldier(BattleConfig::SIDE_LEFT, *soldierConfig, pos.z, tileX + pos.x - deltaX, timeDelay + randomT, turnFace);
                        if (createdID != -1)
                        {
                            if (soldierConfig->soldierConfigData.soldierType == BattleConfig::SoldierType::BLOCK)
                            {
                                changeSoldierToBlockState(createdID);
                            }
                            else
                            {
                                changeRoleColor(createdID);
                                
                                m_createSoldiers.push_back(createdID);
                                _ENTITY_EVENT.emit<BattleEvent::AddEffect>(m_showSoliderEffect, reserse, (tileX + pos.x - deltaX + 0.5) * GRID_WIDTH, 0.0, (pos.z + 0.5) * GRID_HEIGHT, randomT, 0.0f);
                                
                                if(!hasSendAudio)
                                {
                                    hasSendAudio = true;
                                    sendAudio_recruit();
                                }
                            }
                            break;
                        }
                        
                        if (deltaX >= 0) deltaX++;
                        
                        //                      if (mode == UP_AND_DOWN)
                        deltaX = -deltaX;
                    }
                }
                else
                {
                    while ((tileX - (pos.x - deltaX)) < STAGE_WIDTH && (tileX - (pos.x - deltaX)) > 0)
                    {
                        int createdID = _SOLDIER_CREATOR.addSoldier(BattleConfig::SIDE_RIGHT, *soldierConfig, pos.z, (tileX - (pos.x - deltaX)), timeDelay + randomT, turnFace);
                        if (createdID != -1)
                        {
                            if (soldierConfig->soldierConfigData.soldierType == BattleConfig::SoldierType::BLOCK)
                            {
                                changeSoldierToBlockState(createdID);
                            }
                            else
                            {
                                changeRoleColor(createdID, false, true);
                                
                                m_createSoldiers.push_back(createdID);
                                _ENTITY_EVENT.emit<BattleEvent::AddEffect>(m_showSoliderEffect, reserse, ((tileX - (pos.x - deltaX)) + 0.5) * GRID_WIDTH, 0.0, (pos.z + 0.5) * GRID_HEIGHT, randomT, 0.0f);
                                
                                if(!hasSendAudio)
                                {
                                    hasSendAudio = true;
                                    sendAudio_recruit();
                                }
                            }
                            break;
                        }
                        
                        if (deltaX >= 0) deltaX++;
                        
//                      if (mode == UP_AND_DOWN)
                        deltaX = -deltaX;
                    }
                }
            }
        }
    }
    else
    {
        auto& entities = (m_side == BattleConfig::SIDE_LEFT) ? _BATTLE_ENTITY.getRightSoldiers() : _BATTLE_ENTITY.getLeftSoldiers();
        
//        m_delayKill = 0.01;
        m_delayKill = 1.1;
        if (m_holdTime > 0)
        {
            m_holdTime += m_delayKill;
        }

        int turnNum = entities.size();
        if (m_skillActionData->config["turnNum"])
        {
            int turnCalcNum = caclFormulaResult(*m_skillActionData->config["turnNum"]->s);
            if (turnCalcNum >=0 && turnCalcNum < turnNum)
            {
                turnNum = turnCalcNum;
            }
        }
        
        m_killSoldiers.clear();
        for(auto& entity : entities)
        {
            if (turnNum == 0) break;
            
            m_killSoldiers.push_back(entity);
            _ENTITY_EVENT.emit(BattleEvent::StrategySoldierDizzy(entity, m_delayKill));
//            _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(pEffectGroup, reserse, *heroEntity, 0.1, m_skillActionData->config["dur"]->d);
            _ENTITY_EVENT.emit<BattleEvent::AddEffectToEntity>(m_showSoliderEffect, false, entity, 0.0, 0.0);
//            auto position = entity.component<BattleComponent::Position>();
//            int tileX = MapHelper::getTileX(position->x);
//            int tileZ = MapHelper::getTileZ(position->z);
//            _ENTITY_EVENT.emit<BattleEvent::AddEffect>(m_showSoliderEffect, false, (tileX + 0.3) * GRID_WIDTH, 0.0, (tileZ + 0.2) * GRID_HEIGHT, 0.0f, 0.0f);
            turnNum--;
        }
    }
    
//    remove();
}


void CallSoldiers::sendAudio_recruit()
{
    std::string eventName = AUDIOEVENTS_RECRUIT;
    std::string triggerName = "skill"; //getHeroSoldierAudioTriggerName(entity);
    BattleEvent::BattleSound event(eventName, triggerName, entityx::Entity());
    _ENTITY_EVENT.emit(event);

}
