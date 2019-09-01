
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model/model_user")

local ctlHighladderGetEnemyRequest = class("ctlHighladderGetEnemyRequest", ctlBase)

function ctlHighladderGetEnemyRequest:name()
    return "ctlHighladderGetEnemyRequest";
end

function ctlHighladderGetEnemyRequest:onSendRequest(_rank)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        rank = _rank,

    }

    local requestType = "HighladderGetEnemyRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)
--    print_data_dump(httpData);

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "highladder.pb?_a=getEnemy&" .. self:snid(),
        self.name());


end

function ctlHighladderGetEnemyRequest:onDecodePb(pb)
    local requestType = "HighladderGetEnemyResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
    --    pbHelper.printPB(httpData);
    return httpData;
end

function ctlHighladderGetEnemyRequest:onSuccess(data)
--need update monsters
--    model_user.pvpOpponents = data.monsters;

    model_user.battleTeam = data.battleTeam;
    model_user.battleNpcTeam = data.monsters;

    --    for i=1,#model_user.selectStageConfirmData.battleTeam do
    --        model_user.selectStageConfirmData.battleTeam[i].currentHP = model_user.selectStageConfirmData.battleTeam[i].HP
    --        model_user.selectStageConfirmData.battleTeam[i].currentMP = model_user.selectStageConfirmData.battleTeam[i].MP
    --    end

    for i=1,#data.monsters do
        data.monsters[i].currentHP = data.monsters[i].HP
        data.monsters[i].currentMP = data.monsters[i].MP
    end
    
end


return ctlHighladderGetEnemyRequest;


