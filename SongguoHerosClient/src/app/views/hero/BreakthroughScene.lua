local BreakthroughScene = class("BreakthroughScene", cc.load("mvc").ViewBase)
BreakthroughScene.NEXTSCENE = "battle/ChapterScene"
BreakthroughScene.RESOURCE_FILENAME = "Scene/DivineScene1.csb"
BreakthroughScene.STOPFRAME = 100

local model_user = _REQUIRE("model/model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage")
local model_monster = _REQUIRE("model/static/model_monster")
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
local model_monsterDestiny = _REQUIRE "model/static/model_monsterDestiny.lua"
local model_monster = _REQUIRE("model/static/model_monster")
local helper_monster = _REQUIRE("model/helper/helper_monster.lua")
local model_breakthrough = _REQUIRE "model/static/model_breakthrough.lua"
local model_equipment = _REQUIRE("model/static/model_equipment.lua")
_REQUIRE("pbCommon.lua")

function BreakthroughScene:onCreate()
    self.data = self.args[1]
    self.monsterId = self.data.monster.id
    self.callback = self.data.callback        --刷新HeroInfSceneUI 
    self.from = self.data.from
    self:initUi()
end

function BreakthroughScene:initUi()
    local DivineScene = self:getResourceNode():getChildByName("DivineScene1")
    DivineScene:setTouchEnabled(true)
    self.Panel_Center = DivineScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    self.Panel_Top = DivineScene:getChildByName("Panel_T")
    self.Panel_Bottom = DivineScene:getChildByName("Panel_B"):getChildByName("Panel_Bottom")
    self.Panel_Ani = DivineScene:getChildByName("Panel_Ani")
    local Panel_Bg = DivineScene:getChildByName("Panel_Bg")
    SCREEN_TOP(DivineScene:getChildByName("Panel_T"))
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_BOTTOM(DivineScene:getChildByName("Panel_B"))

    self.Panel_1 = self.Panel_Center:getChildByName("Panel_1")
    self.Panel_1_B = self.Panel_Bottom:getChildByName("Panel_1")
    self.Panel_L = DivineScene:getChildByName("Panel_L")
    self.Panel_R = DivineScene:getChildByName("Panel_R")
    local  Button_Back=self.Panel_Top:getChildByName("Button_Back")
    self:addBtnTouchEventListener(Button_Back, function(sender,eventType)
        if eventType==2 then
            if self.from and self.from == "MainCityScene" then      --从MainCityScene进入就SWITSCENE
                SWITSCENE("home/MainCityScene")
            else 
                if self.callback then
                    self:callback()     --刷新HeroInfoScene
                end
                self:removeDlgNoAction()
            end
        end
    end)
    
    self.levelChar = {"零", "一", "二", "三", "四", "五", "六", "七", "八", "九"}

    self.currentIndex = 1
    self:initMonsterArr()
    self:addAnimNode()

    --左边翻页按钮
    local Button_right = self.Panel_Center:getChildByName("ProjectNode_3"):getChildByName("Panel_1"):getChildByName("Button_1")
    local Button_left = self.Panel_Center:getChildByName("ProjectNode_4"):getChildByName("Panel_1"):getChildByName("Button_1")
    Button_left:setVisible(true)
    Button_left:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            if self.currentIndex > 1 and self.currentIndex <= table.getn(self.allActiveMonsters) then
                self.currentIndex = self.currentIndex - 1
            elseif self.currentIndex == 1 then
                self.currentIndex = table.getn(self.allActiveMonsters)
            end
            self:initBreakthroughUI()
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
            self:initBreakthroughUI()
        end
    end
    )
    
    self:initTab()
    
    self:initBreakthroughUI()
end

function BreakthroughScene:initTab()
    
    self.buttons = {}
    local nameArr = {"升级","大师","突破","洗髓"}
    for i=1,4 do
        local btn = self.Panel_Center:getChildByName("Button_Cut"..i)
        btn:setTitleText(LANG(nameArr[i]))
        btn:setTag(i)
        self.buttons[i] = btn
        btn:addTouchEventListener(function(sender,eventType)
            if eventType == 2 then
                if sender:getTag() == 2 then
                    --装备强化大师
                    self:showDlg("hero/EquipReinforcementScene",self.data,self.dlgCallBack)
                    self:removeDlgNoAction()
                elseif sender:getTag() == 3 then
                    return
                elseif sender:getTag() == 4 then
                    self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
                    local monster = self.allActiveMonsters[self.currentIndex]
                    if self.from and self.from == "MainCityScene" then      --从MainCityScene进入就SWITSCENE
                        SWITSCENE("hero/DestinyScene", {monster = monster, callback = nil, from = "MainCityScene"})
                    else 
                        if self.callback then
                            self:callback()     --刷新HeroInfoScene
                        end
                        self:removeDlgNoAction()
                    end
                end
            end
        end)
    end
    self:setIndex(3)
