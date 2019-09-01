
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_relationshipLevel", model_static)


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

function model:getActiveRelationshipLevel(relationshipId, monsterQuality)
    local node = self:getNode(relationshipId);
    if(node) then
        local maxLevel = 0;
        local relationship = nil;

        for key, value in pairs(node) do
            if(value.monsterQuality <= monsterQuality and value.level > maxLevel) then
                maxLevel = value.level;
                relationship = value;
            end
        end
        
        return relationship;
    end

    return nil;
end


--interface

function model:getLevelId(item)
    return item.id;
end

function model:getDesc(item)
    return item.description_zhCN;
end

function model:getLevel(item)
    return item.level;
end



local function init()
    model:load("config/static/RelationshipLevel.Json");
end

init();

return model;
