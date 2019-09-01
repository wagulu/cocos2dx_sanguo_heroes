
local model_user = _REQUIRE("model/model_user")
_REQUIRE("pbCommon");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local delayCallback = _REQUIRE("common/delayCallback.lua")
local static_model_stage = _REQUIRE("model/static/model_stage.lua")
local model_area = _REQUIRE "model/static/model_area.lua"

local helper = {};

function helper:getIslandWithId(id)
    for key, value in pairs(model_user.islandsList) do
        if(value.id == id) then
            return value;
        end
    end
    return nil;
end

function helper:getAreaWithId(id)
    for key, value in pairs(model_user.areasList) do
        if(value.id == id) then
            return value;
        end
    end
    return nil;
end

function helper:getStageWithId(id)
    for key, value in pairs(model_user.stagesList) do
        if(value.id == id) then
            return value;
        end
    end
    return nil;
end

function helper:initStage(successCallback)
    local function onListIsland()
        model_areaStorage:fillBattleDataToUser(nill, nil, nil);
    
        if(#(model_user.islandsList) > 0) then
            local function onListArea()
                model_areaStorage:fillBattleDataToUser(model_user.selectIslandId, nil, nil);

                local type = model_user.selectStageType;
                if type==0 then
                    type=pbCommon.StageType.NORMAL;
                end
                
                if(#model_user.areasList > 0) then
                    local function onListStage()
                        model_areaStorage:fillBattleDataToUser(model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageType);

                        local data = nil;
                        successCallback(true, data);
                    end
                    local selectAreaId = model_user.selectAreaId
                    if selectAreaId == 0 then--第一次进关卡 selectAreaId 为最新的一关
                        selectAreaId = model_areaStorage:getCurrentAreaId(type)
                    else --不是第一进关卡 如果解锁了新关卡，那么就弹出提示进入下一章节
                        if self:isGotoNextStage() then
                            selectAreaId = model_areaStorage:getCurrentAreaId(type)
                            PopMsg.getInstance():flashShow(LANG("已开启关卡%s，精英关卡%s",model_area:getName(selectAreaId),model_area:getName(selectAreaId-1)))
                            
                        end
                    end
                    model_user.selectAreaId = selectAreaId;
                    model_user.selectStageType = type;

                    delayCallback:create(onListStage, 0.1);
                end
            end

            model_user.selectIslandId = model_user.islandsList[1].islandId;
            
            delayCallback:create(onListArea, 0.1);
        end
    end

    delayCallback:create(onListIsland, 0.1);
--    ctlAdventureListIslandRequest:sendRequest(onListIsland);
end


function helper:initStageById(isLandId,areaId,stageId,stageType,successCallback)
    local function onListIsland()
        model_user.selectIslandId = isLandId
        model_user.selectAreaId = areaId
        model_user.selectStageType =  stageType
        model_user.selectStageId = stageId
        
        GLOBAL_MY_APP.selectIslandId = isLandId
        GLOBAL_MY_APP.selectAreaId = areaId
        GLOBAL_MY_APP.selectStageType =  stageType
        GLOBAL_MY_APP.selectStageId = stageId
        
        model_areaStorage:fillBattleDataToUser(model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageType);
        local data = nil;
        successCallback(true, data);
    end

    delayCallback:create(onListIsland, 0.1);
end



function helper:isGotoNextStage() --是否跳转到下一章节条件是当前点打过了，并且打之前没打过，而且当前点是最后一个点 并且是普通关卡
    local stages = model_user.stagesList
    return stages[#stages].status > 2  and model_user.selectStageId == #stages and model_user.selectStageStatus <= 2 and model_user.selectStageType == pbCommon.StageType.NORMAL
end

function helper:confirmStage(stageId, successCallback)
    model_user.selectHeroPageIndex=nil    
    model_user.selectMonsterPageIndex=nil 
    model_user.selectStageId = stageId;
    model_user.selectStageStatus = self:getStageWithId(stageId).status
    local model = model_areaStorage:getStage(stageId,model_user.selectStageType)
    model_user.selectStageBattleMode = model.battleMode
    if model_user.selectStageType == pbCommon.StageType.NORMAL then
        local openLevel = static_model_stage:getOpenLevel(model_user.selectStageId)
        if model_user.level < openLevel then
            PopMsg.getInstance():flashShow(LANG("等级不足，暂时无法挑战.\n（请在等级达到%s后尝试）",openLevel))
            return
        end
    end
    local function onStageConfirm(success, data, code, msg)
        if success then
            self:initBattleStage();
        end
        if successCallback then
            successCallback(success, data, code, msg);
        end
    end
    
    ctlAdventureStageConfirmRequest:sendRequest(onStageConfirm,
        model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType);
end

-- battle

function helper:initBattleStage()

    model_user.battleMyTeam = self:createBattleMyTeamFromBattleTeam(model_user.battleTeam);
--    model_user.battleMyTeam = data.battleTeam;

--    model_user.battleNpcTeam = data.NPC;
    model_user.battleProgress = 1;
    model_user.battleWinTimes = 0;
    model_user.battleSelectMonsterId = 0;
    model_user.battleSelectNpcId = 0;
    model_user.battleSelectMonsterIdList = {};
    model_user.battleSelectNpcIdList = {};
    model_user.battleLastFightWin = pbCommon.FightResult.LOSE;
    model_user.battleLastFightStar = 0;
    model_user.battleLastFightStarInfo = nil;
end

function helper:getConfirmStageCostEnergy()
    return model_user.selectStageConfirmData.userStage.energyBefore + model_user.selectStageConfirmData.userStage.energyAfter;
end

--team

function helper:createBattleMyTeamFromBattleTeam(battleTeam)
    local battleMyTeam = {};
    
    for key, value in pairs(battleTeam) do
        local monster = model_monsterStorage:getMonster(value.monsterId);
        if(value.isUse > 0) then
            battleMyTeam[#(battleMyTeam) + 1] = monster;
        end
    end

    for i=1,#battleMyTeam do
        battleMyTeam[i].currentHP = battleMyTeam[i].HP;
        battleMyTeam[i].currentMP = battleMyTeam[i].MP;
    end

    return battleMyTeam;
end


-- mopping times

function helper:canResetAvailableMoppingTimes()
    if(model_user.moppingTicket > 0) then
        return false;
    end
end

function helper:resetAvailableMoppingTimes(successCallback)
    local function onResetTimes(success, data, code, msg)
        if success then
        end
        if successCallback then
            successCallback(success, data, code, msg);
        end
    end
    
    ctlBattleBuyTimesRequest:sendRequest(onResetTimes,
        model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId);
end

function helper:getResetMoppingTimesIconCost()
    return static_model_eliteStageBattleTimesPrice:getCash(model_user.selectEliteStageDailyResetTimes+1);
end

function helper:getAvailableMoppingTimes()
    local stageCostEnergy = self:getConfirmStageCostEnergy();
    local times = math.floor(model_user.energy / stageCostEnergy);
    times = math.min(times, model_user.moppingTicket);

    if(model_user.selectStageType == pbCommon.StageType.MASTER) then
        times = math.min(times, model_user.selectEliteStageLeftBattleTimes);
    end

    return times;
end


function helper:subMoppingTimes(times)
    model_user.moppingTicket = model_user.moppingTicket - times;
    if(model_user.moppingTicket < 0) then
        model_user.moppingTicket = 0;
        release_print("ERROR, model_user.moppingTicket < 0");
    end
    
    if(model_user.selectStageType == pbCommon.StageType.MASTER) then
        model_user.selectEliteStageLeftBattleTimes = model_user.selectEliteStageLeftBattleTimes - times;
        if(model_user.selectEliteStageLeftBattleTimes < 0) then
            model_user.selectEliteStageLeftBattleTimes = 0;
            release_print("ERROR, model_user.selectEliteStageLeftBattleTimes < 0");
        end
    end

end

-- open area treasures box

function helper:isCanOpenSelectAreaTreasuresBox(index)
    if(model_user.stageStarTreasuresList[index].isOpen) then
        return false;
    end
    
    if(model_user.stageTotalStar < model_user.stageStarTreasuresList[index].star) then
        return false;
    end
    
    return true;
end

function helper:openSelectAreaTreasuresBox(index, successCallback)
    local function onOpenSelectAreaTreasuresBox(success, data, code, msg)
        if(success) then
            model_user.stageStarTreasuresList[index].isOpen = true; --hope it will not crash
        end
        
        if successCallback then
            successCallback(success, data, code, msg);
        end
    end

    local boxId = self:getAreaTreasuresBoxId(index);
    ctlAdventureOpenStarTreasureRequest:sendRequest(onOpenSelectAreaTreasuresBox,
        model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageType,
        boxId);
end

function helper:getAreaTreasuresBoxId(index)
    return model_user.stageStarTreasuresList[index].boxId;
end

return helper;
