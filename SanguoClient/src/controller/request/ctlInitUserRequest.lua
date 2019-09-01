
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")

local ctlInitUserRequest = class("ctlAccountGetUidFromSNUidRequest", ctlBase)

function ctlInitUserRequest:name()
    return "ctlInitUserRequest";
end

function ctlInitUserRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitUserRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)
    --print_data_dump(model_user.loginToken, string.len(model_user.loginToken));
    -- print_data_dump(httpData, string.len(httpData));

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=user&" .. self:snid(),
        self.name());


end

function ctlInitUserRequest:onDecodePb(pb)
    local requestType = "InitUserResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitUserRequest:onSuccess(data)

    model_areaStorage:initWithUser(data);
end


return ctlInitUserRequest;


