
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_treasureItemQualityConfig", model_static)

--        "id": 1,
--        "treasureItemId": 1,
--        "level": 1,
--        "propertyName": "HP",
--        "propertyValue": 100,
--        "gain": 10,
--        "ver": "1.0.0"

function model:setNode(node)
    if (self.map[node.treasureItemId] == nil) then
        self.map[node.treasureItemId] = {}
    end

    if (self.map[node.treasureItemId][node.propertyName] == nil) then
        self.map[node.treasureItemId][node.propertyName] = {}
    end

    local group = self.map[node.treasureItemId][node.propertyName]
    group[#(group) + 1] = node
end

function model:getQualityItems(treasureItemId, propertyName)
    return self.map[treasureItemId][propertyName]
end

function model:getAllQualityItems(treasureItemId)
    return self.map[treasureItemId]
end


function model:getActivedQualityDefines(treasureItemId, level)
    local effects = {};

    local items = self:getAllQualityItems(treasureItemId);
    for key, defines in pairs(items) do
        for key, define in pairs(defines) do
            if(define.level <= level) then
                effects[#(effects) + 1] = define;
            end
        end
    end
    
    return effects;
end


local function init()
    model:load("config/static/TreasureItemQualityConfig.Json");
end

init();

return model;
