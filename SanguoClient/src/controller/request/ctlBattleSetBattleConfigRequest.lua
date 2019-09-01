
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local helper_pve = _REQUIRE("model/helper/helper_pve.lua")

local ctlBattleSetBattleConfigRequest = class("ctlBattleSetBattleConfigRequest", ctlBase)

function ctlBattleSetBattleConfigRequest:name()
    return "ctlBattleSetBattleConfigRequest";
end

function ctlBattleSetBattleConfigRequest:onSendRequest(_monsterId, _formationId, _position)
    if(_position == "") then
        _position = pbCommon.MonsterPosition.FRONT1;
    end
    
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
        formationId = _formationId,
        position = _position,
    }

    local requestType = "BattleSetBattleConfigRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "battle.pb?_a=setBattleConfig&" .. self:snid(),
        self.name());

end

function ctlBattleSetBattleConfigRequest:onDecodePb(pb)
    local requestType = "BattleSetBattleConfigResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlBattleSetBattleConfigRequest:onSuccess(data)
    local monster = data.monster;
    model_monsterStorage:updateMonster(monster);
end


return ctlBattleSetBattleConfigRequest;
