
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")

local ctlAdventureListStageRequest = class("ctlAdventureListStageRequest", ctlBase)

function ctlAdventureListStageRequest:name()
    return "ctlAdventureListStageRequest";
end

function ctlAdventureListStageRequest:onSendRequest(_islandId, _areaId, _stageType)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,
        areaId = _areaId,
        stageType = _stageType,
    }

    local requestType = "AdventureListStageRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "adventure.pb?_a=listStage&" .. self:snid(),
        self:name());

end

function ctlAdventureListStageRequest:onDecodePb(pb)
    local requestType = "AdventureListStageResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
--    pbHelper.printPB(httpData);
    return httpData;
end


function ctlAdventureListStageRequest:onSuccess(data)
    model_user.stagesList = data.stagesInfo;
    model_user.stageStarTreasuresList = data.starTreasuresList;
    model_user.stageTotalStar = data.totalStar;
end

return ctlAdventureListStageRequest;
