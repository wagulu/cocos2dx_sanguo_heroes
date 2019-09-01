//
//  SkillShanXian.cpp
//  sanguoClient
//
//  Created by fuchenhao on 4/28/15.
//
//

#include "SkillShanXian.h"
#include "BattleWorld.h"

void SkillShanXian::fireAction(entityx::EntityManager &es)
{
    auto* entity = _BATTLE_ENTITY.getEntity(m_fromId);
    
    /*
    震动：
    1. 起手震动（根据动画时长判定震动，单个震动）
    2. 命中震动（根据动画时长判定震动，单个震动）
    镜头：
    先移动到发起者所在位置，随后快速移动到武将落地的位置（取决于武将落地位置具体在哪）
    效果
    1. 闪现移动距离=5 （该值可配置）
    2. 落地后影响范围为以武将为中心的对角线长度为9的菱形；（该区域形状可配置）
    3. 伤害值=基础（200）+技能等级×技能伤害增量（10）+倍数（1.2）×武将物理攻击值
    4. 对方等级小于等级发起者等级，100%眩晕
    5. 对方等级高于发起者等级，效果生效几率=1-等级差/（等级差+20）
    6. 眩晕时间（秒）=min(2,1+rounddown(技能等级×0.3)/10)
    */
    
    if(entity)
    {
        auto position = (*entity).component<BattleComponent::Position>();
        auto identify = (*entity).component<BattleComponent::Identify>();
        auto tileZ = MapHelper::getTileZ(position->z);
        auto tileX = MapHelper::getTileX(position->x);
        int step = identify->faceTo == BattleConfig::FACE_TO_RIGHT ?  1 : -1;
        int startX = step == 1 ? tileX + 1: tileX - 2;
        
        int maxTile = (int)m_skillActionData->config["tile"]->d;
        int result = tileX + maxTile * step;
        bool shouldMove = true;
        for(int i = 0; i < maxTile; i++)
        {
            int target = _BATTLE_ENTITY.getTarget(tileZ, startX + step * i);
            if(target && MapHelper::getEntityType(target) == BattleConfig::ENTITY_HERO)
            {
                result = tileX + step * i;
                if(i == 0)shouldMove = false;
                break;
            }
        }

        if(shouldMove)
        {
            position->x = result * GRID_WIDTH + HALF_GRID_WIDTH;
            _HERO_MOVEMENT.moveTo(position->x, position->y, position->z, (*entity));
        }
        
        _ENTITY_EVENT.emit<BattleEvent::TriggerSkillAction>(m_entityId, *m_skillActionData->config["trigger"]->s);
    }
    
    remove();
}
