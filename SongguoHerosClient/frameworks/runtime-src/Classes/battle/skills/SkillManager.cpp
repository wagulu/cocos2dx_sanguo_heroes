//
//  SkillManager.cpp
//  Game
//
//  Created by fuchenhao on 2/25/15.
//
//

#include "SkillManager.h"

#include "actions/ShowBackground.h"
#include "actions/PauseBattle.h"
#include "actions/skill/SkillFenShenNuZhan.h"
#include "actions/MultiAttack.h"
#include "actions/attack/AttackDiamondAreaCenterOnHero.h"
#include "actions/attack/AttackDiamondAreaCenterOnTile.h"
#include "actions/attack/AttackOnTile.h"
#include "actions/skill/SkillChiTianBaFangYan.h"
#include "actions/skill/SkillGuiJiShiHun.h"
#include "actions/skill/SkillLieKongFengRen.h"
#include "actions/skill/CallSoldiers.h"
#include "actions/skill/SkillGuiCai.h"
#include "actions/skill/SkillTiaoJi.h"
#include "actions/skill/SkillShunXiQianLi.h"
#include "PlayAnimation.h"
#include "PlaySound.h"
#include "ShakeCamera.h"
#include "MoveCameraToHero.h"
#include "MoveCameraToTile.h"
#include "SkillWuYueHuaZhan.h"
#include "SkillMuLunQiangXi.h"
#include "AttackRectAreaOnHero.h"
#include "SkillBaJiaoShan.h"
#include "IntervalTrigger.h"
#include "AddBuff.h"
#include "AddBuffOnSkillAttack.h"
#include "FindTarget.h"
#include "FindTargetByFormation.h"
#include "FindTileCenterOnMostPeople.h"
#include "FindTileOnEnemyHero.h"
#include "AttackOnFindTarget.h"
#include "SetStateDizzyOnFindTarget.h"
#include "SetStateDizzyOnSkillAttack.h"
#include "SetStateDizzyOnNormalAttack.h"
#include "AddEffectOnSkillAttack.h"
#include "AddEffectOnAddNormalAttack.h"
#include "SkillDuration.h"
#include "ColorTween.h"
#include "AddEntityAnimation.h"
#include "RemoveEntityAnimation.h"
#include "AddEntityEffect.h"
#include "AddTileEffect.h"
#include "ShowHeroEffect.h"
#include "ReduceEnemyMp.h"
#include "AddMpOnReduceEnemyMp.h"
#include "AddHpOnReduceEnemyHp.h"
#include "RemoveShieldAnimationOnUseUp.h"
#include "SkillNiePan.h"
#include "SkillZhanHun.h"
#include "SkillShanXian.h"
#include "SkillHongLianYeHuo.h"
#include "SkillLianHuan.h"
#include "AttackRectArroundHero.h"
#include "AttackRectArroundTile.h"
#include "SkillTianDiWuYong.h"
#include "DisableBreak.h"
#include "LockFireSkill.h"
#include "LockUserControl.h"
#include "LockOppSkill.h"
#include "AssignEnemyHeroCenter.h"
#include "FindRandomTargetAroundTile.h"
#include "DelayTrigger.h"
#include "CancelSkill.h"
#include "SkillChuanSuo.h"
#include "EmptyArrow.h"
#include "SkillArrow.h"
#include "SkillNuJi.h"
#include "TriggerOnSkillAttack.h"
#include "TriggerCameraActions.h"
#include "BuffEffect.h"
#include "TriStageTrigger.h"

SkillManager* SkillManager::s_sharedSkillManager = nullptr;

SkillManager* SkillManager::getInstance()
{
    if(s_sharedSkillManager == nullptr)
    {
        s_sharedSkillManager = new SkillManager();
    }
    return s_sharedSkillManager;
}

