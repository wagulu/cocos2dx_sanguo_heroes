
_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_user = _REQUIRE("model.model_user")

local ctlAdventureStageConfirmRequest = class("ctlAdventureStageConfirmRequest", ctlBase)

function ctlAdventureStageConfirmRequest:name()
    return "ctlAdventureStageConfirmRequest";
end

function ctlAdventureStageConfirmRequest:onSendRequest(_islandId, _areaId, _stageId, _stageType)
    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,
        areaId = _areaId,
        stageId = _stageId,
        stageType = _stageType,

    }

    local requestType = "AdventureStageConfirmRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "adventure.pb?_a=stageConfirm&" .. self:snid(),
        self.name());


end

function ctlAdventureStageConfirmRequest:onDecodePb(pb)
        local requestType = "AdventureStageConfirmResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
--    pbHelper.printPB(httpData);
    return httpData;
end
--function ctlAdventureStageConfirmRequest:testData()
--    local data ={
--    userStage={["coin"] = 36,["energyAfter"] = 6,["energyBefore"] = 0,["exp"] = 12,["starInfo"] = sequence[3]},
--    npc={{["ArPR"] = 1,["DR"] = 0,["ERHPBase"] = 0,["ERKillBase"] = 0,["HP"] = 11210000,["MP"] = 790000,["abilityCD"] = 0,["abilityDodgeRate"] = 0,["abilityHitRate"] = 1,["abilityVamp"] = 0,["arrowSpeed"] = 0,["atkAntiCSR"] = 0.1,["atkDodgeRate"] = 0.05,["atkVamp"] = 0,["atkVampRate"] = 0,["attack"] = 10000,["damageType"] = 1,["healingBonus"] = 0,["holyDamage"] = 0,["id"] = 1,["level"] = 14,["physicalCSDR"] = 1,["physicalDR"] = 0.0104,["physicalResilience"] = 0,["soldier"] = table[37],["speed"] = 251,["spellCSDR"] = 1,["spellCostReduction"] = 0,["spellDR"] = 0.0172,["spellResilience"] = 0,["strikeCD"] = 0.9,["ERLoseBase"] = 0,["SRPR"] = 1,["abilityAntiCSR"] = 0,["abilityCSR"] = 0,["abilityVampRate"] = 0,["atkCSR"] = 0.1,["atkHitRate"] = 0.95,["mark"] = "caocao",["maxSoldier"] = 80,["position"] = "f1",["power"] = 10000,["skills"] = {}}}},
--    battleTeam={{["DR"] = 0,["ERHPBase"] = 0,["ERKillBase"] = 0,["HP"] = 1120,["MP"] = 78,["abilityDodgeRate"] = 0,["abilityHitRate"] = 1,["abilityVamp"] = 0,["abilityVampRate"] = 0,["atkAntiCSR"] = 0.1,["atkDodgeRate"] = 0.05,["atkVamp"] = 0,["atkVampRate"] = 0,["attack"] = 110,["damageType"] = 1,["exp"] = 0,["formations"] = sequence[3],["healingBonus"] = 0,["holyDamage"] = 0,["id"] = 1,["level"] = 1,["physicalCSDR"] = 1,["physicalDR"] = 0.0104,["physicalResilience"] = 0,["soldier"] = table[37],["speed"] = 250,["spellCSDR"] = 1,["spellCostReduction"] = 0,["spellDR"] = 0.0172,["spellResilience"] = 0,["strikeCD"] = 0.9,["ArPR"] = 1,["ERLoseBase"] = 0,["SRPR"] = 1,["abilityAntiCSR"] = 0,["abilityCD"] = 0,["abilityCSR"] = 0,["arrowSpeed"] = 0,["atkCSR"] = 0.1,["atkHitRate"] = 0.95,["mark"] = "caocao",["maxSoldier"] = 15,["position"] = "f1",["power"] = 184,["skills"] = {}}}
--    }
--end


function ctlAdventureStageConfirmRequest:onSuccess(data)
    model_user.selectStageConfirmData = data;
    model_user.battleStage = data.userStage;

    model_user.battleTeam = data.battleTeam;
    model_user.battleNpcTeam = data.NPC;

--    for i=1,#model_user.selectStageConfirmData.battleTeam do
--        model_user.selectStageConfirmData.battleTeam[i].currentHP = model_user.selectStageConfirmData.battleTeam[i].HP
--        model_user.selectStageConfirmData.battleTeam[i].currentMP = model_user.selectStageConfirmData.battleTeam[i].MP
--    end

    for i=1,#model_user.selectStageConfirmData.NPC do
        data.NPC[i].currentHP = data.NPC[i].HP
        data.NPC[i].currentMP = data.NPC[i].MP
    end

    model_user.selectEliteStageLeftBattleTimes = data.userStage.battleTimes;
    model_user.selectEliteStageDailyResetTimes = data.userStage.dailyResetTimes;
    model_user.moppingTicket = data.userStage.moppingTicket;
end

return ctlAdventureStageConfirmRequest;


