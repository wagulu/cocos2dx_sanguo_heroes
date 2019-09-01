
local DestinyScene = class("DestinyScene", cc.load("mvc").ViewBase)
DestinyScene.NEXTSCENE = "battle/ChapterScene"
DestinyScene.RESOURCE_FILENAME = "Scene/DivineScene2.csb"
DestinyScene.STOPFRAME = 100

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

function DestinyScene:onCreate()
    self.data = self.args[1]
    self.monsterId = self.data.monster.id
    self.callback = self.data.callback        --刷新HeroInfSceneUI 
    self.from = self.data.from
    self:initUi()
end

function DestinyScene:initUi()
    local DivineScene = self:getResourceNode():getChildByName("DivineScene")
    DivineScene:setTouchEnabled(true)
    self.Panel_Center = DivineScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    self.Panel_Top = DivineScene:getChildByName("Panel_T")
    self.Panel_Bottom = DivineScene:getChildByName("Panel_B"):getChildByName("Panel_Bottom")
    local Panel_Bg = DivineScene:getChildByName("Panel_Bg")
    SCREEN_TOP(DivineScene:getChildByName("Panel_T"))
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_BOTTOM(DivineScene:getChildByName("Panel_B"))

    self.Panel_2 = self.Panel_Center:getChildByName("Panel_2")
    self.Panel_2_B = self.Panel_Bottom:getChildByName("Panel_2")
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

    self.lastProgress = 0
    local oldImageView = self.Panel_2:getChildByName("Image_Role")
    self.Panel_2:removeChild(oldImageView)
    local ImageView = ccui.ImageView:create()
    ImageView:setName("Image_Role")
    ImageView:setPosition(oldImageView:getPosition())
    ImageView:setLocalZOrder(-1)
    self.Panel_2:addChild(ImageView)

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
            self:initDestinyUI()
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
            self:initDestinyUI()
        end
    end
    )
    self:initTab()

    self:initDestinyUI()

end

function DestinyScene:initTab()
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
                    self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
                    local monster = self.allActiveMonsters[self.currentIndex]
                    if self.from and self.from == "MainCityScene" then      --从MainCityScene进入就SWITSCENE
                        SWITSCENE("hero/BreakthroughScene", {monster = monster, callback = nil, from = "MainCityScene"})
                    else 
                        if self.callback then
                            self:callback()     --刷新HeroInfoScene
                        end
                        self:removeDlgNoAction()
                    end
                elseif sender:getTag() == 4 then
                    return
                end
            end
        end)
    end
    self:setIndex(4)
end

