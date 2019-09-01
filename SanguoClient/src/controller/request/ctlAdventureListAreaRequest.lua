
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")

local ctlAdventureListAreaRequest = class("ctlAdventureListAreaRequest", ctlBase)

function ctlAdventureListAreaRequest:name()
    return "ctlAdventureListAreaRequest";
end

function ctlAdventureListAreaRequest:onSendRequest(_islandId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,

    }

    local requestType = "AdventureListAreaRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "adventure.pb?_a=listArea&" .. self:snid(),
        self:name());

end

function ctlAdventureListAreaRequest:onDecodePb(pb)
    local requestType = "AdventureListAreaResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
--    pbHelper.printPB(httpData);
    return httpData;
end

function ctlAdventureListAreaRequest:onSuccess(data)
    model_user.areasList = data.areasInfo;
end


return ctlAdventureListAreaRequest;
