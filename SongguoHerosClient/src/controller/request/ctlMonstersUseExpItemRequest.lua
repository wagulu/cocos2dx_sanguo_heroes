


_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua");
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua");
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua");

local ctlMonstersUseExpItemRequest = class("ctlMonstersUseExpItemRequest", ctlBase)

function ctlMonstersUseExpItemRequest:name()
    return "ctlMonstersUseExpItemRequest";
end

function ctlMonstersUseExpItemRequest:onSendRequest(_monsterId, _itemId, _count)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
        itemId = _itemId,
        count = _count,

    }

    local requestType = "MonstersUseExpItemRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=useExpItem&" .. self:snid(),
        self:name());


end

function ctlMonstersUseExpItemRequest:onDecodePb(pb)
    local requestType = "MonstersUseExpItemResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end


function ctlMonstersUseExpItemRequest:onSuccess(data)
    model_monsterStorage:updateMonster(data.monster);
    model_itemStorage:updateItem(data.item);
end


return ctlMonstersUseExpItemRequest;

