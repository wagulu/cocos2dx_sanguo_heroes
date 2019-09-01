
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_treasureItemQualityType", model_static)

local EFFECT_TYPE = {};
EFFECT_TYPE.SELF = "self"
EFFECT_TYPE.ITEM = "Item"

function model:setNode(node)
    if(self.map[node.type] == nil) then
        self.map[node.type] = {};
    end
    
    if(self.map[node.type][node.level] == nil) then
        self.map[node.type][node.level] = {};
    end
    
    local group = self.map[node.type][node.level];
    group[#(group) + 1] = node;
end


function model:getDefines(type, level)
--    local effects = {};
--    local node = self:getNode(type);
--    for key, value in pairs(node) do
--        if(value.level <= level) then
--            for var=1, #(value) do
--                effects[#(effects) + 1] = value[var];
--            end
--        end
--    end
--    
--    return effects;

    local node = self:getNode(type);
    return node[level];
end

-- interface

function model:isRequirement_needSelf(requirement)
    return requirement.material == EFFECT_TYPE.SELF;
end

function model:isRequirement_needMaterial(requirement)
    return requirement.material == EFFECT_TYPE.ITEM;
end

function model:getRequirementCount(requirement)
    return requirement.count;
end

function model:getDefine(type, level)
    local node = self:getNode(type);
    return node[level];
end

local function init()
    model:load("config/static/TreasureItemQualityType.Json");
end

init();

return model;
