
local HeroInfoScene = class("HeroInfoScene", cc.load("mvc").ViewBase)
HeroInfoScene.NEXTSCENE = "battle/ChapterScene"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local helper_monster = _REQUIRE("model/helper/helper_monster.lua")
local model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua");
local model_monster = _REQUIRE("model/static/model_monster.lua")
local model_monsterSkill = _REQUIRE("model.static.model_monsterSkill")
HeroInfoScene.STOPFRAME = 45
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local model_treasureItem =  _REQUIRE "model/static/model_treasureItem.lua"
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
local model_monsterSkillUpType = _REQUIRE("model/static/model_monsterSkillUpType.lua")
local model_skillUpTimesPrice = _REQUIRE("model/static/model_skillUpTimesPrice.lua")

local helper_monster_data = _REQUIRE("model/helper/helper_monster_data.lua")
local helper_relationship = _REQUIRE("model/helper/helper_relationship.lua")
local model_extension_relationship_treasure = _REQUIRE("model/extension/model_extension_relationship_treasure.lua")
local static_model_relationshipItem = _REQUIRE("model/static/model_relationshipItem.lua")
local static_model_relationship = _REQUIRE("model/static/model_relationship.lua")
local static_model_monsterLevel = _REQUIRE "model/static/model_monsterLevel.lua"
local model_equipment = _REQUIRE("model/static/model_equipment.lua")
local porterty = _REQUIRE "common/AttributeName.lua"
local model_user = _REQUIRE("model/model_user")
local model_monster = _REQUIRE("model/static/model_monster")
local model_monsterQualityUpLevelType = _REQUIRE("model/static/model_monsterQualityUpLevelType")

_REQUIRE("pbCommon.lua")

--local pbCommon = _REQUIRE "pb/pbCommon.lua"
--_REQUIRE "src/packages/packet/Packet_Regester"

HeroInfoScene.RESOURCE_FILENAME = "Scene/HeroInfoScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function HeroInfoScene:onCreate()
    --    release_print("========= WinPopupScene create");
    --    local panel_Bottom=self:getResourceNode():getChildByName("Panel_Bottom")
    --    local  button_Retry=panel_Bottom:getChildByName("Button_Retry")
    --    button_Retry:addTouchEventListener(handler(self,self.btnBackCallback)) 

    --release_print(_REQUIRE("battle/ChapterSceneData").type);
    
    self.data = self.args[1]
    self.from = self.args[2]
    self.otherTeam =  self.args[3]
    self.dlgCallBack = self.args[4]
    --self.from = "others"
    self.leftTime = -1
    self.newItemsArr = {}
    self.leftAndRightShow = true
    local Panel_HeroInfoScene = self:getResourceNode():getChildByName("Panel_HeroInfoScene")
    Panel_HeroInfoScene:setTouchEnabled(true)
    self.Panel_Left = Panel_HeroInfoScene:getChildByName("Panel_L"):getChildByName("Panel_Left")
    self.Panel_Right = Panel_HeroInfoScene:getChildByName("Panel_R"):getChildByName("Panel_Right")
    local Panel_Top = Panel_HeroInfoScene:getChildByName("Panel_T"):getChildByName("Panel_Top")
    local Panel_Bg = Panel_HeroInfoScene:getChildByName("Panel_Bg")
    local Panel_Side = self.Panel_Right:getChildByName("Panel_Side")
    self.Panel_Bottom = Panel_Side:getChildByName("Panel_Bottom")
    --self.Panel_Right:getChildByName("Sprite_Quality_Num_39"):setVisible(false)
    SCREEN_TOP(Panel_HeroInfoScene:getChildByName("Panel_T"))
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_CENTER(Panel_HeroInfoScene)
    local Node_Top=Panel_Top:getChildByName("Node_Top")
    self:initTopBar(Node_Top)
    local oldImageView = self.Panel_Right:getChildByName("Image_Role")
    self.Panel_Right:removeChild(oldImageView)
    local ImageView = ccui.ImageView:create()
    ImageView:setTag(1000)
    ImageView:setPosition(304.52,277.66)
    ImageView:setLocalZOrder(-1)
    self.Panel_Right:addChild(ImageView)

    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:sceneBack(function()
                Timer:kill(self.timerId)
--                SWITSCENE("hero/HeroListScene")
            end)
            
            self.dlgCallBack()
            self:removeDlgNoAction()
        end
    end
    )
    --背景图
    if self.from == "mine" then
        Panel_Bg:getChildByName("Image_Bg"):loadTexture("Share/bg/bg_picture01.jpg")
    else
        Panel_Bg:getChildByName("Image_Bg"):loadTexture("Share/bg/popup_bg.jpg")
    end
    self:initUi()
end

function HeroInfoScene:frameStopCallBack()
    --self:initUi()
end

function HeroInfoScene:initUi()
    --    local panel_Bottom=self:getResourceNode():getChildByName("Panel_Bottom")
    --    for i=1, 5 do
    --        local item=panel_Bottom:getChildByName("Node_Item1_"..i)
    --
    --    end
    
    
    self:addPlays(self.Panel_Bottom)


    self:getMembersArr()

    self:initData()
    
end

function HeroInfoScene:addPlays(Panel_Bottom)
    --给武将升阶和升星按钮添加特效
    local btnArr = {Panel_Bottom:getChildByName("Button_Jinjie1"),Panel_Bottom:getChildByName("Button_Jinjie2")}
    self.nodeArr = {}
    for m = 1,2 do
        local node,nodeAction=self:createNode("Node/Node_CircleBtnReady.csb")
        btnArr[m]:addChild(node)
        self.nodeArr[m] = node
        nodeAction:gotoFrameAndPlay(0,true)           
        node:setPosition(cc.p(btnArr[m]:getContentSize().width/2,btnArr[m]:getContentSize().height/2))
        node:setVisible(false)
        --        node:getChildByName("Panel_1"):setTouchEnabled(false)
        --        node:getChildByName("Panel_1"):getChildByName("Panel_Anim1_0"):setTouchEnabled(false)
        --        node:getChildByName("Panel_1"):getChildByName("Button_Jinjie1"):setTouchEnabled(false)
        --        node:getChildByName("Panel_1"):getChildByName("Panel_Anim1"):setTouchEnabled(false)
    end

    --给装备上面添加可进化动画
    self.upArrowNode = {}
    for n = 1,6 do
        local item=self.Panel_Right:getChildByName("Node_Item"..n)
        local Node_HeroInfo_Item = item:getChildByName("Node_HeroInfo_Item")
        local node,nodeAction=self:createNode("Node/Node_UpArrow.csb")
        local Panel_UpArrow = node:getChildByName("Panel_UpArrow"):getChildByName("UpArrow0007_1")
        Node_HeroInfo_Item:addChild(node)
        self.upArrowNode[n] = node
        nodeAction:gotoFrameAndPlay(0,true) 
        local x =  Node_HeroInfo_Item:getContentSize().width -Panel_UpArrow:getContentSize().width*1/5
        local y =  Node_HeroInfo_Item:getContentSize().height-Panel_UpArrow:getContentSize().height/3       
        node:setPosition(cc.p(x ,y))
    end

    --武将升阶成功动画
    local node1,nodeAction1=self:createNode("Node/Node_AdvanceAnim_1.csb")
    local node2,nodeAction2=self:createNode("Node/Node_AdvanceAnim_1b.csb")
    self.Panel_Right:addChild(node1)
    self.Panel_Right:addChild(node2)
    self.node1 = node1
    self.node2 = node2
    self.nodeAction1 = nodeAction1
    self.nodeAction2 = nodeAction2
    nodeAction1:gotoFrameAndPause(0) 
    nodeAction2:gotoFrameAndPause(0)
    node1:setVisible(false)
    node2:setVisible(false)
    local addNum = 17
    node1:setPosition(cc.p(self.Panel_Right:getContentSize().width/2 + addNum,self.Panel_Right:getContentSize().height/2- addNum))
    node2:setPosition(cc.p(self.Panel_Right:getContentSize().width/2+ addNum,self.Panel_Right:getContentSize().height/2- addNum))
    node1:setLocalZOrder(0)
    node2:setLocalZOrder(-2)
    --武将升星成功动画
    local node3,nodeAction3=self:createNode("Node/Node_RisingStar_Anim1.csb")
    local node4,nodeAction4=self:createNode("Node/Node_RisingStar_Anim1b.csb")
    local node5,nodeAction5=self:createNode("Node/Node_RisingStarAnim_1.csb")
    self.Panel_Right:getChildByName("Panel_Side"):addChild(node3)
    self.Panel_Right:addChild(node4)
    self.Panel_Right:addChild(node5)
    self.node3 = node3
    self.node4 = node4
    self.node5 = node5
    self.nodeAction3 = nodeAction3
    self.nodeAction4 = nodeAction4
    self.nodeAction5 = nodeAction5
    self.nodeAction3:gotoFrameAndPause(0) 
    self.nodeAction4:gotoFrameAndPause(0)
    self.nodeAction5:gotoFrameAndPause(0)
    self.node3:setVisible(false)
    self.node4:setVisible(false)
    self.node5:setVisible(false)
    self:setStarPlayPosition()
