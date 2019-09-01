
local static_model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
local dateHelper = _REQUIRE("common/dateHelper.lua")
local model_user = _REQUIRE("model/model_user")
local model_monster = _REQUIRE("model/static/model_monster.lua");
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua");

local helper = {};



function helper:getList(needAllActived, needAllAheadActive, needAllExpired)
    needAllActived = needAllActived or true;
    needAllAheadActive = needAllAheadActive or true;
    needAllExpired = needAllExpired or false;

    local list = {};

    for key, value in pairs(model_user.shoplistContents_item) do
        local need = false;
    
        if(needAllActived and self:isActived(value)) then
            need = true;
        end
        
        if(needAllAheadActive and self:isAheadActive(value)) then
            need = true;
        end

        if(needAllExpired and self:isExpired(value)) then
            need = true;
        end

        if(need) then
            list[#(list) + 1] = value;
        end
    end
    
    return list;
end

function helper:getItemWithIndex(index)
    return model_user.shoplistContents_item[index];
end


function helper:getItemWithId(id)
    for key, value in pairs(model_user.shoplistContents_item) do
        if(value.goodsPrize.id == id) then
            return value;
        end
    end
    return nil;
end

function helper:updateItem(item)
    for key, value in pairs(model_user.shoplistContents_item) do
        if(value.goodsPrize.id == item.goodsPrize.id) then
            model_user.shoplistContents_item[key] = value;
            break;
        end
    end
end


--interface

function helper:isUseCoin(item)
    return item.priceType == PriceType.COIN;
end

function helper:isUseCash(item)
    return item.priceType == PriceType.CASH;
end



function helper:isDiscount(item)
    return item.originalPrice > 0;
end

function helper:getDisplayOriginalPrice(item)
    if(item.originalPrice == 0) then
        return item.price;
    end
    return item.originalPrice;
end


-- active

function helper:isActiveForever(item)
    local startTime = item.startTime;
    local endTime = item.endTime;
    return (startTime == 0 or endTime == 0);
end

function helper:isActived(item)
    local time = self:getUniformActiveTime(item);
    return time > 0;
end

function helper:isExpired(item)
    local time = self:getUniformActiveTime(item);
    return time == 0;
end

function helper:isAheadActive(item)
    local time = self:getUniformActiveTime(item);
    return time < 0;
end

-- calculate uniform time stamp
-- returns:
-- ==0, item is expired
-- <0, item is innactive and returns how long local time ahead to active time
-- >0, item is actived and returns how long it will be expired
function helper:getUniformActiveTime(item)
    if(self:isActiveForever(item)) then
        return 99999999;
    end

    local startTime = item.startTime;
    local endTime = item.endTime;
    local time = model_user:getCurrentServerTimestamp();
    if(time < startTime) then
        return time - startTime;
    end
    if(time >= startTime and time <= endTime) then
        return endTime - time;
    end
    return 0;
end



return helper;
