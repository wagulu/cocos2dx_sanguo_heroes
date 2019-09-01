
_REQUIRE("model/user_timer")
local dateHelper = _REQUIRE("common/dateHelper.lua")

local model = {};

model.serverId = 1;
model.userName = "";
model.password = "";
model.snUid = "";
model.loginToken = "";
model.uid = "";
model.userKey = "";

--time stamp
model.serverTimestamp = 0;
model.serverTimestampAnchorLocalTime = 0;
function model:setCurrentServerTimestamp(sec)
    self.serverTimestamp = sec;
    self.serverTimestampAnchorLocalTime = dateHelper:getLocalTimestamp();
end
function model:getCurrentServerTimestamp()
    local time = dateHelper:getLocalTimestamp();
    local diffTime = time - self.serverTimestampAnchorLocalTime;
    return self.serverTimestamp + math.abs(diffTime);
end

--basic info
model.energy = 0;
model.energyMax = 1;
model.vitality = 0;
model.vitalityMax = 1;
model.time_to_next_energy = 0;
model.time_to_next_vitality = 0;
model.time_to_energy_max = 0;
model.time_to_vitality_max = 0;
model.exp = 0;
model.expToNextLevel = 1;
model.level = 1;
model.coin = 0;
model.cash = 0;
model.gacha_token = 0;
model.prestige = 0;

model.skillUpPoint = 0;
model.skillUpPointMax = 10;
model.time_to_next_skillUpPoint = 0;
model.skillUpPointBuyMaxCash = 10000

model.dailyBuySkillUpPointTimes = 0;
model.dailyBuySkillUpPointTimesAnchorServerTimestamp = 0;

function model:setDailyBuySkillUpPointTimes(times)
    self.dailyBuySkillUpPointTimes = times;
    self.dailyBuySkillUpPointTimesAnchorServerTimestamp = self:getCurrentServerTimestamp();
end

function model:getDailyBuySkillUpPointTimes()
    if(dateHelper:isSameDay(self.dailyBuySkillUpPointTimesAnchorServerTimestamp, self:getCurrentServerTimestamp())) then
        return self.dailyBuySkillUpPointTimes;
    else
        return 0;
    end
end

model.dailyBuyEnergyTimes = 0;
model.dailyBuyEnergyTimesAnchorServerTimestamp = 0;
function model:setDailyBuyEnergyTimes(times)
    self.dailyBuyEnergyTimes = times;
    self.dailyBuyEnergyTimesAnchorServerTimestamp = self:getCurrentServerTimestamp();
end
function model:getDailyBuyEnergyTimes()
    if(dateHelper:isSameDay(self.dailyBuyEnergyTimesAnchorServerTimestamp, self:getCurrentServerTimestamp())) then
        return self.dailyBuyEnergyTimes;
    else
        return 0;
    end
end

model.dailyBuyVitalityTimes = 0;
model.dailyBuyVitalityTimesAnchorServerTimestamp = 0;
function model:setDailyBuyVitalityTimes(times)
    self.dailyBuyVitalityTimes = times;
    self.dailyBuyVitalityTimesAnchorServerTimestamp = self:getCurrentServerTimestamp();
end
function model:getDailyBuyVitalityTimes()
    if(dateHelper:isSameDay(self.dailyBuyEnergyTimesAnchorServerTimestamp, self:getCurrentServerTimestamp())) then
        return self.dailyBuyVitalityTimes;
    else
        return 0;
    end
end

model.vipLevel = 0;

model.battleTeam = nil;
--model.monsterList = {};

--pve
model.islandsList = {};
model.areasList = {};
model.stagesList = {};
model.stageStarTreasuresList = {};
model.stageTotalStar = 0;
model.selectIslandId = 0;
model.selectAreaId = 0;
model.selectStageId = 0;
model.selectStageType = 0;
model.selectStageBattleMode = 0;

model.selectStageConfirmData = nil;
model.selectEliteStageLeftBattleTimes = 0;
model.selectEliteStageDailyResetTimes = 0;
model.moppingTicket = 0;
model.moppingResult = nil;
model.preEnergy = 0;--战斗前体力值
model.preMonsterLevel = 0; --战斗前玩家等级
model.preEnergyMax = 0; --战斗前玩家体力上限
model.preVitality = 0; --战斗前玩家精力
model.preVitalityMax = 0; --战斗前玩家精力上限
model.preBattleTeam = {} --战斗前玩家武将信息
model.preRank = 0;--战斗前玩家的排名

--battle
model.battleStage = nil;

--NOTE, get from stage confirm, these monsters is ONLY used for battle
--I do NOT update them if get new monster from server by other request
model.battleMyTeam = {};

model.battleNpcTeam = {};
model.battleProgress = 1;
model.battleWinTimes = 0;
model.battleSelectMonsterId = 0;
model.battleSelectNpcId = 0;
model.battleSelectMonsterIdList = {};
model.battleSelectNpcIdList = {};
model.battleLastFightWin = 0;
model.battleLastFightStar = 0;
model.battleLastFightStarInfo = nil;

--monster upgrade
model.selectMonsterUpgradeMonsterId = 0; --210010
model.selectMonsterUpgradeResult = nil; --210010

