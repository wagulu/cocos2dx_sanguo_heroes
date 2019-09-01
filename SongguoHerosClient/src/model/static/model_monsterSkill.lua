
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterSkill", model_static)

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
 --等级最大偏移量（技能等级+等级偏移量不能超过武将等级）
function model:getMaxLevelOffset(id)
    local node = self:getNode(id);
    if(node) then
        return node.maxLevelOffset;
    end
    return "";
end

function model:getLevelUpType(id)
    local node = self:getNode(id);
    if(node) then
        return node.levelUpType;
    end
    return "";
end

function model:getEnerySpeed(id)
    local node = self:getNode(id);
    if(node) then
        return tonumber(node.energySpeed);
    end
    return 0;
end

local function init()
    model:load("config/static/MonsterSkill.Json");
end

init();

return model;
