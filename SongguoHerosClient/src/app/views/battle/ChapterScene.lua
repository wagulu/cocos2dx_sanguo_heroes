
local ChapterScene = class("ChapterScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"

ChapterScene.RESOURCE_FILENAME = "Scene/ChapterScene.csb"
ChapterScene.PRESCENE = "home/MainCityScene"  
ChapterScene.NEXTSCENE = "battle/BattleScene"-- 的发射点发
ChapterScene.STOPFRAME = 47
local sceneLogic = _REQUIRE "battle/WinPopupSceneLogic"
local sceneData=_REQUIRE "battle/ChapterSceneData"
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE "model/model_monsterStorage.lua"
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local delayCallback = _REQUIRE("common/delayCallback.lua")

function ChapterScene:onCreate()
--    self.index = self.args[1]
--    cclog("========= ChapterScene create");
    sceneData.type = self.args[1]
    --sceneData.areaIndex =1
    local Panel_ChapterScene=self:getResourceNode():getChildByName("Panel_ChapterScene")
    local Panel_Bg_Center_Base=Panel_ChapterScene:getChildByName("Panel_Bg_Center_Base")
    local Panel_Bg_Center=Panel_Bg_Center_Base:getChildByName("Panel_Bg_Center")
    local Panel_Top_Base=Panel_ChapterScene:getChildByName("Panel_Top_Base")
    local Panel_MaskBg_Base=Panel_ChapterScene:getChildByName("Panel_MaskBg_Base")
    local Panel_MaskBg_Center=Panel_MaskBg_Base:getChildByName("Panel_MaskBg_Center")
    local panel_Bottom=Panel_ChapterScene:getChildByName("Panel_Bottom")
    --local ProjectNode_Btn_Right=Panel_MaskBg_Center:getChildByName("ProjectNode_Btn_Right")
    --local ProjectNode_Btn_Left=Panel_MaskBg_Center:getChildByName("ProjectNode_Btn_Left")
    local Panel_ArrowL=Panel_Bg_Center:getChildByName("Panel_ArrowL")
    local ProjectNode_ArrowL=Panel_ArrowL:getChildByName("ProjectNode_ArrowL")
    local Panel_l=ProjectNode_ArrowL:getChildByName("Panel_1")
    
    local Panel_ArrowR=Panel_Bg_Center:getChildByName("Panel_ArrowR")
    local ProjectNode_ArrowR=Panel_ArrowR:getChildByName("ProjectNode_ArrowR")
    local Panel_R=ProjectNode_ArrowR:getChildByName("Panel_1")
    
    self.Button_ArrowLeft=Panel_l:getChildByName("Button_1")
    self.Button_ArrowRight=Panel_R:getChildByName("Button_1")
    
    
--    local ChapterPoint_Done=self:createNode("Node/ChapterPoint_Done.csb")
--    local ChapterPoint_Done2=self:createNode("Node/ChapterPoint_Done2.csb")
--    local ChapterPoint_Small_Done=self:createNode("Node/ChapterPoint_Small_Done.csb")
--    local ChapterPoint_Small_Done2=self:createNode("Node/ChapterPoint_Small_Done2.csb")
--    self.ChapterPoint_DoneP = ChapterPoint_Done:getChildByName("Panel_1")
--    self.ChapterPoint_Done2P = ChapterPoint_Done2:getChildByName("Panel_3")
--    self.ChapterPoint_Small_DoneP = ChapterPoint_Small_Done:getChildByName("Panel_1")
--    self.ChapterPoint_Small_Done2P = ChapterPoint_Small_Done2:getChildByName("Panel_1")
    local beginPos = {x = 0, y = 0} 
    local function onTouchesBegan(touches, event)     
        beginPos = touches:getLocation()
        --cclog("beginPos:"..beginPos.x)
        return true
    end
    local function changStage(subIndex)
        
        local currentIndex = sceneData.areaIndex + subIndex
        if currentIndex<=0 then
            return
        end
        if currentIndex>sceneData:getAreaLen() then
            return
        end
        if subIndex>0 then--往后翻页的话判断是否解锁
            if(currentIndex<sceneData:getAreaLen())then
                local index = sceneData.areaIndex
                if model_user.selectStageType == pbCommon.StageType.MASTER then --精英下一关解锁条件得是下一章节的最后一个点通关，普通的话是当前章节的最后一个点通关
                    index = currentIndex
                end
                if model_areaStorage:isLockNextArea(index) then
                        
                else
                    if model_user.selectStageType == pbCommon.StageType.MASTER then --精英下一关解锁条件得是下一章节的最后一个点通关，普通的话是当前章节的最后一个点通关
                        PopMsg.getInstance():flashShow(LANG("先通关普通关卡，才能开启精英关卡"))
                    else
                        PopMsg.getInstance():flashShow(LANG("下一关未解锁"))
                    end
                    
                    cclog("dddddddddddddddddd")
                    return
                end

            end
        end
        
        self:getStageList(model_user.selectStageType,currentIndex)
    end
    local function onTouchesMoved(touches, event)
        local location = touches:getLocation()
        local moveX = location.x - beginPos.x
        --cclog("moveX:"..moveX)
        if moveX<0 and math.abs(moveX)>100 then --往左滑动  向后翻页
            changStage(1)
        elseif moveX>0 and math.abs(moveX)>100 then
            changStage(-1)
        end
    end

    local listener = cc.EventListenerTouchOneByOne:create()
    --listener:setSwallowTouches(true)
    listener:registerScriptHandler(onTouchesBegan,cc.Handler.EVENT_TOUCH_BEGAN )
    --listener:registerScriptHandler(onTouchesMoved,cc.Handler.EVENT_TOUCH_MOVED )
    listener:registerScriptHandler(onTouchesMoved,cc.Handler.EVENT_TOUCH_ENDED )

    local ball = ccui.ImageView:create("png/btn.png")

    --    ball:setTouchEnabled(true)
    ball:setPosition(200,200)
    Panel_ChapterScene:addChild(ball)
    local eventDispatcher = ball:getEventDispatcher()
    eventDispatcher:addEventListenerWithSceneGraphPriority(listener, ball)
    ball:setVisible(false)
    
    self.Button_ArrowLeft:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            changStage(-1)
        end

    end)
    self.Button_ArrowRight:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            changStage(1)
        end

    end)
    local panel_Top=Panel_Top_Base:getChildByName("Panel_Top")
    local  button_Back=panel_Top:getChildByName("Button_Back")
    button_Back:addTouchEventListener(handler(self,self.onClickHandler)) 
    local Node_HUD_Top=panel_Top:getChildByName("Node_HUD_Top")
    self:initTopBar(Node_HUD_Top)
    self.buttons = {}
    local Panel_Button = panel_Bottom:getChildByName("Panel_Button")
    local Button_PuTong = Panel_Button:getChildByName("Button_PuTong")
    local Button_JingYing = Panel_Button:getChildByName("Button_JingYing")
    self.buttons[1] = Button_PuTong
    Button_PuTong:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:setIndex(sender:getTag())
            Button_PuTong:setLocalZOrder(1)
            Button_JingYing:setLocalZOrder(0)
            self:getStageList(pbCommon.StageType.NORMAL,model_areaStorage:getCurrentAreaId(pbCommon.StageType.NORMAL))
            --self:initMap(pbCommon.StageType.NORMAL,sceneData.areaIndex)
        end

    end)

    self.buttons[2] = Button_JingYing
    Button_JingYing:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            local eIndex = model_areaStorage:getCurrentAreaId(pbCommon.StageType.MASTER)
            if eIndex == 0 then
                PopMsg.getInstance():flashShow(LANG("先通过普通关卡，才能开启精英关卡。"))
            else
                self:setIndex(sender:getTag())
                Button_PuTong:setLocalZOrder(0)
                Button_JingYing:setLocalZOrder(1)
                self:getStageList(pbCommon.StageType.MASTER,eIndex)
            end
        end

    end)
    sceneData.areaIndex = model_user.selectAreaId
    self:init(sceneData.areaIndex)
    if HTTP_OFFLINE then
        self:init(sceneData.areaIndex)
    end
