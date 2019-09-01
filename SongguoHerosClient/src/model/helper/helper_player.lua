
local model_static_library = _REQUIRE("model/static/model_static_library.lua")
local model_user = _REQUIRE("model/model_user")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");

local helper = {};


function helper:canBuyEnergy()
    local cash = self:getBuyEnergyCost();
    return cash <= model_user.cash;
end

function helper:getBuyEnergyCost(times)
    if(times == nil or times == 0) then
        times = model_user:getDailyBuyEnergyTimes();
    end
    
    return static_model_energyTimesPrice:getCash(times+1);
end


function helper:canBuyVitality()
    local cash = self:getBuyVitalityCost();
    return cash <= model_user.cash;
end

function helper:getBuyVitalityCost(times)
    if(times == nil or times == 0) then
        times = model_user:getDailyBuyVitalityTimes();
    end
    
    return static_model_vitalityTimesPrice:getCash(times+1);
end


return helper;
