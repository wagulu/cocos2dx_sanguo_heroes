
local static_model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
local dateHelper = _REQUIRE("common/dateHelper.lua")
local model_user = _REQUIRE("model/model_user")
local model_monster = _REQUIRE("model/static/model_monster.lua");
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua");

local dateHelper = _REQUIRE("common/dateHelper.lua")

local helper = {};


function helper:getItemWithId(id)
    for key, value in pairs(model_user.highladderShop.items) do
        if(value.prize.id == id) then
            return value;
        end
    end
    return nil;
end

function helper:getItemWithIndex(index)
    return model_user.highladderShop.items[index];
end


function helper:getResetTime()
    return model_user.highladderShop.resetTime;
end

function helper:isTimeToRefresh()
    local localTime = model_user:getCurrentServerTimestamp();
    return localTime >= self:getResetTime();
end


function helper:getRefreshCost()
    return model_user.highladderShop.refreshPrestige;
end



return helper;
