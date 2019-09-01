
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlAccountNetworkLoginRequest = class("ctlAccountNetworkLoginRequest", ctlBase)

function ctlAccountNetworkLoginRequest:name()
    return "ctlAccountNetworkLoginRequest";
end

function ctlAccountNetworkLoginRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        snUserName = model_user.userName,
        password = model_user.password,
        snUid = model_user.snUid,

    }

    local requestType = "AccountNetworkLoginRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "account.pb?_a=networkLogin&" .. self:snid(),
        self.name());


end

function ctlAccountNetworkLoginRequest:onDecodePb(pb)
    local requestType = "AccountNetworkLoginResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlAccountNetworkLoginRequest:onSuccess(data)
    release_print("get user loginToken = " .. tostring(data.loginToken));
    --print_data_dump(data.loginToken, string.len(data.loginToken));
    model_user.loginToken = data.loginToken;
end


return ctlAccountNetworkLoginRequest;


