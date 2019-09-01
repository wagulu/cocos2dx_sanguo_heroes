
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlHighladderDrawRequest = class("ctlHighladderDrawRequest", ctlBase)

function ctlHighladderDrawRequest:name()
    return "ctlHighladderDrawRequest";
end

function ctlHighladderDrawRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "HighladderDrawRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=draw&" .. self:snid(),
        self.name());


end

function ctlHighladderDrawRequest:onDecodePb(pb)
    local requestType = "HighladderDrawResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderDrawRequest:onSuccess(data)
end


return ctlHighladderDrawRequest;


