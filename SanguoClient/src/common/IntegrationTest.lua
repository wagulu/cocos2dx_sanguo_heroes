
local model_user = _REQUIRE("model.model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_treasureBoxStorage = _REQUIRE("model/model_treasureBoxStorage.lua")
local model_mailStorage = _REQUIRE("model/model_mailStorage.lua")

local helper_pve = _REQUIRE("model/helper/helper_pve.lua")
local helper_pvp = _REQUIRE("model/helper/helper_pvp.lua")
local helper_battle = _REQUIRE("model/helper/helper_battle.lua")
local helper_treasure = _REQUIRE("model/helper/helper_treasure.lua")
local helper_shoplist_item = _REQUIRE("model/helper/helper_shoplist_item.lua")
local helper_monster = _REQUIRE("model/helper/helper_monster.lua")

_REQUIRE("pb.pbCommon")
_REQUIRE("controller/ctlGlobal")
local initRequestHelper = _REQUIRE("controller/helper/initRequestHelper.lua")

local delayCallback = _REQUIRE("common/delayCallback.lua")


-----------------------------------------------------------
--environment

local TEST_TIMES = 1;
local TEST_TIMES_MAX = 10;

local startIntegrationTest;
local endIntegrationTest;

-----------------------------------------------------------
--test star treasure box


local function onOpenStarTreasureRequest(success, data, code, msg)
    if(success) then
        CTL_INFO("star treasure box complete");
    else
        CTL_WARNING(msg);
    end

    endIntegrationTest();
    
end
local function openStarTreasureRequest()
    helper_pve:openSelectAreaTreasuresBox(1, onOpenStarTreasureRequest);
end



-----------------------------------------------------------
--test pvp

local function onHighladderGetOthersMonsters(success, data, code, msg)
    if(success) then
        CTL_INFO("high ladder complete");
        openStarTreasureRequest();
    else
        CTL_WARNING(msg);
    end

end
local function highladderGetOthersMonsters()
    ctlHighladderGetOthersMonstersRequest:sendRequest(onHighladderGetOthersMonsters, 1);
end


local function onHighladderGetRankingReward(success, data, code, msg)
    if(success) then
        highladderGetOthersMonsters();
    else
        CTL_WARNING(msg);
    end

end
local function highladderGetRankingReward()
    ctlHighladderGetRankingRewardRequest:sendRequest(onHighladderGetRankingReward);
end


local function onHighladderGetRanking(success, data, code, msg)
    if(success) then
        highladderGetRankingReward();
    else
        CTL_WARNING(msg);
    end

end
local function highladderGetRanking()
    ctlHighladderGetRankingRequest:sendRequest(onHighladderGetRanking);
end


local function onHighladderDraw(success, data, code, msg)
    if(success) then
        highladderGetRanking();
    else
        CTL_WARNING(msg);
    end

end
local function highladderDraw()
    ctlHighladderDrawRequest:sendRequest(onHighladderDraw);
end



local pvpBattleBattleResult;
local pvpBattleExecBattle;

local function onPvpBattleBattleResult(success, data, code, msg)
    if(success) then
        if(model_user.battleWinTimes < #(model_user.battleNpcTeam)) then
            pvpBattleExecBattle();
        else
            --over
            CTL_INFO("pvp battle complete");
            highladderDraw();
        end
    else
        CTL_WARNING(msg);
    end
end
pvpBattleBattleResult = function ()
    local rank = model_user.pvpRanks[1].rank;
    ctlHighladderBattleResultRequest:sendRequest(onPvpBattleBattleResult,
        rank, model_user.battleProgress, model_user.battleSelectMonsterId, model_user.battleSelectNpcId, model_user.battleLastFightWin);
    model_user.battleProgress = model_user.battleProgress + 1;
end


local function onPvpBattleExecBattle(success, data, code, msg)
    if(success) then
        model_user.battleLastFightWin = pbCommon.FightResult.WIN;
        model_user.battleWinTimes = model_user.battleWinTimes + 1;
        pvpBattleBattleResult();
    else
        CTL_WARNING(msg);
    end
end
pvpBattleExecBattle = function()
    local rank = model_user.pvpRanks[1].rank;
    model_user.battleSelectNpcId = model_user.battleNpcTeam[model_user.battleWinTimes + 1].id;
    ctlHighladderExecBattleRequest:sendRequest(onPvpBattleExecBattle,
        rank, model_user.battleProgress, model_user.battleSelectMonsterId, model_user.battleSelectNpcId);
end




local function onHighladderGetEnemy(success, data, code, msg)
    if(success) then
        model_user.battleSelectMonsterId = model_user.battleTeam[1].monsterId; --data.battleTeam[1].id;
    
        pvpBattleExecBattle();
    else
        CTL_WARNING(msg);
    end

end
local function highladderGetEnemy()
    local rank = model_user.pvpRanks[1].rank;
    helper_pvp:getEnemy(rank, onHighladderGetEnemy);
end


local function onHighladderGetList(success, data, code, msg)
    if(success) then
        highladderGetEnemy();
    else
        CTL_WARNING(msg);
    end

end
local function highladderGetList()
    ctlHighladderGetListRequest:sendRequest(onHighladderGetList);
end


-----------------------------------------------------------
--test pvp shop


local function onHighladderBuyItem(success, data, code, msg)
    if(success) then
        CTL_INFO("highladder shop complete");
        highladderGetList();
--        openStarTreasureRequest();
    else
        CTL_WARNING(msg);
        highladderGetList();
--        openStarTreasureRequest();
    end

end
local function highladderBuyItem()
    ctlHighladderBuyItemRequest:sendRequest(onHighladderBuyItem, 1);
end


local function onHighladderRefreshShop(success, data, code, msg)
    if(success) then
        CTL_INFO("highladder refresh shop complete");
        highladderBuyItem();
    else
        CTL_WARNING(msg);
        highladderBuyItem();
    end

end
local function highladderRefreshShop()
    ctlHighladderRefreshShopRequest:sendRequest(onHighladderRefreshShop);
end


local function onHighladderGetShop(success, data, code, msg)
    if(success) then
        CTL_INFO("highladder get shop complete");
        highladderRefreshShop();
    else
        CTL_WARNING(msg);
        highladderRefreshShop();
    end

end
local function highladderGetShop()
    ctlHighladderGetShopRequest:sendRequest(onHighladderGetShop);
end



-----------------------------------------------------------
--test pve


local function onBuyMoppingTimes(success, data, code, msg)
    if(success) then
        CTL_INFO("stage battle mopping complete");
        highladderGetShop();
    else
        CTL_WARNING(msg);
        highladderGetShop();
    end
end
local function buyMoppingTimes()
    helper_pve:resetAvailableMoppingTimes(onBuyMoppingTimes);
end



local function onBattleMopping(success, data, code, msg)
    if(success) then
        buyMoppingTimes();
    else
        CTL_WARNING(msg);
        buyMoppingTimes();
    end
end
local function battleMopping()
    ctlBattleMoppingRequest:sendRequest(onBattleMopping,
        model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
        helper_pve:getAvailableMoppingTimes());
end




local battleBattleResult;
local battleExecBattle;

local function onBattleBattleResult(success, data, code, msg)
    if(success) then
        if(model_user.battleWinTimes < #(model_user.battleNpcTeam)) then
            battleExecBattle();
        else
        --over
            CTL_INFO("stage battle complete");
            battleMopping();
        end
    else
        CTL_WARNING(msg);
    end
end
battleBattleResult = function ()
    ctlBattleBattleResultRequest:sendRequest(onBattleBattleResult,
        model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
        model_user.battleLastFightStarInfo, model_user.battleLastFightWin,
        model_user.battleProgress, {model_user.battleSelectMonsterId}, {model_user.battleSelectNpcId});
    model_user.battleProgress = model_user.battleProgress + 1;
end


local function onBattleExecBattle(success, data, code, msg)
    if(success) then
        model_user.battleLastFightWin = pbCommon.FightResult.WIN;
        model_user.battleWinTimes = model_user.battleWinTimes + 1;
        model_user.battleLastFightStar = 1;
        if(model_user.battleWinTimes >= #(model_user.battleNpcTeam)) then
            model_user.battleLastFightStarInfo = {
                {flag = 1, status = 1},
                {flag = 2, status = 1},
                {flag = 3, status = 1},
            };
        else
            model_user.battleLastFightStarInfo = nil;
        end
        battleBattleResult();
    else
        CTL_WARNING(msg);
    end
end
battleExecBattle = function()
    model_user.battleSelectNpcId = model_user.battleNpcTeam[model_user.battleWinTimes + 1].id;
    ctlBattleExecBattleRequest:sendRequest(onBattleExecBattle,
        model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
        model_user.battleProgress, {model_user.battleSelectMonsterId}, {model_user.battleSelectNpcId});
end


local function onSetBattleConfig(success, data, code, msg)
    if(success) then
        battleExecBattle();
    else
        CTL_WARNING(msg);
    end
end
local function setBattleConfig()
    ctlBattleSetBattleConfigRequest:sendRequest(onSetBattleConfig,
        model_user.battleSelectMonsterId, helper_battle:getActiveFormationId(), helper_battle:getActivePosition());
end


local function onStageConfirm(success, data, code, msg)
    if(success) then
        model_user.battleSelectMonsterId = model_user.battleTeam[1].monsterId; --data.battleTeam[1].id;
        setBattleConfig();
    else
        CTL_WARNING(msg);
    end
end
local function stageConfirm()
    if(#(model_user.stagesList) > 0) then
        model_user.selectStageId = model_user.stagesList[1].id;
        helper_pve:confirmStage(model_user.selectStageId, onStageConfirm);
    end
end


local function onListStage(success, data, code, msg)
    if(success) then
        stageConfirm();
    else
        CTL_WARNING(msg);
    end
end
local function listStage()
    if(#(model_user.areasList) > 0) then
        model_user.selectAreaId = model_user.areasList[1].areaId;
        model_user.selectStageType = pbCommon.StageType.NORMAL;
        ctlAdventureListStageRequest:sendRequest(onListStage, model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageType);
    end
end


local function onListArea(success, data, code, msg)
    if(success) then
        listStage();
    else
        CTL_WARNING(msg);
    end
end
local function listArea()
    if(#(model_user.islandsList) > 0) then
        model_user.selectIslandId = model_user.islandsList[1].islandId;
        ctlAdventureListAreaRequest:sendRequest(onListArea, model_user.selectIslandId);
    end
end


local function onListIsland(success, data, code, msg)
    if(success) then
        listArea();
    else
        CTL_WARNING(msg);
    end
end
local function listIsland()
    ctlAdventureListIslandRequest:sendRequest(onListIsland);
end


-----------------------------------------------------------
--test monster level up

local function onMonstersEquipmentMaterialMerge(success, data, code, msg)
    if(success) then
        CTL_INFO("level up complete");
        listIsland();
    else
        CTL_WARNING(msg);
        listIsland();
    end
end
local function monstersEquipmentMaterialMerge()
    model_user.selectEquipmentMaterialMergeId = 1;
    ctlMonstersEquipmentMaterialMergeRequest:sendRequest(onMonstersEquipmentMaterialMerge, model_user.selectEquipmentMaterialMergeId);
end



local function onMonstersQualityUp(success, data, code, msg)
    if(success) then
        monstersEquipmentMaterialMerge();
    else
        CTL_WARNING(msg);
        monstersEquipmentMaterialMerge();
    end
end
local function monstersQualitUp()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    model_user.selectMonsterUpgradeMonsterId = firstMonsterId;
    ctlMonstersQualityUpRequest:sendRequest(onMonstersQualityUp, model_user.selectMonsterUpgradeMonsterId);
end



local function onMonstersStarUp(success, data, code, msg)
    if(success) then
        monstersQualitUp();
    else
        CTL_WARNING(msg);
        monstersQualitUp();
    end
end
local function monstersStarUp()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    model_user.selectMonsterUpgradeMonsterId = firstMonsterId;
    ctlMonstersStarUpRequest:sendRequest(onMonstersStarUp, model_user.selectMonsterUpgradeMonsterId);
end


local function onMonstersEquipmentQualityUp(success, data, code, msg)
    if(success) then
        monstersStarUp();
    else
        CTL_WARNING(msg);
        monstersStarUp();
    end
end
local function monstersEquipmentQualityUp()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    model_user.selectEquipmentUpMonsterId = firstMonsterId;
    model_user.selectEquipmentUpWeaponId = 630110;
    ctlMonstersEquipmentQualityUpRequest:sendRequest(onMonstersEquipmentQualityUp,
        model_user.selectEquipmentUpgradeMonsterId, model_user.selectEquipmentUpgradeWeaponId);
end



local function onMonstersEquipmentLevelUp(success, data, code, msg)
    if(success) then
        monstersEquipmentQualityUp();
    else
        CTL_WARNING(msg);
        monstersEquipmentQualityUp();
    end
end
local function monstersEquipmentLevelUp()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    model_user.selectEquipmentUpgradeMonsterId = firstMonsterId;
    model_user.selectEquipmentUpgradeWeaponId = 630110;
    model_user.selectEquipmentUpgradeTimes = 1;
    ctlMonstersEquipmentLevelUpRequest:sendRequest(onMonstersEquipmentLevelUp,
        model_user.selectEquipmentUpgradeMonsterId, model_user.selectEquipmentUpgradeWeaponId, model_user.selectEquipmentUpgradeTimes);
end


local function onMonstersTreasureItemChange(success, data, code, msg)
    if(success) then
        monstersEquipmentQualityUp();
    else
        CTL_WARNING(msg);
        monstersEquipmentQualityUp();
    end
end
local function monstersTreasureItemChange()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    local slot = 1;
    local itemId = 1;
    ctlMonstersChangeTreasureItemRequest:sendRequest(onMonstersTreasureItemChange, firstMonsterId, slot, itemId);
end


local function onMonstersTreasureItemMerge(success, data, code, msg)
    if(success) then
        monstersTreasureItemChange();
    else
        CTL_WARNING(msg);
        monstersTreasureItemChange();
    end
end
local function monstersTreasureItemMerge()
    ctlMonstersTreasureItemMergeRequest:sendRequest(onMonstersTreasureItemMerge, 1);
end


local function onMonstersTreasureItemQualityUp(success, data, code, msg)
    if(success) then
        monstersTreasureItemMerge();
    else
        CTL_WARNING(msg);
        monstersTreasureItemMerge();
    end
end
local function monstersTreasureItemQualityUp()
    ctlMonstersTreasureItemQualityUpRequest:sendRequest(onMonstersTreasureItemQualityUp, 1);
end

local function onMonstersTreasureItemLevelUp(success, data, code, msg)
    if(success) then
        monstersTreasureItemQualityUp();
    else
        CTL_WARNING(msg);
        monstersTreasureItemQualityUp();
    end
end
local function monstersTreasureItemLevelUp()
    model_user.selectTreasureItemUpgradeTargetInventoryId = 1;
    model_user.selectTreasureItemUpgradeConsumeId = 1;
    model_user.selectTreasureItemUpgradeConsumeCount = 1;
    helper_treasure:levelUp(onMonstersTreasureItemLevelUp);
end



--local function onMonsterInfo(success, data, code, msg)
--    if(success) then
--        CTL_INFO("get monster info");
--        monstersEquipmentLevelUp();
--    else
--        CTL_WARNING(msg);
--    end
--end
--local function monsterInfo()
--    local haveMonsters = model_monsterStorage:getAllHaveMonsters();
--    ctlMonstersInfoRequest:sendRequest(onMonsterInfo, haveMonsters[1].id);
--end
--
--
--local function onListMonsters(success, data, code, msg)
--    if(success) then
--        monsterInfo();
--    else
--        CTL_WARNING(msg);
--    end
--end
--local function listMonsters()
--    ctlMonstersListRequest:sendRequest(onListMonsters);
--end

local function onMonsterSkillUp(success, data, code, msg)
    if(success) then
        CTL_INFO("monster skill up");
        monstersTreasureItemLevelUp();
    else
        CTL_WARNING(msg);
        monstersTreasureItemLevelUp();
    end
end
local function monsterSkillUp()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    local skill = haveMonsters[1].skills[1];
    ctlMonstersSkillUpRequest:sendRequest(onMonsterSkillUp, firstMonsterId, skill.id);
end

local function onMonsterMerge(success, data, code, msg)
    if(success) then
        CTL_INFO("merge monster");
        monsterSkillUp();
    else
        CTL_WARNING(msg);
        monsterSkillUp();
    end
end
local function monsterMerge()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    ctlMonstersMergeRequest:sendRequest(onMonsterMerge, firstMonsterId);
end

local function onMonsterBreakthrough(success, data, code, msg)
    if(success) then
        CTL_INFO("breakthrough monster");
        onMonsterMerge();
    else
        CTL_WARNING(msg);
        onMonsterMerge();
    end
end
local function monsterBreakthrough()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    if not(helper_monster:canBreakthrough(firstMonsterId)) then
        onMonsterBreakthrough(false, nil, 0, "");
    else
        helper_monster:tryBreakthrough(firstMonsterId, onMonsterBreakthrough);
    end
end



-----------------------------------------------------------
--test player formation

local function onOpenMail(success, data, code, msg)
    if(success) then
        CTL_INFO("open mail");
        monsterBreakthrough();
    else
        CTL_WARNING(msg);
        monsterBreakthrough();
    end
end

local function openMail()
    local channel = DEPOSIT_CHANNEL.SYS;
    local mails = model_mailStorage:collectMail(channel);
    if(#(mails) > 0) then
        local firstMail = mails[1];
        local key = firstMail.key;
        ctlDepositReceiveRequest:sendRequest(onOpenMail, key, channel);
    else
        onOpenMail(false, nil, 0, "");
    end
end


local function onGetMailList(success, data, code, msg)
    if(success) then
        CTL_INFO("get mail list");
        openMail();
    else
        CTL_WARNING(msg);
        openMail();
    end
end

local function getMailList()
    ctlDepositGetListRequest:sendRequest(onGetMailList);
end


-----------------------------------------------------------
--test player formation


local function onModifyBattleTeam(success, data, code, msg)
    if(success) then
        CTL_INFO("modify battle team complete");
        getMailList();
    else
        CTL_WARNING(msg);
        getMailList();
    end
end
local function modifyBattleTeam()
    ctlPlayerModifyBattleMonsterRequest:sendRequest(onModifyBattleTeam);
end


local function onChangeBattleTeam(success, data, code, msg)
    if(success) then
        CTL_INFO("change battle team complete");
        modifyBattleTeam();
    else
        CTL_WARNING(msg);
        modifyBattleTeam();
    end
end
local function changeBattleTeam()
    ctlPlayerChangeBattleTeamRequest:sendRequest(onChangeBattleTeam);
end


local function onMonsterChangeFormation(success, data, code, msg)
    if(success) then
        CTL_INFO("monster change formation complete");
        changeBattleTeam();
    else
        CTL_WARNING(msg);
        changeBattleTeam();
    end
end
local function monsterChangeFormation()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    local slotId = 2;
    local formationId = 2;
    ctlMonstersChangeFormationRequest:sendRequest(onMonsterChangeFormation, firstMonsterId, slotId, formationId);
end


-----------------------------------------------------------
--test treasure box


local function onUseTreasureBox(success, data, code, msg)
    if(success) then
        CTL_INFO("use treasure box complete");
        monsterChangeFormation();
    else
        CTL_WARNING(msg);
        monsterChangeFormation();
    end

end
local function useTreasureBox()
    local boxes = model_treasureBoxStorage:getBoxes();
    if(#(boxes) > 0) then
        ctlPlayerUseTreasureBoxRequest:sendRequest(onUseTreasureBox, boxes[1].groupId, 1);
    else
        onUseTreasureBox(true, nil, 0, nil);
    end
end



-----------------------------------------------------------
--test player


local function onPlayerUseExpItem(success, data, code, msg)
    if(success) then
        CTL_INFO("player use exp item complete");
        --        listMonsters();
        useTreasureBox();
    else
        CTL_WARNING(msg);
        useTreasureBox();
    end
end
local function playerUseExpItem()
    local haveMonsters = model_monsterStorage:getAllActiveMonsters();
    local firstMonsterId = haveMonsters[1].id;
    ctlMonstersUseExpItemRequest:sendRequest(onPlayerUseExpItem, firstMonsterId, 1, 1);
end


local function onPlayerUseItem(success, data, code, msg)
    if(success) then
        CTL_INFO("player use item complete");
        --        listMonsters();
        playerUseExpItem();
    else
        CTL_WARNING(msg);
        playerUseExpItem();
    end
end
local function playerUseItem()
    ctlPlayerUseItemRequest:sendRequest(onPlayerUseItem, 1, 1);
end




local function onPlayerBuySkillPoint(success, data, code, msg)
    if(success) then
        CTL_INFO("player buy skill point complete");
        --        listMonsters();
        playerUseItem();
    else
        CTL_WARNING(msg);
        playerUseItem();
    end
end
local function playerBuySkillPoint()
    ctlPlayerBuySkillUpPointRequest:sendRequest(onPlayerBuySkillPoint);
end


local function onPlayerBuyEnergy(success, data, code, msg)
    if(success) then
        CTL_INFO("player buy energy complete");
        --        listMonsters();
        playerBuySkillPoint();
    else
        CTL_WARNING(msg);
        playerBuySkillPoint();
    end
end
local function playerBuyEnergy()
    ctlPlayerBuyEnergyRequest:sendRequest(onPlayerBuyEnergy);
end


local function onPlayerBuyVitality(success, data, code, msg)
    if(success) then
        CTL_INFO("player buy vitality complete");
        playerBuyEnergy();
    else
        CTL_WARNING(msg);
        playerBuyEnergy();
    end
end
local function playerBuyVitality()
    ctlPlayerBuyVitalityRequest:sendRequest(onPlayerBuyVitality);
end


-----------------------------------------------------------
--test gacha

local function onBuyGacha(success, data, code, msg)
    if(success) then
        CTL_INFO("buy gacha process complete");
        playerBuyVitality();
    else
        CTL_WARNING(msg);
        playerBuyVitality();
    end
end
local function buyGacha()
    ctlGachaRollGachaRequest:sendRequest(onBuyGacha, 1);
end


-----------------------------------------------------------
--test shop

local function onShopBuyItem(success, data, code, msg)
    if(success) then
        CTL_INFO("shop process complete");
        buyGacha();
    else
        CTL_WARNING(msg);
        buyGacha();
    end
end
local function shopBuyItem()
    local list = helper_shoplist_item:getList(true, false, false);
    if(#(list) > 0) then
        ctlShopBuyItemRequest:sendRequest(onShopBuyItem, pbCommon.ShopListType.ITEM, list[1].goodsPrize.id);
    else
        onShopBuyItem(false, nil, -1, "no shop list items");
    end
end


local function onShoplist(success, data, code, msg)
    if(success) then
        shopBuyItem();
    else
        CTL_WARNING(msg);
        shopBuyItem();
    end
end
local function shoplist()
    ctlShopListRequest:sendRequest(onShoplist, pbCommon.ShopListType.ITEM);
end

-----------------------------------------------------------
--test enterance

startIntegrationTest = function()
    CTL_INFO("integration test main flow start, time = " .. TEST_TIMES);
    shoplist();
end

endIntegrationTest = function()
    CTL_INFO("integration test main flow end, time = " .. TEST_TIMES);

    TEST_TIMES = TEST_TIMES + 1;
    if(TEST_TIMES <= TEST_TIMES_MAX) then
        local function onTimer()
            startIntegrationTest();
        end

        delayCallback:create(onTimer, 2);
    else
        CTL_INFO("integration test complete");
    end
end

-----------------------------------------------------------
--start

local username = "dingning";
local password = "dingning";
local usermail = "mail@mail.com";

initRequestHelper:init(username, password, usermail, true, startIntegrationTest)
