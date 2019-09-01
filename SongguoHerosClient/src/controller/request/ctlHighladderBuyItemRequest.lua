
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local helper_shoplist_item = _REQUIRE("model/helper/helper_shoplist_item.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_highladderShop = _REQUIRE("model/helper/helper_highladderShop.lua")

local ctlHighladderBuyItemRequest = class("ctlHighladderBuyItemRequest", ctlBase)

function ctlHighladderBuyItemRequest:name()
    return "ctlHighladderBuyItemRequest";
end

function ctlHighladderBuyItemRequest:onSendRequest(_pos)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        pos = _pos
    }

    local requestType = "HighladderBuyItemRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=buyItem&" .. self:snid(),
        self.name());


end

function ctlHighladderBuyItemRequest:onDecodePb(pb)
    local requestType = "HighladderBuyItemResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderBuyItemRequest:onSuccess(data)
    helper_drop:getDrop(data.prize);
    model_user.highladderShop = data.shop;
end


return ctlHighladderBuyItemRequest;


