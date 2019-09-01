
local static_model_gacha = _REQUIRE("model/static/model_gacha.lua")
local dateHelper = _REQUIRE("common/dateHelper.lua")
local model_user = _REQUIRE("model/model_user.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")

local GACHA_ITEM_DEFINE =
{
    {
        cd = 300,
        itemId = 20,
        itemType = pbCommon.ObjectType.Item_NormalGachaTicket,
        consume = 1,
    },
    {
        cd = 0,
        itemId = 20,
        itemType = pbCommon.ObjectType.Item_NormalGachaTicket,
        consume = 10,
    },
    {
        cd = 82800,
        itemId = 0,
        itemType = pbCommon.ObjectType.cash,
        consume = 1,
    },
    {
        cd = 0,
        itemId = 0,
        itemType = pbCommon.ObjectType.cash,
        consume = 10,
    },
    {
        cd = 0,
        itemId = 0,
        itemType = pbCommon.ObjectType.cash,
        consume = 1,
    },
    {
        cd = 0,
        itemId = 0,
        itemType = pbCommon.ObjectType.cash,
        consume = 1,
    },
};


local storage = {};
storage.map = {};

function storage:setNode(node)
    self.map[node.id] = node;
end

function storage:getNode(id)
    return self.map[id];
end

function storage:getLastFreeBuyTimestamp(id)
    return self:getNode(id).lastFreeTime;
end

function storage:getUsedFreeBuyTimes(id)
    return self:getNode(id).freeTimes;
end

function storage:getTotalBuyTimes(id)
    return self:getNode(id).times;
end

function storage:getTotalBuyTimesWithType(type)
    local times = 0;

    for id, value in pairs(self.map) do
        local item = GACHA_ITEM_DEFINE[id];
        if(item.itemType == type) then
            times = times + value.times * item.consume;
        end
    end

    return times;
end

function storage:getResetTimestamp(id)
    return self:getNode(id).resetFreeTime;
end

function storage:getResetNormalTimestamp(id)
    return self:getNode(id).resetTime;
end

--interface

function storage:getConfig(id)
    local config = static_model_gacha:getNode(id);
    return config;
end

function storage:getConstant(id)
    return GACHA_ITEM_DEFINE[id];
end

function storage:canFreeBuy(id)
    local totalFreeTimes = static_model_gacha:getTotalFreeTimes(id);
    local usedFreeTimes = self:getUsedFreeBuyTimes(id);
    if(usedFreeTimes >= totalFreeTimes) then
        return false;
    end
    
    local lastFreeBuyTimestamp = self:getLastFreeBuyTimestamp(id);
    local currentTimestamp = dateHelper:getLocalTimestamp();
    if(currentTimestamp - lastFreeBuyTimestamp < self:getConstant(id).cd) then
        return false;
    end
    
    return true;
end

function storage:canBuy(id)
    if(self:canFreeBuy(id)) then
        return true;
    end

    local requirePoint = static_model_gacha:getRequirePoint(id);    
    local constant = self:getConstant(id);

    if(constant.itemType == pbCommon.ObjectType.cash) then
        return model_user.cash >= requirePoint;
    elseif(constant.itemType == pbCommon.ObjectType.Item_NormalGachaTicket) then
        return model_itemStorage:getCount(constant.id, constant.type) >= requirePoint;
    end
    
    return false;
end

function storage:isTimeToResetStorage()
    local currentTimestamp = dateHelper:getLocalTimestamp();
    local nextResetTimestamp = currentTimestamp + 1;

    for key, value in pairs(self.map) do
        local resetTimestamp = self:getResetTimestamp(key);
        if(resetTimestamp <= nextResetTimestamp) then
            nextResetTimestamp = resetTimestamp;
        end
    end
    
    return currentTimestamp <= nextResetTimestamp;
end



function storage:init(data)
    for key, value in pairs(data.gachaInfo) do
        self:setNode(value);
    end
end

function storage:updateItem(item)
    self:setNode(item);
end

return storage;