end

function ChapterScene:getStageList(type,areaIndex)

    cclog("============== ChapterScene:getStageList");
    self:setIndex(type)
    sceneData.areaIndex = areaIndex
    self.Button_ArrowLeft:setVisible(sceneData.areaIndex~=1)
    self.Button_ArrowRight:setVisible(sceneData.areaIndex~=sceneData:getAreaLen())

    local function onListStage()
        model_areaStorage:fillBattleDataToUser(model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageType);

        if sceneData.areaIndex ~= areaIndex then --当前地区index 与传递的index不同说明是在切换不同的地图
            self:sceneBack(function()
                self:init(areaIndex)
                self:getActionNode():gotoFrameAndPlay(0,false)
            end)
        else
            self:init(areaIndex)
        end
    end

    if(areaIndex<=sceneData:getAreaLen()) then
        model_user.selectAreaId = sceneData:getAreaData()[areaIndex].areaId;
        model_user.selectStageType = type;
        cclog("Model_user.selectIslandId"..model_user.selectIslandId..":"..model_user.selectStageType..":model_user.selectAreaId"..model_user.selectAreaId.."sceneData.areaIndex:"..areaIndex)

        delayCallback:create(onListStage, 0.1);
    end
end

 

function ChapterScene:init(areaIndex)
    self:initReward()
    self:initMap(model_user.selectStageType)
    if(sceneData.type~=nil) then
        local function popSpotScene()
            self:showDlg("battle/SpotPopupScene",model_user.selectStageId)
        end
        local function popVersusScene()
            self:showDlg("battle/VersusScene")
        end
        local  pSequence
        if sceneData.type ==1 then
            pSequence = cc.Sequence:create(cc.DelayTime:create(.1),cc.CallFunc:create(popSpotScene))
        elseif sceneData.type ==2 then
            pSequence = cc.Sequence:create(cc.DelayTime:create(.1),cc.CallFunc:create(popVersusScene))
        end 

        self:runAction(pSequence)

    end
