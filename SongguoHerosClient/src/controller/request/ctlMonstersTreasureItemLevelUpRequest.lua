


_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua");
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua");
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua");

local ctlMonstersTreasureItemLevelUpRequest = class("ctlMonstersTreasureItemLevelUpRequest", ctlBase)

function ctlMonstersTreasureItemLevelUpRequest:name()
    return "ctlMonstersTreasureItemLevelUpRequest";
end

function ctlMonstersTreasureItemLevelUpRequest:onSendRequest(_inventoryId, _itemId, _count)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        inventoryTreasureItemId = _inventoryId,
        itemId = _itemId,
        count = _count,

    }

    local requestType = "MonstersTreasureItemLevelUpRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=treasureItemLevelUp&" .. self:snid(),
        self:name());


end

function ctlMonstersTreasureItemLevelUpRequest:onDecodePb(pb)
    local requestType = "MonstersTreasureItemLevelUpResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end


function ctlMonstersTreasureItemLevelUpRequest:onSuccess(data)
    model_itemStorage:updateItem(data.item);
    model_treasureItemStorage:updateItem(data.treasureItem);
end


return ctlMonstersTreasureItemLevelUpRequest;