end

function BreakthroughScene:initBreakthroughUI()
    self.Panel_Center:getChildByName("Text_Title"):setString(LANG("突破"))
    self.Panel_Center:getChildByName("Text_Hint"):setVisible(false)
    --装备信息
    local allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
    local monster = allActiveMonsters[self.currentIndex]
    local Image_Icon = self.Panel_1:getChildByName("Image_Icon")
    local Text_Name1 = self.Panel_1:getChildByName("Text_Name1")
    local Text_Name3 = self.Panel_1:getChildByName("Text_Name3")
    Image_Icon:loadTexture(model_equipment:getIcon(monster.equipment[1].id,monster.equipment[1].quality))
    Text_Name1:setString(LANG("%s lv%s", model_equipment:getName(monster.equipment[1].id), monster.equipment[1].level))
    local color, splits = model_monsterStorage:getEquipColor(monster.equipment[1].quality)
    Text_Name1:setColor(color)
    Text_Name3:setString(LANG(model_monster:getName(monster.id)))
    local level = model_breakthrough:getLevelLimit(monster.breakthroughLevel);
    local equipmentLevel = monster.equipment[1].level;
    self.Panel_1:getChildByName("Text_4"):setString(monster.equipment[1].level.."/")
    self.Panel_1:getChildByName("Text_5"):setString("")
    self.Panel_1:getChildByName("Text_6"):setString(model_breakthrough:getLevelLimit(monster.breakthroughLevel))

    --力量、智力、统帅、生命
    --当前属性
    local Effect = model_breakthrough:getEffect(monster.breakthroughLevel)
    local Panel_Item_L = self.Panel_L:getChildByName("Panel_Item1")
    Panel_Item_L:getChildByName("Text_DQSX"):setString(LANG("当前属性"))
    Panel_Item_L:getChildByName("Text_Item_1"):setString(LANG("%s品专属", self.levelChar[monster.breakthroughLevel + 1]))
    Panel_Item_L:getChildByName("text_1"):setString(LANG("力量攻击+%s",Effect.Strength))
    Panel_Item_L:getChildByName("text_2"):setString(LANG("智力攻击+%s",Effect.Intelligence))
    Panel_Item_L:getChildByName("text_3"):setString(LANG("统帅防御+%s",Effect.Leadership))
    Panel_Item_L:getChildByName("text_4"):setString(LANG("生命防御+%s",Effect.HP))
    Panel_Item_L:getChildByName("text_5"):setString("")
    --下一级属性
    local nextEffect = model_breakthrough:getEffect(monster.breakthroughLevel + 1)
    local Panel_Item_R = self.Panel_R:getChildByName("Panel_Item1")
    Panel_Item_R:getChildByName("Text_DQSX"):setString(LANG("提升后属性"))
    Panel_Item_R:getChildByName("text_5"):setString("")
    if monster.breakthroughLevel == 9 then
        Panel_Item_R:getChildByName("Text_Item_1"):setString(LANG("专属品级已满"))
        for i=1,4 do
            Panel_Item_R:getChildByName("text_"..i):setString(LANG("-----"))
        end
    else
        Panel_Item_R:getChildByName("Text_Item_1"):setString(LANG("%s品专属", self.levelChar[monster.breakthroughLevel + 1 + 1]))
        Panel_Item_R:getChildByName("text_1"):setString(LANG("力量攻击+%s",nextEffect.Strength))
        Panel_Item_R:getChildByName("text_2"):setString(LANG("智力攻击+%s",nextEffect.Intelligence))
        Panel_Item_R:getChildByName("text_3"):setString(LANG("统帅防御+%s",nextEffect.Leadership))
        Panel_Item_R:getChildByName("text_4"):setString(LANG("生命防御+%s",nextEffect.HP))
    end

    --突破材料
    local ProjectNode_1 = self.Panel_1_B:getChildByName("ProjectNode_1")
    local ProjectNode_2 = self.Panel_1_B:getChildByName("ProjectNode_2")
    local BreakthroughItemId = model_monster:getBreakthroughItemId()
    local BreakthroughExclusiveItemId = model_monster:getBreakthroughExclusiveItemId(monster.id)
    ProjectNode_1:getChildByName("Panel_Object"):getChildByName("Image_1"):loadTexture(model_static_library:getAsset(BreakthroughItemId, pbCommon.ObjectType.BreakthroughItem))
    ProjectNode_1:getChildByName("Panel_Object"):getChildByName("Image_2"):loadTexture(model_static_library:getFrame(BreakthroughItemId, pbCommon.ObjectType.BreakthroughItem))
    self.Panel_1_B:getChildByName("Text_3"):setString(LANG(model_static_library:getName(BreakthroughItemId, pbCommon.ObjectType.BreakthroughItem)))
    local breakthroughItemCount = model_breakthrough:getBreakthroughItemCount()         --突破石现有量
    local breakthroughNeed = model_breakthrough:getItemNum(monster.breakthroughLevel)   --每次突破需要的突破石数量
    if breakthroughItemCount < breakthroughNeed then
        self.Panel_1_B:getChildByName("Text_4"):setString("")
        self.Panel_1_B:getChildByName("Text_6"):setString(breakthroughItemCount)
    else
        self.Panel_1_B:getChildByName("Text_4"):setString(breakthroughItemCount)
        self.Panel_1_B:getChildByName("Text_6"):setString("")
    end
    self.Panel_1_B:getChildByName("Text_5"):setString("/"..breakthroughNeed)

    ProjectNode_2:getChildByName("Panel_Object"):getChildByName("Image_1"):loadTexture(model_static_library:getAsset(BreakthroughExclusiveItemId, pbCommon.ObjectType.BreakthroughItem))
    ProjectNode_2:getChildByName("Panel_Object"):getChildByName("Image_2"):loadTexture(model_static_library:getFrame(BreakthroughExclusiveItemId, pbCommon.ObjectType.BreakthroughItem))
    self.Panel_1_B:getChildByName("Text_7"):setString(LANG(model_static_library:getName(BreakthroughExclusiveItemId, pbCommon.ObjectType.BreakthroughItem)))
    local exclusiveItemCount = model_breakthrough:getExclusiveItemCount(monster.id)
    local exclusiveItemNeed = model_breakthrough:getExclusiveItemNum(monster.breakthroughLevel)
    if exclusiveItemCount < exclusiveItemNeed then
        self.Panel_1_B:getChildByName("Text_8"):setString("")
        self.Panel_1_B:getChildByName("Text_10"):setString(exclusiveItemCount)
    else
        self.Panel_1_B:getChildByName("Text_8"):setString(exclusiveItemCount)
        self.Panel_1_B:getChildByName("Text_10"):setString("")
    end
    self.Panel_1_B:getChildByName("Text_9"):setString("/"..exclusiveItemNeed)
    local needCoin = model_breakthrough:getCoin(monster.breakthroughLevel)
    self.Panel_1_B:getChildByName("Image_TextBg"):getChildByName("Text_2"):setString(needCoin)

    --突破
    local Button_1 = self.Panel_1_B:getChildByName("Button_1")
    Button_1:setTitleText(LANG("突破"))
    if breakthroughItemCount < breakthroughNeed then
        Button_1:setTitleText(LANG("立即购买"))
    end


    self:addBtnTouchEventListener(Button_1,function(sender,eventType)
        if eventType==2 then
            if breakthroughItemCount >= breakthroughNeed then
                if exclusiveItemCount < exclusiveItemNeed then
                    PopMsg.getInstance():flashShow(LANG("您的%s道具不足", model_static_library:getName(BreakthroughExclusiveItemId, pbCommon.ObjectType.BreakthroughItem)))
                    return
                end
                if model_user.coin < needCoin then
                    PopMsg.getInstance():flashShow(LANG("你的银币不足"))
                    return
                end
                if(equipmentLevel < level) then
                    PopMsg.getInstance():flashShow(LANG("突破等级未到"))
                    return;
                end
                if monster.breakthroughLevel >= 9 then
                    PopMsg.getInstance():flashShow(LANG("突破等级已满"))
                    return;
                end
                self:buyBreakthrough(monster.id)
            else
                self:showDlg("hero/ChipDropScene",{id = BreakthroughItemId,type = pbCommon.ObjectType.BreakthroughItem, needNum = breakthroughNeed}) 
            end

        end
    end)