end

function ChapterScene:dlgCallBack(data)--领取宝箱回调
    if data.cmd == "openTreasure" then
        sceneData:starTreasuresList()[data.index].isOpen = true
        self:initReward()
    end
end

function ChapterScene:initReward()
    local rewardList = sceneData:starTreasuresList() or {}
    local Panel_ChapterScene=self:getResourceNode():getChildByName("Panel_ChapterScene")
    local panel_Bottom=Panel_ChapterScene:getChildByName("Panel_Bottom")
    local Panel_Reward_PsBar = panel_Bottom:getChildByName("Panel_Reward_PsBar")
    local LoadingBar_1 = Panel_Reward_PsBar:getChildByName("LoadingBar_1")
    local totalStar =sceneData:getTotalStar()
    --local currStar = sceneData:getMaxStar()
    local currStar = model_user.stageTotalStar
    LoadingBar_1:setPercent(currStar/totalStar *100)
    for i=1,#rewardList do
        local rewardInfo = rewardList[i]
        local txt = Panel_Reward_PsBar:getChildByName("Text_"..i)
        local Image = Panel_Reward_PsBar:getChildByName("Image_"..i)
        Image:setTouchEnabled(true)
        local needStar = rewardInfo.star
        local Particle_1 = Panel_Reward_PsBar:getChildByName("Node_Ani_"..i)
        Particle_1:setVisible(currStar>=needStar and rewardInfo.isOpen==false)
        if rewardInfo.isOpen==true then 
            Image:loadTexture("Box/Box"..i.."_3.png")
            Image:addTouchEventListener(function(sender,eventType)
                if eventType==2 then
                    PopMsg.getInstance():flashShow(LANG("已经领取过了!"))
                end

            end)
        else
            if currStar>=needStar then
                Image:loadTexture("Box/Box"..i.."_2.png")
            else
                Image:loadTexture("Box/Box"..i.."_1.png")
            end
            
            self:addBtnTouchEventListener(Image,function(sender,eventType)
                if eventType ==0 then
                    sender:stopAllActions();
                    sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
                elseif eventType==2 then
                    self:showDlg("battle/StarAwardPopupScene",rewardInfo,i,currStar>=needStar)
                end

            end)
        end
        if txt then
        	txt:setString(rewardInfo.star)
        end
        
        
    end
end

