
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_gachaStorage = _REQUIRE("model/model_gachaStorage.lua")

local ctlInitGachaRequest = class("ctlInitGachaRequest", ctlBase)

function ctlInitGachaRequest:name()
    return "ctlInitGachaRequest";
end

function ctlInitGachaRequest:onSendRequest()
    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitGachaRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=gacha&" .. self:snid(),
        self:name());

end

function ctlInitGachaRequest:onDecodePb(pb)
    local requestType = "InitGachaResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitGachaRequest:onSuccess(data)
    model_gachaStorage:init(data);
end


return ctlInitGachaRequest;
