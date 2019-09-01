local model_user = _REQUIRE("model/model_user.lua")
local helper_pve = _REQUIRE("model/helper/helper_pve.lua")

local helper = {}

function helper:getEnemy(_rank, successCallback)
    local function onGetEnemy(success, data, code, msg)
        if success then
            helper_pve:initBattleStage();
        end
        if successCallback then
            successCallback(success, data, code, msg);
        end
    end

    ctlHighladderGetEnemyRequest:sendRequest(onGetEnemy, _rank);
end



return helper;