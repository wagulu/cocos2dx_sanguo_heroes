
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlInitBattleTeamRequest = class("ctlInitBattleTeamRequest", ctlBase)

function ctlInitBattleTeamRequest:name()
    return "ctlInitBattleTeamRequest";
end

function ctlInitBattleTeamRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitBattleTeamRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)
    --print_data_dump(model_user.loginToken, string.len(model_user.loginToken));
    -- print_data_dump(httpData, string.len(httpData));

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=battleTeam&" .. self:snid(),
        self.name());


end

function ctlInitBattleTeamRequest:onDecodePb(pb)
    local requestType = "InitBattleTeamResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitBattleTeamRequest:onSuccess(data)
    model_user.battleTeam = data.battleTeam;
end


return ctlInitBattleTeamRequest;


