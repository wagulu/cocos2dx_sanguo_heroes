
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")

local ctlShopListRequest = class("ctlShopListRequest", ctlBase)

function ctlShopListRequest:name()
    return "ctlShopListRequest";
end

function ctlShopListRequest:onSendRequest(_shopType)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        shopType = _shopType,
    }

    local requestType = "ShopListRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "ShopList.pb?_a=&" .. self:snid(),
        self.name());

end

function ctlShopListRequest:onDecodePb(pb)
    local requestType = "ShopListResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlShopListRequest:onSuccess(data)
    model_user.shoplistContents_item = data.goodsList;
end


return ctlShopListRequest;