end

--武将升星成功动画
function HeroInfoScene:setStarPlayPosition()
    local addNum1 = 17
    local star = self.Panel_Right:getChildByName("Panel_Side"):getChildByName("Image_Star_"..(self.data.star))
    self.node3:setPosition(cc.p(star:getPositionX(),star:getPositionY()))
    self.node4:setPosition(cc.p(self.Panel_Right:getContentSize().width/2+ addNum1,self.Panel_Right:getContentSize().height/2- addNum1))
    self.node5:setPosition(cc.p(self.Panel_Right:getContentSize().width/2+ addNum1,self.Panel_Right:getContentSize().height/2- addNum1))
    self.node3:setLocalZOrder(2)
    self.node4:setLocalZOrder(-3)
    self.node5:setLocalZOrder(1)  
end

function HeroInfoScene:initData()
    local Panel_Side = self.Panel_Right:getChildByName("Panel_Side")
    local Panel_Enemy = self.Panel_Right:getChildByName("Panel_Enemy")
    Panel_Enemy:setVisible(false)
    Panel_Side:setVisible(false)
    if self.from == "mine" then
        Panel_Side:setVisible(true)
        self:initSelfData()
    elseif self.from == "others" then
        Panel_Enemy:setVisible(true)
        self:initOtherData()
    elseif self.from == "normal" then
        Panel_Enemy:setVisible(false)
        Panel_Side:setVisible(false) 
        self:initNormalData()
    end
end

function HeroInfoScene:initSelfData()
    local Panel_Side = self.Panel_Right:getChildByName("Panel_Side")
    local Button_left = Panel_Side:getChildByName("ProjectNode_L"):getChildByName("Panel_1"):getChildByName("Button_1")
    local Button_right = Panel_Side:getChildByName("ProjectNode_R"):getChildByName("Panel_1"):getChildByName("Button_1")
    --玩家所有拥有的武将
    self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
    
    for i = 1,table.getn(self.allActiveMonsters) do
        if self.allActiveMonsters[i].id == self.data.id then
            self.currentIndex = i
        end
    end

--    Button_left:setVisible(self.currentIndex ~= 1)
--    Button_right:setVisible(self.currentIndex ~= table.getn(self.allActiveMonsters))

    --左边翻页按钮
    Button_left:setVisible(true)
    Button_left:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            if self.currentIndex > 1 and self.currentIndex <= table.getn(self.allActiveMonsters) then
                self.currentIndex = self.currentIndex - 1
            elseif self.currentIndex == 1 then
                self.currentIndex = table.getn(self.allActiveMonsters)
            end
            cclog("len = "..#(self.allActiveMonsters))
            cclog("currentIndex = "..self.currentIndex)
            self.data = self.allActiveMonsters[self.currentIndex]
            self:initSelfData()
            self:setStarPlayPosition()--
        end
    end
    )
    --右边翻页按钮
    Button_right:setVisible(true)
    Button_right:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            if self.currentIndex >= 1 and self.currentIndex < table.getn(self.allActiveMonsters) then
                self.currentIndex = self.currentIndex + 1
            elseif self.currentIndex == table.getn(self.allActiveMonsters) then
                self.currentIndex = 1
            end
            
            self.data = self.allActiveMonsters[self.currentIndex]
            cclog("len = "..#(self.allActiveMonsters))
            cclog("currentIndex = "..self.currentIndex)
            cclog("id = "..self.data.id)
            self:initSelfData()
            self:setStarPlayPosition()--
        end
    end
    )
    
    --进阶
    local Panel_Bottom = Panel_Side:getChildByName("Panel_Bottom")
    local  Button_Jinjie1=Panel_Bottom:getChildByName("Button_Jinjie1")
    local  Button_Jinjie2=Panel_Bottom:getChildByName("Button_Jinjie2")
    local  Button_Jinjie3=Panel_Bottom:getChildByName("Button_Jinjie3")
--    local  Button_Jinjie4=Panel_Bottom:getChildByName("Button_Jinjie4")
--    local  Button_Jinjie5=Panel_Bottom:getChildByName("Button_Jinjie5")
    local  Button_Chip=Panel_Side:getChildByName("Button_Chip")
--    Text_Jinjie1:setString(LANG("进阶"))
    Button_Jinjie1:setTouchEnabled(true)
    Button_Jinjie2:setTouchEnabled(true)
    Button_Jinjie3:setTouchEnabled(true)
--    Button_Jinjie4:setTouchEnabled(true)
--    Button_Jinjie5:setTouchEnabled(true)
    Button_Jinjie1:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            --            self:showDlg("hero/JinjieOrJinHua",1,self.data)
            local tableJinjie = helper_monster:canMonsterQualityUp(self.data.id)
            if tableJinjie.id == 1 then
                PopMsg.getInstance():flashShow(LANG("武将所有装备进阶到%s才可进阶",tableJinjie.index))
            elseif tableJinjie.id == 2 then
                PopMsg.getInstance():flashShow(LANG("武将等级达到%s级才可进阶",tableJinjie.index))
            else
                local typeId = model_monster:getQualityLevelTypeId(self.data.id);
                local needCoin = model_monsterQualityUpLevelType:getNeedCoin(typeId,self.data.quality)
                self:showDlg("common/CommonPopScene",{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text=LANG("武将进阶，消耗银两%s",needCoin),fun=function(data)
                    if data.cmd=="confirm" then
                        if model_user.coin < needCoin then
                            PopMsg.getInstance():flashShow(LANG("银两不足"))
                        else
                            ctlMonstersQualityUpRequest:sendRequest(handler(self,self.onMonstersQualityUp),self.data.id);
                        end
                        
                    elseif data.cmd =="cancel" then 
    
                    end
                end})
            end
        end
    end
    )
    --升星
--    Text_Jinjie2:setString(LANG("升星"))
    Button_Jinjie2:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            if self.data.star == 5 then
                PopMsg.getInstance():flashShow(LANG("已经升级到最大星级"))
                return
            end
            --判断是否可以升星
            local isJinhua = helper_monster:canMonsterStarUp(self.data.id)
            if isJinhua then
                local starNeedCoin = model_monsterStar:getCoin(self.data.star + 1)
                self:showDlg("common/CommonPopScene",{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text=LANG("武将升星，消耗银两%s",starNeedCoin),fun=function(data)
                    if data.cmd=="confirm" then
                        if model_user.coin < starNeedCoin then
                            PopMsg.getInstance():flashShow(LANG("银两不足"))
                        else
                            self.starArr ={LANG("武力：%s",math.floor(helper_monster_data:calculateStrength(self.data.id))),math.floor(helper_monster_data:calculateStrengthNextStar(self.data.id)),
                                LANG("智力：%s",math.floor(helper_monster_data:calculateIntelligence(self.data.id))),math.floor(helper_monster_data:calculateIntelligenceNextStar(self.data.id)),
                                LANG("统率力：%s",math.floor(helper_monster_data:calculateLeadership(self.data.id))),math.floor(helper_monster_data:calculateLeadershipNextStar(self.data.id)),}
                            ctlMonstersStarUpRequest:sendRequest(handler(self,self.onMonstersStarUp),self.data.id);
                        end
                    elseif data.cmd =="cancel" then 

                    end
                end})
            else
                PopMsg.getInstance():flashShow(LANG("碎片不足，无法升星"))
            end
            --            self:showDlg("hero/JinjieOrJinHua",2)
        end

    end
    )

    --升级
--    Text_Jinjie3:setString(LANG("升级"))
    Button_Jinjie3:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
--            self:showDlg("treasure/TreasureStrongPanel",self.data,function(data)
--                --刷新提升等级的武将信息
--                if data.cmd == "LevelUp" then
--                    self:updateInfo()
--                    self:initPanel(self.type)--
--                end
--            end,1)
            self:showDlg("hero/HeroUpgradeScene",self.data,function(data)
                --刷新提升等级的武将信息
                if data.cmd == "LevelUp" then
                    self:updateInfo()
                    self:initPanel(self.type)--
                end
            end)
        end
    end)
    
    --强化大师
--    Button_Jinjie4:addTouchEventListener(function(sender,eventType)
--        if  eventType == 2 then
--            self:showDlg("hero/ReinforcementScene",self.data,function(data)
--                self:updateInfo()
--                self:initPanel(self.type)--
--            end)
--        end
--    end)
    
 --洗髓
