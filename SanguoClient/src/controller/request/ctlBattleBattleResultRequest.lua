

_REQUIRE("pbHelper")
_REQUIRE("pb.pbCommon")

local helper_drop = _REQUIRE("model/helper/helper_drop.lua")
local ctlBase = _REQUIRE("controller.request.ctlBase")
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

local ctlBattleBattleResultRequest = class("ctlBattleBattleResultRequest", ctlBase)

function ctlBattleBattleResultRequest:name()
    return "ctlBattleBattleResultRequest";
end

function ctlBattleBattleResultRequest:onSendRequest(_islandId, _areaId, _stageId, _stageType, _battleStar,
    _wonBattle, _progress, _monsterIds, _NPCIds)

    local pbInData = {
        api = pbCommon.struct_apiRequest(),
        islandId = _islandId,
        areaId = _areaId,
        stageId = _stageId,
        stageType = _stageType,
        wonBattle = _wonBattle,
        progress = _progress,
        monsterIds = _monsterIds,
        NPCIds = _NPCIds,
    }
    
    if _battleStar then
        pbInData.battleStar = _battleStar;
--        model_areaStorage:updateStage(_stageId, _stageType, _battleStar);
    end

    local requestType = "BattleBattleResultRequest";
    local httpData = pbHelper.getEncodePB(requestType, pbInData)

    _REQUIRE("SanGuoLib").sendHttp(
        httpData,
        string.len(httpData),
        self:urlBase() .. "battle.pb?_a=battleResult&" .. self:snid(),
        self.name());


end

function ctlBattleBattleResultRequest:onDecodePb(pb)
        local requestType = "BattleBattleResultResponse";
    local httpData = pbHelper.getDecodePB(requestType, pb);
--    pbHelper.printPB(httpData);
    return httpData;
end


function ctlBattleBattleResultRequest:onSuccess(data)
    for key, monster in pairs(data.monsters) do
        model_monsterStorage:updateMonster(monster);
    end

    model_areaStorage:updateWithBattleResult(data);

    for key, value in pairs(data.treasures) do
        helper_drop:getDrop(value);
    end
end

function ctlBattleBattleResultRequest:onFail()
    cclog("战斗结果请求失败")

    if not(INTEGRATION_TEST) then
        GLOBAL_MY_APP:showDlg("common/CommonPopScene",{},{confirmStr=LANG("确定"),isSigleBtn=true,text=LANG("战斗结果请求失败,请重新登陆"),
            fun=function(data)
                if data.cmd=="confirm" then
                    SWITSCENE("login/LoginScene")
                end
            end 
        })
    end
end



return ctlBattleBattleResultRequest;

