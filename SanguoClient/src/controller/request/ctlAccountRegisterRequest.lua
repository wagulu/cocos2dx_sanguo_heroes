
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlAccountRegisterRequest = class("ctlAccountRegisterRequest", ctlBase)

function ctlAccountRegisterRequest:name()
    return "ctlAccountRegisterRequest";
end

function ctlAccountRegisterRequest:onSendRequest(_userName, _passWord, _email)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        userName = _userName,
        passWord = _passWord,
        email = _email,

    }
    model_user.userName = _userName;
    model_user.password = _passWord;

    local requestType = "AccountRegisterRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "account.pb?_a=register&" .. self:snid(),
        self.name());


end

function ctlAccountRegisterRequest:onDecodePb(pb)
    local requestType = "AccountRegisterResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlAccountRegisterRequest:onSuccess(data)
    release_print("get user snUid = " .. tostring(data.snUid));
    model_user.snUid = data.snUid;
end


function ctlAccountRegisterRequest:onFail()
--do nothing
end

return ctlAccountRegisterRequest;


