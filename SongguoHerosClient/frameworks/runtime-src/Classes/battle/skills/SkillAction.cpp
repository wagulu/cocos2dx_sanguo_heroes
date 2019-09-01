//
//  SkillAction.cpp
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//

#include "SkillAction.h"
#include "BattleWorld.h"
#include "SkillObject.h"
#include "EnumParser.h"

SkillAction::SkillAction() : ActiveObject()
{
    m_fired = false;
    m_endTime = -1;
}

SkillAction::~SkillAction()
{
    
}

void SkillAction::update(entityx::EntityManager &es, float elapse, float dt)
{
    
}

void SkillAction::fireAction(entityx::EntityManager &es)
{
    m_fired = true;
}

void SkillAction::onTriggerFired(const std::string& trigger)
{
    
}

/***
 
 1.
 封锁时间/眩晕时间1/buff时间/debuff时间/不死时间/战魂HP/buff增量
 max(5,min(10,rounddown(abilityLevel/10)))
 
 "max" (min, max, divide)
 
 2.
 判定效果是否生效（眩晕/封锁等）
 根据“目标等级”与“技能等级+等级修正”两个值判定，如技能等级+等级修正大于目标等级，100%生效，否则效果生效几率=1-等级差/(等级差+20)
 
 "enable" (base修正等级, step)
 
 3.
 减血/减MP/补血
 基础+技能等级×技能伤害增量+倍数*武将法术攻击值（or武将物理攻击值）
 
 "damage" (base, step, property, factor)
 
 4.
 眩晕时间2/士兵召唤数量/MP增益百分比/战魂存活时间/士兵自爆几率/buff增量/普攻眩晕几率增量
 min(2,1+rounddown(技能等级×0.3)/10)
 
 "min" (min, base, factor, divide)
 
 5.
 波动HP/盾HP/附加真实伤害/复活后HP百分比
 3000+abilityLevel*100
 
 "hp" (base, step)
 
 6.
 buff/debuff
 3000+abilityLevel*100
 
 "buff" (base, step)
 
 ***/

float SkillAction::caclFormulaResult(const std::string name)
{
    auto* formula = m_formula->getFormula(name);
    
    if(formula->formula == "max")
    {
        return max(formula->min, min(formula->max, floor((float)m_skillData->level / formula->divide)));
    }
    else if(formula->formula == "min")
    {
        return min(formula->min, formula->base + floor((float)m_skillData->level * formula->factor) / formula->divide);
    }
    else if(formula->formula == "damage")
    {
        auto* pEntity = _BATTLE_ENTITY.getEntity(m_fromId);
        if (pEntity)
        {
            auto property = pEntity->component<BattleComponent::Property>();
            auto value = (float)property->get(EnumParserPropertyName().getValue(formula->property));
            return formula->base + (float)m_skillData->level * formula->step + formula->factor * value;
        }
    }
    else if(formula->formula == "hp")
    {
        return formula->base + (float)m_skillData->level * formula->step;
    }
    else if(formula->formula == "buff")
    {
        return formula->base + (float)m_skillData->level * formula->step;
    }
    
    return 0;
}

bool SkillAction::caclFormulaEnable(const std::string name, int targetId)
{
    auto* formula = m_formula->getFormula(name);
    
    if(formula->formula == "enable")
    {
        auto* pOppHeroEntity = _BATTLE_ENTITY.getEntity(targetId);
        if (pOppHeroEntity != nullptr)
        {
            int oppHeroLevel = pOppHeroEntity->component<BattleComponent::General>()->config.level;
            
            float hitRate = 0;
            float level = (float)m_skillData->level + formula->base;
            if (level >= oppHeroLevel)
            {
                hitRate = 100;
            }
            else
            {
                float levelDiff = abs(oppHeroLevel - level);
                hitRate = (1.0f - levelDiff / (levelDiff + formula->step)) * 100;
            }
            
            float diceValue = cocos2d::random<int>(1, 100);
            if (diceValue <= hitRate)
            {
                return true;//you win!
            }
        }
    }
    return false;
}
