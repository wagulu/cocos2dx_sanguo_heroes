
local TreasureReinforcementScene = class("ReinforcementScene1", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_monster = _REQUIRE("model/static/model_monster.lua")
local model_equipment = _REQUIRE("model/static/model_equipment.lua")
local porterty = _REQUIRE "common/AttributeName.lua"
local model_equipmentStrengthenMasterConfig = _REQUIRE("model/static/model_equipmentStrengthenMasterConfig.lua")
local model_treasureItemStrengthenMasterConfig = _REQUIRE("model/static/model_treasureItemStrengthenMasterConfig.lua")
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local static_model_treasureItem =  _REQUIRE "model/static/model_treasureItem.lua"
TreasureReinforcementScene.STOPFRAME = 55

TreasureReinforcementScene.RESOURCE_FILENAME = "Scene/ReinforcementScene1.csb"


function TreasureReinforcementScene:onCreate()
    self.data = self.args[1]
    self.fun = self.args[2]
    self.treasureItemStrengthenLevel = 0 --当前珍宝强化等级
    --自己所有的武将列表
    local allActiveMonsters1 = model_monsterStorage:getAllActiveMonsters()
    --先筛选出珍宝足够4件的武将
    self.allActiveMonsters = {}
    for i = 1,table.getn(allActiveMonsters1) do
        local isTreasuresEnough = true
        for j = 1,table.getn(allActiveMonsters1[i].treasureItems) do
            if allActiveMonsters1[i].treasureItems[j].inventoryTreasureItemId == 0 then
                isTreasuresEnough = false
            end
        end
        if isTreasuresEnough == true then
            self.allActiveMonsters[table.getn(self.allActiveMonsters) + 1] = allActiveMonsters1[i]
        end
    end
    --看当前武将是哪个武将序列
    for t = 1,table.getn(self.allActiveMonsters) do
        if self.data.id == self.allActiveMonsters[t].id then
            self.heroIndex = t
        end
    end
 
    local HeroUpgradeScene = self:getResourceNode():getChildByName("ReinforcementScene")
    HeroUpgradeScene:setTouchEnabled(true)
    local Panel_Bg=HeroUpgradeScene:getChildByName("Panel_Bg")
    local Panel_C = HeroUpgradeScene:getChildByName("Panel_C")
    local Panel_B=HeroUpgradeScene:getChildByName("Panel_B")
    local Panel_T = HeroUpgradeScene:getChildByName("Panel_T")
    self.Panel_Center =Panel_C:getChildByName("Panel_Center")

    self.Panel_Bottom=Panel_B:getChildByName("Panel_Bottom")
    SCREEN_TOP(Panel_T)
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_BOTTOM(Panel_B)
    
    --人物
    local Panel_2 = self.Panel_Center:getChildByName("Panel_2")
    local oldImageView = Panel_2:getChildByName("Image_Role")
    Panel_2:removeChild(oldImageView)
    self.ImageView = ccui.ImageView:create()
    self.ImageView:setPosition(329.99,259.68)
    self.ImageView:setLocalZOrder(-1)
    Panel_2:addChild(self.ImageView)

    local Button_Back = Panel_T:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.dlgCallBack({cmd = "LevelUp"})
            self:removeDlgNoAction()
        end
    end
    )

    local Button_Master_1 = self.Panel_Center:getChildByName("Panel_Item1"):getChildByName("Button_Master_1")
    Button_Master_1:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            --装备强化大师
            self:showDlg("hero/EquipReinforcementScene",self.data,self.fun)
            self:removeDlgNoAction()
        end
    end
    )

    self.buttons = {}
    local nameArr = {"升级","大师","突破","洗髓"}
    for i=1,4 do
        local btn = self.Panel_Center:getChildByName("Button_Cut"..i)
        btn:setTitleText(LANG(nameArr[i]))
        btn:setTag(i)
        self.buttons[i] = btn
        btn:addTouchEventListener(function(sender,eventType)
            if eventType == 2 then
                if sender:getTag() == 1 then
                    --升级
                    self:showDlg("hero/HeroUpgradeScene",self.data,self.fun)
                    self:removeDlgNoAction()
                elseif sender:getTag() == 3 then

                elseif sender:getTag() == 4 then

                end
            end
        end)

    end
    self:setIndex(2) 
    
    --左右翻页
    self.Button_1 = self.Panel_Center:getChildByName("ProjectNode_4"):getChildByName("Panel_1"):getChildByName("Button_1")
    self.Button_1:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.heroIndex = self.heroIndex - 1
            self:setArrowShow()

            self:initPanel()
        end
    end)

    self.Button_2 = self.Panel_Center:getChildByName("ProjectNode_3"):getChildByName("Panel_1"):getChildByName("Button_1")
    self.Button_2:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.heroIndex = self.heroIndex + 1
            self:setArrowShow()

            self:initPanel()
        end
    end)

    self:setArrowShow()
    