function ChapterScene:initMap(type)
    SanGuoAnimationLib.freeAllAdvancedAnimation();
    SanGuoAnimationLib.freeAllAdvancedAnimationAsset();
    local Panel_ChapterScene=self:getResourceNode():getChildByName("Panel_ChapterScene")
    local Panel_Top_Base=Panel_ChapterScene:getChildByName("Panel_Top_Base")
    local Panel_Top=Panel_Top_Base:getChildByName("Panel_Top")
    local Panel_Bg_Center_Base=Panel_ChapterScene:getChildByName("Panel_Bg_Center_Base")
    SCREEN_TOP(Panel_Top_Base)
    local panel_Bottom=Panel_ChapterScene:getChildByName("Panel_Bottom")
    SCREEN_BOTTOM(panel_Bottom)
    
    local Panel_Base = panel_Bottom:getChildByName("Panel_Base")
    local txt_hulaoguan_1 = Panel_Base:getChildByName("Text_HLG")
    local areaData = sceneData:getAreaData()[sceneData.areaIndex]
    local name =sceneData:getAreaName()
    txt_hulaoguan_1:setString(name)
    --local Panel_15=panel_Bottom:getChildByName("Panel_15")
    local Text_4=Panel_Base:getChildByName("Text_4")
    Text_4:setString(LANG("章节：%s/%s",sceneData.areaIndex,sceneData:getAreaLen()))
    self:setIndex(type)
    self.Button_ArrowLeft:setVisible(sceneData.areaIndex~=1)
    self.Button_ArrowRight:setVisible(sceneData.areaIndex~=sceneData:getAreaLen())
    local Panel_Bg_Center=Panel_Bg_Center_Base:getChildByName("Panel_Bg_Center")
    local Node_Chapter_3 = Panel_Bg_Center:getChildByName("Node_Chapter_3")
    Node_Chapter_3:removeAllChildren()
    Node_Chapter_3:setOpacity(0)
    Node_Chapter_3:runAction(cc.FadeIn:create(1))
    local node,nodeAction=self:createNode("Node/Chapter_"..math.min(sceneData.areaIndex,4)..".csb")
    local Panel_Chapter_1 = node:getChildByName("Panel_Chapter_1")
    local Panel_Point = Panel_Chapter_1:getChildByName("Panel_Point")
    local ProjectNode_4 = Panel_Chapter_1:getChildByName("ProjectNode_4")
    local ProjectNode_4_0 = Panel_Chapter_1:getChildByName("ProjectNode_4_0")
    local ChapterPoint_S4_1 = Panel_Chapter_1:getChildByName("ChapterPoint_S4_1")
    
    local Image_LIne = Panel_Chapter_1:getChildByName("Image_LIne")
    local Image_LIne_JY = Panel_Chapter_1:getChildByName("Image_LIne_JY")
    Image_LIne:setVisible(type == pbCommon.StageType.NORMAL)
    Image_LIne_JY:setVisible(type == pbCommon.StageType.MASTER)
    --local Image_Chapter1_Mask = Panel_23:getChildByName("Image_Chapter1_Mask")
    --Image_Chapter1_Mask:setVisible(false)
    
    local Node_Chapter_Bg = Panel_Bg_Center:getChildByName("Node_Chapter_Bg")
    local bgPanel = Node_Chapter_Bg:getChildByName("Panel_Chapter_1")
    local Panel_BG = bgPanel:getChildByName("Panel_BG")
    local Panel_23 = bgPanel:getChildByName("Panel_23")
    local Image_Chapter1_BG = Panel_BG:getChildByName("Image_Chapter1_BG")
    SCREEN_SCALE_BG(Image_Chapter1_BG)
    --SCREEN_SCALE_BG(Image_Chapter1_Mask)
    local team = model_user.battleTeam
    local Panel_Button_0=panel_Bottom:getChildByName("Panel_Button_0")
    local ProjectNode_Myteam_6=Panel_Button_0:getChildByName("ProjectNode_Myteam_6")
    local Button_BZ=ProjectNode_Myteam_6:getChildByName("Button_BZ")
    self:addBtnTouchEventListener(Button_BZ,function(sender,eventType)
        if eventType ==  2 then
            self:showDlg("battle/ChooseHeroInBattleScene",1)
            
            --SWITSCENE("battle/BattleTeamScene",1)
        end
    end)
