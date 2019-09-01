
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_TreasureItemQualityCoinType", model_static)

local EFFECT_TYPE = {};
EFFECT_TYPE.SELF = "self"
EFFECT_TYPE.ITEM = "item"

function model:setNode(node)
    if(self.map[node.type] == nil) then
        self.map[node.type] = {};
    end
    
    if(self.map[node.type][node.level] == nil) then
        self.map[node.type][node.level] = {};
    end
    
    self.map[node.type][node.level] = node;
end

function model:getDefines(type, level)
    local node = self:getNode(type);
    return node[level];
end

function model:getCoin(type, level)
    local node = self:getNode(type);
    if node[level] then
        return node[level].coin;
    end
    
end



local function init()
    model:load("config/static/TreasureItemQualityCoinType.Json");
end

init();

return model;
