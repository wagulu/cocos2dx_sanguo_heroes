
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlPlayerBuyEnergyRequest = class("ctlPlayerBuyEnergyRequest", ctlBase)

function ctlPlayerBuyEnergyRequest:name()
    return "ctlPlayerBuyEnergyRequest";
end

function ctlPlayerBuyEnergyRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "PlayerBuyEnergyRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "player.pb?_a=buyEnergy&" .. self:snid(),
        self.name());


end

function ctlPlayerBuyEnergyRequest:onDecodePb(pb)
    local requestType = "PlayerBuyEnergyResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlPlayerBuyEnergyRequest:onSuccess(data)
--    model_user:setDailyBuyEnergyTimes(data.newEnergy);
end


return ctlPlayerBuyEnergyRequest;


