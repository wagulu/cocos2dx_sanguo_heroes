
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")

local ctlMonstersChangeTreasureItemRequest = class("ctlMonstersChangeTreasureItemRequest", ctlBase)

function ctlMonstersChangeTreasureItemRequest:name()
    return "ctlMonstersChangeTreasureItemRequest";
end

function ctlMonstersChangeTreasureItemRequest:onSendRequest(_monsterId, _slotId, _inventoryTreasureItemId)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
        slotId = _slotId,
        inventoryTreasureItemId = _inventoryTreasureItemId,

    }

    local requestType = "MonstersChangeTreasureItemRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=changeTreasureItem&" .. self:snid(),
        self.name());


end

function ctlMonstersChangeTreasureItemRequest:onDecodePb(pb)
    local requestType = "MonstersChangeTreasureItemResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersChangeTreasureItemRequest:onSuccess(data)
    model_monsterStorage:updateMonster(data.monster);

    for key, value in pairs(data.treasureItems) do
        model_treasureItemStorage:updateItem(value);
    end
end


return ctlMonstersChangeTreasureItemRequest;


