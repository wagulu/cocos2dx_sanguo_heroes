
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_equipmentQualityLevelUpGain", model_static)

function model:setNode(node)
    local type = node.type
    local quality = tonumber(node.quality)

    if(self.map[type] == nil) then
        self.map[type] = {}
    end

    self.map[type][quality] = node
end

function model:getGain(type, quality)
    local node = self.map[type][quality]
    if(node) then
        return tonumber(node.value)
    end
    return 0
end

local function init()
    model:load("config/static/EquipmentQualityLevelUpGain.Json");
end

init();

return model;