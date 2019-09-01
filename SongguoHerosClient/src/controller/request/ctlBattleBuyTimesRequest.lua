
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_pve = _REQUIRE("model/helper/helper_pve.lua")

local ctlBattleBuyTimesRequest = class("ctlBattleBuyTimesRequest", ctlBase)

function ctlBattleBuyTimesRequest:name()
    return "ctlBattleBuyTimesRequest";
end

function ctlBattleBuyTimesRequest:onSendRequest(_islandId, _areaId, _stageId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,
        areaId = _areaId,
        stageId = _stageId,
    }

    local requestType = "BattleBuyTimesRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "battle.pb?_a=buyTimes&" .. self:snid(),
        self.name());

end

function ctlBattleBuyTimesRequest:onDecodePb(pb)
    local requestType = "BattleBuyTimesResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlBattleBuyTimesRequest:onSuccess(data)
    local newTimes = data.newTimes;
    model_user.selectStageConfirmData.userStage.battleTimes = newTimes;
    model_user.selectEliteStageLeftBattleTimes = newTimes;
end


return ctlBattleBuyTimesRequest;
