
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_area", model_static)

function model:getName(id)
    local node = self:getNode(id);
    if(node) then
        return node.name_zhCN;
    end
    return "";
end

function model:getDesc(id)
    local node = self:getNode(id);
    if(node) then
        return node.description_zhCN;
    end
    return "";
end

function model:getAsset(id)
    local node = self:getNode(id);
    if(node) then
        return node.asset;
    end
    return "";
end

--interface

function model:collectAreas(islandId)
    local areas = {};
    
    for key, value in pairs(self.map) do
        if(value.islandId == islandId) then
            areas[#(areas) + 1] = value;
        end
    end
    
    return areas;
end


local function init()
    model:load("config/static/Area.Json");
end

init();

return model;
