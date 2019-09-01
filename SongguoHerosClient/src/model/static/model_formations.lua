
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_formations", model_static)

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

function model:getIcon(id)
    local node = self:getNode(id);
    if(node) then
        return "Formation/Formation_A_"..id..".png"
    end
    return "";
end

local function init()
    model:load("config/static/Formation.Json");
end

init();

return model;
