local model_treasureItemStrengthenMasterConfig = _REQUIRE("model/static/model_treasureItemStrengthenMasterConfig.lua")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")

local extension = {}

function extension:calculateAttribute(id, attribute, value, baseValue) 

	local storage = model_monsterStorage:getMonster(id)
    local isTreasuresEnough = true

    for _, treasureItem in pairs(storage.treasureItems) do
        if (treasureItem.inventoryTreasureItemId == 0) then
        	isTreasuresEnough = false
        end
    end

    if (isTreasuresEnough) then
    	local treasureItemStrengthenLevel = model_treasureItemStrengthenMasterConfig:getStrengthLevel(id)
		local gains = model_treasureItemStrengthenMasterConfig:getStrengthEffectGroup(treasureItemStrengthenLevel)

    	for _, gain in pairs(gains) do
        	if (attribute == gain.propertyName) then
            	value = value + gain.propertyValue
        	end
        end
    end

    return value, baseValue
end

return extension