


_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_treasureBoxStorage = _REQUIRE("model/model_treasureBoxStorage.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua");

local ctlPlayerUseTreasureBoxRequest = class("ctlPlayerUseTreasureBoxRequest", ctlBase)


function ctlPlayerUseTreasureBoxRequest:name()
    return "ctlPlayerUseTreasureBoxRequest";
end

function ctlPlayerUseTreasureBoxRequest:onSendRequest(_groupId, _count)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        boxId = _groupId,
        count = _count,

    }

    local requestType = "PlayerUseTreasureBoxRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "player.pb?_a=useTreasureBox&" .. self:snid(),
        self.name());


end

function ctlPlayerUseTreasureBoxRequest:onDecodePb(pb)
    local requestType = "PlayerUseTreasureBoxResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlPlayerUseTreasureBoxRequest:onSuccess(data)
    for key, value in pairs(data.prizes) do
        helper_drop:getDrop(value);
    end
    
    for key, value in pairs(data.treasureBoxes) do
        model_treasureBoxStorage:updateItem(value);
    end
    
--    model_monsterStorage:initFromPb(data);
end


return ctlPlayerUseTreasureBoxRequest;


