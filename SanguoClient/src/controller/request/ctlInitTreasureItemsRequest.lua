
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")

local ctlInitTreasureItemsRequest = class("ctlInitTreasureItemsRequest", ctlBase)

function ctlInitTreasureItemsRequest:name()
    return "ctlInitTreasureItemsRequest";
end

function ctlInitTreasureItemsRequest:onSendRequest()
    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitTreasureItemsRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=treasureItems&" .. self:snid(),
        self:name());

end

function ctlInitTreasureItemsRequest:onDecodePb(pb)
    local requestType = "InitTreasureItemsResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitTreasureItemsRequest:onSuccess(data)
    model_treasureItemStorage:initFromPb(data);
end


return ctlInitTreasureItemsRequest;
