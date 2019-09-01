
--        "id": 1,
--        "relationshipId": 1,
--        "propertyName": "Power",
--        "propertyOperator": "Addition",
--        "propertyValue": 5,
--        "ver": ""

--monsterId

_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_relationshipItemGain", model_static)

function model:setNode(node)
    local relationshipId = node.relationshipId
    if(self.map[relationshipId] == nil) then
        self.map[relationshipId] = {}
    end
    local group = self.map[relationshipId]
    group[#(group) + 1] = node
end

function model:getRelationship(relationshipId)
    return self.map[relationshipId]
end

local function init()
    model:load("config/static/RelationshipItemGain.Json")
end

init();

return model