function DestinyScene:initDestinyUI()
    self.Panel_Center:getChildByName("Text_Title"):setString(LANG("洗髓"))

    --武将信息
    local allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
    local monster = allActiveMonsters[self.currentIndex]
    local color,splits = model_monsterStorage:getColor(monster.id)
    self.Panel_2:getChildByName("Image_Role"):loadTexture((model_monster:getHalf(monster.id)))
    self.Panel_2:getChildByName("Text_level"):setString(LANG("lv%s", monster.level))
    local name = monster.name
    self.Panel_2:getChildByName("Text_heroname"):setString(LANG("%s",model_monster:getName(monster.id)))
    self.Panel_2:getChildByName("Text_heroname"):setColor(color)
    self.Panel_2:getChildByName("Sprite_Quality_Num"):setString(LANG(""))
    self.Panel_2:getChildByName("Sprite_Quality_Num"):setColor(color)
    if table.getn(splits) == 2 then
        self.Panel_2:getChildByName("Sprite_Quality_Num"):setString(LANG("+"..splits[2]))
    end
    local Panel_Star = self.Panel_2:getChildByName("Panel_Star")
    --星级显示
    for k = 1,5 do
        Panel_Star:getChildByName("Star_"..k):setVisible(k<= monster.star)
        Panel_Star:getChildByName("Star_"..k):setTouchEnabled(false)
    end
    Panel_Star:getChildByName("Job"):loadTexture("Job/Icon_job"..model_monster:getType(monster.id)..".png")

    --当前属性
    local Panel_Item_L = self.Panel_L:getChildByName("Panel_Item1")
    Panel_Item_L:getChildByName("Text_DQSX"):setString(LANG("当前属性"))
    Panel_Item_L:getChildByName("Text_Item_1"):setString(LANG("洗髓等级%s级", monster.destinyLevel))
    Panel_Item_L:getChildByName("text_1"):setString(LANG("物理攻击+%s",model_monsterDestiny:getMonsterEffectAttack(monster.id)).."%")
    Panel_Item_L:getChildByName("text_2"):setString(LANG("法术攻击+%s",model_monsterDestiny:getMonsterEffectPower(monster.id)).."%")
    Panel_Item_L:getChildByName("text_3"):setString(LANG("物理防御+%s",model_monsterDestiny:getMonsterEffectArmor(monster.id)).."%")
    Panel_Item_L:getChildByName("text_4"):setString(LANG("法术防御+%s",model_monsterDestiny:getMonsterEffectResistance(monster.id)).."%")
    Panel_Item_L:getChildByName("text_5"):setString("")

    --下一级属性
    local Panel_Item_R = self.Panel_R:getChildByName("Panel_Item1")
    Panel_Item_L:getChildByName("Text_DQSX"):setString(LANG("当前属性"))
    Panel_Item_R:getChildByName("text_5"):setString("")
    if monster.destinyLevel == 20 then
        Panel_Item_R:getChildByName("Text_Item_1"):setString(LANG("洗髓等级已经满"))
        for i=1,4 do
            Panel_Item_R:getChildByName("text_"..i):setString(LANG("-----"))
        end
    else
        Panel_Item_R:getChildByName("Text_Item_1"):setString(LANG("洗髓等级%s级", monster.destinyLevel + 1))
        Panel_Item_R:getChildByName("text_1"):setString(LANG("物理攻击+%s",model_monsterDestiny:getNextMonsterEffectAttack(monster.id)).."%")
        Panel_Item_R:getChildByName("text_2"):setString(LANG("法术攻击+%s",model_monsterDestiny:getNextMonsterEffectPower(monster.id)).."%")
        Panel_Item_R:getChildByName("text_3"):setString(LANG("物理防御+%s",model_monsterDestiny:getNextMonsterEffectArmor(monster.id)).."%")
        Panel_Item_R:getChildByName("text_4"):setString(LANG("法术防御+%s",model_monsterDestiny:getNextMonsterEffectResistance(monster.id)).."%")
    end
    --经验
    local rate = model_monsterDestiny:getMonsterLevelUpRate5(monster.id)
    self.Panel_2_B:getChildByName("LoadingBar_1"):setPercent(rate * 100)
    self.actionDivine_Node_AniBar:gotoFrameAndPause(rate * 44)
    local needExp = model_monsterDestiny:getExp(monster.destinyLevel);
    local curExp = monster.destinyExp
    self.Panel_2_B:getChildByName("Text_Num"):setString(LANG("%s/%s",curExp, needExp))
    local Text_Hint1 = self.Panel_2_B:getChildByName("Text_Hint1")
    local Text_12 = self.Panel_2_B:getChildByName("Text_12")
    local des = self:getDestinyRateDes(rate)
    Text_Hint1:setString(LANG("天命值每天24:00清零，当前有%s直接升级",des))
    Text_12:setString(LANG(des))
    --洗髓丹
    local needCoin = model_monsterDestiny:getItemCount(monster.destinyLevel)
    local curCoin = model_monsterDestiny:getDestinyItemCount()
    local Image_TextBg = self.Panel_2_B:getChildByName("Image_TextBg")
    if curCoin < needCoin then
        Image_TextBg:getChildByName("Text_2"):setString("")
        Image_TextBg:getChildByName("Text_4"):setString(LANG("%s",curCoin))
    else
        Image_TextBg:getChildByName("Text_2"):setString(LANG("%s",curCoin))
        Image_TextBg:getChildByName("Text_4"):setString("")
    end
    Image_TextBg:getChildByName("Text_3"):setString(LANG("/%s",needCoin))

    local Image_Icon = self.Panel_2_B:getChildByName("Panel_Btn")
    if monster.level >= 35 and monster.star >= 3 then
        Image_Icon:setTouchEnabled(true)
        self.Panel_Center:getChildByName("Text_Hint"):setVisible(false)
    else
        Image_Icon:setTouchEnabled(false)
        self.Panel_Center:getChildByName("Text_Hint"):setVisible(true)
        self.Panel_Center:getChildByName("Text_Hint"):setString(LANG("武将达到35级且达到三星时方可进行洗髓"))
    end

    local isNotSend = false
    Image_Icon:addTouchEventListener(function (sender, eventType)
        if eventType ==0 then
            self.actionDivine_Ani_Icon_once:gotoFrameAndPlay(0, 55, false)
            self:addTouchAnim()

            if monster.destinyLevel >= 20 then
                PopMsg.getInstance():flashShow(LANG("洗髓等级已满"))
                return
            end
            if monster.level < 35 and monster.star < 3 then
                PopMsg.getInstance():flashShow(LANG("武将不足35级或未达到3星，无法进行洗髓"))
                return
            end
            if curCoin < needCoin then
                PopMsg.getInstance():flashShow(LANG("洗髓丹不足，无法进行洗髓"))
                return
            end

            self.count = 0
            self.curCoin = model_monsterDestiny:getDestinyItemCount()
            self.DestinyTimerId = Timer:start(function()
                --destinyLevelNext
                self.count = self.count + 1
                self.curCoin = self.curCoin - needCoin
                local levelExp = model_monsterDestiny:getExp(monster.destinyLevel)          --升级所需经验
                local exp = model_monsterDestiny:getItemCount(monster.destinyLevel)      --每次经验
                local addExp = exp * self.count
                local curExp = (monster.destinyExp + addExp)

                if self.curCoin < 0 then                                 --洗髓丹不够
                    local num = self.count - 1
                    if num > 0 then
                        self:buyUseDestiny(monster, num)
                    end
                    Timer:kill(self.DestinyTimerId)
                    return
                end

                if self.count >= monster.destinyLevelNext or curExp >= levelExp then      --直接升级
                    self:updateExpProgress(curExp, levelExp, self.curCoin, needCoin)
                    self:buyUseDestiny(monster, self.count)
                    Timer:kill(self.DestinyTimerId)
                else
                    self:updateExpProgress(curExp, levelExp, self.curCoin, needCoin)                                         --提升经验
                    isNotSend = true
                end


            end, 0.2, nil)

        elseif eventType == 2 or eventType == 3 then
            self:removeTouchAnim()
            Timer:kill(self.DestinyTimerId)
            if isNotSend then
                self:buyUseDestiny(monster, self.count)
                isNotSend = false
            end
        end
    end)