SkillManager::SkillManager(void)
{
    registerBuilder<ShowBackground>("show_background");
    registerBuilder<PauseBattle>("pause_battle");
    registerBuilder<PlayAnimation>("play_animation");
    registerBuilder<PlaySound>("play_sound");
    registerBuilder<ShowHeroEffect>("show_hero_effect");
    registerBuilder<SkillFenShenNuZhan>("skill_fen_shen_nu_zhan");
    registerBuilder<SkillLieKongFengRen>("skill_lie_kong_feng_ren");
    registerBuilder<AddBuff>("add_buff");
    registerBuilder<BuffEffect>("buff_effect");
    registerBuilder<AddBuffOnSkillAttack>("add_buff_on_skill_attack");
    registerBuilder<MultiAttack>("multi_attack");
    registerBuilder<AttackRectAreaOnHero>("attack_rect");
    registerBuilder<MoveCameraToHero>("move_camera_to_hero");
    registerBuilder<MoveCameraToTile>("move_camera_to_tile");
    registerBuilder<ShakeCamera>("shake_camera");
    registerBuilder<TriggerCameraActions>("camera_action");
    registerBuilder<AttackDiamondAreaCenterOnHero>("attack_diamond_area_center_on_hero");
    registerBuilder<AttackDiamondAreaCenterOnTile>("attack_diamond_area_center_on_tile");
    registerBuilder<AttackOnFindTarget>("attack_on_find_target");
    registerBuilder<AttackOnTile>("skill_attack_on_tile");
    registerBuilder<AttackRectArroundHero>("attack_rect_arround_hero");
    registerBuilder<AttackRectArroundTile>("attack_rect_arround_tile");
    
    registerBuilder<SkillChiTianBaFangYan>("skill_chi_tian_ba_fang_yan");
    registerBuilder<SkillGuiJiShiHun>("skill_gui_ji_shi_hun");
    registerBuilder<SkillWuYueHuaZhan>("skill_wu_yue_hua_zhan");
    registerBuilder<SkillMuLunQiangXi>("skill_mu_lun_qiang_xi");
    registerBuilder<SkillGuiCai>("skill_gui_cai");
    registerBuilder<SkillTiaoJi>("skill_tiao_ji");
    registerBuilder<SkillShunXiQianLi>("skill_shun_xi_qian_li");
    registerBuilder<SkillNiePan>("nie_pan");
    registerBuilder<SkillZhanHun>("zhan_hun");
    registerBuilder<SkillShanXian>("skill_shan_xian");
    registerBuilder<SkillTianDiWuYong>("tian_di_wu_yong");
    registerBuilder<SkillHongLianYeHuo>("hong_lian_ye_huo");
    registerBuilder<SkillChuanSuo>("chuan_suo");
    registerBuilder<SkillLianHuan>("skill_lian_huan");
    registerBuilder<EmptyArrow>("empty_arrow");
    registerBuilder<SkillArrow>("skill_arrow");
    registerBuilder<SkillBaJiaoShan>("skill_ba_jiao_shan");
    registerBuilder<SkillNuJi>("skill_nu_ji");
    registerBuilder<TriggerOnSkillAttack>("trigger_on_skill_attack");
    registerBuilder<TriStageTrigger>("tri_stage_trigger");
    
    registerBuilder<CallSoldiers>("call_solders");
    registerBuilder<CallSoldiers>("turn_solders");
    registerBuilder<ColorTween>("color_tween");
    registerBuilder<FindTarget>("find_target");
    registerBuilder<FindTargetByFormation>("find_target_by_formation");
    registerBuilder<FindTileCenterOnMostPeople>("find_tile_center_on_most_people");
    registerBuilder<FindTileOnEnemyHero>("find_tile_on_enemy_hero");
    registerBuilder<SetStateDizzyOnFindTarget>("set_state_dizzy_on_find_target");
    registerBuilder<SetStateDizzyOnSkillAttack>("set_state_dizzy_on_skill_attack");
    registerBuilder<SetStateDizzyOnNormalAttack>("set_state_dizzy_on_normal_attack");
    registerBuilder<AddEffectOnSkillAttack>("add_effect_on_skill_attack");
    registerBuilder<AddEffectOnAddNormalAttack>("add_effect_on_add_normal_attack");
    registerBuilder<SkillDuration>("skill_duration");
    registerBuilder<IntervalTrigger>("interval_trigger");
    registerBuilder<AddEntityAnimation>("add_entity_animation");
    registerBuilder<RemoveEntityAnimation>("remove_entity_animation");
    registerBuilder<AddEntityEffect>("add_entity_effect");
    registerBuilder<AddTileEffect>("add_tile_effect");
    registerBuilder<ReduceEnemyMp>("reduce_enemy_mp");
    registerBuilder<AddMpOnReduceEnemyMp>("add_mp_on_reduce_enemy_mp");
    registerBuilder<AddHpOnReduceEnemyHp>("add_hp_on_reduce_enemy_hp");
    registerBuilder<RemoveShieldAnimationOnUseUp>("remove_shield_animation_on_use_up");
    registerBuilder<AssignEnemyHeroCenter>("assign_enemy_hero_center");
    registerBuilder<FindRandomTargetAroundTile>("find_random_target_around_tile");
    
    registerBuilder<CancelSkill>("cancel_skill");
    registerBuilder<DisableBreak>("disable_break");
    registerBuilder<LockOppSkill>("lock_opp_skill");
    registerBuilder<LockFireSkill>("lock_fire_skill");
    registerBuilder<LockUserControl>("lock_user_control");
    registerBuilder<DelayTrigger>("delay_trigger");
};

SkillManager::~SkillManager(void)
{
    for(auto it = m_builderMap.begin(); it != m_builderMap.end(); ++it)
    {
        delete it->second;
    }
};
    
template<typename TActionClass>
void SkillManager::registerBuilder(const std::string& key)
{
    auto it = m_builderMap.find(key);
    if (it == m_builderMap.end())
    {
        m_builderMap[key] = new ActionBuilder<TActionClass>();
    }
};
    
SkillAction* SkillManager::buildAction(const std::string& actionName)
{
    auto it = m_builderMap.find(actionName);
    if (it == m_builderMap.end()) return nullptr;
    return it->second->buildAction();
};

SkillObject* SkillManager::createSkill(BattleConfig::SkillData* skillData, int fromId, const entityx::Entity::Id& entityId)
{
    auto skill = new SkillObject(skillData, fromId, entityId);
    
    for(auto* pActionData : skillData->pConfig->actions)
    {
        if (pActionData->startTime >= 0)
        {
            auto* pAction = buildAction(pActionData->name);
            if(pAction) skill->addAction(pAction, pActionData);
        }
    }
    return skill;
}