--    if model_user.level >= 25 then
--        Button_Jinjie5:setVisible(true)
--    else
--        Button_Jinjie5:setVisible(false)
--    end
--     Button_Jinjie5:addTouchEventListener(function(sender,eventType)
--         if  eventType == 2 then
--            --self:showDlg("hero/AdvancedScene",self.data.id, 1)
--            self:showDlg("hero/AdvancedListScene")
--            self:updateInfo()
--            self:initPanel(self.type)--
--         end
--     end)

    Button_Chip:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:showDlg("hero/ChipDropScene",{id = self.data.id,type = pbCommon.ObjectType.MonsterPart, needNum = model_monsterStar:getStar(self.data.star+1)}) 
        end 
    end
    )
    
    self:setStarInfo()
    self:initTab()
--
--    --右侧武将信息
    self:rightPanleInfo();
end

function HeroInfoScene:setStarInfo()
    local Panel_Side = self.Panel_Right:getChildByName("Panel_Side")
    --星级显示
    for i = 1,5 do
        local star = Panel_Side:getChildByName("Image_Star_"..i);
        star:setVisible(self.data.star>=i)
    end
    if self.data.star == 5 then
        Panel_Side:getChildByName("LoadingBar_1"):getChildByName("Text_1"):setString(LANG("已达到最高星级"))
        Panel_Side:getChildByName("LoadingBar_1"):setPercent(100)
    else
        --玩家拥有当前武将的碎片数/武将升星所需碎片数-----------------

        local monster = model_monsterStorage:getMonster(self.data.id);
        --需要碎片数量
        local needParts = model_monsterStar:getStar(self.data.star+1);
        --        local needParts = model_monsterStar:getTotalStar(self.data.star);
        --当前碎片数量
        local currentParts = model_monsterPartStorage:getCount(self.data.id);
        Panel_Side:getChildByName("LoadingBar_1"):setPercent(100*currentParts/needParts)
        Panel_Side:getChildByName("LoadingBar_1"):getChildByName("Text_1"):setString((currentParts .."/".. needParts))
    end
end

function HeroInfoScene:initOtherData()
    local Panel_Enemy = self.Panel_Right:getChildByName("Panel_Enemy")
    local Button_left = Panel_Enemy:getChildByName("ProjectNode_L_0"):getChildByName("Panel_1"):getChildByName("Button_1")
    local Button_right = Panel_Enemy:getChildByName("ProjectNode_R_0"):getChildByName("Panel_1"):getChildByName("Button_1")
    
    local space = 3
    local scrollView = Panel_Enemy:getChildByName("ScrollView2")
    local node,item
    node = self:createNode("Node/Node_RisingStar_Head.csb")
    item = node:getChildByName("Panel_RisingStar_Head")
    local contentSize = item:getContentSize()
    
    local num = #(self.otherTeam)
    local sSize = cc.size(math.max(scrollView:getContentSize().width, (contentSize.width + space) * num), 
        contentSize.height + 5);
    scrollView:setInnerContainerSize(sSize)
    
    for i=1,num do
        local newItem
        newItem = item:clone()
        newItem:setTag(i)
        scrollView:addChild(newItem)
        local index = i - 1
       
        local x = (contentSize.width/2) + contentSize.width * index + space

        local y = contentSize.height / 2 + 10
        newItem:setPosition(cc.p(x, y))
        
        local monster = self.otherTeam[i]
        newItem:setVisible(true)
        local Image_Hero = newItem:getChildByName("Image_Hero")
        local Panel_Star = newItem:getChildByName("Panel_Star")
        local Image__Job = newItem:getChildByName("Image__Job")
        Image_Hero:loadTexture(model_static_library:getAsset(monster.id, pbCommon.ObjectType.Monster))
        Image__Job:loadTexture("Job/Icon_job"..model_monster:getType(monster.id)..".png")
        --星级显示
        for i = 1,5 do
            local star = Panel_Star:getChildByName("Image_Star_"..i);
            star:setVisible(self.data.star>=i)
        end
        newItem:setTouchEnabled(true)
        newItem:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                self.data = monster
                self:initOtherData()
            end
        end)
    end
    
    
    --星级显示
    for i = 1,5 do
        local star = Panel_Enemy:getChildByName("Image_Star"..i.."_"..i);
        star:setVisible(self.data.star>=i)
    end
    
    --私聊
    local Button_Chat = Panel_Enemy:getChildByName("Button_Chat");
    Button_Chat:addTouchEventListener(function(sender,eventType)
       if eventType==2 then
           
       end
    end)
    
    local container = scrollView:getInnerContainer()
    
    --左边翻页按钮
    Button_left:setVisible(true)
    Button_left:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            local position = container:getPosition()
            local index = math.floor(position/contentSize.width)
            local targetIndex = index - 1
            local targetPosition = contentSize.width * targetIndex
            local showNum = 4       --最多显示数量
            if targetPosition < (contentSize.width) * (num - showNum) * -1 then
                return
            end
            container:setPositionX(targetPosition)
            --(contentSize.width + space) * num
        end
    end
    )
    --右边翻页按钮
    Button_right:setVisible(true)
    Button_right:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            local position = container:getPosition()
            local index = math.floor(position/contentSize.width)
            local targetIndex = index + 1
            local targetPosition = contentSize.width * targetIndex
            local showNum = 4       --最多显示数量
            if targetPosition > 0 then
                targetPosition = 0
            end
            container:setPositionX(targetPosition)
        end
    end
    )
    
    self:initTab()
    
    --右侧武将信息
    self:rightPanleInfo();
end

function HeroInfoScene:initNormalData()
    self:initTab()
    --右侧武将信息
    self:rightPanleInfo();
end

function HeroInfoScene:initTab()
    self.buttons = {}
    self.panelNames = {}
    if self.from == "mine" then
        self.panelNames ={
            {name="信息",funName = "Panel_Info"},
            {name="宝物",funName = "Panel_Treasure"},
            {name ="技能",funName = "Panel_Skill"},
            {name ="战魂",funName = "Panel_Fighting"}
        }
    elseif self.from == "others" then
        self.panelNames ={
            {name="信息",funName = "Panel_Info"},
            {name="宝物",funName = "Panel_Treasure"},
            {name ="技能",funName = "Panel_Skill"}
        }
    else
        self.panelNames ={
            {name="信息",funName = "Panel_Info"},
            {name ="技能",funName = "Panel_Skill"}
        }
    end
    
    for i=1,4 do
        local btn = self.Panel_Left:getChildByName("Button_Select_"..i)
        self.buttons[i] = btn
        if i <= #(self.panelNames) then
            btn:setVisible(true)
            btn:setTouchEnabled(true)
            btn:addTouchEventListener(function(sender,eventType)
                if eventType ==0 then
                    sender:stopAllActions();
                    sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
                elseif eventType==2 then
                    self:initPanel(sender:getTag())
                end
            end)
        else
            btn:setVisible(false)
            btn:setTouchEnabled(false)
        end
       
    end
    self.treasureData={{id =1,type=1},{id =2,type=2},{id =3,type=3}}
    self:initPanel(1)
end

--升星成功返回
function HeroInfoScene:onMonstersStarUp(success, data, code, msg)
    if(success) then
        local preData = self.data
        model_monsterPartStorage:setCount(data.monster.id,data.monsterPartCount)
        self:updateInfo()
        self:initPanel(self.type)--
        self:setStarPlayPosition()
        self.node3:setVisible(true)
        self.node4:setVisible(true)
        self.node5:setVisible(true)
        self.nodeAction3:gotoFrameAndPlay(0,false) 
        self.nodeAction4:gotoFrameAndPlay(0,false)
        self.nodeAction5:gotoFrameAndPlay(0,false)
        Timer:runOnce(function()
            self:showDlg("hero/JinjieOrJinHua",2,data.monster,preData,self.starArr)
            self.node3:setVisible(false)
            self.node4:setVisible(false)
            self.node5:setVisible(false)
        end,1.5)
        
--        self.dlgCallBack({cmd="MonstersStarUpSuccess"})
    else
    end
end
--进阶成功返回
function HeroInfoScene:onMonstersQualityUp(success, data, code, msg)
    if(success) then
        local preData = self.data
        self:updateInfo()
        self:initPanel(self.type)--
        self:setStarPlayPosition()
        self.node1:setVisible(true)
        self.node2:setVisible(true)
        self.nodeAction1:gotoFrameAndPlay(0,false) 
        self.nodeAction2:gotoFrameAndPlay(0,false)
        Timer:runOnce(function()
            self:showDlg("hero/JinjieOrJinHua",1,data.monster,preData)
            self.node1:setVisible(false)
            self.node2:setVisible(false)
        end,2)
