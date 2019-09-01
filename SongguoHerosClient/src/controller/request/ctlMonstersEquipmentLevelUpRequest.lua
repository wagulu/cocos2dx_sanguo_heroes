
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local ctlMonstersEquipmentLevelUpRequest = class("ctlMonstersEquipmentLevelUpRequest", ctlBase)

function ctlMonstersEquipmentLevelUpRequest:name()
    return "ctlMonstersEquipmentLevelUpRequest";
end

function ctlMonstersEquipmentLevelUpRequest:onSendRequest(_monsterId, _equipmentId, _times)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
        equipmentId = _equipmentId,
        times = _times,
    }

    local requestType = "MonstersEquipmentLevelUpRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=equipmentLevelUp&" .. self:snid(),
        self.name());

end

function ctlMonstersEquipmentLevelUpRequest:onDecodePb(pb)
    local requestType = "MonstersEquipmentLevelUpResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersEquipmentLevelUpRequest:onSuccess(data)
    model_user.selectEquipmentUpgradResult = data;
    
    model_monsterStorage:updateMonster(data.monster);
end


return ctlMonstersEquipmentLevelUpRequest;
