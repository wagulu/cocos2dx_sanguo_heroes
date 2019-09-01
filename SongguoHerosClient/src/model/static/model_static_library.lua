
static_model_area = _REQUIRE("model/static/model_area.lua")
static_model_eliteStage = _REQUIRE("model/static/model_eliteStage.lua")
static_model_eliteStageStar = _REQUIRE("model/static/model_eliteStageStar.lua")
static_model_island = _REQUIRE("model/static/model_island.lua")
static_model_item = _REQUIRE("model/static/model_item.lua")
static_model_material = _REQUIRE("model/static/model_material.lua")
static_model_materialSynthesis = _REQUIRE("model/static/model_materialSynthesis.lua")
static_model_monster = _REQUIRE("model/static/model_monster.lua")
static_model_monsterPart = _REQUIRE("model/static/model_monsterPart.lua")
static_model_monsterSkill = _REQUIRE("model/static/model_monsterSkill.lua")
static_model_monsterExpand = _REQUIRE("model/static/model_monsterExpand.lua")
static_model_monsterQuality = _REQUIRE("model/static/model_monsterQuality.lua")
static_model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
static_model_npcBattle = _REQUIRE("model/static/model_npcBattle.lua")
--static_model_npcSoldier = _REQUIRE("model/static/model_npcSoldier.lua")
static_model_stage = _REQUIRE("model/static/model_stage.lua")
static_model_stageStar = _REQUIRE("model/static/model_stageStar.lua")
static_model_eliteStageBattleTimesPrice = _REQUIRE("model/static/model_eliteStageBattleTimesPrice.lua")
static_model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
static_model_formations = _REQUIRE("model/static/model_formations.lua")
static_model_energyTimesPrice = _REQUIRE("model/static/model_energyTimesPrice.lua")
static_model_vitalityTimesPrice = _REQUIRE("model/static/model_vitalityTimesPrice.lua")
static_model_monsterQualityUpLevelType = _REQUIRE("model/static/model_monsterQualityUpLevelType.lua")
static_model_vipPrivileges = _REQUIRE("model/static/model_VIPPrivileges.lua")
static_model_treasureItem = _REQUIRE("model/static/model_treasureItem.lua")
static_model_treasureItemPart = _REQUIRE("model/static/model_treasureItemPart.lua")
static_model_treasureItemQualityType = _REQUIRE("model/static/model_treasureItemQualityType.lua")
static_model_treasureItemQualityConfig = _REQUIRE("model/static/model_treasureItemQualityConfig.lua")
static_model_treasureItemLevelType = _REQUIRE("model/static/model_treasureItemLevelType.lua")
static_model_treasureItemLevelConfig = _REQUIRE("model/static/model_treasureItemLevelConfig.lua")
static_model_treasureItemSuitConfig = _REQUIRE("model/static/model_treasureItemSuitConfig.lua")
static_model_userLevel = _REQUIRE("model/static/model_userLevel.lua")
static_model_monsterLevel = _REQUIRE("model/static/model_monsterLevel.lua")
static_model_equipment = _REQUIRE("model/static/model_equipment.lua")
static_model_equipmentQualityEvolutionMaterial = _REQUIRE("model/static/model_equipmentQualityEvolutionMaterial.lua")
static_model_equipmentQualityBaseAttribute = _REQUIRE("model/static/model_equipmentQualityBaseAttribute.lua")
static_model_equipmentQualityLevelUpGain = _REQUIRE("model/static/model_equipmentQualityLevelUpGain.lua")
static_model_relationship = _REQUIRE("model/static/model_relationship.lua")
static_model_relationshipItem = _REQUIRE("model/static/model_relationshipItem.lua")
static_model_relationshipItemGain = _REQUIRE("model/static/model_relationshipItemGain.lua")
static_model_relationshipLevel = _REQUIRE("model/static/model_relationshipLevel.lua")
static_model_relationshipLevelGain = _REQUIRE("model/static/model_relationshipLevelGain.lua")
static_model_relationshipSynthesis = _REQUIRE("model/static/model_relationshipSynthesis.lua")
static_model_gacha = _REQUIRE("model/static/model_gacha.lua")
static_model_gachaToken = _REQUIRE("model/static/model_gachaToken.lua")
static_model_gachaView = _REQUIRE("model/static/model_gachaView.lua")
static_model_monsterSkillUpType = _REQUIRE("model/static/model_monsterSkillUpType.lua")
static_model_dropWay = _REQUIRE("model/static/model_dropWay.lua")
static_model_equipmentStrengthenMaster = _REQUIRE("model/static/model_equipmentStrengthenMaster.lua")
static_model_equipmentStrengthenMasterConfig = _REQUIRE("model/static/model_equipmentStrengthenMasterConfig.lua")
static_model_breakthrough = _REQUIRE("model/static/model_breakthrough.lua")
static_model_monsterDestiny = _REQUIRE("model/static/model_monsterDestiny.lua")


_REQUIRE("pbCommon.lua")

local model = {};
--model.ItemType={
--    ITEM="item",
--    MONSTER="Monster",
--    MONSTERPART="MonsterPart",
--    TREASUREITEM="TreasureItem",
--    TREASUREITEMPART="TreasureItemPart",
--    MATERIAL="material",
--    TREASUREBOX="treasure_box",
--    }

function model:getLibrary(type)
    local library = nil;
    
    if(pbCommon.isTypeItem(type)) then
        library = static_model_item;
    elseif(type == pbCommon.ObjectType.Monster) then
        library = static_model_monster;
    elseif(type == pbCommon.ObjectType.MonsterPart) then
        library = static_model_monsterPart;
    elseif(type == pbCommon.ObjectType.TreasureItem) then
        library = static_model_treasureItem
    elseif(type == pbCommon.ObjectType.TreasureItemPart) then
        library = static_model_treasureItemPart
    elseif(type == pbCommon.ObjectType.Material) then
        library = static_model_material;
    elseif(type == pbCommon.ObjectType.TreasureBox) then
    elseif(type == pbCommon.ObjectType.FavorabilityItem) then
    elseif(type == pbCommon.ObjectType.BreakthroughItem) then  
    end

    return library;
end

function model:getName(id, type)
    local library = self:getLibrary(type);
    if(library) then
        return library:getName(id);
    end
    return "";
end

function model:getDesc(id, type)
    local library = self:getLibrary(type);
    if(library) then
        return library:getDesc(id);
    end
    return "";
end

function model:getAsset(id, type)
    local library = self:getLibrary(type);
    if(library) then
        local id = library:getAsset(id);
        if(pbCommon.isTypeItem(type)) then
            type = pbCommon.ObjectType.Item
        end
        cclog(type.."/"..type.."_"..id..".png")
        return type.."/"..type.."_"..id..".png";
    end
    return "";
end

function model:getQuality(id, type)
    local library = self:getLibrary(type);
    if(library) then
        return library:getQuality(id);
    end
    return 0;
end

function model:getColor(id,type)
    local quality = self:getQuality(id,type)
    return pbCommon.Colors[tonumber(quality)]
end

function model:getFrame(id, type)
    local library = self:getLibrary(type);
    if(library) then
        local quality = library:getQuality(id);
        return "frame/frame_"..quality..".png";
    end
    return 0;
end

return model;