--    --判断珍宝是否足够4件
--    self.isTreasuresEnough = true
--    for j = 1,table.getn(self.data.treasureItems) do
--        if self.data.treasureItems[j].inventoryTreasureItemId == 0 then
--            self.isTreasuresEnough = false
--        end
--    end
    
    self:initPanel()
end

function TreasureReinforcementScene:setArrowShow()
    self.Button_1:setVisible(self.heroIndex ~= 1)
    self.Button_2:setVisible(self.heroIndex ~= table.getn(self.allActiveMonsters))
    self.data = self.allActiveMonsters[self.heroIndex]
end

--装备的强化大师属性显示的是数值，珍宝的显示的是百分比，需要减去1
function TreasureReinforcementScene:initTopHeroInfo()
    local color,splits = model_monsterStorage:getColor(self.data.id)
    local Panel_2 = self.Panel_Center:getChildByName("Panel_2")
    local Text_heroname = Panel_2:getChildByName("Text_heroname")
    local Sprite_Quality_Num = Panel_2:getChildByName("Sprite_Quality_Num")
    local Panel_Star = Panel_2:getChildByName("Panel_Star")
    local Panel_Item1 = self.Panel_Bottom:getChildByName("Panel_1"):getChildByName("Panel_Item1")
    local Panel_Item2 = self.Panel_Bottom:getChildByName("Panel_1"):getChildByName("Panel_Item2")
    --职业
    Panel_Star:getChildByName("Job"):loadTexture("Job/Icon_job"..model_monster:getType(self.data.id)..".png")
    --等级
    Panel_2:getChildByName("Text_level"):setString("LV".. self.data.level)
    --名字
    if table.getn(splits) == 2 then
        Text_heroname:setString(LANG("%s",model_monster:getName(self.data.id)))
        Sprite_Quality_Num:setString(LANG("+"..splits[2]))
    else
        Text_heroname:setString(LANG("%s",model_monster:getName(self.data.id)))
        Sprite_Quality_Num:setString(LANG(""))
    end
    --星级
    for i = 1,5 do
        local star = Panel_Star:getChildByName("Star_" .. i)
        star:setVisible(i <= self.data.star)
    end
    
    --人物
    self.ImageView:loadTexture(model_monster:getHalf(self.data.id))
    
    
    --强化属性信息
    local currentLevel
    local miniLevel
    --是否如果玩家当前强化大师等级达到最高级
    local typeToHighestLevel = 0
    if self.treasureItemStrengthenLevel == 0 then
        currentLevel = 0
    else
        currentLevel= model_treasureItemStrengthenMasterConfig:getMinLevel(self.treasureItemStrengthenLevel)
    end
    typeToHighestLevel = model_treasureItemStrengthenMasterConfig:getHighestLevel()
    if typeToHighestLevel > currentLevel then
        miniLevel = model_treasureItemStrengthenMasterConfig:getMinLevel(self.treasureItemStrengthenLevel+1)
    else
        miniLevel = typeToHighestLevel
    end
    
    self.Panel_Center:getChildByName("Panel_Item1"):getChildByName("Text_Hint_1"):setString(LANG("珍宝强化大师%s级",self.treasureItemStrengthenLevel))
    Panel_Item1:getChildByName("Text_DQSX"):setString(LANG("珍宝强化大师%s级",self.treasureItemStrengthenLevel))
    

    --是否玩家当前强化大师等级是0级
    Panel_Item1:getChildByName("Text_5"):setVisible(self.treasureItemStrengthenLevel == 0)
    Panel_Item1:getChildByName("Panel_Text"):setVisible(self.treasureItemStrengthenLevel ~= 0)
    if self.treasureItemStrengthenLevel == 0 then
        Panel_Item1:getChildByName("Text_5"):setString(LANG("6件装备强化到%s级\n 可获得能力提升",self.needLevel))
    end
    
    Panel_Item2:getChildByName("Text_5"):setVisible(currentLevel == typeToHighestLevel)
    Panel_Item2:getChildByName("Panel_Text"):setVisible(currentLevel < typeToHighestLevel)
    Panel_Item2:getChildByName("Text_DQSX"):setVisible(currentLevel < typeToHighestLevel)
    if currentLevel == typeToHighestLevel then
        Panel_Item1:getChildByName("Text_5"):setString(LANG("珍宝强化大师已达到最高级"))
    else
        Panel_Item2:getChildByName("Text_DQSX"):setString(LANG("珍宝强化大师%s级(所有珍宝强化到%s级)",self.treasureItemStrengthenLevel+1,miniLevel))
    end
    
    --装备强化大师增加绝对值属性，珍宝强化大师增加百分比属性，最多4条属性
    local groupArr = {}
    local group1
    local group2
    if self.treasureItemStrengthenLevel ~= 0 then
        group1 = model_treasureItemStrengthenMasterConfig:getStrengthEffectGroup(self.treasureItemStrengthenLevel)
    end
    if currentLevel <= model_treasureItemStrengthenMasterConfig:getHighestLevel() then
        group2 = model_treasureItemStrengthenMasterConfig:getStrengthEffectGroup(self.treasureItemStrengthenLevel+1)
    end
    groupArr = {group1,group2}
    --当前的属性
    local propertyNameArr = {}
    for k = 1,4 do
        if self.treasureItemStrengthenLevel ~= 0 then
            local text = Panel_Item1:getChildByName("Panel_Text"):getChildByName("text_"..k)
