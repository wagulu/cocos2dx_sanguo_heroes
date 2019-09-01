
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")

local ctlMonstersUseDestinyItemRequest = class("ctlMonstersUseDestinyItemRequest", ctlBase)

function ctlMonstersUseDestinyItemRequest:name()
    return "ctlMonstersUseDestinyItemRequest";
end

function ctlMonstersUseDestinyItemRequest:onSendRequest(_monsterId)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,

    }

    local requestType = "MonstersUseDestinyItemRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)
    --print_data_dump(model_user.loginToken, string.len(model_user.loginToken));
    -- print_data_dump(httpData, string.len(httpData));

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=useDestinyItem&" .. self:snid(),
        self.name());


end

function ctlMonstersUseDestinyItemRequest:onDecodePb(pb)
    local requestType = "MonstersUseDestinyItemResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersUseDestinyItemRequest:onSuccess(data)
    model_monsterStorage:updateMonster(data.monster);
    
    model_itemStorage:updateItem(data.item);
end


return ctlMonstersUseDestinyItemRequest;


