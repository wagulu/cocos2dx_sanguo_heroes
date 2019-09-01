
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_treasureItem", model_static)

function model:setNode(node)
    self.map[node.id] = node;
    if self.treasureSuitMap[node.suitId] == nil then
    	self.treasureSuitMap[node.suitId] = {}
    end
    local group = self.treasureSuitMap[node.suitId]
    group[#(group) + 1] = node
end

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

function model:getQuality(id)
    local node = self:getNode(id);
    if(node) then
        return node.quality;
    end
    return 1;
end

function model:getSuits(suitId)
    return self.treasureSuitMap[suitId]
end

function model:getSuitId(id)
    local node = self:getNode(id);
    if(node) then
        return node.suitId;
    end
    return 0
end

function model:getLevelUpType(id)
    local node = self:getNode(id);
    if(node) then
        return node.levelUpType;
    end
    return 0
end

function model:prepair()
    self.treasureSuitMap = {}
end

local function init()
    model:prepair()
    model:load("config/static/TreasureItem.Json");
end

init();

return model;