--        local group = model_equipmentStrengthenMasterConfig:getStrengthEffectGroup(currentLevel)
            if k<= table.getn(groupArr[1]) then
                local value
                value = (groupArr[1][k].propertyValue - 1)*100 .. "%"
                text:setString(LANG("%s+%s",porterty.data[groupArr[1][k].propertyName],value))
                propertyNameArr[k] = groupArr[1][k].propertyName
            end
            text:setVisible(k<= table.getn(groupArr[1])) 
        end
    end
    --下一级别的属性（前提是当前不是最高级别了）
    if typeToHighestLevel > currentLevel then
    	for m = 1,4 do
           local text
           if table.getn(propertyNameArr) > 0 then
                for n = 1,4 do
                    if groupArr[2][m].propertyName == propertyNameArr[n] then
                        text = Panel_Item2:getChildByName("Panel_Text"):getChildByName("text_"..n)
                    end
                end
           else
                text = Panel_Item2:getChildByName("Panel_Text"):getChildByName("text_"..m)
           end

--            local group = model_equipmentStrengthenMasterConfig:getStrengthEffectGroup(currentLevel + 1)
            if m<= table.getn(groupArr[2]) then
                local value = (groupArr[2][m].propertyValue - 1)*100 .. "%"
                text:setString(LANG("%s+%s",porterty.data[groupArr[2][m].propertyName],value))
            end
            text:setVisible(m<= table.getn(groupArr[2]))
    	end
    end
