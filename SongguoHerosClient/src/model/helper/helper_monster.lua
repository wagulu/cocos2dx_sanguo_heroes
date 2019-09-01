
local static_model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
local model_user = _REQUIRE("model/model_user")
local model_monster = _REQUIRE("model/static/model_monster.lua");
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua");
local model_breakthrough = _REQUIRE("model/static/model_breakthrough.lua")
local model_monsterDestiny = _REQUIRE("model/static/model_monsterDestiny.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local pbCommon = _REQUIRE("pbCommon.lua")

local ctlMonstersBreakthroughRequest = _REQUIRE("controller/request/ctlMonstersBreakthroughRequest.lua")

local helper = {};

--function helper:setMonsterToMonsterList(monster)
--    for key, value in pairs(model_user.monsterList) do
--        if(value.id == monster.id) then
--            model_user[key] = monster;
--            return;
--        end
--    end
--    
--    local nextIndex = #(model_user.monsterList) + 1;
--    model_user.monsterList[nextIndex] = monster;
--end
--
--function helper:getMonsterFromMonsterList(monsterId)
--    for key, value in pairs(model_user.monsterList) do
--        if(value.id == monsterId) then
--            return value;
--        end
--    end
--    
--    return nil;
--end

-------------------------------------------------------------------------------
--equipment

function helper:getMonsterEquipment(monsterId, equipmentId)
    local monster = model_monsterStorage:getMonster(monsterId);
    if(monster == nil) then
        return nil;
    end
    
    local equipment = nil;
    for key, value in pairs(monster.equipment) do
        if(value.id == equipmentId) then
            equipment = value;
            break;
        end
    end

    return equipment;
end

function helper:hasEnoughEquipmentDependence(monsterId, equipmentId, recursion)
    --only check whether equipment have enough material to upgrad,
    --not check whether these materials can be composed by other materials
    local equipment = self:getMonsterEquipment(monsterId, equipmentId);
    for key, value in pairs(equipment.nextQualityMaterial) do
        local requireMaterialId = value.needMaterialId;
        local requireMaterialCount = value.needMaterialCount;
        local requireMaterialCountCurrent = model_materialStorage:getCount(requireMaterialId);
        if(requireMaterialCountCurrent < requireMaterialCount) then
            if(recursion) then
                local needCount = requireMaterialCount - requireMaterialCountCurrent;
--                if not(self:hasEnoughMaterialRecursion(value.needMaterialId, needCount, true)) then
                if not(self:hasEnoughMaterial(value.needMaterialId, needCount, true)) then---
                    return false;
                end
            else
                return false;
            end
        end
    end
    if table.getn(equipment.nextQualityMaterial) == 0 then --如果没有合成材料，则为false
        return false
    end
    return true;
end

function helper:hasEnoughMaterial(materialId, count, recursion)
    local synthesis = static_model_materialSynthesis:getSynthesis(materialId);
    if synthesis == nil then
    	return false
    end
    for key, value in pairs(synthesis) do
        local requireMaterialId = synthesis.id--static_model_materialSynthesis:getRequirementMaterialId(value);
        local requireMaterialCount = synthesis.count * count--static_model_materialSynthesis:getRequirementMaterialCount(value) * count;
        local requireMaterialCountCurrent = model_materialStorage:getCount(requireMaterialId);
        if(requireMaterialCountCurrent < requireMaterialCount) then
            local needCount = requireMaterialCount - requireMaterialCountCurrent;
            if(recursion) then
                if not(self:hasEnoughMaterial(requireMaterialId, needCount, true)) then
                    return false;
                end
            else
                return false;
            end
        end
    end
    return true;
end


-- equipment level up

function helper:canEquipmentQualityUp(monsterId, equipmentId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local equipment = self:getMonsterEquipment(monsterId, equipmentId);
    if(equipment.quality > monster.quality) then
        return false;
    end
    
    if self:hasEnoughEquipmentDependence(monsterId,equipmentId,true) == false then
    	return false;
    end

    return true;
end
--获取当前武将装备能强化到的的最高等级
function helper:getMonsterEquipmentCurrentMaxLevel(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    return monster.level;
end


-- monster level up

function helper:canMonsterQualityUp(monsterId)
    --id----0：表示可以升级，1：表示武将的6件装备需全部进化同一品质index，2：武将等级需到达特定等级index才能升级
    local monster = model_monsterStorage:getMonster(monsterId);
    local arr = {id = 0,index = 0}
    for key, value in pairs(monster.equipment) do
        if(value.quality <= monster.quality) then
            arr = {id = 1,index = monster.quality+1}
            return arr;
        end
    end
    
    local requireLevel = self:getMonsterQualityUpRequireLevel(monsterId);
    if(requireLevel > monster.level) then
        arr = {id = 2,index = requireLevel}
        return arr;
    end
    
    return arr;
end

function helper:getMonsterQualityUpRequireLevel(monsterId)
    local typeId = model_monster:getQualityLevelTypeId(monsterId);
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = static_model_monsterQualityUpLevelType:getRequireLevel(typeId, monster.quality+1);
    return level;
end

function helper:canMonsterStarUp(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);

    local needStar = static_model_monsterStar:getStar(monster.star+1);
    local currentStar = model_monsterPartStorage:getCount(monsterId);
    if(needStar > currentStar) then
        return false;
    end
    
    if monster.star == 5 then
        return false;
    end

    return true;
end

-- merge

function helper:canMergeMonster(monsterId)
    if(model_monsterStorage:isMonsterActive(monsterId)) then
        return false;
    end
    
    local monster = model_monsterStorage:getMonster(monsterId);

    local totalStar = static_model_monsterStar:getTotalStar(monster.level);
    local currentStar = model_monsterPartStorage:getCount(monsterId);
    if(totalStar > currentStar) then
        return false;
    end

    return true;
end

-- breakthrough

function helper:canBreakthrough(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    
    local breakthroughItemId = model_monster:getBreakthroughItemId(monsterId);
    local breakthroughItemN = model_breakthrough:getItemNum(monster.breakthroughLevel);
    if(model_itemStorage:getCount(breakthroughItemId, pbCommon.ObjectType.BreakthroughItem) < breakthroughItemN) then
        return false;
    end
    
    local breakthroughExclusiveItemId = model_monster:getBreakthroughExclusiveItemId(monsterId);
    local breakthroughExclusiveItemN = model_breakthrough:getExclusiveItemNum(monster.breakthroughLevel);
    if(model_itemStorage:getCount(breakthroughExclusiveItemId, pbCommon.ObjectType.BreakthroughItem) < breakthroughExclusiveItemN) then
        return false;
    end

    local coin = model_breakthrough:getCoin(monster.breakthroughLevel);
    if(model_user.coin < coin) then
        return false;
    end

    local level = model_breakthrough:getLevelLimit(monster.breakthroughLevel);
    local equipmentLevel = monster.equipment[1].level;
    if(equipmentLevel < level) then
        return false;
    end

    return true;
end

function helper:tryBreakthrough(monsterId, successCallback)
    ctlMonstersBreakthroughRequest:sendRequest(successCallback, monsterId);
end

--monsterDestiny
function helper:canDestiny(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local destinyItemId = model_monster:getMonsterDestinyItemId();
    local destinyItemN = model_monsterDestiny:getItemCount(monster.destinyLevel);
    local destinyItemCurrent = model_itemStorage:getCount(destinyItemId, pbCommon.ObjectType.BreakthroughItem)
    
    if monster.level >= 35 and monster.star >=3 and destinyItemCurrent >= destinyItemN then
        return true
    end
    
    return false
end

return helper;
