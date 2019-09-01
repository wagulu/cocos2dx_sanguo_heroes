local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 
    local storage = model_monsterStorage:getMonster(id)
    for key, equipment in pairs(storage.equipment) do
        value = value + self:getAttributeByName(equipment, attribute)
        baseValue = value
    end

    return value, baseValue
end

--    optional int32                              id = 1;                             //装备id
--    optional int32                              level = 2;                          //装备等级
--    optional int32                              quality = 3;                        //装备品阶

function extension:getAttributeByName(equipment, attribute)
    local value = 0
    local id = equipment.id
    local quality = equipment.quality
    local level = equipment.level
    local type = static_model_equipment:getQualityBaseAttributeType(id)
--    local baseAttributes = static_model_equipmentQualityBaseAttribute:getAttributes(type, level, attribute)
    local baseAttributes = static_model_equipmentQualityBaseAttribute:getAttributes(type, quality, attribute)

    if(baseAttributes) then
        for key, baseAttribute in pairs(baseAttributes) do
            local gain = static_model_equipmentQualityLevelUpGain:getGain(baseAttribute.gainType, quality) 
            value = value + tonumber(baseAttribute.propertyValue) + gain * level
        end
    end
    
    return value
end

function extension:getAttributes(equipment)
    local result = {}
    local id = equipment.id
    local level = equipment.level
    local type = static_model_equipment:getQualityBaseAttributeType(id)
--    local all = static_model_equipmentQualityBaseAttribute:getAllAttributes(type, level)
    local all = static_model_equipmentQualityBaseAttribute:getAllAttributes(type, equipment.quality)
    
    for key, value in pairs(all) do
        result[key] = self:getAttributeByName(equipment, key)
    end

    return result
end

function extension:getAttributesNextLevel(equipment)
    equipment.level = equipment.level + 1
    local result = self:getAttributes(equipment)
    equipment.level = equipment.level - 1
    return result
end

function extension:getAttributesNextQuality(equipment)
    equipment.quality = equipment.quality + 1
    local result = self:getAttributes(equipment)
    equipment.quality = equipment.quality - 1
    return result
end

return extension