--        self.dlgCallBack({cmd="MonstersQualityUpSuccess"})
    else
    end
end

function HeroInfoScene:rightPanleInfo()
    --等级和战力值（战力值需要自己算，公式暂时莫有配）
    local currentExp = self.data.exp
    local maxExp = static_model_monsterLevel:getExp(self.data.level)
    self.Panel_Right:getChildByName("Text_1"):setString("LV:".. self.data.level .."  "..currentExp.."/"..maxExp.."\n"..LANG("战斗力:" .. self.data.CE))
    
    --名字

    local color,splits = model_monsterStorage:getColor(self.data.id)
    self.Panel_Right:getChildByName("Text_name_9"):setColor(color)
    self.Panel_Right:getChildByName("Sprite_Quality_Num_39"):setColor(color)
--    self.Panel_Right:getChildByName("Text_name_9"):getVirtualRenderer():enableOutline(cc.c4b(44, 25, 3, 255),1)
    self.Panel_Right:getChildByTag(1000):loadTexture(model_monster:getHalf(self.data.id))
    self.Panel_Right:getChildByName("Image_Job"):loadTexture("Job/Icon_job"..model_monster:getType(self.data.id)..".png")

    if table.getn(splits) == 2 then
        self.Panel_Right:getChildByName("Text_name_9"):setString(LANG("%s",model_monster:getName(self.data.id)))
        self.Panel_Right:getChildByName("Sprite_Quality_Num_39"):setString(LANG("+"..splits[2]))
    else
        self.Panel_Right:getChildByName("Text_name_9"):setString(LANG("%s",model_monster:getName(self.data.id)))
        self.Panel_Right:getChildByName("Sprite_Quality_Num_39"):setString(LANG(""))
    end
    
    self.equipArr = {}
    --装备
    for i=1, 6 do
        local item=self.Panel_Right:getChildByName("Node_Item"..i)
        self.equipArr[i] = item:getChildByName("Node_HeroInfo_Item")
        if self.from == "mine" then
            item:getChildByName("Node_HeroInfo_Item"):setTouchEnabled(true)
            item:getChildByName("Node_HeroInfo_Item"):addTouchEventListener(function(sender,eventType)
                if eventType ==0 then
                    sender:stopAllActions();
                    sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
                elseif eventType==2 then
                    --                SWITSCENE("hero/EquipUpgradeScene")
                    --传进去装备信息和武将信息
                    self:showDlg("hero/EquipUpgradeScene",self.data.equipment[i],self.data,function(data)
                        if data.cmd =="MonstersEquipmentLevelUpSuccess" then --装备强化成功回调
                            self:updateInfo()
                            self:initPanel(self.type)--
                        elseif data.cmd =="MonstersEquipmentQualityUpSuccess" then--装备升阶成功回调
                            self:updateInfo()
                            self:initPanel(self.type)--
                        end
                    end)
                end
            end
            )
        else
            item:getChildByName("Node_HeroInfo_Item"):setTouchEnabled(false)
        end
    end

    for j = 1,table.getn(self.equipArr) do
        local item = self.equipArr[j]
        item:getChildByName("Text_2"):setString("LV:"..self.data.equipment[j].level)--等级
        if self.from == "mine" then
--            item:getChildByName("Text_1"):setVisible(helper_monster:canEquipmentQualityUp(self.data.id,self.data.equipment[j].id) == true)--可进化
            self.upArrowNode[j]:setVisible(helper_monster:canEquipmentQualityUp(self.data.id,self.data.equipment[j].id) == true)
        else
--            item:getChildByName("Text_1"):setVisible(false)--可进化
            self.upArrowNode[j]:setVisible(false)
        end
        
        local Image_1 = item:getChildByName("Image_1")
        local Image_2 = item:getChildByName("Image_2")
        Image_1:loadTexture(model_equipment:getIcon(self.data.equipment[j].id,self.data.equipment[j].quality))
        Image_2:loadTexture(model_equipment:getFrameByQuality(self.data.equipment[j].quality))
    end
    self:setStarInfo()
    --判断进阶条件是否满足，满足的话则“进阶”按钮有特效，如点亮
    
    --判断武将是否可以进阶
    self.nodeArr[1]:setVisible(helper_monster:canMonsterQualityUp(self.data.id).id == 0)
    --判断武将是否可以升星
    self.nodeArr[2]:setVisible(helper_monster:canMonsterStarUp(self.data.id) and self.data.star ~= 5)
end

function HeroInfoScene:dlgCallBack(data)
--    if data.cmd =="remove" then
--        self:initPanel(3)
--    elseif data.cmd =="select" then
--        self:initPanel(3)
--    elseif data.cmd == "change" then
--        self:showDlg("favor/SelectHeroPopScene",{type=2,typeId =data.typeId})--选择珍宝
--    end
end

function HeroInfoScene:initPanel(type)
    self.type = type
    self.data  = model_monsterStorage:getMonster(self.data.id);
    self:setIndex(type)
    local Panel_HeroInfoScene = self:getResourceNode():getChildByName("Panel_HeroInfoScene")
--    local Panel_Left = Panel_HeroInfoScene:getChildByName("Panel_L"):getChildByName("Panel_Left")
    local Text_Title_ZB = self.Panel_Left:getChildByName("Text_Title_ZB")

    local panel=nil
    
    local allPanel = { "Panel_Info", "Panel_Treasure", "Panel_Skill", "Panel_Fighting" }
    for i=1,#allPanel do        --隐藏所有页签
        local view = self.Panel_Left:getChildByName(allPanel[i])
        if view then
            view:setVisible(false);
        end
    end

    for i=1,#(self.panelNames) do
        panel  = self.Panel_Left:getChildByName(self.panelNames[i].funName)
        local Button_Select = self.Panel_Left:getChildByName("Button_Select_"..i)
        Button_Select:setTitleText(self.panelNames[i].name)
        if panel then
            panel:setVisible(i==type)
            if i==type then
                handler(self,self[self.panelNames[i].funName])(panel)
                cclog(self.panelNames[i].name)
                Text_Title_ZB:setString(self.panelNames[i].name)
                Text_Title_ZB:setVisible(i ~= 3)
            end
        end
    end

end

function HeroInfoScene:Panel_Info(panel)
	if self.from == "others" then
        self:Panel_Info_Other(panel)
	else
	   self:Panel_Info_All(panel)
	end
end

