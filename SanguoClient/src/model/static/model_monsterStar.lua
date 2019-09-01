
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterStar", model_static)

function model:getStar(level)
    local node = self:getNode(level);
    if(node) then
        return node.parts;
    end
    return 0;
end

function model:getTotalStar(level)
    local node = self:getNode(level);
    if(node) then
        return node.totalParts;
    end
    return 0;
end

function model:getCoin(starNum)
    local node = self:getNode(starNum);
    if(node) then
        return node.coin;
    end
    return 0;
end

function model:getGain(level)
    local node = self:getNode(level);
    if(node) then
        return node.gain;
    end
    return 0;
end


local function init()
    model:load("config/static/MonsterStar.Json");
end

init();

return model;
