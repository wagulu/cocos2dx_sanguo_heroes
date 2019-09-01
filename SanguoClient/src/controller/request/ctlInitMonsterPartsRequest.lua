
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua")

local ctlInitMonsterPartsRequest = class("ctlInitMonsterPartsRequest", ctlBase)


function ctlInitMonsterPartsRequest:name()
    return "ctlInitMonsterPartsRequest";
end

function ctlInitMonsterPartsRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitMonsterPartsRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=monsterParts&" .. self:snid(),
        self.name());


end

function ctlInitMonsterPartsRequest:onDecodePb(pb)
    local requestType = "InitMonsterPartsResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitMonsterPartsRequest:onSuccess(data)
    model_monsterPartStorage:initFromPb(data);
end


return ctlInitMonsterPartsRequest;


