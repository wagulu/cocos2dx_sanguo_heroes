
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_treasureBox", model_static)

function model:setNode(node)
    self.map[node.id] = node;
    if self.treasureGroupMap[node.groupId] == nil then
        self.treasureGroupMap[node.groupId] = {}
    end
    local group = self.treasureGroupMap[node.groupId]
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

function model:getGroups(groupId)
    return self.treasureSuitMap[groupId]
end

function model:getGroupId(id)
    local node = self:getNode(id);
    if(node) then
        return node.groupId;
    end
    return 0
end

function model:getType(id)
    local node = self:getNode(id);
    if(node) then
        return node.class;
    end
    return ""
end

function model:prepair()
    self.treasureGroupMap = {}
end

local function init()
    model:prepair()
    model:load("config/static/TreasureBox.Json");
end

init();

return model;
