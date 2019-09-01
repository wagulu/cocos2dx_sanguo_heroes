
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local ctlInitMonstersRequest = class("ctlInitMonstersRequest", ctlBase)


function ctlInitMonstersRequest:name()
    return "ctlInitMonstersRequest";
end

function ctlInitMonstersRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitMonstersRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=monsters&" .. self:snid(),
        self.name());


end

function ctlInitMonstersRequest:onDecodePb(pb)
    local requestType = "InitMonstersResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitMonstersRequest:onSuccess(data)
    model_monsterStorage:initFromPb(data);
end


return ctlInitMonstersRequest;


