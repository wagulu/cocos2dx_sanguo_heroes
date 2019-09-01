
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlAccountLoginRequest = class("ctlAccountLoginRequest", ctlBase)

function ctlAccountLoginRequest:name()
    return "ctlAccountLoginRequest";
end

function ctlAccountLoginRequest:onSendRequest(_userName, _passWord)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        userName = _userName,
        passWord = _passWord,

    }
    model_user.userName = _userName;
    model_user.password = _passWord;

    local requestType = "AccountLoginRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "account.pb?_a=login&" .. self:snid(),
        self.name());


end

function ctlAccountLoginRequest:onDecodePb(pb)
    local requestType = "AccountLoginResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlAccountLoginRequest:onSuccess(data)
    release_print("get user snUid = " .. tostring(data.snUid));
    model_user.snUid = data.snUid;
end


return ctlAccountLoginRequest;