end

function TreasureReinforcementScene:initPanel()
    self.equipStrengthenLevel = model_equipmentStrengthenMasterConfig:getStrengthLevel(self.data.id) --当前装备强化等级
    if self.isTreasuresEnough then
        self.treasureItemStrengthenLevel = model_treasureItemStrengthenMasterConfig:getStrengthLevel(self.data.id) --当前珍宝强化等级
    end
    
    for i = 1,4 do
        local Panel_Item = self.Panel_Center:getChildByName("Panel_Item1"):getChildByName("Panel_Icon_"..i):getChildByName("Node_Icon".. i)
       
--        Panel_Item:getChildByName("Panel_Object"):setTouchEnabled(false)
          
        self:setItemInfo(Panel_Item,self.data.treasureItems[i])
        
        local but_icon = self.Panel_Center:getChildByName("Panel_Item1"):getChildByName("Panel_Icon_"..i):getChildByName("Button_Icon".. i)
        but_icon:setTag(i)
        but_icon:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                local treasureItem = model_treasureItemStorage:getNode(self.data.treasureItems[sender:getTag()-1].inventoryTreasureItemId)
                self:showDlg("treasure/TreasureStrongPanel",treasureItem,function()
                    self:setCallback()
                end) 
            end
        end)
    end
    
    self:initTopHeroInfo()
end

function TreasureReinforcementScene:setItemInfo(item,info)
    local Node_Treasure_Icon1 = item:getChildByName("Node_Treasure_Icon1")
    local LoadingBar_1 = Node_Treasure_Icon1:getChildByName("LoadingBar_1")
    local Text_Name = Node_Treasure_Icon1:getChildByName("Text_Name")
    self.needLevel = 0 --获得需要达到等级
    local currentLevel
    Node_Treasure_Icon1:getChildByName("Image_1"):loadTexture("Share/ui/item_comon.png")
--    self:initItemCacha(Node_Treasure_Icon1,{id = info.treasureItemId,type = pbCommon.ObjectType.TreasureItem})

    local Image_Icon = Node_Treasure_Icon1:getChildByName("Image_Icon")
    local Image_1 = Node_Treasure_Icon1:getChildByName("Image_1")
    Image_Icon:loadTexture(model_static_library:getAsset(info.treasureItemId,pbCommon.ObjectType.TreasureItem))
    Image_Icon:setTouchEnabled(true)

    Node_Treasure_Icon1:getChildByName("Image_1"):setTouchEnabled(false)
    local maxLevel = model_treasureItemStrengthenMasterConfig:getHighestLevel()
    local currentLevel --= model_treasureItemStrengthenMasterConfig:getMinLevel(self.treasureItemStrengthenLevel)
    if self.treasureItemStrengthenLevel == 0 then
        currentLevel = 0
    else
        currentLevel= model_treasureItemStrengthenMasterConfig:getMinLevel(self.treasureItemStrengthenLevel)
    end
    if self.treasureItemStrengthenLevel < maxLevel then
        self.needLevel = model_treasureItemStrengthenMasterConfig:getMinLevel(self.treasureItemStrengthenLevel + 1)
    else
        self.needLevel = maxLevel
    end

    local treasureItem = model_treasureItemStorage:getNode(info.inventoryTreasureItemId)
    LoadingBar_1:setPercent(100*treasureItem.level/self.needLevel)
    Text_Name:setString(treasureItem.level .. "/" .. self.needLevel)
--    Text_1:setString(LANG(static_model_treasureItem:getName(info.treasureItemId)))
    
end

function TreasureReinforcementScene:setCallback()
    local ownHeros = model_monsterStorage:getAllActiveMonsters()
    for i = 1,table.getn(ownHeros) do
        if self.data.id == ownHeros[i].id then
            self.data = ownHeros[i]
        end
    end
    self:initPanel()
end
return TreasureReinforcementScene
