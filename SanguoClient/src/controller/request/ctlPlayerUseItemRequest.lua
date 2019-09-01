
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")

local ctlPlayerUseItemRequest = class("ctlPlayerUseItemRequest", ctlBase)

function ctlPlayerUseItemRequest:name()
    return "ctlPlayerUseItemRequest";
end

function ctlPlayerUseItemRequest:onSendRequest(_itemId, _count)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        itemId = _itemId,
        count = _count,

    }

    local requestType = "PlayerUseItemRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "player.pb?_a=useItem&" .. self:snid(),
        self.name());


end

function ctlPlayerUseItemRequest:onDecodePb(pb)
    local requestType = "PlayerUseItemResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlPlayerUseItemRequest:onSuccess(data)
    model_itemStorage:updateItem(data.item);
end


return ctlPlayerUseItemRequest;


