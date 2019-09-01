-------------------
-- all star conditions defined here
-- ATTENTION!! each method should always return true or false
-- return true:  condition passed
-- return false: condition failed
-------------------

_REQUIRE "cocos.cocos2d.json"
_REQUIRE("pbCommon")

local helper = {};

function helper:win()
    return model_user.battleLastFightWin == pbCommon.FightResult.WIN;
end

function helper:hero_dead(noMoreThan)
    noMoreThan = tonumber(noMoreThan);
    local numDead = 0;
    local heroList = model_user.battleMyTeam;
    for i = 1, #(heroList) do
        local hero = heroList[i];
        if hero.currentHP <= 0 then
            numDead = numDead + 1;
        end
    end
    return numDead <= noMoreThan;
end

return helper;
