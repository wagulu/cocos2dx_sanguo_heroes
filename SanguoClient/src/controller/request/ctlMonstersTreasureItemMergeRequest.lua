


_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua");
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua");
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua");

local ctlMonstersTreasureItemMergeRequest = class("ctlMonstersTreasureItemMergeRequest", ctlBase)

function ctlMonstersTreasureItemMergeRequest:name()
    return "ctlMonstersTreasureItemMergeRequest";
end

function ctlMonstersTreasureItemMergeRequest:onSendRequest(_treasureItemPartId)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        treasureItemPartId = _treasureItemPartId,

    }

    local requestType = "MonstersTreasureItemMergeRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=treasureItemMerge&" .. self:snid(),
        self:name());


end

function ctlMonstersTreasureItemMergeRequest:onDecodePb(pb)
    local requestType = "MonstersTreasureItemMergeResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end


function ctlMonstersTreasureItemMergeRequest:onSuccess(data)
    model_treasureItemStorage:updateItem(data.treasureItem);
    model_treasureItemPartStorage:updateItem(data.treasureItemPart);
end


return ctlMonstersTreasureItemMergeRequest;

