local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local helper_relationship = _REQUIRE("model/helper/helper_relationship.lua")

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 
    local storage = model_monsterStorage:getMonster(id)
    local gains = helper_relationship:getActiveRelationshipLevelGains(storage)

    for key, gain in pairs(gains) do
        if gain.propertyOperator == "Addition" then
            value = value + gain.propertyValue
        elseif gain.propertyOperator == "Multiplication" then
            value = value + baseValue * gain.propertyValue
        end
    end

    return value, baseValue
end

return extension