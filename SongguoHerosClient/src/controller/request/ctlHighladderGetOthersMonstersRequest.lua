
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlHighladderGetOthersMonstersRequest = class("ctlHighladderGetOthersMonstersRequest", ctlBase)

function ctlHighladderGetOthersMonstersRequest:name()
    return "ctlHighladderGetOthersMonstersRequest";
end

function ctlHighladderGetOthersMonstersRequest:onSendRequest(_rank)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        rank = _rank,

    }

    local requestType = "HighladderGetOthersMonstersRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=getOthersMonsters&" .. self:snid(),
        self.name());


end

function ctlHighladderGetOthersMonstersRequest:onDecodePb(pb)
    local requestType = "HighladderGetOthersMonstersResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderGetOthersMonstersRequest:onSuccess(data)
--need update monsters
    model_user.pvpTopOpponent = data.monsters;
end


return ctlHighladderGetOthersMonstersRequest;


