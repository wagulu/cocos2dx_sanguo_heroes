
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_relationshipSynthesis", model_static)


function model:setNode(node)
    if(self.map[node.relationshipId] == nil) then
        self.map[node.relationshipId] = {};
    end
    
    local group = self.map[node.relationshipId];
    group[#(group) + 1] = node;
end

function model:getNode(relationshipId)
    return self.map[relationshipId];
end

function model:getSynthesis(relationshipId)
    return self:getNode(relationshipId);
end

local function init()
    model:load("config/static/RelationshipSynthesis.Json");
end

init();

return model;
