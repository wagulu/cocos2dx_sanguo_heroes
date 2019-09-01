
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")

local ctlAdventureListIslandRequest = class("ctlAdventureListIslandRequest", ctlBase)

function ctlAdventureListIslandRequest:name()
    return "ctlAdventureListIslandRequest";
end

function ctlAdventureListIslandRequest:onSendRequest()
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
    }

    local requestType = "AdventureListIslandRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "adventure.pb?_a=listIsland&" .. self:snid(),
        self.name());

end

function ctlAdventureListIslandRequest:onDecodePb(pb)
    local requestType = "AdventureListIslandResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
--    pbHelper.printPB(httpData);
    return httpData;
end

function ctlAdventureListIslandRequest:onSuccess(data)
    model_user.islandsList = data.islandsInfo;
end


return ctlAdventureListIslandRequest;
