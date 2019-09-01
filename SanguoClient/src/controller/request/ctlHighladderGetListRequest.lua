
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlHighladderGetListRequest = class("ctlHighladderGetListRequest", ctlBase)

function ctlHighladderGetListRequest:name()
    return "ctlHighladderGetListRequest";
end

function ctlHighladderGetListRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "HighladderGetListRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=getList&" .. self:snid(),
        self.name());


end

function ctlHighladderGetListRequest:onDecodePb(pb)
    local requestType = "HighladderGetListResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderGetListRequest:onSuccess(data)
    model_user.pvpRanks = data.ranks;
    model_user.pvpSelf = data.self;
end


return ctlHighladderGetListRequest;


