

local SKILLUPPOINT_INTERVAL_SEC = 600;
local ENERGY_RECOVER_INTERVAL_SEC = 360;
local VITALITY_INTERVAL_SEC = 600;

local function onTimer_recover(dt, data, timerId)
    --cc.Director:getInstance():purgeCachedData()

    release_print("onTimer_recover");
    local model_user = _REQUIRE("model.model_user")
    local time = os.time();
--    model_user:addEnergy(1);
--    local event = cc.EventCustom:new("initTabBar")
--    cc.Director:getInstance():getEventDispatcher():dispatchEvent(event)
    
    if(model_user.time_to_next_skillUpPoint > 0) then
        if(time >= model_user.time_to_next_skillUpPoint) then
--            model_user.energyRefreshTime = time;
            model_user.time_to_next_skillUpPoint = model_user.time_to_next_skillUpPoint + SKILLUPPOINT_INTERVAL_SEC;
            if not(model_user:isSkillUpPointFull()) then
                model_user:addSkillUpPoint(1);
            end
        end
    end

    if(model_user.time_to_next_energy > 0) then
        if(time >= model_user.time_to_next_energy) then
--            model_user.energyRefreshTime = time;
            model_user.time_to_next_energy = model_user.time_to_next_energy + ENERGY_RECOVER_INTERVAL_SEC;
            if not(model_user:isEnergyFull()) then
                model_user:addEnergy(1);        
--                local event = cc.EventCustom:new("initTabBar")
--                cc.Director:getInstance():getEventDispatcher():dispatchEvent(event)

            end
        end
    end
    
    if(model_user.time_to_next_vitality > 0) then
        if(time >= model_user.time_to_next_vitality) then
--            model_user.vitalityRefreshTime = time;
            model_user.time_to_next_vitality = model_user.time_to_next_vitality + VITALITY_INTERVAL_SEC;
            if not(model_user:isVitalityFull()) then
                model_user:addVitality(1);
--                local event = cc.EventCustom:new("initTabBar")
--                cc.Director:getInstance():getEventDispatcher():dispatchEvent(event)
            end
        end
    end

end


local timerId = Timer:start(onTimer_recover, 20)

