
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlAccountGetUidFromSNUidRequest = class("ctlAccountGetUidFromSNUidRequest", ctlBase)

function ctlAccountGetUidFromSNUidRequest:name()
    return "ctlAccountGetUidFromSNUidRequest";
end

function ctlAccountGetUidFromSNUidRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        loginKey = model_user.loginToken,
        snUid = model_user.snUid,
        serverId = model_user.serverId, --1

    }

    local requestType = "AccountGetUidFromSNUidRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)
    --print_data_dump(model_user.loginToken, string.len(model_user.loginToken));
   -- print_data_dump(httpData, string.len(httpData));

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "account.pb?_a=getUidFromSNUid&" .. self:snid(),
        self.name());


end

function ctlAccountGetUidFromSNUidRequest:onDecodePb(pb)
    local requestType = "AccountGetUidFromSNUidResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlAccountGetUidFromSNUidRequest:onSuccess(data)
--    require("app.MyApp"):create():showDlg("common/CommonPopScene",nill,{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text="get user uid = " .. tostring(data.uid)})
    release_print("get user uid = " .. tostring(data.uid));
    release_print("get user userKey = " .. tostring(data.userKey));
    model_user.uid = data.uid;
    model_user.userKey = data.userKey;
end


return ctlAccountGetUidFromSNUidRequest;


