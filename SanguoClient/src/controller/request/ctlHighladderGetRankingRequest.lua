
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlHighladderGetRankingRequest = class("ctlHighladderGetRankingRequest", ctlBase)

function ctlHighladderGetRankingRequest:name()
    return "ctlHighladderGetRankingRequest";
end

function ctlHighladderGetRankingRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "HighladderGetRankingRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=getRanking&" .. self:snid(),
        self.name());


end

function ctlHighladderGetRankingRequest:onDecodePb(pb)
    local requestType = "HighladderGetRankingResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderGetRankingRequest:onSuccess(data)
    model_user.pvpTopRanks = data.rankings;
    model_user.pvpTopSelf = data.self;
end


return ctlHighladderGetRankingRequest;


