
local model_user = _REQUIRE("model/model_user")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")
_REQUIRE("pb/pbCommon.lua")

local helper = {};


function helper:changeTeamRequest()
end

function helper:modifyTeamRequest()
end

--interface

function helper:isMonsterInTeam(monsterId, battleTeam)
    battleTeam = battleTeam or model_user.battleTeam;

    for key, value in pairs(battleTeam) do
        if(value.monsterId == monsterId) then
            return true;
        end
    end
    return false;
end

function helper:isMonsterInBattle(monsterId, battleTeam)
    battleTeam = battleTeam or model_user.battleTeam;

    for key, value in pairs(battleTeam) do
        if(value.monsterId == monsterId and value.inUse > 0) then
            return true;
        end
    end
    return false;
end

--function helper:setInBattle(monsterId, pos)
--    if(pos > 0) then
--        for key, value in pairs(model_user.battleTeam) do
--            if(value.inUse == pos) then
--                value.inUse = 0;
--                break;
--            end
--        end
--    end
--
--    for key, value in pairs(model_user.battleTeam) do
--        if(value.monsterId == monsterId) then
--            value.inUse = pos;
--            break;
--        end
--    end
--end

function helper:setInTeam(monsterId, pos, battleTeam)
    battleTeam = battleTeam or model_user.battleTeam;

--    local oldNode = nil;

    for key, value in pairs(battleTeam) do
        if(value.monsterId == monsterId) then
--            oldNode = model_user.battleTeam[key];
            value.monsterId = 0;
            if (value.pos >= 1 and value.pos <= 3) then
                value.isUse = value.pos;
            end
            break;
        end
    end
    
    if(pos > 0) then
        local node = battleTeam[pos];

        node.pos = pos;
        node.monsterId = monsterId;
        
        if(pos >= 1 and pos <= 3) then
            node.isUse = pos;
        else
            node.isUse = 0;
        end

--        if(oldNode) then
--            node.isUse = oldNode.isUse;
--        else
--            node.isUse = 0;
--        end
    end
end

return helper;
