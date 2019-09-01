
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local ctlInitMaterialRequest = class("ctlInitMaterialRequest", ctlBase)


function ctlInitMaterialRequest:name()
    return "ctlInitMaterialRequest";
end

function ctlInitMaterialRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitMaterialRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=material&" .. self:snid(),
        self.name());


end

function ctlInitMaterialRequest:onDecodePb(pb)
    local requestType = "InitMaterialResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitMaterialRequest:onSuccess(data)
    model_materialStorage:initFromPb(data);
end


return ctlInitMaterialRequest;


