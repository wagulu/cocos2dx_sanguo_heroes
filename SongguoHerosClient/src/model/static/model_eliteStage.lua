
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_eliteStage", model_static)

function model:getName(id)
    local node = self:getNode(id);
    if(node) then
        return node.name_zhCN;
    end
    return "";
end

function model:getOpenLevel(id)
    local node = self:getNode(id);
    if(node) then
        return node.openLevel;
    end
    return 1;
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

function model:geCoin(id)
    local node = self:getNode(id);
    if(node) then
        return node.coin;
    end
    return 0;
end

function model:geExp(id)
    local node = self:getNode(id);
    if(node) then
        return node.exp;
    end
    return 0;
end

function model:geIsBoss(id)
    local node = self:getNode(id);
    if(node) then
        return node.isBoss;
    end
    return false;
end

function model:getBattleMode(id)
    local node = self:getNode(id);
    if(node) then
        return node.battleMode;
    end
    return 0;
end

local function init()
    model:load("config/static/EliteStage.Json");
end

init();

return model;
