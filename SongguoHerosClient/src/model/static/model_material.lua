
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_material", model_static)

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

function model:getQuality(id)
    local node = self:getNode(id);
    if(node) then
        return node.quality;
    end
    return 1;
end

function model:getAsset(id)
    local node = self:getNode(id);
    if(node) then
        return node.asset;
    end
    return "";
end

function model:getMergeCoin(id)
    local node = self:getNode(id);
    if(node) then
        return node.mergeCoin;
    end
    return "";
end

local function init()
    model:load("config/static/Material.Json");
end

init();

return model;
