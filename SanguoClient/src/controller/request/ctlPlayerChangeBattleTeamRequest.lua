
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlPlayerChangeBattleTeamRequest = class("ctlPlayerChangeBattleTeamRequest", ctlBase)

function ctlPlayerChangeBattleTeamRequest:name()
    return "ctlPlayerChangeBattleTeamRequest";
end

function ctlPlayerChangeBattleTeamRequest:onSendRequest(battleTeam)

    local _battleTeam = battleTeam or model_user.battleTeam;

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        battleTeam = _battleTeam,

    }

    local requestType = "PlayerChangeBattleTeamRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)
    --print_data_dump(model_user.loginToken, string.len(model_user.loginToken));
    -- print_data_dump(httpData, string.len(httpData));

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "player.pb?_a=changeBattleTeam&" .. self:snid(),
        self.name());


end

function ctlPlayerChangeBattleTeamRequest:onDecodePb(pb)
    local requestType = "PlayerChangeBattleTeamResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlPlayerChangeBattleTeamRequest:onSuccess(data)
    model_user.battleTeam = data.battleTeam;
end


return ctlPlayerChangeBattleTeamRequest;


