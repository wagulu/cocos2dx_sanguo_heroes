
local AdvancedScene = class("AdvancedScene", cc.load("mvc").ViewBase)
AdvancedScene.NEXTSCENE = "battle/ChapterScene"
AdvancedScene.RESOURCE_FILENAME = "Scene/DivineScene.csb"
AdvancedScene.STOPFRAME = 100

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

function AdvancedScene:onCreate()
    self.monsterId = self.args[1]
    self:initUi()
end

function AdvancedScene:initUi()
    local DivineScene = self:getResourceNode():getChildByName("DivineScene")
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
    self.Panel_2 = self.Panel_Center:getChildByName("Panel_2")
    self.Panel_1_B = self.Panel_Bottom:getChildByName("Panel_1")
    self.Panel_2_B = self.Panel_Bottom:getChildByName("Panel_2")
    self.Panel_L = DivineScene:getChildByName("Panel_L")
    self.Panel_R = DivineScene:getChildByName("Panel_R")
    local  Button_Back=self.Panel_Top:getChildByName("Button_Back")
    self:addBtnTouchEventListener(Button_Back, function(sender,eventType)
        if eventType==2 then
            self:removeDlgNoAction()
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
    local Button_Cut2 = self.Panel_Center:getChildByName("Button_Cut2")
    local Button_Cut1 = self.Panel_Center:getChildByName("Button_Cut1")
    self.buttons = {}
    self.buttons[1] = Button_Cut2
    self.buttons[2] = Button_Cut1
    
    Button_Cut2:setTitleText(LANG("突破"))
    Button_Cut1:setTitleText(LANG("洗髓"))
    if model_user.level < 35 then           --用户等级低于35级不显示"洗髓"标签
        Button_Cut1:setVisible(false)
    end
    --突破
    self:addBtnTouchEventListener(Button_Cut2,function(sender,eventType)
        if eventType==2 then
            if self.type == 1 then
                return
            end
            self:switchTag(1)
        end
    end)
    --洗髓
    self:addBtnTouchEventListener(Button_Cut1,function(sender,eventType)
        if eventType==2 then
            if self.type == 2 then
                return
            end
            self:switchTag(2)
        end
    end)
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
            self:switchTag(self.type)
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
            self:switchTag(self.type)
        end
    end
    )
    
    self:switchTag(1)
    
end

function AdvancedScene:refreshPage()
    self:switchTag(self.type)
end

function AdvancedScene:switchTag(type)
    
    self.type = type
    self:setIndex(type)
    
    if self.type == 1 then
        self:initBreakthroughUI()
    else
        self:initDestinyUI()
    end
end

function AdvancedScene:initDestinyUI()
	self:visibleUI(2)
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
--
function AdvancedScene:buyUseDestiny(monster, num)
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

function AdvancedScene:updateExpProgress(curExp, levelExp, curCoin, needCoin)
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

function AdvancedScene:addTouchAnim()
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

function AdvancedScene:removeTouchAnim()
    self.Panel_2_B:removeChild(self.Node_Ani_Icon_more)
end

function AdvancedScene:showDestinyAction(oldDestinyLevel, oldExp)
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

function AdvancedScene:getDestinyRateDes(rate)
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

function AdvancedScene:initBreakthroughUI()
	self:visibleUI(1)
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

function AdvancedScene:buyBreakthrough(monsterId)
    if helper_monster:canBreakthrough(monsterId) then
       ctlMonstersBreakthroughRequest:sendRequest(function(success, data, code, msg)
        if(success) then
                self:showBreakthroughAction()
            self:initBreakthroughUI()
        end
    end, monsterId)
    end
end

function AdvancedScene:showBreakthroughAction(parameters)
    PopMsg.getInstance():flashShow(LANG("突破等级提升"))
    self:runDivine_EndAction()
end

function AdvancedScene:visibleUI(type)
    
    self.Panel_1:setVisible(false)
    self.Panel_2:setVisible(false)
    self.Panel_1_B:setVisible(false)
    self.Panel_2_B:setVisible(false)
    
    if type == 1 then
        self.Panel_1:setVisible(true)
        self.Panel_1_B:setVisible(true)
    else
        self.Panel_2:setVisible(true)
        self.Panel_2_B:setVisible(true)
    end
    
    
end

function AdvancedScene:addAnimNode()
    
    --突破装备背景动画
    local Node_4 = self.Panel_Ani:getChildByName("Node_4")
    local Node_Ani_Divine_End, actionDivine_End = self:createNode("Node/Node_Ani_Divine_End.csb")
    self.actionDivine_End = actionDivine_End
    self.Node_Ani_Divine_End = Node_Ani_Divine_End
    actionDivine_End:gotoFrameAndPause(0)
    self.Panel_Ani:addChild(Node_Ani_Divine_End)
    Node_Ani_Divine_End:setPosition(cc.p(Node_4:getPositionX(), Node_4:getPositionY()))
    
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

function AdvancedScene:runDivine_EndAction()
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

function AdvancedScene:initMonsterArr()
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


return AdvancedScene