
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")

local ctlInitItemsRequest = class("ctlInitItemsRequest", ctlBase)

function ctlInitItemsRequest:name()
    return "ctlInitItemsRequest";
end

function ctlInitItemsRequest:onSendRequest()
    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "InitItemsRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=items&" .. self:snid(),
        self:name());

end

function ctlInitItemsRequest:onDecodePb(pb)
    local requestType = "InitItemsResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitItemsRequest:onSuccess(data)
    model_itemStorage:initFromPb(data);
end


return ctlInitItemsRequest;
