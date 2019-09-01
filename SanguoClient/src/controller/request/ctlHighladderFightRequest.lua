
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlHighladderFightRequest = class("ctlHighladderFightRequest", ctlBase)

function ctlHighladderFightRequest:name()
    return "ctlHighladderFightRequest";
end

function ctlHighladderFightRequest:onSendRequest(_rank, _monsterIds, _isWinner)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        rank = _rank,
        monsterIds = _monsterIds,
        isWinner = _isWinner,

    }

    local requestType = "HighladderFightRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=fight&" .. self:snid(),
        self.name());


end

function ctlHighladderFightRequest:onDecodePb(pb)
    local requestType = "HighladderFightResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderFightRequest:onSuccess(data)
    --need update monsters
end


return ctlHighladderFightRequest;


