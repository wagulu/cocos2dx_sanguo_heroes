
local model_user = _REQUIRE("model/model_user")
_REQUIRE("pb/pbCommon.lua")

local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua")
local model_itemStorage = _REQUIRE "model/model_itemStorage.lua"
local delayCallback = _REQUIRE("common/delayCallback.lua");

local helper = {};

function helper:levelUp(successCallback)
    local function onSuccess(success, data, code, msg)
        if(success) then
        end
        if(successCallback) then
            successCallback(success, data, code, msg);
        end
    end

    ctlMonstersTreasureItemLevelUpRequest:sendRequest(onSuccess,
        model_user.selectTreasureItemUpgradeTargetInventoryId,
        model_user.selectTreasureItemUpgradeConsumeId,
        model_user.selectTreasureItemUpgradeConsumeCount);
end


function helper:getLevelTypeDefine(treasureId)
    local level = model_treasureItemStorage:getLevel(treasureId);
    local levelUpType = model_treasureItemStorage:getLevelUpType(treasureId);
    return model_treasureItemLevelType:getDefine(levelUpType, level + 1);
end


function helper:getQualityTypeDefines(treasureId)
    local level = model_treasureItemStorage:getQuality(treasureId);
    local levelUpType = model_treasureItemStorage:getQualityUpType(treasureId);
    return static_model_treasureItemQualityType:getDefines(levelUpType,level + 1);
end


-- merge

function helper:canMerge(partId)
    local part = model_treasureItemPartStorage:getNode(partId);
    local currentNumber = model_treasureItemPartStorage:getCount(partId);
    local needNumber = static_model_treasureItemPart:getNeedNumber();
    return currentNumber >= needNumber;
end

-- level up

function helper:getLevelUpExp(itemInventoryId)
    local item = model_treasureItemStorage:getNode(itemInventoryId);
    local type = self:getLevelTypeDefine(item.treasureItemId);
    return type.exp;
end

function helper:canLevelUp(itemInventoryId)
    local item = model_treasureItemStorage:getNode(itemInventoryId);
    local needExp = self:getLevelUpExp(itemInventoryId);
    return item.exp >= needExp;
end

-- quality up

function helper:needConfirmConsumeItemToQualityUp(itemInventoryId)
    local item = model_treasureItemStorage:getNode(itemInventoryId);
    local effects = self:getQualityTypeDefines(itemInventoryId);

    for key, value in pairs(effects) do
        --need treasure item
        if(static_model_treasureItemQualityType:isRequirement_needSelf(value)) then
            local excludeInventoryIdList = {};
            excludeInventoryIdList[item.inventoryTreasureItemId] = 1;
            local freeItems = model_treasureItemStorage:collectItemWithTypeId(item.treasureItemId, true, excludeInventoryIdList);
            if(#(freeItems) == 0) then
                return false;
            end

            for freeItemKey, freeItem in pairs(freeItems) do
                if(freeItem.level == 1 and freeItem.quality == 1) then
                    return false;
                end
            end
            return true;
            
        end

    end

    return false;
end

function helper:canQualityUp(itemInventoryId)
    local item = model_treasureItemStorage:getNode(itemInventoryId);
    local effects = self:getQualityTypeDefines(itemInventoryId);
    
    for key, value in pairs(effects) do
        --need treasure item
        if(static_model_treasureItemQualityType:isRequirement_needSelf(value)) then
            local excludeInventoryIdList = {};
            excludeInventoryIdList[item.inventoryTreasureItemId] = 1;
            local freeItems = model_treasureItemStorage:collectItemWithTypeId(item.treasureItemId, true, excludeInventoryIdList);
            if(#(freeItems) == 0) then
                return false,itemInventoryId,pbCommon.ObjectType.TreasureItem,1;
            end
        end

        --need item        
        if(static_model_treasureItemQualityType:isRequirement_needMaterial(value)) then
            local needCount = static_model_treasureItemQualityType:getRequirementCount(value);
            local currentCount = model_itemStorage:getCount(
                pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_ID,
                pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_TYPE);
            if(currentCount < needCount) then
                return false,pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_ID,pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_TYPE,needCount;
            end
        end
    end
    
    return true;
end


--equip

function helper:unequipTreasure(itemInventoryId, callback)
    local treasure = model_treasureItemStorage:getNode(itemInventoryId);
    if(treasure.useMonster > 0) then
        ctlMonstersChangeTreasureItemRequest:sendRequest(callback, treasure.useMonster, treasure.quippedType, 0);
    else
        local function onDelayCallback()
            callback(false, nil, 0, nil);
        end
    
        delayCallback:create(onDelayCallback, 0.1);
    end
end

function helper:equipTreasure(itemInventoryId,monsterId, callback)
    local treasure = model_treasureItemStorage:getNode(itemInventoryId);

    local function onEquip(success, data, code, msg)
        if(success) then
        else
        end
        callback(success, data, code, msg);
    end
    local function equip()
        ctlMonstersChangeTreasureItemRequest:sendRequest(onEquip, monsterId, treasure.quippedType, itemInventoryId);
    end

    if(treasure.useMonster > 0) then
        local function onUnequip(success, data, code, msg)
            if(success) then
                equip();
            else
                callback(success, data, code, msg);
            end
        end

        self:unequipTreasure(itemInventoryId, onUnequip);
    else
        equip();
    end
end


return helper;
