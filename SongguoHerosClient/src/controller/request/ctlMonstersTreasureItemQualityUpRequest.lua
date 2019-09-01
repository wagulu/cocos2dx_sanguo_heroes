


_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua");
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua");
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua");

local ctlMonstersTreasureItemQualityUpRequest = class("ctlMonstersTreasureItemQualityUpRequest", ctlBase)

function ctlMonstersTreasureItemQualityUpRequest:name()
    return "ctlMonstersTreasureItemQualityUpRequest";
end

function ctlMonstersTreasureItemQualityUpRequest:onSendRequest(_inventoryTreasureItemId)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        inventoryTreasureItemId = _inventoryTreasureItemId,

    }

    local requestType = "MonstersTreasureItemQualityUpRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=treasureItemQualityUp&" .. self:snid(),
        self:name());


end

function ctlMonstersTreasureItemQualityUpRequest:onDecodePb(pb)
    local requestType = "MonstersTreasureItemQualityUpResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end


function ctlMonstersTreasureItemQualityUpRequest:onSuccess(data)
    model_treasureItemStorage:updateItem(data.treasureItem);
    for key, value in pairs(data.removeTreasureItemInventoryIds) do
        model_treasureItemStorage:removeItem(value);
    end
    model_itemStorage:updateItem(data.item);
end


return ctlMonstersTreasureItemQualityUpRequest;

