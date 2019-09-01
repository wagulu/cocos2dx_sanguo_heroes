_REQUIRE("pbCommon")

local SpotPopupScene = class("SpotPopupScene", cc.load("mvc").ViewBase)
SpotPopupScene.NEXTSCENE = "battle/BattleScene"
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE "model/model_monsterStorage.lua"
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local model_npcBattle = _REQUIRE("model/static/model_npcBattle.lua")
--_REQUIRE "src/packages/packet/Packet_Regester"

SpotPopupScene.RESOURCE_FILENAME = "Scene/SpotPopupScene.csb"
local sceneData=_REQUIRE "battle/ChapterSceneData"
local helper_player = _REQUIRE "model/helper/helper_player.lua"
function SpotPopupScene:onCreate()
    local id =self.args[1]
    local data = self.args[2]
    --cclog(dump(data))
    self:initUi(data)
end


--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
--function SpotPopupScene:onCreate()
--    local id = self.args[1];
--    helper_pve:confirmStage(id, handler(self, self.initUi));
-->>>>>>> caa3da0278e5d974d737796777dd60d1f76bade0
--end

function SpotPopupScene:initUi(data)

    local id =self.args[1]
    local stageInfo = sceneData:getStageInfo(id)
    local Panel_SpotPopupScene=self:getResourceNode():getChildByName("Panel_SpotPopupScene")
    Panel_SpotPopupScene:setTouchEnabled(true)
    local Panel_Top_Base=Panel_SpotPopupScene:getChildByName("Panel_Top_Base")
    local Panel_Center_Base=Panel_SpotPopupScene:getChildByName("Panel_Center_Base")
    local Panel_Center=Panel_Center_Base:getChildByName("Panel_Center")
    self.Panel_Center = Panel_Center
    local Panel_Bg_Base=Panel_SpotPopupScene:getChildByName("Panel_Bg_Base")
    local Panel_Fire_Base=Panel_SpotPopupScene:getChildByName("Panel_Fire_Base")
    Panel_Bg_Base:setTouchEnabled(true)
    Panel_Fire_Base:setTouchEnabled(true)
    local Panel_BlackBg_Base=Panel_SpotPopupScene:getChildByName("Panel_BlackBg_Base")
    local Panel_Info=Panel_Center:getChildByName("Panel_Info")
    local Panel_Group1=Panel_Info:getChildByName("Panel_Group1")
    local Panel_Group2=Panel_Info:getChildByName("Panel_Group2")
    local Text_Title=Panel_Center:getChildByName("Text_Title")
    local stageModel = model_areaStorage:getStage(id,model_user.selectStageType)
    Text_Title:setString(stageModel.name_zhCN)
    local npc = sceneData:getNpc()
    for i=1,6 do
        local Node_Enemy = Panel_Group1:getChildByName("Node_Enemy"..i)
        Node_Enemy:setVisible(i<=#npc)
        if i<=#npc then
            self:initPanel_RisingStar_Head(Node_Enemy,npc[i],true)
            local Panel_RisingStar_Head = Node_Enemy:getChildByName("Panel_RisingStar_Head")
            local Image_Head_bg = Panel_RisingStar_Head:getChildByName("Image_Head_bg")
            local isBoss = model_npcBattle:isBos(stageModel.battleId,npc[i].NPCId)
            if isBoss == 1 then
                Image_Head_bg:loadTexture("frame/frame_boss.png")
            end
        end
        
    end
    local treasures = sceneData:getTreasures()
    for i=1,6 do
        local item = Panel_Group2:getChildByName("Node_object_"..i)
        local Panel_Object = item:getChildByName("Panel_Object")
        item:setVisible(i<=#treasures)
        if i<=#treasures then
            self:initItemCacha(Panel_Object,{id = treasures[i]["value"], type = treasures[i]["type"]})
            self:addBtnTouchEventListener(Panel_Object,function(sender,eventType)
                if  eventType == 2 then
                    self:showDlg("common/PropsPopScene", {id = treasures[i]["value"], type = treasures[i]["type"]})
                end
            end)
        end
        
    end
    
    SCREEN_SCALE_BG(Panel_Bg_Base)
    SCREEN_SCALE_BG(Panel_BlackBg_Base)
    SCREEN_TOP(Panel_Top_Base)
    local Panel_Top=Panel_Top_Base:getChildByName("Panel_Top")
    local Button_Back = Panel_Top:getChildByName("Button_Back")
    self:addBtnTouchEventListener(Button_Back,function(sender,eventType)
        if  eventType == 2 then
            self:sceneBack(function()
                sceneData.type = nil
                self:removeDlgNoAction()
            end)
            
        end
    end)
    
    
    
    
    
    
    
    local btnGo=Panel_Center:getChildByName("Button_Go")
    btnGo:setTitleText(LANG("出征"))
    btnGo:setBright(true)
    if model_user.selectStageType == pbCommon.StageType.MASTER and model_user.selectEliteStageLeftBattleTimes <= 0 then
        btnGo:setBright(false)
    end
    
    local function onClickGo(sender, eventType)
        if eventType == 2 then
            local costEnergy = helper_pve:getConfirmStageCostEnergy()
            if model_user.energy < costEnergy then--体力不足
                self:buyEnergy()
                return
            end
            if model_user.selectStageType == pbCommon.StageType.MASTER and model_user.selectEliteStageLeftBattleTimes <= 0 then
                PopMsg.getInstance():flashShow(LANG("本关卡挑战次数不足，请购买后重试"))
                return
            end
            if(PVE_QUICK_BATTLE) then
                local battleBattleResult = function ()
                    ctlBattleBattleResultRequest:sendRequest(function(sucess,data)
                        if sucess then
                            self:showDlg("battle/WinPopupScene",data)
                            --SWITSCENE("battle/ChapterScene")
                        end
                    end,
                    model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
                    model_user.battleLastFightStarInfo, model_user.battleLastFightWin,
                    model_user.battleProgress, model_user.battleSelectMonsterIdList, model_user.battleSelectNpcIdList);
                    model_user.battleProgress = model_user.battleProgress + 1;
                end


                local function onBattleExecBattle(success, data, code, msg)
                    if(success) then
                        model_user.battleLastFightWin = pbCommon.FightResult.WIN;
                        model_user.battleWinTimes = 1;
                        model_user.battleLastFightStar = 1;
                        model_user.battleLastFightStarInfo = {
                            {flag = 1, status = 1},
                            {flag = 2, status = 1},
                            {flag = 3, status = 1},
                        --            {flag = 4, status = 1},
                        --            {flag = 5, status = 1},
                        };
                        battleBattleResult();
                    end
                end
                model_user.battleSelectMonsterId = sceneData:getBattleTeam()[1].id
                model_user.battleSelectNpcId = sceneData:getNpc()[1].id
                
                model_user.battleSelectMonsterIdList = {}
                model_user.battleSelectNpcIdList = {}
                if model_user.selectStageBattleMode == pbCommon.BattleMode.NORMAL then
                    table.insert(model_user.battleSelectMonsterIdList, model_user.battleSelectMonsterId)
                    table.insert(model_user.battleSelectNpcIdList, model_user.battleSelectNpcId)
                elseif model_user.selectStageBattleMode == pbCommon.BattleMode.DUEL then
                    for key, value in pairs(model_user.battleMyTeam) do
                        table.insert(model_user.battleSelectMonsterIdList, value.id)
                    end
                    for key, value in pairs(model_user.battleNpcTeam) do
                        table.insert(model_user.battleSelectNpcIdList, value.id)
                    end
                end
                
                local enemyIds
                if model_user.isPvp then
                    enemyIds = model_user.battleSelectNpcIdList
                else
                    enemyIds = {}
                    for i = 1, #(model_user.battleSelectNpcIdList) do
                        for key, value in pairs(model_user.battleNpcTeam) do
                            if value.id == model_user.battleSelectNpcIdList[i] then
                                table.insert(enemyIds, value.NPCId)
                            end
                        end
                    end
                end
                
                model_user.battleProgress=1
                ctlBattleExecBattleRequest:sendRequest(onBattleExecBattle,
                    model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
                    model_user.battleProgress, model_user.battleSelectMonsterIdList, enemyIds);
            else
                --self:showDlg("battle/VersusScene",data)
                if model_user.selectStageBattleMode == pbCommon.BattleMode.NORMAL then
                    self:showDlg("battle/VersusScene",data)
                else
                    self:showDlg("battle/VersusDuelScene",data)
                end
                
                self:removeDlgNoAction()
            end
              
            
        end

    end
    
    btnGo:addTouchEventListener(onClickGo)
    local userStage = sceneData:getUserStage()
    local Image_Text_Bg=Panel_Center:getChildByName("Image_Text_Bg")
    local Text_Power=Image_Text_Bg:getChildByName("Text_Power")
    local Text_LastTime=Image_Text_Bg:getChildByName("Text_LastTime")
    Text_Power:setString(LANG("体力消耗：%s",helper_pve:getConfirmStageCostEnergy()))
    Text_LastTime:ignoreContentAdaptWithSize(true)
    if model_user.selectStageType == pbCommon.StageType.MASTER then
        Text_LastTime:setString(LANG("今日剩余次数：%s",model_user.selectEliteStageLeftBattleTimes))
    else
        Text_LastTime:setString(LANG("今日剩余次数：无限"))
    end
    
    local Panel_FullStars=Panel_Center:getChildByName("Panel_FullStars")
    Panel_FullStars:setVisible(stageInfo.isMaster==true)
    local Text_25=Panel_FullStars:getChildByName("Text_25")
    Text_25:setString(LANG("拥有扫荡卷： %s",model_user.moppingTicket))
    
    local Button_Sweep=Panel_Center:getChildByName("Button_Sweep")
    local Button_SweepTen=Panel_Center:getChildByName("Button_SweepTen")
    
    Button_Sweep:setVisible(stageInfo["isMaster"]==true)--主点才显示 
    Button_SweepTen:setVisible(stageInfo["isMaster"]==true)
    local times = 10
    if(model_user.selectStageType == pbCommon.StageType.MASTER) then
        times = math.min(times, model_user.selectEliteStageLeftBattleTimes);
    end
    Button_SweepTen:setBright(times>0)
    Button_Sweep:setBright(times>0)
    Button_SweepTen:setTitleText(LANG("扫荡%s次",times))
    Button_Sweep:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            local singleTimes = 1
            if model_user.selectStageType == pbCommon.StageType.MASTER then
                singleTimes = math.min(1, model_user.selectEliteStageLeftBattleTimes)
            end
            self:sweep(singleTimes)
        end
    end)
    
    Button_SweepTen:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            self:sweep(times)
        end
    end)
    
    self:initStarUI(Panel_Center);
    
    local btnBuy=Panel_Center:getChildByName("Button_Buy")
    local canBuyTimes = helper_player:canBuyVitality()
    btnBuy:setBright(canBuyTimes==true)
   -- local t = (model_user.selectStageType == pbCommon.StageType.MASTER and userStage.battleTimes<=0)
    btnBuy:setVisible((model_user.selectStageType == pbCommon.StageType.MASTER and model_user.selectEliteStageLeftBattleTimes <= 0))
    btnBuy:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            if canBuyTimes==false then
                PopMsg.getInstance():flashShow(LANG("元宝不足，不能重置次数。"))
            	return
            end
            self:showDlg("common/CommonPopScene",{confirmStr=LANG("购买"),cancelStr=LANG("取消"),text=LANG("是否花费%s元宝将次数重置",helper_pve:getResetMoppingTimesIconCost()),fun=function(data)
                if data.cmd=="confirm" then
                    helper_pve:resetAvailableMoppingTimes(function(sucess,data)
                        if sucess then
                            self:initUi()
--                            if self.dialog then
--                                self.dialog:removeDlgNoAction()
--                            end
                            
                        end
                        
                    end);
                elseif data.cmd =="cancel" then	
                    
                end
            end})
        end
    end)
