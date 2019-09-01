
_REQUIRE("cocos/cocos2d/json.lua")

--    "1": {
--        "id": 1,
--        "level": 1,
--        "propertyName": "HPPercentage",
--        "propertyValue": 1,
--        "propertyOperator": "Multiplication",
--        "ver": "1.0.0"
--    },


local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_treasureItemStrengthenMaster", model_static)

function model:setNode(node)
    self.map[node.id] = node;

    if (self.levelGroupMap[node.level] == nil) then
        self.levelGroupMap[node.level] = {};
    end
    local group = self.levelGroupMap[node.level];
    group[#(group) + 1] = node;
end

function model:getGroup(level)
    return self.levelGroupMap[level];
end


function model:getPropertyName(node)
    return node.propertyName;
end

function model:getPropertyValue(node)
    return node.propertyValue;
end


function model:prepair()
    self.levelGroupMap = {};
end

local function init()
    model:prepair();
    model:load("config/static/TreasureItemStrengthenMaster.Json");
end

init();

return model;
