local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 
    local storage = model_monsterStorage:getMonster(id)

    local relationships = self:getRelationshipTreasure(id)
    
    for _, relationshipId in pairs(relationships) do
        local gains = static_model_relationshipItemGain:getRelationship(relationshipId)
        for _, gain in pairs(gains) do
            if (attribute == gain.propertyName) then
                if (gain.propertyOperator == "Addition") then
                    value = value + gain.propertyValue
                elseif (gain.propertyOperator == "Multiplication") then
                    value = value + baseValue * gain.propertyValue
                end
            end
        end
    end
        
    return value, baseValue
end


--获取某个英雄身上已经激活的所有珍宝缘分
function extension:getRelationshipTreasure(monsterId)
    local storage = model_monsterStorage:getMonster(monsterId)
    local treasureItems = {}
    
    for key, treasureItem in pairs(storage.treasureItems) do
        local inventoryId = treasureItem.inventoryTreasureItemId
        if (inventoryId > 0) then
            local treasureInventory = model_treasureItemStorage:getNode(inventoryId)
            treasureItems[treasureInventory.treasureItemId] = true
        end
    end
    
    local relationships = static_model_relationshipItem:getMonsterRelationship(monsterId)
    local result = {}
    
--        "id": 1,
--        "monsterId": 1,
--        "itemType": "TreasureItem",
--        "itemId": 1,
--        "flag": 1,
--        "ver": "",
--        "name_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206",
--        "description_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206"
    if (relationships) then
        for key, relationship in pairs(relationships) do
            if (relationship["itemType"] == "TreasureItem") then
                if (treasureItems[relationship["itemId"]] == true) then
                    result[#(result) + 1] = relationship["id"]
                end
            end
        end
    end

    return result
end

--获取和某个珍宝有缘分的所有英雄列表
function extension:getRelationshipMonster(treausureId)
    local relationships = static_model_relationshipItem:getTreasureRelationship(treausureId)
    local result = {}
    if relationships == nil then
        return result
    end
--        "id": 1,
--        "monsterId": 1,
--        "itemType": "TreasureItem",
--        "itemId": 1,
--        "flag": 1,
--        "ver": "",
--        "name_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206",
--        "description_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206"
    
    for key, relationship in pairs(relationships) do
        if (relationship["itemType"] == "TreasureItem") then
            if not (self:contain(result, relationship["monsterId"])) then
                result[#(result) + 1] = relationship["monsterId"]
            end
        end
    end
    return result
end

--获取某个人可能拥有的所有珍宝缘分
function extension:getAllRelationshipsByMonster(monsterId)
    local storage = model_monsterStorage:getMonster(monsterId)
    local treasureItems = {}

    for key, treasureItem in pairs(storage.treasureItems) do
        local inventoryId = treasureItem.inventoryTreasureItemId
        if (inventoryId > 0) then
            local treasureInventory = model_treasureItemStorage:getNode(inventoryId)
            treasureItems[treasureInventory.treasureItemId] = true
        end
    end

    local relationships = static_model_relationshipItem:getMonsterRelationship(monsterId)
    local result = {}

    --        "id": 1,
    --        "monsterId": 1,
    --        "itemType": "TreasureItem",
    --        "itemId": 1,
    --        "flag": 1,
    --        "ver": "",
    --        "name_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206",
    --        "description_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206"
 
    for key, relationship in pairs(relationships) do
        if (relationship["itemType"] == "TreasureItem") then
            local group = {}
            result[#(result) + 1] = group
            group["detail"] = relationship
            group["gain"] = static_model_relationshipItemGain:getRelationship(relationship.id)
            if (treasureItems[relationship["itemId"]] == true) then
                group["activation"] = true
            else
                group["activation"] = false
            end
        end
    end

    return result
end

--获取某个人可能拥有的所有珍宝缘分
function extension:getAllRelationshipsByMonster(monsterId)
    local storage = model_monsterStorage:getMonster(monsterId)
    local treasureItems = {}

    for key, treasureItem in pairs(storage.treasureItems) do
        local inventoryId = treasureItem.inventoryTreasureItemId
        if (inventoryId > 0) then
            local treasureInventory = model_treasureItemStorage:getNode(inventoryId)
            treasureItems[treasureInventory.treasureItemId] = true
        end
    end

    local relationships = static_model_relationshipItem:getMonsterRelationship(monsterId)
    local result = {}

    --        "id": 1,
    --        "monsterId": 1,
    --        "itemType": "TreasureItem",
    --        "itemId": 1,
    --        "flag": 1,
    --        "ver": "",
    --        "name_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206",
    --        "description_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206"
    if relationships == nil then
        return result
    end
    for key, relationship in pairs(relationships) do
        if (relationship["itemType"] == "TreasureItem") then
            local group = {}
            result[#(result) + 1] = group
            group["detail"] = relationship
            if (treasureItems[relationship["itemId"]] == true) then
                group["activation"] = true
            else
                group["activation"] = false
            end
        end
    end

    return result
end

function extension:contain(group, element)
    for key, value in pairs(group) do
        if (value == element) then
            return true
        end
    end
    return false
end

return extension



