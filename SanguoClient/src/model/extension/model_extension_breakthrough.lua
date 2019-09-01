local model_breakthrough = _REQUIRE "model/static/model_breakthrough.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 
    if attribute == "Strength" or attribute == "Intelligence" or attribute == "Leadership" or attribute == "HP" then
    	local monster = model_monsterStorage:getMonster(id)
    	local effect = model_breakthrough:getEffect(monster.breakthroughLevel)
    	value = value + effect[attribute]
    end
    return value, baseValue
end

return extension