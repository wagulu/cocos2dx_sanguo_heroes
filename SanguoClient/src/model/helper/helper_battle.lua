
local model_User = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local helper = {};

function helper:getActiveFormationId(id)
    if(id == 0 or id == nil) then
        id = model_User.battleSelectMonsterId;
    end

    local monster = model_monsterStorage:getMonster(id);
    for key, value in pairs(monster.formations) do
        if(value.isUse) then
            return value.formationId;
        end
    end
    return 0;
end

function helper:getActivePosition(id)
    if(id == 0 or id == nil) then
        id = model_User.battleSelectMonsterId;
    end

    local monster = model_monsterStorage:getMonster(id);
    return monster.position;
end

function helper:getAllFormation(id)
    if(id == 0 or id == nil) then
        id = model_User.battleSelectMonsterId;
    end

    local monster = model_monsterStorage:getMonster(id);
    return monster.formations;
end

function helper:updateConfirmFormation(monsterId,position,formationId)
    local battleTeam = model_user.battleMyTeam; --.selectStageConfirmData["battleTeam"]
    for key, value in pairs(battleTeam) do
        if(value.id == monsterId) then
            value.position = position
            for i=1,3 do
                value.formations[i].isUse = false
                if i==formationId then
                    value.formations[i].isUse = true
                end
            end
        end
    end
     
end


return helper;