end

function SpotPopupScene:buyEnergy()
    self:showDlg("common/CommonPopScene",{confirmStr=LANG("购买"),cancelStr=LANG("取消"),text=LANG("体力不足是否花费%s元宝购买%s体力。",helper_player:getBuyEnergyCost(),150),fun=function(data)
        if data.cmd=="confirm" then
            ctlPlayerBuyEnergyRequest:sendRequest(function(sucess,data)
                if sucess then
                    self:initUi()
--                    if self.dialog then
--                        self.dialog:removeDlgNoAction()
--                    end
                end
            end);
        end
    end})
end

function SpotPopupScene:sweep(times)
    model_user.preMonsterLevel = model_user.level
    model_user.preEnergy = model_user.energy
    model_user.preEnergyMax = model_user.energyMax
    model_user.preVitality = model_user.vitality
    model_user.preVitalityMax = model_user.vitalityMax
    
    local userStage = sceneData:getUserStage()
    --local times = math.min(10,helper_pve:getAvailableMoppingTimes())
    local costEnergy = helper_pve:getConfirmStageCostEnergy()
    if userStage.moppingTicket==0 then
        PopMsg.getInstance():flashShow(LANG("扫荡券不足1个，无法扫荡。"))
    elseif self.starNum~=3 then
        PopMsg.getInstance():flashShow(LANG("未到3星，不可扫荡。"))
    elseif times <= 0 and model_user.selectStageType == pbCommon.StageType.MASTER then
        PopMsg.getInstance():flashShow(LANG("本关卡挑战次数不足，请购买后重试"))
    elseif model_user.energy < costEnergy then
        self:buyEnergy()
    else