--    for i=1,3 do
--        local teamData = team[i]
--        local ProjectNode=ProjectNode_Myteam_6:getChildByName("ProjectNode_"..i)
--        if teamData.isUse >0 then
--            local monster = model_monsterStorage:getMonster(teamData.monsterId)
--            self:initPanel_RisingStar_Head(ProjectNode,monster)
--        else
--            self:initPanel_RisingStar_Head(ProjectNode)
--        end
--        
--        
--    end
    if ProjectNode_4 then
    	ProjectNode_4:setVisible(false)
    end
    if ProjectNode_4_0 then
        ProjectNode_4_0:setVisible(false)
    end
    if ChapterPoint_S4_1 then
        ChapterPoint_S4_1:setVisible(false)
    end
    Node_Chapter_3:addChild(node)
    local data = sceneData:getStageData()
    if HTTP_OFFLINE and sceneData.areaIndex==2  then
    	data ={{id=1,status=3,isMaster=false}}
    end
    if #data==0 then
    	return
    end
    local maxStageLen=22 --先写死
    local curentIndex=sceneData:getCurrentIndex()
    local realStageList = sceneData:getMaxLen()
    --local Panel_1=node:getChildByName("Panel_1")
    
    local function disPlayBossPoint(itemInfo)
        local node = self:createNode("Node/ChapterPoint_Select_Boss.csb")
        node:getChildByName("Panel_1"):getChildByName("daqiao0001_2"):setVisible(false)
        local monster = SanGuoAnimationLib.createAdvancedAnimation("hero_"..itemInfo.icon, "animation_idle");
        monster:setScale(0.4)
        if sceneData:isAttackNode(itemInfo) then
            node:getChildByName("ProjectNode_1"):setVisible(false)
        else
            node:getChildByName("ProjectNode_1"):setVisible(true)	
        end
        node:getChildByName("Panel_1"):addChild(monster);
        monster:setPosition(node:getChildByName("Panel_1"):getChildByName("daqiao0001_2"):getPosition())
        monster:setLocalZOrder(node:getChildByName("Panel_1"):getChildByName("daqiao0001_2"):getLocalZOrder())
        return node
    end
    
    local function disPlayDualPoint(itemInfo)
        local node = self:createNode("Node/ChapterPoint_Select_Duel.csb")
        node:getChildByName("Panel_1"):getChildByName("Particle_9"):setVisible(false)
        node:getChildByName("Panel_1"):getChildByName("ProjectNode_Duel"):setVisible(false)
        local dual = self:createNode("Node/Node_Duel.csb",true)
        node:getChildByName("Panel_1"):addChild(dual);
        dual:setPosition(node:getChildByName("Panel_1"):getChildByName("ProjectNode_Duel"):getPosition())
        dual:setLocalZOrder(node:getChildByName("Panel_1"):getChildByName("ProjectNode_Duel"):getLocalZOrder())
        --node:getChildByName("Panel_1"):getChildByName("ProjectNode_Duel"):setVisible(false)
        if sceneData:isAttackNode(itemInfo) then --可以打的
            node:getChildByName("ProjectNode_1"):setVisible(false)
            
        else
            node:getChildByName("ProjectNode_1"):setVisible(true)   
        end
        return node
    end
    
    for i=1, maxStageLen do
        local item
        if type == pbCommon.StageType.NORMAL  then
            item=Panel_Point:getChildByName("Node_"..i)
        else
            if i<=realStageList then
                item=Panel_Point:getChildByName("Node_"..model_user.masterPointList[i])
            end
           
        end
        
        if item~=nil then
            if i<=realStageList then
                item:setVisible(true)
                item:removeAllChildren()
                local itemInfo = data[i]
                local pointName
                local node
                local stageModel = model_areaStorage:getStage(itemInfo.id,type)
                if sceneData:isAttackNode(itemInfo) then
                    if itemInfo.isMaster==false then
                        if  i==curentIndex then--当前点
                            node = self:createNode("Node/ChapterPoint_Select_Small.csb")--小点未打
                            node:getChildByName("Panel_1"):setVisible(false)
                            --pointName = "ChapterPoint_Small_Done2" --小点未打
                        else
                            --node = self:createNode("Node/ChapterPoint_Small_Done.csb")
                            node = self:createNode("Node/ChapterPoint_Select_Small.csb")--小点打过
                            node:getChildByName("Panel_1"):getChildByName("Particle_9"):setVisible(false)
                            node:getChildByName("Button_Point_Lock"):setVisible(false)
                            local Button_Point_B = node:getChildByName("Panel_1"):getChildByName("Button_Point_B")
                            Button_Point_B:setTouchEnabled(false)
                         --pointName = "ChapterPoint_Small_Done"
                       	end
                        
                    else
                        --cclog("itemInfo.isMaster"..itemInfo.isMaster)
                        --node = self:createNode("Node/ChapterPoint_Done.csb")
                        node = self:createNode("Node/ChapterPoint_Select.csb")--大点打过显示星星
                        if stageModel.isBoss then
                            node = disPlayBossPoint(itemInfo)
                        end
                        
                        if stageModel.battleMode == pbCommon.BattleMode.DUEL then
                            node = disPlayDualPoint(itemInfo)
                        end
                        
                        node:getChildByName("Panel_1"):getChildByName("Particle_9"):setVisible(false)
                        local Icon_Boss_Green_1 = node:getChildByName("Panel_1"):getChildByName("Icon_Boss_Green_1")
                        if Icon_Boss_Green_1 then
                            Icon_Boss_Green_1:setVisible(false)
                        end
                        node:getChildByName("ProjectNode_1"):setVisible(false)
                     --pointName = "ChapterPoint_Done"--大点打过显示星星
                    end
                else
                    if itemInfo.isMaster==false then --С
                        node = self:createNode("Node/ChapterPoint_Select_Small.csb")--小点未打
                        node:getChildByName("Panel_1"):setVisible(false)
                        local Button_Point_B = node:getChildByName("Button_Point_Lock")
                        Button_Point_B:setBright(true)
                        Button_Point_B:setTouchEnabled(true)
                        self:addBtnTouchEventListener(Button_Point_B,function(sender,eventType)
                            if eventType == 2 then
                                PopMsg.getInstance():flashShow(LANG("不可以跳过未解锁的战斗点哦！"))
                            end

                        end)
                        --pointName = "ChapterPoint_Small_Done2" --小点未打
                    else
                        --cclog("itemInfo.isMaster"..itemInfo.isMaster)
                        node = self:createNode("Node/ChapterPoint_Select.csb")--大点未打过不显示星星
                        if stageModel.isBoss then
                            node = disPlayBossPoint(itemInfo)
                        end
                        
                        if stageModel.battleMode == pbCommon.BattleMode.DUEL then
                            node = disPlayDualPoint(itemInfo)
                        end
                        node:getChildByName("Panel_1"):getChildByName("Particle_9"):setVisible(false)
                        local Icon_Boss_Green_1 = node:getChildByName("Panel_1"):getChildByName("Icon_Boss_Green_1")
                        if Icon_Boss_Green_1 then
                            Icon_Boss_Green_1:setVisible(false)
                        end;
                       -- pointName = "ChapterPoint_Done"--大点未打过不显示星星
                    end
                end
                item:addChild(node)
                node:setName("point")
                item:setTag(itemInfo.id)
                if (itemInfo.isMaster==true or i==curentIndex) then --已完成并且是主线点
                    local Button_Point_B = node:getChildByName("Panel_1"):getChildByName("Button_Point_B")
                    
                    local canAttack = sceneData:isAttackNode(itemInfo)
                    if itemInfo.isMaster==true then
                        local Panel_Star = node:getChildByName("Panel_1"):getChildByName("Panel_Star")
                        local ProjectNode_boss = node:getChildByName("Panel_1"):getChildByName("ProjectNode_boss")
                        if ProjectNode_boss then
                            local Image_Bg = ProjectNode_boss:getChildByName("Panel_RisingStar_Head"):getChildByName("Image_Hero")
                            Image_Bg:loadTexture(model_static_library:getAsset(itemInfo.icon,pbCommon.ObjectType.Monster))
                            cclog("icon:**************************"..itemInfo.icon)
                        end
                        
                        if Panel_Star then
                            Panel_Star:setVisible(canAttack)
                        end
                        local starInfoList = itemInfo.starInfo or{}
                        if Panel_Star then
                            for i = 1, 3 do
                                local spriteStar = Panel_Star:getChildByName("Image_LevelStar_" .. i);
                                if i <= #starInfoList then
                                    local starInfo = starInfoList[i];
                                    if starInfo.flag >= 1 and starInfo.flag <= 3 then
                                        spriteStar:setVisible(starInfo.status > 0);
                                    else
                                        spriteStar:setVisible(false)
                                    end
                                else
                                    spriteStar:setVisible(false)
                                end
                                
                            end
                        end
                    else
                        Button_Point_B = node:getChildByName("Button_Point_Lock")
                    end
                    
                    if canAttack then
                        Button_Point_B:setBright(true)
                        Button_Point_B:setTag(itemInfo.id)
                        Button_Point_B:setTouchEnabled(true)
                        self:addBtnTouchEventListener(Button_Point_B,handler(self,self.onClickNode))
                    else
                        Button_Point_B:setBright(true)
                        Button_Point_B:setTouchEnabled(true)
                        self:addBtnTouchEventListener(Button_Point_B,function(sender,eventType)
                            if eventType == 2 then
                                PopMsg.getInstance():flashShow(LANG("不可以跳过未解锁的战斗点哦！"))
                            end
                            
                            end)
                    end
                else
                        
                end
            else
                item:setVisible(false)
            end
        end
    end
    local currentItem
    if sceneData:isSwitch() then
        curentIndex = GLOBAL_MY_APP.selectStageId
    end
    if type == pbCommon.StageType.NORMAL  then
        currentItem = Panel_Point:getChildByName("Node_"..curentIndex)
    else
        currentItem=Panel_Point:getChildByName("Node_"..model_user.masterPointList[curentIndex])

    end
    
    local Node_Point_Now
    local action
