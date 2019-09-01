


_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");

local ctlMonstersStarUpRequest = class("ctlMonstersStarUpRequest", ctlBase)

function ctlMonstersStarUpRequest:name()
    return "ctlMonstersStarUpRequest";
end

function ctlMonstersStarUpRequest:onSendRequest(_monsterId)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        monsterId = _monsterId,

    }

    local requestType = "MonstersStarUpRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "monsters.pb?_a=starUp&" .. self:snid(),
        self:name());


end

function ctlMonstersStarUpRequest:onDecodePb(pb)
    local requestType = "MonstersStarUpResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
--    pbHelper.printPB(httpData);
    return httpData;
end


function ctlMonstersStarUpRequest:onSuccess(data)
    model_monsterStorage:updateMonster(data.monster);
    
    model_user.selectMonsterUpgradeResult = data;
end


return ctlMonstersStarUpRequest;

