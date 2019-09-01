
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlHighladderGetRankingRewardRequest = class("ctlHighladderGetRankingRewardRequest", ctlBase)

function ctlHighladderGetRankingRewardRequest:name()
    return "ctlHighladderGetRankingRewardRequest";
end

function ctlHighladderGetRankingRewardRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "HighladderGetRankingRewardRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=getRankingReward&" .. self:snid(),
        self.name());


end

function ctlHighladderGetRankingRewardRequest:onDecodePb(pb)
    local requestType = "HighladderGetRankingRewardResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderGetRankingRewardRequest:onSuccess(data)
end


return ctlHighladderGetRankingRewardRequest;


