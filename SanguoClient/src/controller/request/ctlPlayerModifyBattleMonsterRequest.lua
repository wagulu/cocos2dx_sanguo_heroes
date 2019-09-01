
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlPlayerModifyBattleTeamRequest = class("ctlPlayerModifyBattleTeamRequest", ctlBase)

function ctlPlayerModifyBattleTeamRequest:name()
    return "ctlPlayerModifyBattleTeamRequest";
end

function ctlPlayerModifyBattleTeamRequest:onSendRequest(battleTeam)

    battleTeam = battleTeam or model_user.battleTeam;

    local function createBattleTeamMonsters()
        local list = {};
    
        for var=1, 3 do
            for key, value in pairs(battleTeam) do
                if(value.isUse == var) then
                    list[#(list) + 1] = value.monsterId;
                    break;
                end
            end
        end
        
        return list;
    end

    local _battleTeamMonsters = createBattleTeamMonsters();

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        battleTeamMonsters = _battleTeamMonsters,

    }

    local requestType = "PlayerModifyBattleMonsterRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)
    --print_data_dump(model_user.loginToken, string.len(model_user.loginToken));
    -- print_data_dump(httpData, string.len(httpData));

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "player.pb?_a=modifyBattleMonster&" .. self:snid(),
        self.name());


end

function ctlPlayerModifyBattleTeamRequest:onDecodePb(pb)
    local requestType = "PlayerModifyBattleMonsterResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlPlayerModifyBattleTeamRequest:onSuccess(data)
    model_user.battleTeam = data.battleTeam;
end


return ctlPlayerModifyBattleTeamRequest;


