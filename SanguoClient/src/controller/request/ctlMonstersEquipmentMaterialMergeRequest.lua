
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local ctlMonstersEquipmentMaterialMergeRequest = class("ctlMonstersEquipmentMaterialMergeRequest", ctlBase)

function ctlMonstersEquipmentMaterialMergeRequest:name()
    return "ctlMonstersEquipmentMaterialMergeRequest";
end

function ctlMonstersEquipmentMaterialMergeRequest:onSendRequest(_materialId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        materialId = _materialId,
    }

    local requestType = "MonstersEquipmentMaterialMergeRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=equipmentMaterialMerge&" .. self:snid(),
        self.name());

end

function ctlMonstersEquipmentMaterialMergeRequest:onDecodePb(pb)
    local requestType = "MonstersEquipmentMaterialMergeResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersEquipmentMaterialMergeRequest:onSuccess(data)
    model_user.selectEquipmentMaterialMergeResult = data;

    for key, value in pairs(data.materials) do
        model_materialStorage:setCount(value.id, value.count);
    end
end


return ctlMonstersEquipmentMaterialMergeRequest;
