
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local helper_shoplist_item = _REQUIRE("model/helper/helper_shoplist_item.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_highladderShop = _REQUIRE("model/helper/helper_highladderShop.lua")

local ctlHighladderBattleResultRequest = class("ctlHighladderBattleResultRequest", ctlBase)

function ctlHighladderBattleResultRequest:name()
    return "ctlHighladderBattleResultRequest";
end

function ctlHighladderBattleResultRequest:onSendRequest(_rank, _progress, _monsterId, _enemyMonsterId, _wonBattle)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),

        rank = _rank,
        progress = _progress,
        monsterId = _monsterId,
        enemyMonsterId = _enemyMonsterId,
        wonBattle = _wonBattle,
    }

    local requestType = "HighladderBattleResultRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=battleResult&" .. self:snid(),
        self.name());


end

function ctlHighladderBattleResultRequest:onDecodePb(pb)
    local requestType = "HighladderBattleResultResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderBattleResultRequest:onSuccess(data)
--    helper_drop:getDrop(data.prize);
--    helper_highladderShop:updateItem(data.shop);
    model_user.pvpFightResult = data;

    for key, value in pairs(data.fightPrizes) do
        helper_drop:getDrop(value);
    end
end


return ctlHighladderBattleResultRequest;


