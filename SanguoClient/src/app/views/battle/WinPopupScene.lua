
local WinPopupScene = class("WinPopupScene", cc.load("mvc").ViewBase)
WinPopupScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
WinPopupScene.STOPFRAME = 180
WinPopupScene.RESOURCE_FILENAME = "Scene/WinPopupScene.csb"
local sceneData=_REQUIRE "battle/ChapterSceneData"
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE "model/model_monsterStorage.lua"
local model_stage = _REQUIRE "model/static/model_stage.lua"
local static_model_monsterLevel = _REQUIRE "model/static/model_monsterLevel.lua"
local helper_pvp = _REQUIRE("model/helper/helper_pvp.lua")
local model_user = _REQUIRE("model.model_user")
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
function WinPopupScene:onCreate()
    release_print("========= WinPopupScene create");
    self.data = self.args[1]
    self.isDraw = false --是否翻牌
    local Panel_Win=self:getResourceNode():getChildByName("Panel_Win")
    Panel_Win:setTouchEnabled(true)
    local Panel_Bottom_Base=Panel_Win:getChildByName("Panel_Bottom_Base")
    self.panel_Bottom=Panel_Bottom_Base:getChildByName("Panel_Bottom")
    local  button_Retry=self.panel_Bottom:getChildByName("Button_Retry")
    button_Retry:addTouchEventListener(handler(self,self.btnBackCallback)) 
    button_Retry:setVisible(model_user.isPvp == false)
    self.Button_Next=self.panel_Bottom:getChildByName("Button_Next")
    self.Button_Next:setVisible(false)
    self.Button_Next:addTouchEventListener(function(sender, eventType)
        if eventType == 2 then
            if model_user.isPvp then
                --                    helper_pvp:getEnemy(model_user.currentBattleRank, handler(self,self.onHighladderGetEnemy));
                ctlHighladderGetListRequest:sendRequest(handler(self,self.cutArenaPanel));
            else
                helper_pve:initStage(function(sucess,data)
                    if sucess then
                        SWITSCENE(WinPopupScene.NEXTSCENE)
                    end
                end)

            end
            self:sceneBack(function()
                
            end)
        end
    end) 
    local Panel_Center_Base=Panel_Win:getChildByName("Panel_Center_Base")
    local Panel_Center=Panel_Center_Base:getChildByName("Panel_Center")
    local Panel_Center_Base1=Panel_Center:getChildByName("Panel_Center_Base1")
    local Text_Level=Panel_Center_Base1:getChildByName("Text_Level")
    local Text_Exp=Panel_Center_Base1:getChildByName("Text_Exp")
    local Text_Coins=Panel_Center_Base1:getChildByName("Text_Coins")
    local Button_InfoCheck=Panel_Center_Base1:getChildByName("Button_InfoCheck")
    Button_InfoCheck:setVisible(false)
    Text_Level:setString(LANG("LV:%s",model_user.level))
    local coin = model_stage:geCoin(model_user.selectStageId)
    Text_Coins:setString(LANG("银两:%s",coin))
    local stageModel = model_areaStorage:getStage(id,model_user.selectStageType)
    if model_user.isPvp == false then
        Text_Exp:setString(LANG("exp:%s",model_areaStorage:getExp(model_user.selectStageId,model_user.selectStageType)))
    else
        Text_Exp:setString(LANG("账号经验:%s",0))
    end
    
    --release_print(_REQUIRE("battle/ChapterSceneData").type);
    
    -- 根据星级显示star, 临时解决方案
    local panelStarList = Panel_Win:getChildByName("Panel_Top_Base"):getChildByName("Panel_Top"):getChildByName("Panel_Top_Base");
    local iconStarList = {
        panelStarList:getChildByName("Image_3"),
        panelStarList:getChildByName("Image_Star2"),
        panelStarList:getChildByName("Image_Star3")
    };
    local numStarGet = model_user.battleLastFightStar;
    for i = 1, #(iconStarList) do
        local iconStar = iconStarList[i];
        if model_user.isPvp then
            iconStar:setVisible(false);
        else
            iconStar:setVisible(numStarGet >= i);
        end
    end
    local panelFullStars = Panel_Win:getChildByName("Panel_Top_Base"):getChildByName("Panel_Top"):getChildByName("Panel_FullStars");
    panelFullStars:setVisible(numStarGet >= 3);
    local npc = sceneData:getBattleTeam()
    for i=1,5 do
        local Node_Hero = self.panel_Bottom:getChildByName("Node_Item1_"..i)
        Node_Hero:setVisible(i<=#npc)
        local Panel_RewardItem = Node_Hero:getChildByName("Panel_RewardItem")
        
        if i<=#npc then
            local Image_Head_bg = Panel_RewardItem:getChildByName("Image_2")
            local Image_Hero = Panel_RewardItem:getChildByName("Image_1")
            Image_Hero:loadTexture(model_monster:getIcon(npc[i]["id"]))
            Image_Head_bg:loadTexture(model_static_library:getQuality(npc[i]["id"],"Monster"))
            local bar = Panel_RewardItem:getChildByName("bar")
            local Text_1 = bar:getChildByName("Text_1")
            local LoadingBar_1 = bar:getChildByName("LoadingBar_1")
            local stageID = model_user.selectStageId
            local exp = model_stage:geExp(model_user.selectStageId)
            local monsterData = model_monsterStorage:getMonster(npc[i]["id"])
            local toatlExp =static_model_monsterLevel:getExp(monsterData.level)
            if toatlExp == 0 or model_user.isPvp then
                LoadingBar_1:setPercent(100)
                Text_1:setString(LANG("exp+%s",0))
            else
                LoadingBar_1:setPercent(monsterData.exp/toatlExp*100)
                Text_1:setString(LANG("exp+%s",exp))
            end
            
            
        end
    end
    
    


    local Panel_Text = Panel_Win:getChildByName("Panel_Top_Base"):getChildByName("Panel_Top"):getChildByName("Panel_Text")
    Panel_Text:setVisible(model_user.isPvp)
    if model_user.isPvp == false then
        local stateInfo = sceneData:getStageInfo(model_user.selectStageId)
        button_Retry:setVisible(stateInfo.isMaster == true)
        self.Button_Next:setVisible(true)
        local treasures = self.data.treasures
        local Panel_Item2 = self.panel_Bottom:getChildByName("Panel_Item2")
        self.panel_Bottom:getChildByName("Panel_Item3"):setVisible(model_user.isPvp)
        for i=1,5 do
            local item = Panel_Item2:getChildByName("Node_Item2_"..i)
            local Panel_Object = item:getChildByName("Panel_Object")
            item:setVisible(i<=#treasures)
            if i<=#treasures then
                self:initItemCacha(Panel_Object,{id = treasures[i].value,type = treasures[i].type,num = treasures[i].content})
                Panel_Object:setTouchEnabled(true)
                self:addBtnTouchEventListener(Panel_Object,function(sender,eventType)
                    if  eventType == 2 then
                        self:showDlg("common/PropsPopScene", {id = treasures[i]["value"], type = treasures[i]["type"]})
                    end
                end)
            end
        end
    else
        
        button_Retry:setVisible(false)
        for m = 1,3 do
--            local item_cacha = self.panel_Bottom:getChildByName("Panel_Item3"):getChildByName("ProjectNode_"..m):getChildByName("Panel_Icon")
--            local icon = item_cacha:getChildByName("Panel_Object"):getChildByName("Image_1")
--            local icon_bg = item_cacha:getChildByName("Panel_Object"):getChildByName("Image_2")
            --            self.panel_Bottom:getChildByName("Panel_Item3"):getChildByName("ProjectNode_"..m):gotoFrameAndPause(0)
            self.panel_Bottom:getChildByName("Panel_Item3"):getChildByName("ProjectNode_"..m):setVisible(false)
        end
        
        Panel_Text:getChildByName("Text_2"):setString(model_user.preRank)
        local Text_3 = Panel_Text:getChildByName("Text_3")
        if self.data.rankDelta > 0 then
            Text_3:setString(self.data.rank)
            --名次提升特效
            local node,nodeAction=self:createNode("Node/Node_TopBaseUp_Anim.csb")
            Text_3:addChild(node)
            nodeAction:gotoFrameAndPlay(0,true)           
            node:setPosition(cc.p(Text_3:getContentSize().width/2- node:getChildByName("Panel_upAnim"):getContentSize().width/4,Text_3:getContentSize().height/2))
        end
        Text_3:setVisible(self.data.rankDelta > 0)
        Panel_Text:getChildByName("Image_2"):setVisible(self.data.rankDelta > 0)

    end
    self.panel_Bottom:getChildByName("Panel_Item3"):setVisible(model_user.isPvp)
    self.panel_Bottom:getChildByName("Panel_Item2"):setVisible(model_user.isPvp == false)
end

function WinPopupScene:initItemInfo(Panel_Object,data)
    local Image_Icon = Panel_Object:getChildByName("Image_1")
    local Image_Bg = Panel_Object:getChildByName("Image_2")
    local Text_1 = Panel_Object:getChildByName("Text_1")
    Image_Icon:loadTexture(model_static_library:getAsset(data.id,data.type))
    Image_Bg:loadTexture(model_static_library:getFrame(data.id,data.type))
end

function WinPopupScene:frameStopCallBack()
    if model_user.isPvp then
        --翻牌特效
        local Panel_Item3 = self.panel_Bottom:getChildByName("Panel_Item3")
        Panel_Item3:setVisible(model_user.isPvp == true)
        self.nodeActionArr = {}
        self.nodeArr = {}
        self.fanpaiBtns = {}
        for j = 1,3 do
            local node,nodeAction=self:createNode("Node/Node_Animation_Draw.csb")
            nodeAction:gotoFrameAndPause(0)
            node:getChildByName("Panel_Icon"):getChildByName("Button_1"):setTag(j)
            self.nodeActionArr[j] = nodeAction
            self.nodeArr[j] = node

            local ProjectNode = Panel_Item3:getChildByName("ProjectNode_"..j)
            ProjectNode:setVisible(false)
            Panel_Item3:addChild(node)
            node:setPosition(cc.p(ProjectNode:getPositionX(),ProjectNode:getPositionY()))
            local Button_1 = node:getChildByName("Panel_Icon"):getChildByName("Button_1")
            self.fanpaiBtns[j] = Button_1
            Button_1:setTag(j)
            node:getChildByName("Panel_Icon"):getChildByName("Panel_Object"):setTouchEnabled(false)
            Button_1:addTouchEventListener(function(sender,eventType)
                if eventType==2 then
                    local node0,nodeAction0=self:createNode("Node/Node_Animation_Draw_Select.csb")
                    self.fanpaiBtns[sender:getTag()]:addChild(node0)
                    nodeAction0:gotoFrameAndPlay(0,true)           
                    node0:setPosition(cc.p(self.fanpaiBtns[sender:getTag()]:getContentSize().width/2,self.fanpaiBtns[sender:getTag()]:getContentSize().height/2))
                
                    self:initItemInfo(self.nodeArr[sender:getTag()]:getChildByName("Panel_Icon"):getChildByName("Panel_Object"),self.data.prize)
                    self:setDraw(sender:getTag())
                end
            end)
        end

        --在玩家无操作5秒后，自动播放翻牌动画，随机翻开一个，0.5秒后翻开其他两个，并出现继续按钮
        Timer:runOnce(function()
            if self.isDraw == false then
                local num = math.random(1,3)
                self:setDraw(num)
            end
        end,5)
        
        --历史排名奖励弹窗
        Timer:runOnce(function()
            if table.getn(self.data.highRankPrizes) > 0 and self.data.rank < 10000 then
                self:showDlg("arena/RecordBreakingAward",self.data)
            end
        end,0.08)
    end 
    
    --玩家等级升级弹窗
    if model_user.preMonsterLevel < model_user.level then
        self:showDlg("battle/LevelUpPopup")
    end
    --判断玩家武将是否升级，升级则播放特效
    local npc1 = model_user.preBattleTeam
    for p = 1,table.getn(npc1) do
        
        local monsterData = model_monsterStorage:getMonster(npc1[p]["id"])
        if monsterData.level > npc1[p]["level"] then
            local ProjectNode = self.panel_Bottom:getChildByName("ProjectNode_".. p)
            local node,nodeAction=self:createNode("Nodes/Node_Animation_RisingStar.csb")
            ProjectNode:addChild(node)
            nodeAction:gotoFrameAndPlay(0,false) 
        end
    end
end

function WinPopupScene:setDraw(index)
    for m = 1,3 do
        self.nodeArr[m]:getChildByName("Panel_Icon"):getChildByName("Button_1"):setTouchEnabled(false)
    end
    
    self.isDraw = true
    self.nodeActionArr[index]:gotoFrameAndPlay(0,90,false)
    
    --翻牌边框
    local button_1 = self.nodeArr[index]:getChildByName("Panel_Icon"):getChildByName("Button_1")
    local node,nodeAction=self:createNode("Node/Node_Animation_Draw_Select.csb")
    button_1:addChild(node)
    nodeAction:gotoFrameAndPlay(0,true)           
    node:setPosition(cc.p(button_1:getContentSize().width/2,button_1:getContentSize().height/2))
    
    Timer:runOnce(function()
        local num = 0
        for k = 1,3 do
            num = num+1
            if k ~= index then
                self:initItemInfo(self.nodeArr[k]:getChildByName("Panel_Icon"):getChildByName("Panel_Object"),self.data.displayPrizes[num])
                self.nodeActionArr[k]:gotoFrameAndPlay(0,90,false)
            else
                num = num - 1
            end
        end
    end,1)
    self.Button_Next:setVisible(true)
end

--翻牌奖励
function WinPopupScene:getPrize(info)
    local address
    if info.type == "item" then--道具
        address = "Item/Item_"..info.id..".png"
    elseif info.type == "monsterParts" then --武将碎片
        address = "MonsterPart/MonsterPart_"..info.id..".png"
    elseif info.type == "TreasureItemParts" then --珍宝碎片
        address = "TreasureItemPart/TreasureItemPart_"..info.id..".png"
	end
    return address
end

function WinPopupScene:initUi()
    local panel_Bottom=self:getResourceNode():getChildByName("Panel_Bottom")
    for i=1, 5 do
        local item=panel_Bottom:getChildByName("Node_Item1_"..i)
        
    end
end

function WinPopupScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        if model_user.isPvp then
            helper_pvp:getEnemy(model_user.currentBattleRank, handler(self,self.onHighladderGetEnemy));
        else
            helper_pve:confirmStage(model_user.selectStageId, function(success, data, code, msg)
                if success then
                    SWITSCENE(WinPopupScene.NEXTSCENE,1)
                end
            end);
        end
    end
end

function WinPopupScene:onHighladderGetEnemy(success, data, code, msg)
    if(success) then
        SWITSCENE("battle/VersusScene",data,1)
    else
    end
end

function WinPopupScene:cutArenaPanel(success, data, code, msg)
    if(success) then
        SWITSCENE("arena/ArenaScene",data)
    else
    end
end

return WinPopupScene
