//
//  AISystem.cpp
//  Game
//
//  Created by fuchenhao on 3/11/15.
//
//

#include "AISystem.h"
#include "cocos2d.h"
#include "BattleWorld.h"
#include "DebugHelper.h"

AISystem::~AISystem()
{
    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_START_DELEGATE);
    cocos2d::Director::getInstance()->getEventDispatcher()->removeCustomEventListeners(EVENT_STOP_DELEGATE);
}

void AISystem::startDelegate(cocos2d::EventCustom* event)
{
    auto* entity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
    
    if(entity)
    {
        auto hero = (*entity).component<BattleComponent::General>();
        if(hero->config.strategy == "")
        {
            //AIManager::getInstance()->createAI("ai_delegate", LEFT_HERO);
        }
        else
        {
            AIManager::getInstance()->createAI(hero->config.strategy, LEFT_HERO);
        }
    }
}

void AISystem::stopDelegate(cocos2d::EventCustom* event)
{
    auto* entity = _BATTLE_ENTITY.getEntity(LEFT_HERO);
    
    if(entity)
    {
        auto aiComp = (*entity).component<BattleComponent::AI>();
        aiComp->clearAllAI();
        aiComp->strategy = "";
    }
}


void AISystem::configure(entityx::EventManager& events)
{
    events.subscribe<BattleEvent::AddAI>(*this);
    
    cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_START_DELEGATE, CC_CALLBACK_1(AISystem::startDelegate, this));
    cocos2d::Director::getInstance()->getEventDispatcher()->addCustomEventListener(EVENT_STOP_DELEGATE, CC_CALLBACK_1(AISystem::stopDelegate, this));
};

void AISystem::receive(const BattleEvent::AddAI& e)
{
#if(ACTIVE_AI)
    AIManager::getInstance()->createAI(e.name, e.heroId);
#endif
};

void AISystem::update(entityx::EntityManager &es, entityx::EventManager &events, double dt)
{
    BattleComponent::AI::Handle aiComp;
    
    for (entityx::Entity entity : es.entities_with_components(aiComp))
    {
        for (auto* ai : aiComp->ai)
        {
            switch(ai->state)
            {
                case BattleConfig::AI_WAITING:
                {
                    bool isOk = true;
                    for(auto condition : *ai->conditions)
                    {
                        if(!condition->isOk())
                        {
                            isOk = false;
                        }
                    }
                    if(isOk)
                    {
                        ai->elapse = 0;
                        ai->state = BattleConfig::AI_DELAY;
                        for(auto condition : *ai->conditions)
                        {
                            condition->reset();
                        }
                    }
                    break;
                }
                
                case BattleConfig::AI_DELAY:
                {
                    ai->elapse += dt;
                    if(ai->elapse >= ai->action->getDelay())
                    {
                        ai->state = BattleConfig::AI_WORKING;
                    }
                    break;
                }
                
                case BattleConfig::AI_WORKING:
                {
                    if(ai->action->isComplete())
                    {
                        if(ai->repeated)
                        {
                            ai->action->reset();
                            ai->state = BattleConfig::AI_WAITING;
                        }
                        else
                        {
                            ai->state = BattleConfig::AI_COMPLETE;
                        }
                    }
                    else
                    {
                        ai->action->tryAction();
                    }
                    break;
                }
                
                case BattleConfig::AI_COMPLETE:
                {
                    break;
                }
            }
        }
        aiComp->clearCompletedAI();
    }
};
