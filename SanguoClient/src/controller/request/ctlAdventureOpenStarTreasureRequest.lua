
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")

local ctlAdventureOpenStarTreasureRequest = class("ctlAdventureOpenStarTreasureRequest", ctlBase)

function ctlAdventureOpenStarTreasureRequest:name()
    return "ctlAdventureOpenStarTreasureRequest";
end

function ctlAdventureOpenStarTreasureRequest:onSendRequest(_islandId, _areaId, _stageType, _starTreasureId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,
        areaId = _areaId,
        stageType = _stageType,
        starTreasureId = _starTreasureId,

    }

    local requestType = "AdventureOpenStarTreasureRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "adventure.pb?_a=openStarTreasure&" .. self:snid(),
        self:name());

end

function ctlAdventureOpenStarTreasureRequest:onDecodePb(pb)
    local requestType = "AdventureOpenStarTreasureResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlAdventureOpenStarTreasureRequest:onSuccess(data)
    for key, value in pairs(data.starTreasureBoxInfo) do
        helper_drop:getDrop(value);
    end
end


return ctlAdventureOpenStarTreasureRequest;
