local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 

    local storage = model_monsterStorage:getMonster(id)
    
    local treasureItemSuits = {}
    
    --optional int32                              slotId = 1;                         //珍宝槽id
    --optional int32                              inventoryTreasureItemId = 2;        //珍宝id
    
    --单件加成
    for key, treasureItem in pairs(storage.treasureItems) do
        local inventoryId = treasureItem.inventoryTreasureItemId
        if (inventoryId > 0) then
            local treasureInventory = model_treasureItemStorage:getNode(inventoryId)
            value = value + self:getAttributeLevelByName(treasureInventory, attribute)
            value = value + self:getAttributeQualityByName(treasureInventory, attribute)
            
            local suitId = static_model_treasureItem:getSuitId(treasureInventory.treasureItemId)
            if (treasureItemSuits[suitId] == nil) then
                treasureItemSuits[suitId] = 0
            end
            treasureItemSuits[suitId] = treasureItemSuits[suitId] + 1
        end
    end
        
    --套装加成
--        "treasureItemCount": 4,
--        "propertyName": "ATK",
--        "propertyOperator": "Multiplication","Addition"
--        "propertyValue": 0,
    for suitId, count in pairs(treasureItemSuits) do
        if (suitId > 0) then
            local configs = static_model_treasureItemSuitConfig:getConfigs(suitId, attribute)
            if (configs) then
                for key, config in pairs(configs) do
                    if (count >= config.treasureItemCount) then
                        if (config.propertyOperator == "Addition") then
                            value = value + config.propertyValue
                        elseif (config.propertyOperator == "Multiplication") then
                            value = value + baseValue * config.propertyValue
                        end
                    end
                end
            end
        end
    end

    return value, baseValue
end

