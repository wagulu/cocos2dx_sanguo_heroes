local model_equipmentStrengthenMasterConfig = _REQUIRE("model/static/model_equipmentStrengthenMasterConfig.lua")

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 
    local equipStrengthenLevel = model_equipmentStrengthenMasterConfig:getStrengthLevel(id)
    local gains = model_equipmentStrengthenMasterConfig:getStrengthEffectGroup(equipStrengthenLevel)

    for _, gain in pairs(gains) do
        if (attribute == gain.propertyName) then
            value = value + gain.propertyValue
        end
    end

    return value, baseValue
end

return extension