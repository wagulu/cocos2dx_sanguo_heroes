
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local ctlMonstersChangeFormationRequest = class("ctlMonstersChangeFormationRequest", ctlBase)

function ctlMonstersChangeFormationRequest:name()
    return "ctlMonstersChangeFormationRequest";
end

function ctlMonstersChangeFormationRequest:onSendRequest(_monsterId, _slotId, _formationId)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
        slotId = _slotId,
        formationId = _formationId,

    }

    local requestType = "MonstersChangeFormationRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=changeFormation&" .. self:snid(),
        self.name());


end

function ctlMonstersChangeFormationRequest:onDecodePb(pb)
    local requestType = "MonstersChangeFormationResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersChangeFormationRequest:onSuccess(data)
    model_monsterStorage:updateMonster(data.monster);
end


return ctlMonstersChangeFormationRequest;