--        self:showDlg("battle/MopResultScene",{})
--        self:removeDlgNoAction()
        ctlBattleMoppingRequest:sendRequest(function(sucess,data)
            if sucess then
                self:showDlg("battle/MopResultScene",data)
                self:removeDlgNoAction()
            end
        end,
        model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
        times);
    end
    
end

function SpotPopupScene:initStarUI(panelCenter)
    local panelInfo = panelCenter:getChildByName("Panel_Info");
    local panelStars = panelCenter:getChildByName("Panel_FullStars");
    local Image_FullStars_Bg = panelStars:getChildByName("Image_FullStars_Bg");
    local Text_25_0 = panelStars:getChildByName("Text_25_0");
    local Button_Sweep=panelCenter:getChildByName("Button_Sweep")
    local Button_SweepTen=panelCenter:getChildByName("Button_SweepTen")
    local starInfoList = model_user.battleStage.starInfo;
    local starNum=0
    for i = 1, #(starInfoList) do
        local starInfo = starInfoList[i];
        if starInfo.flag >= 1 and starInfo.flag <= 3 then
            local spriteStar = panelStars:getChildByName("Image_Star" .. i);
            spriteStar:setVisible(starInfo.status > 0);
            if starInfo.status > 0 then
                starNum=starNum+1
            end
        end
    end
    self.starNum = starNum
    Image_FullStars_Bg:setVisible(starNum == 3)
    Text_25_0:setVisible(starNum ~= 3)
    Button_Sweep:setVisible(starNum == 3)
    Button_SweepTen:setVisible(starNum == 3)
end

--function MainScene:onRequest()
--    release_print("dddddddddddddddd")
--end
--
--
--function onHttpResponse()
--    release_print("get http response")
--end

return SpotPopupScene