function HeroInfoScene:Panel_Info_All(panel)
    --    local Panel_Hero_bg = panel:getChildByName("Panel_Hero_bg")
    --    Panel_Hero_bg:setVisible(false)
    --    local originSize = Panel_Hero_bg:getContentSize()
    --    local ScrollView_3 = panel:getChildByName("ScrollView_3")
    --    local Panel_Hero_bg1 =Panel_Hero_bg:clone()
    --    local Panel_Hero_bg2 =Panel_Hero_bg:clone()
    --    local Panel_Hero_bg3 =Panel_Hero_bg:clone()
    --    local Panel_Hero_bg4 =Panel_Hero_bg:clone()
    --    local Panel_Hero_bg5 =Panel_Hero_bg:clone()
    --    local Panel_Hero_bg6 =Panel_Hero_bg:clone()
    --    --Panel_Hero_bg1:setCapInsets(cc.rect(40, 40, 58, 55))
    --    Panel_Hero_bg1:setContentSize(cc.size(originSize.width,80))
    --    ScrollView_3:addChild(Panel_Hero_bg1)
    --    Panel_Hero_bg1:setVisible(true)
    --    Panel_Hero_bg1:setPositionY(200)
    local scrollView = panel:getChildByName("ScrollView_1")
    local monster = self.data
    if self.Image_Bg_Brown == nil then

        local Image_Bg_Brown = scrollView:getChildByName("Image_Bg_Brown")
        local Text_Title_1 = scrollView:getChildByName("Text_Title_1")
        --local Text_Attribute = scrollView:getChildByName("Text_Attribute")
        local Text_Attribute = scrollView:getChildByName("Text_System")     --字体改动，原来字体没有"郃"字
        local Image_6 = scrollView:getChildByName("Image_6")
        local Image_9 = scrollView:getChildByName("Image_9") 
        local Button_2 = scrollView:getChildByName("Button_2")
        local Image_Bg_Brown_Dark = scrollView:getChildByName("Image_Bg_Brown_Dark")
        scrollView:setAnchorPoint(cc.p(0,1))
        --Image_9:removeFromParent(false)
        Image_6:addChild(Image_9)
        Image_9:setPosition(cc.p(43.5,43.5))
        self.Image_Bg_Brown = Image_Bg_Brown
        self.Image_Bg_Brown_Dark = Image_Bg_Brown_Dark
        self.Text_Title_1 = Text_Title_1
        self.Text_Attribute = Text_Attribute
        self.Image_6 = Image_6
        self.Button_2 = Button_2
        self.originSize = self.Image_Bg_Brown:getContentSize()
        panel:addChild(self.Image_Bg_Brown)
        panel:addChild(self.Text_Title_1)
        panel:addChild(self.Text_Attribute)
        panel:addChild(self.Image_6)
        panel:addChild(self.Button_2)
        panel:addChild(self.Image_Bg_Brown_Dark)
        self.Image_Bg_Brown:setVisible(false)
        self.Text_Title_1:setVisible(false)
        self.Text_Attribute:setVisible(false)
        self.Image_6:setVisible(false)
        self.Button_2:setVisible(false)
        self.Image_Bg_Brown_Dark:setVisible(false)
    end
    scrollView:removeAllChildren()

    local space = 20
    local txtSpace = 40
    local txtSize = 22
    --local names = {LANG("武将属性"),LANG("人物缘分"),LANG("珍宝缘分"),LANG("好感度"),LANG("觉醒能力"),LANG("武将简介")}
    local names = {LANG("武将属性"),LANG("人物缘分"),LANG("珍宝缘分"),LANG("武将简介")}
    --
    local relationTreasure = model_extension_relationship_treasure:getAllRelationshipsByMonster(self.data.id)
    local txtHeight = 0
    for i=1,#relationTreasure do
        local detail = relationTreasure[i].detail
        local itemId = detail.itemId
        local f = self.from
        self["relationTreasure_"..i] = self.Text_Attribute:clone()
        
        if self.from == "mine" and relationTreasure[i].activation == true then
            self["relationTreasure_"..i]:setColor(cc.c3b(255,255,255))
        else
            self["relationTreasure_"..i]:setColor(cc.c3b(255,255,255))
        end
       
        --self["relationTreasure_"..i]:setString(LANG("[%s]装备%s%s",detail.name_zhCN,static_model_relationshipItem:getTreasureRelationship(itemId).name_zhCN,detail.description_zhCN))
        self["relationTreasure_"..i]:setString(detail.description_zhCN)
        local strLen = self["relationTreasure_"..i]:getStringLength()
        local size = self["relationTreasure_"..i]:getVirtualRendererSize()
        local height = math.ceil(size.width/ 300) * (size.height +10)
        self["relationTreasure_"..i]:setContentSize(cc.size(300,height))
        self["relationTreasureHeight_"..i] = height
        txtHeight = txtHeight + height
        
        self["relationTreasureName_"..i] = self.Text_Attribute:clone()
        self["relationTreasureName_"..i]:setString(detail.name_zhCN)
        self["relationTreasureName_"..i]:setColor(cc.c3b(75,255,85))
        txtHeight = txtHeight + txtSize + 5
    end
    --
    local relationLevel = helper_relationship:getRelationshipLevel(monster, self.members)
    local relationHeight = 0
    for i=1,#relationLevel do

        local relationItem = static_model_relationship:getRelationItem(relationLevel[i].effect.relationshipId)
        self["relationLevel_"..i] = self.Text_Attribute:clone()
        --self["relationLevel_"..i]:setString(LANG("[%sLv.%s]%s%s",relationItem.name_zhCN,relationLevel[i].effect.level,relationItem.description_zhCN,relationLevel[i].effect.description_zhCN))
        local str = relationItem.description_zhCN..relationLevel[i].effect.description_zhCN
        self["relationLevel_"..i]:setString(str)
        if relationLevel[i].actived == true then
            self["relationLevel_"..i]:setColor(cc.c3b(255,0,0))
        else
            self["relationLevel_"..i]:setColor(cc.c3b(255,255,255))
        end
        local strLen = self["relationLevel_"..i]:getStringLength()
        local size = self["relationLevel_"..i]:getVirtualRendererSize()
        local height = math.ceil(size.width/ 300) * (size.height +10)
        self["relationLevel_"..i]:setContentSize(cc.size(300,height))
        self["relationLevelHeight_"..i] = height
        relationHeight = relationHeight + height
        
        self["relationName_"..i] = self.Text_Attribute:clone()
        self["relationName_"..i]:setColor(cc.c3b(75,255,85))
        local name = relationItem.name_zhCN.."Lv"..relationLevel[i].effect.level
        self["relationName_"..i]:setString(name)
        relationHeight = relationHeight + txtSize + 5
    end

    --
    local baseAttibute = helper_monster_data:getBaseMonsterProp(self.data.id)
    local finalAttibute = helper_monster_data:getFinalMonsterProp(self.data.id)
    local txtCount = 0
    for key,value in pairs(finalAttibute) do
        if (tonumber(value) ~= 0 and key ~= "Strength" and key ~= "Intelligence" and key ~= "Leadership") then
            txtCount = txtCount + 1
        end
    end 

    local txtDesc = self.Text_Attribute:clone()
    txtDesc:setString(monster.description)
    local strLen = txtDesc:getStringLength()
    local size = txtDesc:getVirtualRendererSize()
    local txtDescHeight = math.ceil(size.width/ self.originSize.width) * size.height
    txtDesc:setContentSize(cc.size(300,txtDescHeight))
    txtDesc:setVisible(true)
    --self.size5 = 120 + txtHeight
    self.size1 =(txtCount +6)*  txtSpace + 200
    self.size2 = relationHeight + 120
    self.size3 = txtHeight + 120

    --self.size4 = 100              好感度
    --self.size5 = 100              觉醒能力
    self.size4 = txtDescHeight + 120
    local innerHeight = 0
    for i=1,4 do
        innerHeight = innerHeight +self["size"..i]
    end
    --TODO
    innerHeight = innerHeight + #names * 26
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,innerHeight));
    local currentY = sSize.height
    scrollView:setInnerContainerSize(sSize)
    
    local Bg_Dark = self.Image_Bg_Brown:clone()
    Bg_Dark:setVisible(true)
    Bg_Dark:setContentSize(cc.size(self.originSize.width, innerHeight))
    --Bg_Dark:setAnchorPoint(cc.p(0.5,0.5))
    scrollView:addChild(Bg_Dark)
    --Bg_Dark:setPosition(cc.p(sSize.width/2,innerHeight-txtSpace))
    Bg_Dark:setPosition(cc.p(self.originSize.width/2,currentY))
    --TODO

    for i =1,#names do
        self["bg"..i]=self.Image_Bg_Brown_Dark:clone()
        scrollView:addChild(self["bg"..i])
        self["bg"..i]:setVisible(true)
        --TODO
        self["bg"..i]:setContentSize(cc.size(self.originSize.width - 30,self["size"..i]))
        if i == 1 then
            currentY = currentY - space
        else
            currentY = currentY - self["size"..(i-1)] - space
        end

        self["bg"..i]:setPosition(cc.p(self.originSize.width/2,currentY))
        self["title_"..i] = self.Text_Title_1:clone()
        self["title_"..i]:setVisible(true)
        self["title_"..i]:setString(names[i])
        self["title_"..i]:ignoreContentAdaptWithSize(true)
        self["title_"..i]:setAnchorPoint(cc.p(0.5,0.5))
        --self["title_"..i]:setPosition(cc.p(sSize.width/2,self["size"..i]-txtSpace))
        self["title_"..i]:setPosition(cc.p(self["bg"..i]:getContentSize().width/2,self["size"..i]-txtSpace))
        self["bg"..i]:addChild(self["title_"..i])
    end
    --标题改动
