
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local helper_shoplist_item = _REQUIRE("model/helper/helper_shoplist_item.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")

local ctlHighladderRefreshShopRequest = class("ctlHighladderRefreshShopRequest", ctlBase)

function ctlHighladderRefreshShopRequest:name()
    return "ctlHighladderRefreshShopRequest";
end

function ctlHighladderRefreshShopRequest:onSendRequest()

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

    }

    local requestType = "HighladderRefreshShopRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=refreshShop&" .. self:snid(),
        self.name());


end

function ctlHighladderRefreshShopRequest:onDecodePb(pb)
    local requestType = "HighladderRefreshShopResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderRefreshShopRequest:onSuccess(data)
    model_user.highladderShop = data.shop;
end


return ctlHighladderRefreshShopRequest;


