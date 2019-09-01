
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_relationshipLevelGain", model_static)


function model:setNode(node)
    if(self.map[node.relationshipLevelId] == nil) then
        self.map[node.relationshipLevelId] = {};
    end
    
    local group = self.map[node.relationshipLevelId];
    group[#(group) + 1] = node;
end

function model:getNode(relationshipLevelId)
    return self.map[relationshipLevelId];
end

function model:getGain(relationshipLevelId)
    return self:getNode(relationshipLevelId);
end


--interface

function model:getPropertyName(item)
    return item.propertyName;
end

function model:getPropertyOperator(item)
    return item.propertyOperator;
end

function model:getPropertyValue(item)
    return item.propertyValue;
end



local function init()
    model:load("config/static/RelationshipLevelGain.Json");
end

init();

return model;