end

function DestinyScene:buyUseDestiny(monster, num)
    if helper_monster:canDestiny(monster.id) == false then
        return
    end
    local allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
    local monster = allActiveMonsters[self.currentIndex]
    local oldDestinyLevel = monster.destinyLevel
    local oldExp = monster.destinyExp
    ctlMonstersDestinyLevelUpRequest:sendRequest(function(success, data, code, msg)
        if(success) then
            self:showDestinyAction(oldDestinyLevel, oldExp)
            self:initDestinyUI()
        else
            self:initDestinyUI()
        end
    end, monster.id, num);
end

function DestinyScene:updateExpProgress(curExp, levelExp, curCoin, needCoin)
    local rate = curExp / levelExp
    local progress = 44 * rate
    self.actionDivine_Node_AniBar:gotoFrameAndPause(progress)
    self.Panel_2_B:getChildByName("LoadingBar_1"):setPercent(rate * 100)
    self.Panel_2_B:getChildByName("Text_Num"):setString(LANG("%s/%s",curExp, levelExp))

    local Image_TextBg = self.Panel_2_B:getChildByName("Image_TextBg")
    if curCoin < needCoin then
        Image_TextBg:getChildByName("Text_2"):setString("")
        Image_TextBg:getChildByName("Text_4"):setString(LANG("%s/",curCoin))
    else
        Image_TextBg:getChildByName("Text_2"):setString(LANG("%s/",curCoin))
        Image_TextBg:getChildByName("Text_4"):setString("")
    end
    Image_TextBg:getChildByName("Text_3"):setString(LANG("%s",needCoin))
end

function DestinyScene:addTouchAnim()
    --长按闪光
    local Node_Ani_Icon_more, actionDivine_Ani_Icon_more = self:createNode("Node/Node_Ani_Icon_more.csb")
    self.actionDivine_Ani_Icon_more = actionDivine_Ani_Icon_more
    self.Node_Ani_Icon_more = Node_Ani_Icon_more
    actionDivine_Ani_Icon_more:gotoFrameAndPause(0)
    self.Panel_2_B:addChild(Node_Ani_Icon_more)
    Node_Ani_Icon_more:setPosition(cc.p(self.ProjectNode_Icon:getPositionX(), self.ProjectNode_Icon:getPositionY()))
    Node_Ani_Icon_more:setName("Node_Ani_Icon_more")
    self.actionDivine_Ani_Icon_more:gotoFrameAndPlay(0, 30, false)
end

function DestinyScene:removeTouchAnim()
    self.Panel_2_B:removeChild(self.Node_Ani_Icon_more)
end

