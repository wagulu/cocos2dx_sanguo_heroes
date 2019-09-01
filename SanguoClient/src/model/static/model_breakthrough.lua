
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_breakthrough", model_static)

local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_monster = _REQUIRE("model/static/model_monster.lua");
_REQUIRE("pbCommon.lua")

function model:getItemNum(id)
    local node = self:getNode(id);
    if(node) then
        return node.itemNum;
    end
    return 0;
end

function model:getExclusiveItemNum(id)
    local node = self:getNode(id);
    if(node) then
        return node.exclusiveItemNum;
    end
    return 0;
end

function model:getBreakthroughItemCount()
    local breakthroughItemId = model_monster:getBreakthroughItemId();
    return model_itemStorage:getCount(breakthroughItemId, pbCommon.ObjectType.BreakthroughItem)
end

function model:getExclusiveItemCount(id)
    local exclusiveItemItemId = model_monster:getBreakthroughExclusiveItemId(id);
    return model_itemStorage:getCount(exclusiveItemItemId, pbCommon.ObjectType.BreakthroughItem)
end

function model:getCoin(id)
    local node = self:getNode(id);
    if(node) then
        return node.coin;
    end
    return 0;
end

function model:getLevelLimit(id)
    local node = self:getNode(id);
    if(node) then
        return node.levelLimit;
    end
    return 0;
end


function model:getEffect(id)
    local effect = {};

    local node = self:getNode(id);
    if(node) then
        effect.Strength = node.Strength;
        effect.Intelligence = node.Intelligence;
        effect.Leadership = node.Leadership;
        effect.HP = node.HP;
    end
    
    return effect;
end



function model:prepair()
end

local function init()
    model:prepair();
    model:load("config/static/Breakthrough.Json");
end

init();

return model;
