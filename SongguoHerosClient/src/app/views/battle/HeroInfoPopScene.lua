local HeroInfoPopScene = class("HeroInfoPopScene", cc.load("mvc").ViewBase)
HeroInfoPopScene.NEXTSCENE = "battle/ChapterScene"

local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_extension_relationship_treasure = _REQUIRE("model/extension/model_extension_relationship_treasure.lua")
local helper_relationship = _REQUIRE("model/helper/helper_relationship.lua")
local static_model_relationship = _REQUIRE("model/static/model_relationship.lua")
local helper_monster_data = _REQUIRE("model/helper/helper_monster_data.lua")
local porterty = _REQUIRE "common/AttributeName.lua"


HeroInfoPopScene.RESOURCE_FILENAME = "Scene/SelectPopup.csb"
function HeroInfoPopScene:onCreate()
   
    self:initUi()
end

function HeroInfoPopScene:initUi()
--    local HeroInfoPopScene = self:getResourceNode():getChildByName("Scene")
--    HeroInfoPopScene:setTouchEnabled(true)
    
    local monsterId = self.args[1]
    self.battleData = self.args[2]
    
    self.data = model_monsterStorage:getMonster(monsterId)
    
    cclog("id = "..self.data.id)
    cclog("id = "..self.battleData.from)
    local SelectPopup = self:getResourceNode():getChildByName("SelectPopup")
    SelectPopup:setTouchEnabled(true)
    local Panel_Center = SelectPopup:getChildByName("Panel_Center")
    local Panel_Bg = SelectPopup:getChildByName("Panel_Bg")
    local Panel_Info = Panel_Center:getChildByName("Panel_Info")
    local Button_Close = Panel_Center:getChildByName("Button_Close")
    SCREEN_SCALE_BG(Panel_Bg)
    --SCREEN_CENTER(Panel_Center)
    self:addBtnTouchEventListener(Button_Close, function (sender, eventType)
        if eventType==2 then
            self:removeDlgNoAction()
        end
    end)
    for i = 1,4 do
        Panel_Center:getChildByName("Button_Select_"..i):setVisible(false)
    end
    local Text_Title_QMCJ = Panel_Center:getChildByName("Text_Title_QMCJ")
    Text_Title_QMCJ:setString(LANG("%s", self.data.name))
    
    self:getMembersArr()
    self:Panel_Info(Panel_Info)
end

function HeroInfoPopScene:getMembersArr()
    self.members = {}
    if self.battleData.monsterItems == nil then
        return
    end

    for key, value in pairs(self.battleData.monsterItems) do
        if(value.monsterId > 0) then
            table.insert(self.members,value.monsterId)
            cclog("monsterId = "..value.monsterId)
        end
    end
end

