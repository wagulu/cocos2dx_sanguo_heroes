
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")

local ctlMonstersMergeRequest = class("ctlMonstersMergeRequest", ctlBase)

function ctlMonstersMergeRequest:name()
    return "ctlMonstersMergeRequest";
end

function ctlMonstersMergeRequest:onSendRequest(_monsterId)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,
    }

    local requestType = "MonstersMergeRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=merge&" .. self:snid(),
        self.name());

end

function ctlMonstersMergeRequest:onDecodePb(pb)
    local requestType = "MonstersMergeResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlMonstersMergeRequest:onSuccess(data)
    model_monsterStorage:updateMergeMonster(data.monster);
end


return ctlMonstersMergeRequest;
