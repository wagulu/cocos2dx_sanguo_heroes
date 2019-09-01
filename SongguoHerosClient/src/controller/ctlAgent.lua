_REQUIRE("controller/ctlGlobal")

local ctlAgent = {};

local dispatchCache = {};
dispatchCache.queue = {};


local REQUEST_MAP =
    {
        ctlInitMaterialRequest,
        ctlInitMaterialSynthesisRequest,
        ctlInitMonstersRequest,
        ctlInitMonsterPartsRequest,
        ctlInitItemsRequest,
        ctlInitTreasureItemPartsRequest,
        ctlInitTreasureItemsRequest,
        ctlInitBattleTeamRequest,
        ctlInitGachaRequest,
        ctlInitTreasureBoxRequest,
        ctlInitUserRequest,
    
        ctlAdventureListIslandRequest,
        ctlAdventureListAreaRequest,
        ctlAdventureListStageRequest,
        ctlAdventureOpenStarTreasureRequest,
        ctlAdventureStageConfirmRequest,
        
        ctlBattleExecBattleRequest,
        ctlBattleBattleResultRequest,
        ctlBattleMoppingRequest,
        ctlBattleSetBattleConfigRequest,
        ctlBattleBuyTimesRequest,
        
--        ctlMonstersListRequest,
        ctlMonstersStarUpRequest,
        ctlMonstersQualityUpRequest,
        ctlMonstersEquipmentLevelUpRequest,
        ctlMonstersEquipmentQualityUpRequest,
--        ctlMonstersInfoRequest,
        ctlMonstersEquipmentMaterialMergeRequest,
        ctlMonstersMergeRequest,
        ctlMonstersChangeFormationRequest,
        ctlMonstersTreasureItemLevelUpRequest,
        ctlMonstersTreasureItemMergeRequest,
        ctlMonstersTreasureItemQualityUpRequest,
        ctlMonstersChangeTreasureItemRequest,
        ctlMonstersSkillUpRequest,
        ctlMonstersUseExpItemRequest,
        ctlMonstersBreakthroughRequest,
        ctlMonstersUseDestinyItemRequest,
        
        
        ctlAccountGetUidFromSNUidRequest,
        ctlAccountLoginRequest,
        ctlAccountNetworkLoginRequest,
        ctlAccountRegisterRequest,
        
        ctlPlayerBuyEnergyRequest,
        ctlPlayerBuyVitalityRequest,
        ctlPlayerUseItemRequest,
        ctlPlayerChangeBattleTeamRequest,
        ctlPlayerModifyBattleMonsterRequest,
        ctlPlayerUseTreasureBoxRequest,
        ctlPlayerBuySkillUpPointRequest,

        ctlShopListRequest,
        ctlShopBuyItemRequest,

        ctlHighladderBuyItemRequest,
        ctlHighladderGetShopRequest,
        ctlHighladderRefreshShopRequest,
        
        ctlHighladderDrawRequest,
--        ctlHighladderFightRequest,
        ctlHighladderGetListRequest,
        ctlHighladderGetRankingRequest,
        ctlHighladderGetRankingRewardRequest,
        ctlHighladderGetEnemyRequest,
        ctlHighladderGetOthersMonstersRequest,
        ctlHighladderExecBattleRequest,
        ctlHighladderBattleResultRequest,
        
        ctlGachaRollGachaRequest,

        ctlDepositGetListRequest,
        ctlDepositReceiveRequest,
        ctlHighladderQuickBattleRequest,
        
        ctlMonstersDestinyLevelUpRequest,
    }


-- handler

function onHttpRequestResponse(eventType, table)
    dispatchCache:pushCallback(eventType, table);
end

function disposeHttpRequestResponse(eventType, table)
    release_print("get request response, event = " .. eventType);

    table[1] = table[1] and (table[4] > 0); --http return success & data len > 0
    local success = table[1];
    local code = table[2];
    local data = table[3];
    local dataLen = table[4];
    local msg = "success = " .. tostring(success) .. "\t" .. "code = " .. tostring(code) .. "\t" .. "dataLen = " .. tostring(dataLen);
    if(dataLen > 0) then
        release_print(msg);
    else
        release_print("ERROR, ========================================");
        release_print("ERROR, server response data length is ZERO");
        release_print("ERROR, " .. msg);
        release_print("ERROR, ========================================");
    end

    local request;
    for key, value in pairs(REQUEST_MAP) do
        if(value:name() == eventType) then
            request = value;
            break;
        end
    end
    if(request ~= nil) then
        request:cbOnline(eventType, table);
    else
        release_print("ERROR, unknown request event = " .. eventType);
    end


end


-- callback queue

function dispatchCache:pushCallback(_eventType, _table)
    local index = 1 + table.maxn(self.queue);
    self.queue[index] = {
        eventType = _eventType,
        table = _table,
    };
end

function dispatchCache:dispatchCallback()
    for key, value in pairs(self.queue) do
        disposeHttpRequestResponse(value.eventType, value.table);
        table.remove(self.queue, key);
        break;
    end
end

local function onTimer_dispatchCallback(dt, data, timerId)
    dispatchCache:dispatchCallback();
end

local timerId = Timer:start(onTimer_dispatchCallback, 0.01)



return ctlAgent;
