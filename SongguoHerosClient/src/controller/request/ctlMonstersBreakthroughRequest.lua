
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_pve = _REQUIRE("model/helper/helper_pve.lua")

local ctlMonstersBreakthroughRequest = class("ctlMonstersBreakthroughRequest", ctlBase)

function ctlMonstersBreakthroughRequest:name()
    return "ctlMonstersBreakthroughRequest";
end

function ctlMonstersBreakthroughRequest:onSendRequest(_monsterId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
    }

    local requestType = "MonstersBreakthroughRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=breakthrough&" .. self:snid(),
        self.name());

end

function ctlMonstersBreakthroughRequest:onDecodePb(pb)
    local requestType = "MonstersBreakthroughResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersBreakthroughRequest:onSuccess(data)
    model_monsterStorage:updateMonster(data.monster);
    
    for key, value in pairs(data.items) do
        model_itemStorage:updateItem(value);
    end
end


return ctlMonstersBreakthroughRequest;