--equipment upgrade
model.selectEquipmentUpgradeMonsterId = 0;
model.selectEquipmentUpgradeWeaponId = 0;
model.selectEquipmentUpgradeTimes = 0;
model.selectEquipmentUpgradResult = nil;
model.selectEquipmentMaterialMergeId = 0;
model.selectEquipmentMaterialMergeResult = nil;


--treasure item upgrade
model.selectTreasureItemUpgradeTargetInventoryId = 0;
model.selectTreasureItemUpgradeConsumeId = 0;
model.selectTreasureItemUpgradeConsumeCount = 0;
model.selectTreasureItemUpgradeResult = nil;


-- shoplist
model.shoplistContents_item = {};


--pvp
model.highladderShop = {};
model.pvpRanks = nil;
model.pvpSelf = nil;
model.pvpFightResult = nil;
--model.pvpTopOpponent = nil;
model.pvpTopRanks = nil;
model.pvpTopSelf = nil;
model.pvpTopOpponent = nil;
model.currentBattleRank = nil -- 当前战斗的武将排名
model.isPvp = false --当前战斗是否是pvp战斗
model.isAllQuickBattle = false -- 是否全场快速战斗


-- interface
function model:readUserDataFromResponse(pb)
    local sec = tonumber(pb["api"].timestamp, 10);
    if(sec) then
        self:setCurrentServerTimestamp(sec);

        pb = pb["api"]["event"]["user"];

        self:addEnergy(pb.energy - self.energy, false);
--        self.energy = pb.energy;
        self.energyMax = pb.energy_max;
        self:addVitality(pb.vitality - self.vitality, false);
--        self.vitality = pb.vitality;
        self.vitalityMax = pb.vitality_max;
        self.time_to_next_energy = pb.time_to_next_energy;
        self.time_to_next_vitality = pb.time_to_next_vitality;
        self.time_to_energy_max = pb.time_to_energy_max;
        self.time_to_vitality_max = pb.time_to_vitality_max;
        self.exp = pb.exp;
        self.expToNextLevel = pb.exp_for_current_level;
        self.level = pb.level;
        self:addCoin(pb.coin - self.coin)
        self:addCash(pb.cash - self.cash)   
        self.gacha_token = pb.gacha_token;
        self.prestige = pb.prestige;
        self.skillUpPoint = pb.skill_up_point;
        self.time_to_next_skillUpPoint = pb.time_to_next_skill_up_point;
        self:addDepositCount(pb.deposit_count)
        self:setDailyBuySkillUpPointTimes(pb.skill_up_point_buy_times);
        self:setDailyBuyEnergyTimes(pb.energy_buy_times);
        self:setDailyBuyVitalityTimes(pb.vitality_buy_times);
    end
    
end

function model:addDepositCount(e)
    self.deposit_count = 0
    for key,value in pairs(e) do
        self.deposit_count = self.deposit_count + value.count
    end
   
    NotificationCenter_postNotification({msg = "UPDATE_DEPOSIT_COUNT"});
end

function model:addCoin(e)
    self.coin = self.coin + e;
    if(self.coin > 0) then
        NotificationCenter_postNotification({msg = "UPDATE_COIN_POINT"});
    end
    if(self.coin < 0) then
        release_print("ERROR, user coin < 0");
    end
end

function model:addCash(e)
    self.cash = self.cash + e;
    if(self.cash > 0) then
        NotificationCenter_postNotification({msg = "UPDATE_CASH_POINT"});
    end
    if(self.cash < 0) then
        release_print("ERROR, user cash < 0");
    end
end

function model:isSkillUpPointFull()
    return self.skillUpPoint >= self.skillUpPointMax;
end

function model:addSkillUpPoint(e, limitToMax)
    --limitToMax = limitToMax or true;

    self.skillUpPoint = self.skillUpPoint + e;
    if(limitToMax and self.skillUpPoint > self.skillUpPointMax) then
        self.skillUpPoint = self.skillUpPointMax;
    end
    if(self.skillUpPoint < 0) then
        release_print("ERROR, user skillUpPoint < 0");
    end
end

function model:isEnergyFull()
    return self.energy >= self.energyMax;
end

function model:addEnergy(e, limitToMax)
    --limitToMax = limitToMax a true;

    self.energy = self.energy + e;
    if(limitToMax and self.energy > self.energyMax) then
        self.energy = self.energyMax;
    end
    if(self.energy > 0) then
        NotificationCenter_postNotification({msg = "UPDATE_ENERGY_POINT"});
    end
    if(self.energy < 0) then
        release_print("ERROR, user energy < 0");
    end
end

function model:isVitalityFull()
    return self.vitality >= self.vitalityMax;
end

function model:addVitality(e, limitToMax)
    --limitToMax = limitToMax or true;

    self.vitality = self.vitality + e;
    if(limitToMax and self.vitality > self.vitalityMax) then
        self.vitality = self.vitalityMax;
    end
    if self.vitality > 0 then
        NotificationCenter_postNotification({msg = "UPDATE_VITALITY_POINT"});
    end
    if(self.vitality < 0) then
        release_print("ERROR, user vitality < 0");
    end
end


return model;