--    if(data[curentIndex].isMaster == false) then
--        Node_Point_Now, action =self:createNode("Node/Chapter_Arrow_Point.csb",true)
--        Node_Point_Now:setScaleX(0.93)
--        Node_Point_Now:setScaleY(0.99)
--    else
--        Node_Point_Now,action =self:createNode("Node/Chapter_Arrow_Point.csb",true)
--        Node_Point_Now:setScale(1.2)
--    end
    Node_Point_Now,action =self:createNode("Node/Chapter_Arrow_Point_Green.csb",true)
    --Node_Point_Now:setPosition(currentItem:getPosition())
    --Node_Point_Now:getChildByName("Panel_1"):setTouchEnabled(false)
    Node_Point_Now:setName("Node_Point_Now")
    currentItem:addChild(Node_Point_Now)
    Node_Point_Now:setVisible(sceneData:isDisplayPoint() or sceneData:isSwitch())
    Node_Point_Now:setPositionY(20)
    currentItem:getChildByName("point"):getChildByName("Panel_1"):getChildByName("Particle_9"):setVisible(sceneData:isDisplayPoint())
    currentItem:getChildByName("point"):getChildByName("Panel_1"):setVisible(true)
    local Icon_Boss_Green_1 = currentItem:getChildByName("point"):getChildByName("Panel_1"):getChildByName("Icon_Boss_Green_1")
    if Icon_Boss_Green_1 then
        Icon_Boss_Green_1:setVisible(sceneData:isDisplayPoint() or sceneData:isSwitch())
    end
    
    
end
local helper_pve = _REQUIRE("model.helper.helper_pve")
function ChapterScene:onClickNode(sender,eventType)
    if eventType == 2 then
        local id =sender:getTag()
        helper_pve:confirmStage(id, function(success, data, code, msg)
            if success then
                self:showDlg("battle/SpotPopupScene",sender:getTag(),data)
            end
        end);
    end
end


function ChapterScene:onClickHandler(sender, eventType)
    if eventType == 2 then
        self:sceneBack(function()
            if GLOBAL_MY_APP.isSwitch == true then
                GLOBAL_MY_APP:setSwitch(false)
            else
                SWITSCENE(ChapterScene.PRESCENE)
            end


        end)
    end
end
return ChapterScene
