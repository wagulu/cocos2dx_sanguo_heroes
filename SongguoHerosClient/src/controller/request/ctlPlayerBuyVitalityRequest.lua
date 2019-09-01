
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlPlayerBuyVitalityRequest = class("ctlPlayerBuyVitalityRequest", ctlBase)

function ctlPlayerBuyVitalityRequest:name()
    return "ctlPlayerBuyVitalityRequest";
end

function ctlPlayerBuyVitalityRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "PlayerBuyVitalityRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "player.pb?_a=buyVitality&" .. self:snid(),
        self.name());


end

function ctlPlayerBuyVitalityRequest:onDecodePb(pb)
    local requestType = "PlayerBuyVitalityResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlPlayerBuyVitalityRequest:onSuccess(data)
--    model_user:setDailyBuyVitalityTimes(data.newVitality);
end


return ctlPlayerBuyVitalityRequest;


