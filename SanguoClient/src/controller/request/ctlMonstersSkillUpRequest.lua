
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local ctlMonstersSkillUpRequest = class("ctlMonstersSkillUpRequest", ctlBase)

function ctlMonstersSkillUpRequest:name()
    return "ctlMonstersSkillUpRequest";
end

function ctlMonstersSkillUpRequest:onSendRequest(_monsterId, _skillId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
        skillId = _skillId,
    }

    local requestType = "MonstersSkillUpRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=skillUp&" .. self:snid(),
        self.name());

end

function ctlMonstersSkillUpRequest:onDecodePb(pb)
    local requestType = "MonstersSkillUpResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersSkillUpRequest:onSuccess(data)
    model_monsterStorage:updateMonster(data.monster);
end


return ctlMonstersSkillUpRequest;
