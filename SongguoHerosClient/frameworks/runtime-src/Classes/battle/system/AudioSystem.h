//
//  AudioSystem.h
//  sanguoClient
//
//  Created by fu.chenhao on 6/4/15.
//
//

#ifndef __sanguoClient__AudioSystem__
#define __sanguoClient__AudioSystem__

#include <stdio.h>

#include "BattleComponent.h"
#include "BattleEvent.h"
#include "BattleWorld.h"
#include "../skills/SkillManager.h"
#include "cocos2d.h"
#include "BattleAudioConfig.h"
#include "BattleWorld.h"


//soldier
#define AUDIOEVENTS_SOLDIER_DEAD    "soldierDead";
#define AUDIOEVENTS_SOLDIER_HIT     "soldierHit";
#define AUDIOEVENTS_SOLDIER_DODGE   "soldierDodge";
#define AUDIOEVENTS_SOLDIER_SKILL_HURT     "soldierSkillHurt";

//hero
#define AUDIOEVENTS_HERO_ATTACK_MAN "heroAttackMan";
#define AUDIOEVENTS_HERO_ATTACK_WOMAN   "heroAttackWoman";

#define AUDIOEVENTS_HERO_DEAD_MAN   "heroDeadMan";
#define AUDIOEVENTS_HERO_DEAD_WOMAN "heroDeadWoman";

#define AUDIOEVENTS_HERO_SKILL_HURT     "heroSkillHurt";
#define AUDIOEVENTS_HERO_DODGE  "heroDodge";
#define AUDIOEVENTS_HERO_HIT    "heroHit";
#define AUDIOEVENTS_HERO_MOVE    "heroMove";
#define AUDIOEVENTS_HERO_ASSAULT    "heroAssault";

//game
#define AUDIOEVENTS_ARMY_ASSAULT   "armyAssault";
#define AUDIOEVENTS_BATTLE_START    "battleStart";
#define AUDIOEVENTS_ARROWSKILL      "skillArrow";

//skill
#define AUDIOEVENTS_RECRUIT     "recruit";


class AudioSystem : public entityx::System<AudioSystem>, public entityx::Receiver<AudioSystem>
{
public:
    void configure(entityx::EventManager& events) override;
    
    void receive(const BattleEvent::BattleSound& e);

    void update(entityx::EntityManager &es, entityx::EventManager &events, double dt) override;
    
    ~AudioSystem();
    
private:

};


static bool isHeroWoman(entityx::Entity& entity)
{
    static int HERO_WOMEN_IDS[] = {40};

    
//    if(!entity.valid())
//    {
//        return false;
//    }
    
    auto hero = entity.component<BattleComponent::General>();
//    if(!hero.valid())
//    {
//        return false;
//    }
    

    int heroId = hero->config.id;
    for(int id : HERO_WOMEN_IDS)
    {
        if(heroId == id)
        {
            return true;
        }
    }
    return false;
}

static std::string getSoldierHittingAudioTriggerName(entityx::Entity& entity)
{
    if(!entity.valid())
    {
        return "";
    }

    auto soldier = entity.component<BattleComponent::Soldier>();
    if(!soldier.valid())
    {
        return "";
    }

    std::string triggerName = "lweapon";
    
    if(soldier->config.soldierType == BattleConfig::SoldierType::ARCHER)
    {
        triggerName = "bow";
    }
    
    return triggerName;
    
}

static std::string getHeroHittingAudioTriggerName(entityx::Entity& entity)
{
    static int HERO_WEAPON_LIGHT[] = {8,11,19,21};
    static int HERO_WEAPON_BLUNT[] = {3,5};

    
    if(!entity.valid())
    {
        return "";
    }
    
    auto hero = entity.component<BattleComponent::General>();
    if(!hero.valid())
    {
        return "";
    }
    
    std::string triggerName = "hweapon";
    
    int heroId = entity.component<BattleComponent::General>()->config.id;
    for(int id : HERO_WEAPON_LIGHT)
    {
        if(heroId == id)
        {
            triggerName = "lweapon";
            break;
        }
    }
    for(int id : HERO_WEAPON_BLUNT)
    {
        if(heroId == id)
        {
            triggerName = "blunt";
            break;
        }
    }
    
    return triggerName;
    
}

/*
static std::string getHeroSoldierAudioTriggerName(entityx::Entity& entity)
{
    if(MapHelper::isHero(entity.component<BattleComponent::Identify>()->id))
    {
        std::string triggerName = "hero";
//        std::string triggerName = (std::stringstream() << "hero_" << entity.component<BattleComponent::General>()->config.id).str();
        return triggerName;
    }
    else
    {
        std::string triggerName = "soldier";
        
//        BattleConfig::SoldierType soldierType = entity.component<BattleComponent::Soldier>()->config.soldierType;
//        if(soldierType == BattleConfig::SoldierType::MELEE)
//        {
//            triggerName = "melee";
//        }
//        else if(soldierType == BattleConfig::SoldierType::ARCHER)
//        {
//            triggerName = "arrow";
//        }

        return triggerName;
    }

}
 */

#endif /* defined(__sanguoClient__AudioSystem__) */
