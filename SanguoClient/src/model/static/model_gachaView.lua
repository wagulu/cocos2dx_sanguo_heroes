
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_gachaView", model_static)

function model:setNode(node)
    local typeId = tonumber(node.typeId)
    if self.map[node.gachaType] == nil then
        self.map[node.gachaType] ={}
    end
    if self.map[node.gachaType][typeId] == nil then
        self.map[node.gachaType][typeId] = {}
    end
    local group = self.map[node.gachaType][typeId]
    group[#(group) + 1] = node
end

function model:getItemByGachaType(gachaType)
    return self.map[gachaType] 
end

local function init()
    model:load("config/static/GachaView.Json");
end

init();

return model;
