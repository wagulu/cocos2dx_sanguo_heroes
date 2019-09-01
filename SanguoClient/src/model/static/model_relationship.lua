
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_relationship", model_static)


function model:setNode(node)
    if(self.map[node.monsterId] == nil) then
        self.map[node.monsterId] = {};
    end
    
    local group = self.map[node.monsterId];
    group[#(group) + 1] = node;
    
    self.relationMap[node.id] = node
end

function model:getNode(monsterId)
    return self.map[monsterId];
end

function model:getRelationItem(id)
    return self.relationMap[id];
end

--interface

function model:getName(item)
    return item.name_zhCN;
end

function model:getDesc(item)
    return item.description_zhCN;
end

function model:prepair()
    self.relationMap = {}
end

local function init()
    model:prepair()
    model:load("config/static/Relationship.Json");
end

init();

return model;