end

function BreakthroughScene:buyBreakthrough(monsterId)
    if helper_monster:canBreakthrough(monsterId) then
        ctlMonstersBreakthroughRequest:sendRequest(function(success, data, code, msg)
            if(success) then
                self:showBreakthroughAction()
                self:initBreakthroughUI()
            end
        end, monsterId)
    end
end

function BreakthroughScene:showBreakthroughAction(parameters)
    PopMsg.getInstance():flashShow(LANG("突破等级提升"))
    self:runDivine_EndAction()
end

function BreakthroughScene:addAnimNode()

    --突破装备背景动画
    local Node_4 = self.Panel_Ani:getChildByName("Node_4")
    local Node_Ani_Divine_End, actionDivine_End = self:createNode("Node/Node_Ani_Divine_End.csb")
    self.actionDivine_End = actionDivine_End
    self.Node_Ani_Divine_End = Node_Ani_Divine_End
    actionDivine_End:gotoFrameAndPause(0)
    self.Panel_Ani:addChild(Node_Ani_Divine_End)
    Node_Ani_Divine_End:setPosition(cc.p(Node_4:getPositionX(), Node_4:getPositionY()))
end

function BreakthroughScene:runDivine_EndAction()
    --    self.actionDivine_End:gotoFrameAndPlay(0, 75,false)
    --    self.Node_Ani_Divine_Start(0, 40,false)
    local oldNode = self.Panel_Ani:getChildByName("Node_Ani_Divine_Start")
    if oldNode then
        self.Panel_Ani:removeChild(oldNode)
    end

    local Node_3 = self.Panel_Ani:getChildByName("Node_3")
    local Node_Ani_Divine_Start, actionDivine_Start = self:createNode("Node/Node_Ani_Divine_Start.csb")
    self.Node_Ani_Divine_Start = Node_Ani_Divine_Start
    actionDivine_Start:gotoFrameAndPause(0)
    self.Panel_Ani:addChild(Node_Ani_Divine_Start)
    Node_Ani_Divine_Start:setName("Node_Ani_Divine_Start")
    Node_Ani_Divine_Start:setPosition(cc.p(Node_3:getPositionX(), Node_3:getPositionY()))

    actionDivine_Start:gotoFrameAndPlay(10,false)
    local start_Panel = Node_Ani_Divine_Start:getChildByName("Panel_1")
    local Panel_Left_Light_1 = start_Panel:getChildByName("Panel_Left_Light_1")
    local Panel_Left_Light_2 = start_Panel:getChildByName("Panel_Left_Light_2")

    local EndNode_WorldPoint = self.Node_Ani_Divine_End:getChildByName("Panel_1"):convertToWorldSpace(cc.p(13,69))   
    --local WorldPoint = self.Node_Ani_Divine_End:getChildByName("Panel_1"):convertToWorldSpace(self.actionDivine_End:getChildByName("Panel_1"):getPoint()) 
    local StartNode_NodePoint = start_Panel:convertToNodeSpace(EndNode_WorldPoint)
    StartNode_NodePoint.y = StartNode_NodePoint.y -  (CC_DESIGN_RESOLUTION.height-CC_SCREEN_SIZE.height)/2
    cclog("StartNode_NodePoint.x = "..StartNode_NodePoint.x)
    cclog("StartNode_NodePoint.y = "..StartNode_NodePoint.y)
    cclog("EndNode_WorldPoint.x = "..EndNode_WorldPoint.x)
    cclog("EndNode_WorldPoint.y = "..EndNode_WorldPoint.y)
    cclog("Panel_Left_Light_1.x = ".. Panel_Left_Light_1:getPositionX())
    cclog("Panel_Left_Light_1.y = ".. Panel_Left_Light_1:getPositionY())

    self.timerId = Timer:runOnce(function()
        Panel_Left_Light_1:runAction(
            cc.Sequence:create(
                cc.MoveTo:create(0.3,cc.p(StartNode_NodePoint.x,StartNode_NodePoint.y)),
                cc.CallFunc:create(function()
                    actionDivine_Start:gotoFrameAndPlay(40,false)
                    self.actionDivine_End:gotoFrameAndPlay(0,75,false)
                end)))

        Panel_Left_Light_2:runAction(
            cc.Sequence:create(
                cc.MoveTo:create(0.3,cc.p(StartNode_NodePoint.x,StartNode_NodePoint.y))
            ))
    end, 0.2)
end

function BreakthroughScene:initMonsterArr()
    self.allActiveMonsters = {}
    self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()

    local index = 1
    for key,value in pairs(self.allActiveMonsters) do

        if value.id == self.monsterId then
            self.currentIndex = index
        end
        index = index + 1
    end
end

return BreakthroughScene