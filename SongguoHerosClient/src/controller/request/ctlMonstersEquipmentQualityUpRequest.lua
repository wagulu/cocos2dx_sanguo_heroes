
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local ctlMonstersEquipmentQualityUpRequest = class("ctlMonstersEquipmentQualityUpRequest", ctlBase)

function ctlMonstersEquipmentQualityUpRequest:name()
    return "ctlMonstersEquipmentQualityUpRequest";
end

function ctlMonstersEquipmentQualityUpRequest:onSendRequest(_monsterId, _equipmentId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
        equipmentId = _equipmentId,
    }

    local requestType = "MonstersEquipmentQualityUpRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=equipmentQualityUp&" .. self:snid(),
        self.name());

end

function ctlMonstersEquipmentQualityUpRequest:onDecodePb(pb)
    local requestType = "MonstersEquipmentQualityUpResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersEquipmentQualityUpRequest:onSuccess(data)
    model_user.selectEquipmentUpgradResult = data;
    
    model_monsterStorage:updateMonster(data.monster);
    
    for key, value in pairs(data.materials) do
        model_materialStorage:setCount(value.id, value.count);
    end
end


return ctlMonstersEquipmentQualityUpRequest;
