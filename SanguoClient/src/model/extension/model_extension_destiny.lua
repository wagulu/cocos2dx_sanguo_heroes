local model_destiny = _REQUIRE "model/static/model_monsterDestiny.lua"

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 
    if attribute == "Attack" then
    	value = value + model_destiny:getMonsterEffectAttack(id)
    elseif attribute == "Power" then
    	value = value + model_destiny:getMonsterEffectPower(id)
    elseif attribute == "Armor" then
    	value = value + model_destiny:getMonsterEffectArmor(id)
    elseif attribute == "Resistance" then
    	value = value + model_destiny:getMonsterEffectResistance(id)
    end
    return value, baseValue
end

return extension