function extension:getSuitProp(monsterId, treasureItemId)

    local items = {}
    local prop = {}
    local targetSuitId = static_model_treasureItem:getSuitId(treasureItemId)
    local storage = model_monsterStorage:getMonster(monsterId)

    --optional int32                              slotId = 1;                         //珍宝槽id
    --optional int32                              inventoryTreasureItemId = 2;        //珍宝id

    for key, treasureItem in pairs(storage.treasureItems) do
        local inventoryId = treasureItem.inventoryTreasureItemId
        if (inventoryId > 0) then
            local treasureInventory = model_treasureItemStorage:getNode(inventoryId)
            local suitId = static_model_treasureItem:getSuitId(treasureInventory.treasureItemId)
            if (suitId == targetSuitId) then
                items[#(items) + 1] = treasureInventory
            end
        end
    end
    
    --        套装加成
    --        "treasureItemCount": 4,
    --        "propertyName": "ATK",
    --        "propertyOperator": "Multiplication","Addition"
    --        "propertyValue": 0,
    local configs = static_model_treasureItemSuitConfig:getAllConfigs(targetSuitId)

    if (configs) then
        for key, attributes in pairs(configs) do
            for attribute, config in pairs(attributes) do
                if(prop[config.treasureItemCount] == nil) then
                    prop[config.treasureItemCount] = {}
                end
                local group = prop[config.treasureItemCount]
                group[#(group) + 1] = config
            end
        end
    end

    return items, prop
end

function extension:getSuitPropNormal(treasureItemId)
    local prop = {}
    local targetSuitId = static_model_treasureItem:getSuitId(treasureItemId)

    local configs = static_model_treasureItemSuitConfig:getAllConfigs(targetSuitId)

    if (configs) then
        for key, attributes in pairs(configs) do
            for attribute, config in pairs(attributes) do
                if(prop[config.treasureItemCount] == nil) then
                    prop[config.treasureItemCount] = {}
                end
                local group = prop[config.treasureItemCount]
                group[#(group) + 1] = config
            end
        end
    end

    return prop
end

--    optional int32                  inventoryTreasureItemId = 1;    //背包序列
--    optional int32                  treasureItemId = 2;             //珍宝id
--    optional int32                  level = 3;                      //珍宝等级
--    optional int32                  exp = 4;                        //珍宝经验
--    optional int32                  quality = 5;                    //珍宝品级
--    optional int32                  quippedType = 6;                //珍宝装备类型
--    optional int32                  useMonster = 7;                 //使用中的武将id 0为没有武将使用

function extension:getAttributeQualityByName(treasureItem, attribute)
    local value = 0
    
    local treasureItemId = treasureItem.treasureItemId
    local quality = treasureItem.quality
    
--        "id": 1,
--        "treasureItemId": 1,
--        "level": 1,
--        "propertyName": "HP",
--        "propertyValue": 100,
--        "gain": 10,
--        "ver": "1.0.0"
    
    local qualityConfigs = static_model_treasureItemQualityConfig:getQualityItems(treasureItemId, attribute)
    if qualityConfigs == nil then
        return value
    end
    for key, qualityConfig in pairs(qualityConfigs) do
        if (qualityConfig.level <= quality) then
            value = value + qualityConfig.propertyValue + qualityConfig.gain * (quality - 1)
        end
    end
            
    return value
end

function extension:getAttributeLevelByName(treasureItem, attribute)
    local value = 0

    local treasureItemId = treasureItem.treasureItemId
    local level = treasureItem.level

    --        "id": 1,
    --        "treasureItemId": 1,
    --        "level": 1,
    --        "propertyName": "HP",
    --        "propertyValue": 100,
    --        "gain": 10,
    --        "ver": "1.0.0"

    local levelConfigs = static_model_treasureItemLevelConfig:getLevelItems(treasureItemId, attribute)
    for key, levelConfig in pairs(levelConfigs) do
        if (levelConfig.level <= level) then
            value = value + levelConfig.propertyValue + levelConfig.gain * (level - 1)
        end
    end

    return value
end

function extension:getAttributes(treasureItem)

    local result = {}
    
    local treasureItemId = treasureItem.treasureItemId
    local quality = treasureItem.quality
    local level = treasureItem.level
    
    local allLevels = static_model_treasureItemLevelConfig:getAllLevelItems(treasureItemId)
    local allQuality = static_model_treasureItemQualityConfig:getAllQualityItems(treasureItemId)

    for key, value in pairs(allLevels) do
        if (result[key] == nil) then
            result[key] = 0
        end
    end

    for key, value in pairs(allQuality) do
        if (result[key] == nil) then
            result[key] = 0
        end
    end
    
    for key, value in pairs(result) do
        result[key] = result[key] + self:getAttributeLevelByName(treasureItem, key)
        result[key] = result[key] + self:getAttributeQualityByName(treasureItem, key)
        if(result[key] == 0) then
            table.remove(result, key)
        end
    end

    return result
end

function extension:getAttributesLevel(treasureItem)

    local result = {}

    local treasureItemId = treasureItem.treasureItemId

    local all = static_model_treasureItemLevelConfig:getAllLevelItems(treasureItemId)

    for key, value in pairs(all) do
        local temp = self:getAttributeLevelByName(treasureItem, key)
        if temp~=0 then
            result[key] = temp 
        end
        
    end

    return result
end

function extension:getAttributesQuality(treasureItem)

    local result = {}

    local treasureItemId = treasureItem.treasureItemId

    local all = static_model_treasureItemQualityConfig:getAllQualityItems(treasureItemId)

    for key, value in pairs(all) do
        local temp = self:getAttributeQualityByName(treasureItem, key)
        if temp ~= 0 then
            result[key] = temp
        end
        
    end

    return result
end

function extension:getAttributesNextLevel(treasureItem)
    treasureItem.level = treasureItem.level + 1
    local result = self:getAttributesLevel(treasureItem)
    treasureItem.level = treasureItem.level - 1
    return result
end

function extension:getAttributesNextQuality(treasureItem)
    treasureItem.quality = treasureItem.quality + 1
    local result = self:getAttributesQuality(treasureItem)
    treasureItem.quality = treasureItem.quality - 1
    return result
end

function extension:getAttributesByLevel(treasureItem,level)
    local oldLevel = treasureItem.level
    treasureItem.level = level
    local result = self:getAttributesLevel(treasureItem)
    treasureItem.level = oldLevel
    return result
end

function extension:getAttributesByQuality(treasureItem,quality)
    local oldQuality = treasureItem.quality
    treasureItem.quality = quality
    local result = self:getAttributesQuality(treasureItem)
    treasureItem.quality = oldQuality
    return result
end

return extension