--    for i =1,#names do
--        self["title_"..i] = self.Text_Title_1:clone()
--        self["title_"..i]:setVisible(true)
--        self["title_"..i]:setString(names[i])
--        self["title_"..i]:ignoreContentAdaptWithSize(true)
--        local size = self["title_"..i]:getVirtualRendererSize()
--        local height = size.height
--        self["title_"..i]:setAnchorPoint(cc.p(0.5,0.5))
--        if i == 1 then
--            currentY = currentY
--        else
--            currentY = currentY - self["size"..(i-1)]
--        end
--        self["title_"..i]:setPosition(cc.p(self.originSize.width/2,currentY))
--        scrollView:addChild(self["title_"..i])
--        local s = self["size"..(i-1)]
--       
--        currentY = currentY - height
--        
--        self["bg"..i]=self.Image_Bg_Brown:clone()
--        scrollView:addChild(self["bg"..i])
--        self["bg"..i]:setVisible(true)
--        self["bg"..i]:setContentSize(cc.size(self.originSize.width,self["size"..i]))
--
--        self["bg"..i]:setPosition(cc.p(self.originSize.width/2,currentY))
--        
--    end
    --
    local id = self.data.id
    --    local wu = helper_monster_data:calculateStrength(id)
    --    local zhi = helper_monster_data:calculateStrength(id)
    --    local tong = helper_monster_data:calculateLeadership(id)

    local wu = math.floor(finalAttibute["Strength"])
    local zhi = math.floor(finalAttibute["Intelligence"])
    local tong = math.floor(finalAttibute["Leadership"])

    local atts1 = {LANG("等级：%s/%s",monster.level,99),LANG("武力：%s",wu),LANG("智力：%s",zhi),LANG("统率：%s",tong),LANG("当前兵种：%s",monster.soldier.name), LANG("最大带兵数: %s", finalAttibute["MaxSoldier"]),LANG("阵型：")}
    local numLevel =0
    for i=1 ,#atts1 do
        numLevel = numLevel + 1
        local txt = self.Text_Attribute:clone()
        txt:setVisible(true)
        txt:setString(atts1[i])
        --TODO
        txt:setPosition(cc.p(25,self.size1-txtSpace*(numLevel+1)))
        self.bg1:addChild(txt)
    end

    for i=1,#monster.formations do
        if monster.formations[i].formationId ~= 0 then
            local formation = self.Image_6:clone()
            formation:setVisible(true)
            self.bg1:addChild(formation)
            formation:setScale(0.8)
            formation:setAnchorPoint(cc.p(0,1))
            formation:setPosition(cc.p(40 + (i-1)*formation:getContentSize().width*0.8,self.size1-txtSpace*(numLevel+1)-10))
        end

    end
    local rate ={AtkHitRate="物理命中率:",
        AtkDodgeRate="物理闪避率:",
        AtkCSR="物理暴击率:",
        AbilityCSR="法术暴击率:",
        AtkAntiCSR="物理抗暴率:",
        AbilityAntiCSR="法术抗暴率:",
        PhysicalCSDR="物理爆伤率:",
        SpellCSDR="法术爆伤率:",
        PhysicalResilience="物理韧性率:",
        SpellResilience="法术韧性率:",
        ArPR="物理穿透率:",
        SRPR="法术穿透率:",
        PhysicalDR="物伤减免率:",
        SpellDR="法伤减免率:",
        AtkVampRate="物理吸血率:",
        AbilityVampRate="法术吸血率:"}
 
    for index,name in pairs(porterty.sequence) do 
        local key = name
        local value = finalAttibute[key]
        
        if (tonumber(value) ~= 0) then
            numLevel = numLevel + 1
            local txt = self.Text_Attribute:clone()
            txt:setVisible(true)
            local final = tonumber(value)
            local base = tonumber(baseAttibute[key])
            local txtSub = ""
            if  final-base ~= 0 then
                if rate[key] ~= nil then
                    txtSub = "+ "..(final-base)*100 .. "%"
                else
                    txtSub = "+ "..(final-base)
                end

            end
            local txtBase =0
            if rate[key] ~= nil then
                txtBase = "+ "..base*100 .. "%"
            else
                txtBase = math.floor(base)
            end
            txt:setString(LANG("%s %s %s",porterty.data[key],txtBase,txtSub))
            txt:setPosition(cc.p(25,self.size1-txtSpace*(numLevel+1) -10 - self.Image_6:getContentSize().height*0.8))
            self.bg1:addChild(txt)
        end
    end

    local innerLevelHeight =0
    for i=1,#relationLevel do
        self["relationLevel_"..i]:setVisible(true)
        self["relationName_"..i]:setVisible(true)
        cclog("dddddddd"..self["relationLevelHeight_"..i])
        
        self["relationName_"..i]:setPosition(cc.p(25,self.title_2:getPositionY() -txtSpace - innerLevelHeight))
        innerLevelHeight = innerLevelHeight + txtSize + 5
        self["relationLevel_"..i]:setPosition(cc.p(25,self.title_2:getPositionY() -txtSpace - innerLevelHeight))
        --标题改动
        --self["relationLevel_"..i]:setPosition(cc.p(50, self.bg2:getContentSize().height -txtSpace - innerLevelHeight))
        self["relationLevel_"..i]:setAnchorPoint(cc.p(0,1))
        self["relationLevel_"..i]:ignoreContentAdaptWithSize(false)
        self.bg2:addChild(self["relationLevel_"..i])
        
        self["relationName_"..i]:setAnchorPoint(cc.p(0,1))
        self["relationName_"..i]:ignoreContentAdaptWithSize(false)
        self.bg2:addChild(self["relationName_"..i])
        innerLevelHeight = innerLevelHeight + self["relationLevelHeight_"..i]
    end

    local innerRelationHeight =0
    for i=1,#relationTreasure do
        self["relationTreasure_"..i]:setVisible(true)
        self["relationTreasureName_"..i]:setVisible(true)
        self["relationTreasureName_"..i]:setPosition(cc.p(25,self.title_3:getPositionY() -txtSpace - innerRelationHeight))
        innerRelationHeight = innerRelationHeight + txtSize + 5
        self["relationTreasure_"..i]:setPosition(cc.p(25,self.title_3:getPositionY() -txtSpace - innerRelationHeight))
        --标题改动
        --self["relationTreasure_"..i]:setPosition(cc.p(50,self.bg3:getContentSize().height -txtSpace - innerRelationHeight))
        self["relationTreasure_"..i]:setAnchorPoint(cc.p(0,1))
        self["relationTreasure_"..i]:ignoreContentAdaptWithSize(false)
        self.bg3:addChild(self["relationTreasure_"..i])
        
        self["relationTreasureName_"..i]:setAnchorPoint(cc.p(0,1))
        self["relationTreasureName_"..i]:ignoreContentAdaptWithSize(false)
        self.bg3:addChild(self["relationTreasureName_"..i])
        innerRelationHeight = innerRelationHeight + self["relationTreasureHeight_"..i]
    end

    self.bg4:addChild(txtDesc)
    txtDesc:setPosition(cc.p(50,self.title_4:getPositionY() - txtSpace))
    txtDesc:setAnchorPoint(cc.p(0,1))
    txtDesc:ignoreContentAdaptWithSize(false)
end

function HeroInfoScene:Panel_Info_Other(panel)
    local scrollView = panel:getChildByName("ScrollView_1")
    local monster = self.data
    if self.Image_Bg_Brown == nil then

        local Image_Bg_Brown = scrollView:getChildByName("Image_Bg_Brown")
        local Text_Title_1 = scrollView:getChildByName("Text_Title_1")
        --local Text_Attribute = scrollView:getChildByName("Text_Attribute")
        local Text_Attribute = scrollView:getChildByName("Text_System")     --字体改动，原来字体没有"郃"字
        local Image_6 = scrollView:getChildByName("Image_6")
        local Image_9 = scrollView:getChildByName("Image_9") 
        local Button_2 = scrollView:getChildByName("Button_2")
        scrollView:setAnchorPoint(cc.p(0,1))
        Image_9:removeSelf()
        Image_6:addChild(Image_9)
        Image_9:setPosition(cc.p(43.5,43.5))
        self.Image_Bg_Brown = Image_Bg_Brown
        self.Text_Title_1 = Text_Title_1
        self.Text_Attribute = Text_Attribute
        self.Image_6 = Image_6
        self.Button_2 = Button_2
        self.originSize = self.Image_Bg_Brown:getContentSize()
        panel:addChild(self.Image_Bg_Brown)
        panel:addChild(self.Text_Title_1)
        panel:addChild(self.Text_Attribute)
        panel:addChild(self.Image_6)
        panel:addChild(self.Button_2)
        self.Image_Bg_Brown:setVisible(false)
        self.Text_Title_1:setVisible(false)
        self.Text_Attribute:setVisible(false)
        self.Image_6:setVisible(false)
        self.Button_2:setVisible(false)
    end
    scrollView:removeAllChildren()
    local space = 20
    local txtSpace = 40
    local names = {LANG("武将属性"),LANG("武将简介")}
    
    --计算高度
    local txtDesc = self.Text_Attribute:clone()
    txtDesc:setString(monster.description)
    local strLen = txtDesc:getStringLength()
    local size = txtDesc:getVirtualRendererSize()
    local txtDescHeight = math.ceil(size.width/ self.originSize.width) * size.height
    txtDesc:setContentSize(cc.size(300,txtDescHeight))
    txtDesc:setVisible(true)
    
    self.size1 = (40 + 6) * 3 + 180
    self.size2 = txtDescHeight + 120
    local innerHeight = 0
    for i=1,2 do
        innerHeight = innerHeight +self["size"..i]
    end
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,innerHeight));
    local currentY = sSize.height
    scrollView:setInnerContainerSize(sSize)
    
    for i =1,#names do
        self["bg"..i]=self.Image_Bg_Brown:clone()
        scrollView:addChild(self["bg"..i])
        self["bg"..i]:setVisible(true)
        self["bg"..i]:setContentSize(cc.size(self.originSize.width,self["size"..i]))
        if i == 1 then
            currentY = currentY
        else
            currentY = currentY - self["size"..(i-1)]
        end

        self["bg"..i]:setPosition(cc.p(self.originSize.width/2,currentY))
        self["title_"..i] = self.Text_Title_1:clone()
        self["title_"..i]:setVisible(true)
        self["title_"..i]:setString(names[i])
        self["title_"..i]:ignoreContentAdaptWithSize(true)
        self["title_"..i]:setAnchorPoint(cc.p(0.5,0.5))
        self["title_"..i]:setPosition(cc.p(sSize.width/2,self["size"..i]-txtSpace))
        self["bg"..i]:addChild(self["title_"..i])
    end
    
    
    local atts1 = {LANG("等级：%s/%s",monster.level,99),LANG("当前兵种：%s",monster.soldier.name),LANG("阵型：")}
    local numLevel =0
    for i=1 ,#atts1 do
        numLevel = numLevel + 1
        local txt = self.Text_Attribute:clone()
        txt:setVisible(true)
        txt:setString(atts1[i])
        txt:setPosition(cc.p(50,self.size1-txtSpace*(numLevel+1)))
        self.bg1:addChild(txt)
    end

    for i=1,#monster.formations do
        if monster.formations[i].formationId ~= 0 then
            local formation = self.Image_6:clone()
            formation:setVisible(true)
            self.bg1:addChild(formation)
            formation:setScale(0.8)
            formation:setAnchorPoint(cc.p(0,1))
            formation:setPosition(cc.p(40 + (i-1)*formation:getContentSize().width*0.8,self.size1-txtSpace*(numLevel+1)-10))
        end

    end
    
    self.bg2:addChild(txtDesc)
    txtDesc:setPosition(cc.p(50,self.title_2:getPositionY() - txtSpace))
    txtDesc:setAnchorPoint(cc.p(0,1))
    txtDesc:ignoreContentAdaptWithSize(false)
