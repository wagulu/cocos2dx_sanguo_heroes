
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local helper_shoplist_item = _REQUIRE("model/helper/helper_shoplist_item.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")

local ctlShopBuyItemRequest = class("ctlShopBuyItemRequest", ctlBase)

function ctlShopBuyItemRequest:name()
    return "ctlShopBuyItemRequest";
end

function ctlShopBuyItemRequest:onSendRequest(_shopType, _itemId, _buyNum)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        shopType = _shopType,
        prizeId = _itemId,
        buyNum = _buyNum
    }

    local requestType = "ShopBuyItemRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "shopBuyItem.pb?_a=&" .. self:snid(),
        self.name());


end

function ctlShopBuyItemRequest:onDecodePb(pb)
    local requestType = "ShopBuyItemResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlShopBuyItemRequest:onSuccess(data)
    helper_shoplist_item:updateItem(data.goods);
    helper_drop:getDrop(data.prize);
end


return ctlShopBuyItemRequest;


