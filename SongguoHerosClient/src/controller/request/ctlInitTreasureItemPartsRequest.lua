
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua")

local ctlInitTreasureItemPartsRequest = class("ctlInitTreasureItemPartsRequest", ctlBase)

function ctlInitTreasureItemPartsRequest:name()
    return "ctlInitTreasureItemPartsRequest";
end

function ctlInitTreasureItemPartsRequest:onSendRequest()
    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitTreasureItemPartsRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=treasureItemParts&" .. self:snid(),
        self:name());

end

function ctlInitTreasureItemPartsRequest:onDecodePb(pb)
    local requestType = "InitTreasureItemPartsResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitTreasureItemPartsRequest:onSuccess(data)
    model_treasureItemPartStorage:initFromPb(data);
end


return ctlInitTreasureItemPartsRequest;
