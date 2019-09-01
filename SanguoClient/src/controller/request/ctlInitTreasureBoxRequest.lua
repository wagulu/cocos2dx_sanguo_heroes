
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_treasureBoxStorage = _REQUIRE("model/model_treasureBoxStorage.lua")

local ctlInitTreasureBoxRequest = class("ctlInitTreasureBoxRequest", ctlBase)

function ctlInitTreasureBoxRequest:name()
    return "ctlInitTreasureBoxRequest";
end

function ctlInitTreasureBoxRequest:onSendRequest()
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
    }

    local requestType = "InitTreasureBoxesRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "init.pb?_a=treasureBoxes&" .. self:snid(),
        self.name());

end

function ctlInitTreasureBoxRequest:onDecodePb(pb)
    local requestType = "InitTreasureBoxesResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlInitTreasureBoxRequest:onSuccess(data)
    model_treasureBoxStorage:initFromPb(data);
end


return ctlInitTreasureBoxRequest;
