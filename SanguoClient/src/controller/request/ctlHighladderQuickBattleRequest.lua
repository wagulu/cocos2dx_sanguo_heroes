
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local helper_shoplist_item = _REQUIRE("model/helper/helper_shoplist_item.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_highladderShop = _REQUIRE("model/helper/helper_highladderShop.lua")

local ctlHighladderQuickBattleRequest = class("ctlHighladderQuickBattleRequest", ctlBase)

function ctlHighladderQuickBattleRequest:name()
    return "ctlHighladderQuickBattleRequest";
end

function ctlHighladderQuickBattleRequest:onSendRequest(_rank, _progress)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

        rank = _rank,
        progress = _progress,
    }

    local requestType = "HighladderQuickBattleRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=quickBattle&" .. self:snid(),
        self.name());


end

function ctlHighladderQuickBattleRequest:onDecodePb(pb)
    local requestType = "HighladderQuickBattleResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderQuickBattleRequest:onSuccess(data)
--    helper_drop:getDrop(data.prize);
--    helper_highladderShop:updateItem(data.shop);
    model_user.pvpFightResult = data;

    for key, value in pairs(data.fightPrizes) do
        helper_drop:getDrop(value);
    end
end


return ctlHighladderQuickBattleRequest;


