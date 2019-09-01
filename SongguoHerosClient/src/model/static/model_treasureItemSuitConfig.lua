
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_treasureItemSuitConfig.lua", model_static)

--    "1": {
--        "id": 1,
--        "suitId": 1,
--        "treasureItemCount": 2,
--        "propertyName": "HP",
--        "propertyOperator": "Addition","Multiplication"
--        "propertyValue": 1000,
--        "ver": "1.0.0"
--    },

--"suitId"  "propertyName" 

function model:setNode(node)
    if (self.map[node.suitId] == nil) then
        self.map[node.suitId] = {}
    end

    if (self.map[node.suitId][node.propertyName] == nil) then
        self.map[node.suitId][node.propertyName] = {}
    end

    local group = self.map[node.suitId][node.propertyName]
    group[#(group) + 1] = node
end

function model:getConfigs(suitId, propertyName)
    return self.map[suitId][propertyName]
end

function model:getAllConfigs(suitId)
    return self.map[suitId]
end

local function init()
    model:load("config/static/TreasureItemSuitConfig.Json")
end

init()

return model