end

function HeroInfoScene:Panel_Treasure(panel)
    local monster = self.data;
    local treasureItems = monster.treasureItems
    for i=1,4 do
        local Button_Icon = panel:getChildByName("Button_Icon_"..i)
        Button_Icon:setEnabled(false)
    end
    for i=1,#treasureItems do
        local Button_Icon = panel:getChildByName("Button_Icon_"..i)
        Button_Icon:setTag(i)
        Button_Icon:setEnabled(false)
        if self.from == "mine" then
            Button_Icon:setEnabled(true)
            Button_Icon:addTouchEventListener(function(sender,eventType)
                if eventType ==2 then
                    if (tonumber(treasureItems[sender:getTag()]["inventoryTreasureItemId"]) ~= 0)  then
                        self:showDlg("treasure/TreasureDetailsPanel",treasureItems[sender:getTag()],monster.id,function(data)
                            self:initPanel(2)
                            self:updateInfo()
                        end)
                    else
                        self:showDlg("treasure/TreasureListPanel",{1,treasureItems[sender:getTag()].slotId,monster.id,function(data)
                            self:initPanel(2)
                            self:updateInfo()
                        end})--选择珍宝
                    end

                end
            end
            )
        end
        
        local treasureItem = treasureItems[i]
        local Node_Icon = panel:getChildByName("Node_Icon_"..i)
        local Panel_Treasure_Icon = Node_Icon:getChildByName("Panel_Treasure_Icon")
        local Image_Icon = Panel_Treasure_Icon:getChildByName("Image_Icon")
        local Image_Add_0 = Panel_Treasure_Icon:getChildByName("Image_Add_0")
        local Image_Add = Panel_Treasure_Icon:getChildByName("Image_Add")
        local Image_1 = Panel_Treasure_Icon:getChildByName("Image_1")
        local Image_Add = Panel_Treasure_Icon:getChildByName("Image_Add")
        local Image_Lock = Panel_Treasure_Icon:getChildByName("ProjectNode_3")
        local isEquiped = (tonumber(treasureItem["inventoryTreasureItemId"]) ~= 0)
        
        local Image_Num1 = panel:getChildByName("Image_Num"..i)
        local Text_Num1 = Image_Num1:getChildByName("Text_Num"..i)
        
        local Text_Name = Panel_Treasure_Icon:getChildByName("Text_Name")
        Image_Add:setVisible(isEquiped == false)
        Image_Icon:setVisible(isEquiped == true)
        Image_Lock:setVisible(false)
        Text_Name:setString("")
        Image_Num1:setVisible(isEquiped)
        if isEquiped then
            local data = model_treasureItemStorage:getNode(treasureItem["inventoryTreasureItemId"])
            Text_Num1:setString(data.level)
            Image_Icon:loadTexture(model_static_library:getAsset(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
            Image_1:loadTexture(model_static_library:getFrame(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
            Text_Name:setString(model_treasureItem:getName(data.treasureItemId))
            Text_Name:setColor( model_static_library:getColor(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
            --Image_Bg:loadTexture(model_static_library:getFrame(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
        else
            Image_1:loadTexture("frame/frame_1.png")
        end
        Image_Add_0:setVisible(isEquiped == false)
        Image_Add:setVisible(isEquiped == false)
    end
end

function HeroInfoScene:Panel_Fighting(panel)
end

function HeroInfoScene:Panel_Skill(panel)
    self.skillPanel = panel
    local scrollView = panel:getChildByName("ScrollView_1")
    local node=self:createNode("Node/Node_HeroInfo_SkillItem.csb")
    local item = node:getChildByName("Panel_HeroInfo_SkillItem")

    local contentSize =item:getContentSize()
    self.space = 0
    local num = table.getn(self.data.skills)
    --scrollView:setContentSize(cc.size(419, 452))
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*math.ceil(num)));
    scrollView:setInnerContainerSize(sSize)
    
    for i=1,num do
        if i>table.getn(self.newItemsArr) then
            local newItem = item:clone()
            local Image_Skill1_Icon = newItem:getChildByName("Image_Skill1_Icon")

            --newItem:setAnchorPoint(cc.p(0,0))
            newItem:setTouchEnabled(false);
            if self.from == "mine" then
                newItem:setTouchEnabled(true);
                newItem:addTouchEventListener(function(sender,eventType)
                    if eventType ==2 then
                        cclog("touchtouche"..sender:getTag())
                        self:showDlg("hero/SkillTipsScene",self.data.skills[i])
                    end
                end
                )
            end


            local index = i-1
            newItem:setTag(index);
            scrollView:addChild(newItem)

            local x=(sSize.width/2)
            local y=sSize.height-(contentSize.height+self.space)*math.floor(index)-contentSize.height/2
            newItem:setPosition(cc.p(x,y))
            self.newItemsArr[i] = newItem
        end
    end
    
    if self.from == "mine" then
        self:updateSkillInfo()
    else
        self:visibleSkillInfo()
    end
end

function HeroInfoScene:visibleSkillInfo()
    for i = 1,table.getn(self.data.skills) do
        local Text_JNMC = self.newItemsArr[i]:getChildByName("Text_JNMC")
        local Text_Level_Num = self.newItemsArr[i]:getChildByName("Text_Level_Num")
        local Text_Cost = self.newItemsArr[i]:getChildByName("Text_Cost")
        local Button_Go_3 = self.newItemsArr[i]:getChildByName("Button_Go_3")
        local Image_Skill1_Icon = self.newItemsArr[i]:getChildByName("Image_Skill1_Icon")
        local Image_Cash = self.newItemsArr[i]:getChildByName("Image_Cash")
        local Text_1 = self.newItemsArr[i]:getChildByName("Text_1")
        
        Text_JNMC:setString(LANG(model_monsterSkill:getName(self.data.skills[i].id)))
        Text_Level_Num:setString("LV"..self.data.skills[i].level)
        Button_Go_3:setEnabled(false)
        Button_Go_3:setVisible(false)
        Text_Cost:setVisible(false)
        Image_Cash:setVisible(false)
        Text_1:setVisible(false)
    end
    local Text_Item_1 = self.skillPanel:getChildByName("Text_Item_1")
    local Text_Item_2 = self.skillPanel:getChildByName("Text_Item_2")
    local Button_1 = self.skillPanel:getChildByName("Button_1")
    local Text_Item_3 = self.skillPanel:getChildByName("Text_Item_3")
    Text_Item_3:setVisible(false)
    Button_1:setVisible(false)
    Text_Item_1:setString(LANG("                      技能"))
    Text_Item_2:setString(LANG(""))
end

function HeroInfoScene:updateSkillInfo()
    --判断技能是否能升级的三个条件：1.技能等级不能超过武将等级，否则不能升级 2：技能是否解锁 3：技能等级是否达到最大等级
    local Text_Item_1 = self.skillPanel:getChildByName("Text_Item_1")
    local Text_Item_2 = self.skillPanel:getChildByName("Text_Item_2")
    for i = 1,table.getn(self.data.skills) do
        local Text_JNMC = self.newItemsArr[i]:getChildByName("Text_JNMC")
        local Text_Level_Num = self.newItemsArr[i]:getChildByName("Text_Level_Num")
        local Text_Cost = self.newItemsArr[i]:getChildByName("Text_Cost")
        local Button_Go_3 = self.newItemsArr[i]:getChildByName("Button_Go_3")
        local Image_Skill1_Icon = self.newItemsArr[i]:getChildByName("Image_Skill1_Icon")
        local Image_Cash = self.newItemsArr[i]:getChildByName("Image_Cash")  

        Text_JNMC:setString(LANG(model_monsterSkill:getName(self.data.skills[i].id)))
        local costNum = model_monsterSkillUpType:getCoin(model_monsterSkill:getLevelUpType(self.data.skills[i].id),self.data.skills[i].level+1)
        Text_Cost:setString(costNum)
        if costNum > model_user.coin then
            Text_Cost:setColor(cc.c3b(0, 0, 255))--红色
        else
            Text_Cost:setColor(cc.c3b(255, 255, 255))--白色
        end

        --技能图标
        Image_Skill1_Icon:loadTexture("icon_skill/icon_skill_"..self.data.skills[i].id..".png")
        local maxLevelOffset = model_monsterSkill:getMaxLevelOffset(self.data.skills[i].id)
        
        --1.解锁了并且升级到目前的最高级：灰色升级按钮+钱
        --2.解锁了并且升级到设计的最高级：灰色升级按钮，不加钱
        --3.未解锁：不显示升级按钮，钱，技能等级，显示文字：进阶到绿/蓝/紫色后解锁，图片灰
        --4.解锁了并且可升级：亮的升级按钮+钱
        local boo = maxLevelOffset + self.data.skills[i].level < self.data.level
        if self.data.skills[i].isUnlock then --判断是否解锁
            if self.data.skills[i].level >=  self.data.level then --升级到目前的最高级
                self:setSkillStatus(self.newItemsArr[i],false,true,0)
            elseif not(boo) then --是否升级到设计的最高级
                self:setSkillStatus(self.newItemsArr[i],false,false,0)
        	else --解锁了并且可升级
                self:setSkillStatus(self.newItemsArr[i],true,true,0)
        	end
            Text_Level_Num:setString("LV"..self.data.skills[i].level)
        else --未解锁
            self:setSkillStatus(self.newItemsArr[i],false,false,i)
        end
        
--        Button_Go_3:setBright(self.data.skills[i].isUnlock and  boo) 
--        Button_Go_3:setTouchEnabled(self.data.skills[i].isUnlock and boo)
--        Text_Cost:setVisible(boo)
--        Image_Cash:setVisible(boo)
        Button_Go_3:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                if model_user.skillUpPoint <= 0 then
                    self:showSkillConsume()
                else
                    ctlMonstersSkillUpRequest:sendRequest(handler(self,self.onMonsterSkillUp), self.data.id, self.data.skills[i].id);
                end
            end
        end
        )

    end
    
    Text_Item_1:setString(LANG("剩余技能点数%s",model_user.skillUpPoint)) 
    if model_user.skillUpPoint == model_user.skillUpPointMax then
--    if(model_user.time_to_next_skillUpPoint <= 0)  then
        Text_Item_2:setString(LANG("(点数已满)"))
    else
        if self.leftTime == -1 then
            self.leftTime = model_user.time_to_next_skillUpPoint - model_user:getCurrentServerTimestamp()

            --恢复倒计时
            --            local timerId
            self.timerId = Timer:start(function()
                if self.leftTime >= 0 then
                    local second = math.floor(self.leftTime % 60);
                    local minute = math.floor((self.leftTime - second) % 3600 / 60);
                    Text_Item_2:setString(LANG("(%s:%s后获得1点)",minute, second))
                    --                    Text_Item_2:setString(LANG("%s:%s",minute, second))
                    self.leftTime  = self.leftTime - 1
                else
                    Text_Item_1:setString(LANG("剩余技能点数%s:",model_user.skillUpPoint)) 
                    Timer:kill(self.timerId)
                    if model_user.time_to_next_skillUpPoint <= 0 then
                        Text_Item_2:setString(LANG("(点数已满)"))
                    end
                end
            end, 1, nil)
        end
    end


    --购买按钮
    local Button_1 = self.skillPanel:getChildByName("Button_1")
    local Text_Item_3 = self.skillPanel:getChildByName("Text_Item_3")
    if model_user.skillUpPoint <= 0 then
        Button_1:setVisible(true)
        Text_Item_3:setVisible(true)
        Text_Item_1:setVisible(false)
        Button_1:setBright(true)
        Button_1:setTouchEnabled(true)
        Button_1:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                self:showSkillConsume()
            end
        end
        )
    else
        Button_1:setVisible(false)
        Button_1:setTouchEnabled(false)
        Text_Item_3:setVisible(false)
        Text_Item_1:setVisible(true)
    end
end

--提示购买是否买技能点以及花费
function HeroInfoScene:showSkillConsume()
    local requireCash = model_user.skillUpPointBuyMaxCash
    if model_skillUpTimesPrice:getCash(model_user:getDailyBuySkillUpPointTimes() + 1) ~= "" then
--        requireCash = model_skillUpTimesPrice:getCash(model_user.skill_up_point_buy_times)
        requireCash = model_skillUpTimesPrice:getCash(model_user:getDailyBuySkillUpPointTimes() + 1)
    else 
--        requireCash = model_skillUpTimesPrice:getCash(1)
    end
    self:showDlg("common/CommonPopScene",{confirmStr=LANG("购买"),cancelStr=LANG("取消"),text=LANG("购买%s点技能点数需要花费%s元宝，是否购买？",model_user.skillUpPointMax,requireCash),fun=function(data)
        if data.cmd=="confirm" then--购买技能点数
            ctlPlayerBuySkillUpPointRequest:sendRequest(handler(self,self.onPlayerBuySkillUpPoint));
        elseif data.cmd =="cancel" then 
        end
    end})
end

function HeroInfoScene:setSkillStatus(item,isBtnBright,isCoinShow,isWriting)
    local Button_Go_3 = item:getChildByName("Button_Go_3")
    local Text_Cost = item:getChildByName("Text_Cost")
    local Image_Cash = item:getChildByName("Image_Cash")
    local Text_Level_Num = item:getChildByName("Text_Level_Num")
    local Text_1 = item:getChildByName("Text_1")
    Button_Go_3:setBright(isBtnBright) 
    Button_Go_3:setTouchEnabled(isBtnBright)
    Button_Go_3:setVisible(isWriting == 0)
    Text_Cost:setVisible(isCoinShow)
    Image_Cash:setVisible(isCoinShow)
    Text_Level_Num:setVisible(isWriting == 0)
    Text_1:setVisible(isWriting ~= 0)
    local arr = {"绿","蓝","紫"}
    if isWriting ~= 0  then
        Text_1:setString(LANG("进阶到%s色后解锁",arr[isWriting - 1]))
    end
end

function HeroInfoScene:updateInfo()
    --自己所有的武将列表
    self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
    for i = 1,table.getn(self.allActiveMonsters) do
        if self.allActiveMonsters[i].id == self.data.id then
            self.data = self.allActiveMonsters[i]
        end
    end
    self:rightPanleInfo()
end

function HeroInfoScene:onMonsterSkillUp(success, data, code, msg)
    if(success) then
        self.data = data.monster
        self:updateSkillInfo()
        self:updateInfo()
        PopMsg.getInstance():flashShow(LANG("剩余技能点数%s",model_user.skillUpPoint))
    else
    end
end

function HeroInfoScene:onPlayerBuySkillUpPoint(success, data, code, msg)
    if(success) then
        self.leftTime = -1
        self:updateSkillInfo()
    else
    end
end
--是否是自己的数据，是则有members，用于显示缘分是否启用
function HeroInfoScene:getMembersArr()
    self.members = {}
    if self.from ~= "mine" then
        return
    end
    
    if model_user.battleTeam == nil then
        return
    end

    for key, value in pairs(model_user.battleTeam) do
        if(value.monsterId > 0) then
            table.insert(self.members,value.monsterId)
            cclog("monsterId = "..value.monsterId)
        end
    end
end


return HeroInfoScene
