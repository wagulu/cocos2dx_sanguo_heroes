_REQUIRE "cocos.cocos2d.json"
_REQUIRE("pb.pbCommon")

local helper_battle_star = _REQUIRE("model.helper.helper_battle_star");
local model_game = _REQUIRE("model/model_game");

local helper = {};

function helper:updateBattleResult(jsonResult)
    local result = json.decode(jsonResult);
    
    self:updateHeroData(result.myTeam.heros, model_user.battleMyTeam);
    self:updateHeroData(result.enemyTeam.heros, model_user.battleNpcTeam);
    self:updateSoldierData(result.myTeam.soldiers);
    self:updateSoldierData(result.enemyTeam.soldiers);
    
    if result.win then
        model_user.battleLastFightWin = pbCommon.FightResult.WIN;
        model_user.battleWinTimes = model_user.battleWinTimes + 1;
    elseif self:allFightHeroDead(result.myTeam.heros) and self:allFightHeroDead(result.enemyTeam.heros) then
        model_user.battleLastFightWin = pbCommon.FightResult.DRAW;
    else
        model_user.battleLastFightWin = pbCommon.FightResult.LOSE;
    end
    
    local nextScene = "";
    if self:allHeroDead(model_user.battleMyTeam) then 
        nextScene = "battle/DefeatScene";
    elseif self:allHeroDead(model_user.battleNpcTeam) then
        if not model_user.isPvp then
            self:evaluateFightStar();
        end
        nextScene = "battle/WinPopupScene";
    else
        nextScene = "battle/VersusScene";
    end
    
    local function onBattleBattleResult(success, data)
        if success then
            GLOBAL_MY_APP:removeDlgs()
            SWITSCENE(nextScene,data);
        end
    end
    
    local function onPvpBattleBattleResult(success, data)
        if success then
            SWITSCENE(nextScene,data,1);--第三个参数为1表示是pvp
        end
    end
    
    if model_user.isPvp then
        local rank = model_user.currentBattleRank;
        ctlHighladderBattleResultRequest:sendRequest(onPvpBattleBattleResult,
            rank, model_user.battleProgress, model_user.battleSelectMonsterId, model_user.battleSelectNpcId, model_user.battleLastFightWin);
    else
        local enemyIds = {}
        for i = 1, #(model_user.battleSelectNpcIdList) do
            for key, value in pairs(model_user.battleNpcTeam) do
                if value.id == model_user.battleSelectNpcIdList[i] then
                    table.insert(enemyIds, value.NPCId)
                end
            end
        end
    
        ctlBattleBattleResultRequest:sendRequest(onBattleBattleResult,
            model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
            model_user.battleLastFightStarInfo, model_user.battleLastFightWin,
            model_user.battleProgress, model_user.battleSelectMonsterIdList, enemyIds);
    end

    model_user.battleProgress = model_user.battleProgress + 1;
    --战斗结束将快速战斗设置为false
    if nextScene ~= "battle/VersusScene" then
--        model_user.isPvp = false
        model_game:setAutoQuickBattle(false)
    end
end

--function helper:updateHeroDataTest(heroResult, heroModel)
--    for i = 1, #(heroResult) do
--        local result = heroResult[i];
--
--        for j = 1, #(heroModel) do
--            local hero = heroModel[j];
--            if result.id == hero.id then
--                hero.HP = 0;
--                hero.MP = result.mp;
--            end
--        end
--    end
--end

function helper:updateHeroData(heroResult, heroModel)
    for i = 1, #(heroResult) do
        local result = heroResult[i];
        
        for j = 1, #(heroModel) do
            local hero = heroModel[j];
            if result.id == hero.id then
                hero.currentHP = result.hp;
                hero.currentMP = result.mp;
            end
        end
    end
end

function helper:updateSoldierData(soldierResult)
    --TODO, lrui, update soldier num after battle
end

function helper:allFightHeroDead(heroResult)
    for i = 1, #(heroResult) do
        local result = heroResult[i];
        if result.hp > 0 then
            return false
        end
    end
    return true;
end

function helper:allHeroDead(team)
    for key, value in pairs(team) do
        if(value.currentHP > 0) then
            return false;
        end
    end
    return true;
end

function helper:evaluateFightStar()
    local starInfoList = model_user.battleStage.starInfo;
    for i = 1, #(starInfoList) do
        local starInfo = starInfoList[i];
        if starInfo.status <= 0 then
            local allPassed = true;
            local conditionList = starInfo.conditions;
            for j = 1, #(conditionList) do
                local condition = conditionList[j].condition;
                -- method defines sign : method_name,[arg1],[arg2],...[argN]
                -- method defines sample: win,          >>>>>   helper_battle_star:win()
                -- method defines sample: hero_dead,1   >>>>>   helper_battle_star:hero_dead(1)
                local methodDefines = string.split(condition, ",");
                local methodPartsNum = table.maxn(methodDefines);
                if methodPartsNum > 0 then
                    local method = helper_battle_star[methodDefines[1]];
                    if method then
                        if not method(helper_battle_star, unpack(methodDefines, 2, methodPartsNum)) then
                            allPassed = false;
                        end
                    else allPassed = false;
                    end
                end
            end
            -- all conditions passed then you can get this star
            if allPassed then
                starInfo.status = 1;
            end 
        end
    end
    
    local finalStarInfoList = {};
    local finalStarNum = 0;
    for i = 1, #(starInfoList) do
        local starInfo = starInfoList[i];
        if starInfo.status > 0 then
            local finalStarInfo = {
                flag = starInfo.flag,
                status = starInfo.status
            };
            table.insert(finalStarInfoList, finalStarInfo);
            
            finalStarNum = finalStarNum + 1;
        end
    end
    
    if finalStarNum > 0 then
        model_user.battleLastFightStarInfo = finalStarInfoList;
    else
        model_user.battleLastFightStarInfo = nil;
    end
    model_user.battleLastFightStar = finalStarNum;
end

return helper;