function DestinyScene:showDestinyAction(oldDestinyLevel, oldExp)
    local allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
    local monster = allActiveMonsters[self.currentIndex]
    if monster.destinyLevel > oldDestinyLevel then
        PopMsg.getInstance():flashShow(LANG("洗髓等级提升"))
        self.actionDivine_RisingStarAnim:gotoFrameAndPlay(0, 106, false)   
        if self.DestinyLvTimerId ~= 0 and self.DestinyLvTimerId ~= nil then
            Timer:kill(self.DestinyLvTimerId)
        end
        self.DestinyLvTimerId = Timer:runOnce(function()
            self.actionDivine_Stop_Ani:gotoFrameAndPause(75)
            self.actionDivine_Stop_Ani:gotoFrameAndPlay(0, 75, false)
        end, 2)
    end

    --    local curDestinyLevel = monster.destinyLevel
    --    local curExp = monster.destinyExp
    --    
    --    local rate = model_monsterDestiny:getMonsterLevelUpRate5(monster.id)
    --    local progress = 44 * rate
    --    if oldDestinyLevel == curDestinyLevel and curExp > oldExp then
    --        --self.actionDivine_Node_AniBar:gotoFrameAndPlay(self.lastProgress, progress,false)
    --        self.actionDivine_Node_AniBar:gotoFrameAndPause(progress)
    --        self.lastProgress = progress
    --    elseif oldDestinyLevel < curDestinyLevel then
    --        self.actionDivine_Node_AniBar:gotoFrameAndPlay(0, 50,false)
    --        self.lastProgress = 0
    --        if self.DestinyTimerId ~= 0 and self.DestinyTimerId ~= nil then
    --            Timer:kill(self.DestinyTimerId)
    --        end
    --        self.DestinyTimerId = Timer:runOnce(function()
    --            self.actionDivine_Node_AniBar:gotoFrameAndPause(0)
    --        end, 1)
    --    end

    --    local curProgress = 0
    --    local targetProgress = 100
    --    local LoadingBar_1 = self.Panel_2_B:getChildByName("LoadingBar_1")
    --    self.actionDivine_Node_AniBar:gotoFrameAndPlay(0, 44,false)
    --    self.id = Timer:start(function()
    --        LoadingBar_1:setPercent(curProgress)
    --        curProgress = curProgress + 2.5
    --        if curProgress > targetProgress then
    --            LoadingBar_1:setPercent(targetProgress)
    --            Timer:kill(self.id)
    --        end
    --    end,0.01,nil)

end

function DestinyScene:getDestinyRateDes(rate)
    local des = ""
    if rate >= 0 and rate <= 0.2 then
        des = "极低概率"
    elseif rate > 0.2 and rate <= 0.4 then
        des = "较低概率"
    elseif rate > 0.4 and rate <= 0.6 then
        des = "普通概率"
    elseif rate > 0.6 then
        des = "较高概率"
    end

    return des
end

function DestinyScene:addAnimNode()

    --洗髓武将升级背景动画
    local Node_2 = self.Panel_2:getChildByName("Node_2")
    local Node_RisingStarAnim_1, actionDivine_RisingStarAnim = self:createNode("Node/Node_RisingStarAnim_1.csb")
    self.actionDivine_RisingStarAnim = actionDivine_RisingStarAnim
    actionDivine_RisingStarAnim:gotoFrameAndPause(0)
    self.Panel_2:addChild(Node_RisingStarAnim_1)
    Node_RisingStarAnim_1:setPosition(cc.p(Node_2:getPositionX(), Node_2:getPositionY()))

    --洗髓进度条
    local ProjectNode_Icon = self.Panel_2_B:getChildByName("ProjectNode_Icon")
    self.ProjectNode_Icon = ProjectNode_Icon
    local Node_Ani_Bar, actionDivine_Node_AniBar = self:createNode("Node/Node_Ani_Bar.csb")
    self.actionDivine_Node_AniBar = actionDivine_Node_AniBar
    actionDivine_Node_AniBar:gotoFrameAndPause(0)
    self.Panel_2_B:addChild(Node_Ani_Bar)
    Node_Ani_Bar:setPosition(cc.p(ProjectNode_Icon:getPositionX(), ProjectNode_Icon:getPositionY()))
    --self.actionDivine_Node_AniBar:gotoFrameAndPlay(0, 50,true)

    --洗髓按钮闪光
    local Node_Ani_Icon_once, actionDivine_Ani_Icon_once = self:createNode("Node/Node_Ani_Icon_once.csb")
    self.actionDivine_Ani_Icon_once = actionDivine_Ani_Icon_once
    actionDivine_Ani_Icon_once:gotoFrameAndPause(0)
    self.Panel_2_B:addChild(Node_Ani_Icon_once)
    Node_Ani_Icon_once:setPosition(cc.p(ProjectNode_Icon:getPositionX(), ProjectNode_Icon:getPositionY()))

    --洗髓升级动画
    local Node_stopAnim, actionDivine_Stop_Ani = self:createNode("Node/Node_stopAnim.csb")
    self.actionDivine_Stop_Ani = actionDivine_Stop_Ani
    actionDivine_Stop_Ani:gotoFrameAndPause(75)
    self.Panel_2:addChild(Node_stopAnim)
    local size = self.Panel_2:getContentSize()
    Node_stopAnim:setPosition(cc.p(size.width/2, size.height/2))
end

function DestinyScene:initMonsterArr()
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

return DestinyScene