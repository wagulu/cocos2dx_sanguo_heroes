
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_equipmentQualityEvolutionMaterial", model_static)


function model:setNode(node)
    if(self.map[node.type] == nil) then
        self.map[node.type] = {};
    end

    if(self.map[node.type][node.quality] == nil) then
        self.map[node.type][node.quality] = {};
    end

    local group = self.map[node.type][node.quality];
    group[#(group) + 1] = node;
end

function model:getDefines(type, quality)
    local node = self:getNode(type);
    return node[quality];
end

-- interface

function model:getMaterialId(requirement)
    return requirement.materialId;
end

function model:getRequirementCount(requirement)
    return requirement.count;
end



local function init()
    model:load("config/static/EquipmentQualityEvolutionMaterial.Json");
end

init();

return model;
