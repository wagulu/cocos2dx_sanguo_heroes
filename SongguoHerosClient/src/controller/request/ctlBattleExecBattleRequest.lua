
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user.lua")

local ctlBattleExecBattleRequest = class("ctlBattleExecBattleRequest", ctlBase)

function ctlBattleExecBattleRequest:name()
    return "ctlBattleExecBattleRequest";
end

function ctlBattleExecBattleRequest:onSendRequest(_islandId, _areaId, _stageId, _stageType, 
    _progress, _monsterIds, _NPCIds)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,
        areaId = _areaId,
        stageId = _stageId,
        stageType = _stageType,
--        battleId = 0,
        progress = _progress,
        monsterIds = _monsterIds,
        NPCIds = _NPCIds,

    }

    local requestType = "BattleExecBattleRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "battle.pb?_a=execBattle&" .. self:snid(),
        self.name());


end

function ctlBattleExecBattleRequest:onDecodePb(pb)
    local requestType = "BattleExecBattleResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
--    pbHelper.printPB(httpData);
    return httpData;
end

function ctlBattleExecBattleRequest:onSuccess(data)
    for key, value in pairs(model_user.battleMyTeam) do
        for i = 1, #(data.monstersInfo) do
            local monsterInfo = data.monstersInfo[i];
            
            if(value.id == monsterInfo.id) then
                local currentHP = value.currentHP;
                local currentMP = value.currentMP;
                model_user.battleMyTeam[key] = monsterInfo;
                model_user.battleMyTeam[key].currentHP = currentHP;
                model_user.battleMyTeam[key].currentMP = currentMP;
                break;
            end
        end
    end

    for key, value in pairs(model_user.battleNpcTeam) do
        for i = 1, #(data.NPCMonstersInfo) do
            local monsterInfo = data.NPCMonstersInfo[i];

            if(value.id == monsterInfo.id) then
                local currentHP = value.currentHP;
                local currentMP = value.currentMP;
                model_user.battleNpcTeam[key] = monsterInfo;
                model_user.battleNpcTeam[key].currentHP = currentHP;
                model_user.battleNpcTeam[key].currentMP = currentMP;
                break;
            end
        end
    end
end

return ctlBattleExecBattleRequest;