function HeroInfoPopScene:Panel_Info(panel)
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
    local monster = model_monsterStorage:getMonster(self.data.id);
    if self.Image_Bg_Brown == nil then

        local Image_Bg_Brown = scrollView:getChildByName("Image_Bg_Brown")
        local Text_Title_1 = scrollView:getChildByName("Text_Title_1")
        local Text_Attribute = scrollView:getChildByName("Text_Attribute")
        local Image_6 = scrollView:getChildByName("Image_6")
        local Image_9 = scrollView:getChildByName("Image_9") 
        local Button_2 = scrollView:getChildByName("Button_2")
        scrollView:setAnchorPoint(cc.p(0,1))
        Image_9:removeSelf()
        Image_6:addChild(Image_9)
        Image_9:setPosition(cc.p(53,52))
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
    local txtSize = 22
    --local names = {LANG("武将属性"),LANG("人物缘分"),LANG("珍宝缘分"),LANG("好感度"),LANG("觉醒能力"),LANG("武将简介")}
    local names = {LANG("武将属性"),LANG("人物缘分"),LANG("珍宝缘分"),LANG("武将简介")}
    --
    local relationTreasure = model_extension_relationship_treasure:getAllRelationshipsByMonster(self.data.id)
    local txtHeight = 0
    for i=1,#relationTreasure do
        local detail = relationTreasure[i].detail
        local itemId = detail.itemId

        self["relationTreasure_"..i] = self.Text_Attribute:clone()
        if relationTreasure[i].activation == true then
            self["relationTreasure_"..i]:setColor(cc.c3b(255,0,0))
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
            --self["relationLevel_"..i]:setColor(cc.c3b(153,255,102))
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
    self.size5 = 120 + txtHeight
    self.size1 =(txtCount +6)*  txtSpace + 160
    self.size2 = relationHeight + 120
    self.size3 = txtHeight + 120

    --self.size4 = 100              好感度
    --self.size5 = 100              觉醒能力
    self.size4 = txtDescHeight + 120
    local innerHeight = 0
    for i=1,4 do
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
    local id = self.data.id
    --    local wu = helper_monster_data:calculateStrength(id)
    --    local zhi = helper_monster_data:calculateStrength(id)
    --    local tong = helper_monster_data:calculateLeadership(id)

    local wu = math.floor(finalAttibute["Strength"])
    local zhi = math.floor(finalAttibute["Intelligence"])
    local tong = math.floor(finalAttibute["Leadership"])

    local atts1 = {LANG("等级：%s/%s",monster.level,99),LANG("武力：%s",wu),LANG("智力：%s",zhi),LANG("统率：%s",tong),LANG("当前兵种：%s",monster.soldier.name),LANG("阵型："), LANG("最大带兵数: %s", finalAttibute["MaxSoldier"])}
    local numLevel =0
    for i=1 ,#atts1 do
        numLevel = numLevel + 1
        local txt = self.Text_Attribute:clone()
        txt:setVisible(true)
        txt:setString(atts1[i])
        txt:setPosition(cc.p(50,self.size1-txtSpace*(numLevel+1)))
        --txt:setColor(cc.c3b(153,255,102))
        txt:setColor(cc.c3b(255,255,255))
        self.bg1:addChild(txt)
    end
    
    if self.battleData ~= nil and self.battleData.from == "BattleTeamScene" then
        local Button_replace = self.Button_2:clone()
        local Button_removed = self.Button_2:clone()
        local btSize = Button_replace:getContentSize()
        Button_replace:setVisible(true)
        Button_replace:setPosition(cc.p(300,self["size1"] - btSize.height * 1.5))
        Button_removed:setVisible(true)
        Button_removed:setPosition(cc.p(300,self["size1"] - btSize.height * 2.5))
        self.bg1:addChild(Button_replace)
        self.bg1:addChild(Button_removed)
        
        Button_replace:setTitleText(LANG("替换"))
        self:addBtnTouchEventListener(Button_replace, function (sender, eventType)
            if eventType==2 then
                cclog("monsterItem.pos = "..self.battleData.monsterItem.pos)
                cclog("len = "..#(self.battleData.monsterItems))
                self:showDlg("battle/BattleHeroListScene", self.battleData.monsterItem, self.battleData.monsterItems, self.battleData.callback)
                self:removeDlgNoAction()
--                model_user.callBackArr[table.getn(model_user.callBackArr)+1] = {"battle/HeroInfoPopScene",self.monsterId,self.battleData}
--                SWITSCENE("battle/BattleHeroListScene", self.battleData.monsterItem, self.battleData.monsterItems)
            end
        end)

        Button_removed:setTitleText(LANG("撤下"))
        self:addBtnTouchEventListener(Button_removed, function (sender, eventType)
            if eventType==2 then
                self.battleData.callback(self.battleData.monsterItem, 0)
                self:removeDlgNoAction()
            end
        end)
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
            txt:setColor(cc.c3b(255,255,255))
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
            txt:setPosition(cc.p(50,self.size1-txtSpace*(numLevel+1) -10 - self.Image_6:getContentSize().height*0.8))
            self.bg1:addChild(txt)
        end
    end

    local innerLevelHeight =0
    for i=1,#relationLevel do
        self["relationLevel_"..i]:setVisible(true)
        self["relationName_"..i]:setVisible(true)
        cclog("dddddddd"..self["relationLevelHeight_"..i])

        self["relationName_"..i]:setPosition(cc.p(50,self.title_2:getPositionY() -txtSpace - innerLevelHeight))
        innerLevelHeight = innerLevelHeight + txtSize + 5
        self["relationLevel_"..i]:setPosition(cc.p(50,self.title_2:getPositionY() -txtSpace - innerLevelHeight))
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
        self["relationTreasureName_"..i]:setPosition(cc.p(50,self.title_3:getPositionY() -txtSpace - innerRelationHeight))
        innerRelationHeight = innerRelationHeight + txtSize + 5
        self["relationTreasure_"..i]:setPosition(cc.p(50,self.title_3:getPositionY() -txtSpace - innerRelationHeight))
        
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


return HeroInfoPopScene