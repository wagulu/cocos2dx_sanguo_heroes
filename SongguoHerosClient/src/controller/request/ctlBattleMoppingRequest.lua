
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_pve = _REQUIRE("model/helper/helper_pve.lua")

local ctlBattleMoppingRequest = class("ctlBattleMoppingRequest", ctlBase)
ctlBattleMoppingRequest.requestTimes = 0;

function ctlBattleMoppingRequest:name()
    return "ctlBattleMoppingRequest";
end

function ctlBattleMoppingRequest:onSendRequest(_islandId, _areaId, _stageId, _stageType, _times)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,
        areaId = _areaId,
        stageId = _stageId,
        stageType = _stageType,
        times = _times,
    }
    
    self.requestTimes = _times;

    local requestType = "BattleMoppingRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "battle.pb?_a=mopping&" .. self:snid(),
        self.name());

end

function ctlBattleMoppingRequest:onDecodePb(pb)
    local requestType = "BattleMoppingResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlBattleMoppingRequest:onSuccess(data)
    model_user.moppingResult = data;

    helper_pve:subMoppingTimes(self.requestTimes);

    for key, value in pairs(data.moppingTreasureList) do
        for key1, value1 in pairs(value.moppingTreasures.treasures) do
            helper_drop:getDrop(value1);
        end
    end
end


return ctlBattleMoppingRequest;
