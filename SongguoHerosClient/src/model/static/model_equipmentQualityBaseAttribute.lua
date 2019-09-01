
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_equipmentQualityBaseAttribute", model_static)

function model:setNode(node)
    local type = node.type
    local level = tonumber(node.level)
    local name = node.propertyName
    
    if(self.map[type] == nil) then
        self.map[type] = {}
    end
    if(self.map[type][level] == nil) then
        self.map[type][level] = {}
    end
    if(self.map[type][level][name] == nil) then
        self.map[type][level][name] = {}
    end
    
    self.map[type][level][name][#(self.map[type][level][name]) + 1] = node
end


function model:getAttributes(type, level, name)
    return self.map[type][level][name]
end

function model:getAllAttributes(type, level)
    return self.map[type][level]
end

local function init()
    model:load("config/static/EquipmentQualityBaseAttribute.Json");
end

init();

return model;