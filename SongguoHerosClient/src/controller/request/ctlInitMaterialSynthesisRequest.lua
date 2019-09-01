
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_materialSynthesis = _REQUIRE("model/static/model_materialSynthesis.lua")

local ctlInitMaterialSynthesisRequest = class("ctlInitMaterialSynthesisRequest", ctlBase)


function ctlInitMaterialSynthesisRequest:name()
    return "ctlInitMaterialSynthesisRequest";
end

function ctlInitMaterialSynthesisRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitMaterialSynthesisRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=materialSynthesis&" .. self:snid(),
        self.name());


end

function ctlInitMaterialSynthesisRequest:onDecodePb(pb)
    local requestType = "InitMaterialSynthesisResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitMaterialSynthesisRequest:onSuccess(data)
    model_materialSynthesis:initFromPb(data);
end


return ctlInitMaterialSynthesisRequest;


