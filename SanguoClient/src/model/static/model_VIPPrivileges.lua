
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_vipPrivileges", model_static)

local model_user = _REQUIRE("model/model_user.lua")


function model:getDailyTotalStageResetTimes(vipLevel)
    if(vipLevel == nil) then
        vipLevel = model_user.vipLevel;
    end

    for key, value in pairs(self.map) do
        if(value.VIPLevel == vipLevel) then
            return value.dailyTotalStageResetTimes;
        end
    end
    return 9999;
end

function model:getDailyTotalEnergyBuyTimes(vipLevel)
    if(vipLevel == nil) then
        vipLevel = model_user.vipLevel;
    end

    for key, value in pairs(self.map) do
        if(value.VIPLevel == vipLevel) then
            return value.dailyTotalEnergyBuyTimes;
        end
    end
    return 9999;
end

function model:getDailyTotalVitalityBuyTimes(vipLevel)
    if(vipLevel == nil) then
        vipLevel = model_user.vipLevel;
    end

    for key, value in pairs(self.map) do
        if(value.VIPLevel == vipLevel) then
            return value.dailyTotalVitalityBuyTimes;
        end
    end
    return 9999;
end


local function init()
    model:load("config/static/VIPPrivileges.Json");
end

init();

return model;
