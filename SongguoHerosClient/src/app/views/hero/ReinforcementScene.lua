
local ReinforcementScene = class("ReinforcementScene", cc.load("mvc").ViewBase)
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
ReinforcementScene.STOPFRAME = 55

ReinforcementScene.RESOURCE_FILENAME = "Scene/ReinforcementScene.csb"


function ReinforcementScene:onCreate()
    self.data = self.args[1]
    self.fun = self.args[2]
    self.equipStrengthenLevel = 0 --当前装备强化等级
    self.treasureItemStrengthenLevel = 0 --当前珍宝强化等级
    local DivineScene = self:getResourceNode():getChildByName("DivineScene")
    DivineScene:setTouchEnabled(true)
    local Panel_Bg = DivineScene:getChildByName("Panel_Bg")
    self.Panel_Center = DivineScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    self.Panel_Bottom = DivineScene:getChildByName("Panel_B"):getChildByName("Panel_Bottom")
    local Panel_Top = DivineScene:getChildByName("Panel_T"):getChildByName("Panel_Top")
    
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    local Button_Back = Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.fun()
            self:removeDlgNoAction()
        end
    end
    )
--    
    self.buttons = {}
    local panelNames = {"装备强化","珍宝强化"}
    for i=1,2 do
        local btn = self.Panel_Center:getChildByName("Button_Cut"..i)
        btn:setTag(i)
        self.buttons[i] = btn
        btn:setTitleText(LANG(panelNames[i]))
        btn:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
               self:initPanel(sender:getTag())
            end
        end)
    end
--
    self:initPanel(1)
    
    --判断珍宝是否足够4件
    self.isTreasuresEnough = true
    for j = 1,table.getn(self.data.treasureItems) do
        if self.data.treasureItems[j].inventoryTreasureItemId == 0 then
            self.isTreasuresEnough = false
        end
    end
    --判断珍宝tab是否可以点击
--    self.buttons[2]:setBright(self.isTreasuresEnough) 
end

--装备的强化大师属性显示的是数值，珍宝的显示的是百分比，需要减去1
function ReinforcementScene:initTopHeroInfo()
    local color,splits = model_monsterStorage:getColor(self.data.id)
    local Panel_Role = self.Panel_Center:getChildByName("Panel_Role")
    local Text_heroname = Panel_Role:getChildByName("Text_heroname")
    local Sprite_Quality_Num = Panel_Role:getChildByName("Sprite_Quality_Num")
    local Panel_Star = Panel_Role:getChildByName("Panel_Star")
    local Panel_Item1 = self.Panel_Center:getChildByName("Panel_Item1")
    local Panel_Item2 = self.Panel_Center:getChildByName("Panel_Item2")
    --职业
    Panel_Star:getChildByName("Job"):loadTexture("Job/Icon_job"..model_monster:getType(self.data.id)..".png")
    --等级
    Panel_Role:getChildByName("Text_level"):setString("LV".. self.data.level)
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
    local oldImageView = self.Panel_Center:getChildByName("Panel_Role"):getChildByName("Image_Role")
    self.Panel_Center:getChildByName("Panel_Role"):removeChild(oldImageView)
    local ImageView = ccui.ImageView:create()
    ImageView:setTag(1000)
    ImageView:setPosition(339.99,255.67)
    ImageView:setLocalZOrder(-1)
    self.Panel_Center:getChildByName("Panel_Role"):addChild(ImageView)
    self.Panel_Center:getChildByName("Panel_Role"):getChildByTag(1000):loadTexture(model_monster:getHalf(self.data.id))
    
    
    --强化属性信息
    local titleStr = {"装备","珍宝"}
    local strengthenNum = {self.equipStrengthenLevel,self.treasureItemStrengthenLevel}
    local currentStrengthenLevel = strengthenNum[self.type]
    local currentLevel
    local miniLevel
--    local current
    --是否如果玩家当前强化大师等级达到最高级
    local typeToHighestLevel = 0
    if self.type == 1 then
        if currentStrengthenLevel == 0 then
            currentLevel = 0
        else
            currentLevel= model_equipmentStrengthenMasterConfig:getMinLevel(currentStrengthenLevel)
        end
        
        typeToHighestLevel = model_equipmentStrengthenMasterConfig:getHighestLevel()
        if typeToHighestLevel > currentLevel then
            miniLevel = model_equipmentStrengthenMasterConfig:getMinLevel(currentStrengthenLevel+1)
        else
            miniLevel = typeToHighestLevel
        end
    else
        if currentStrengthenLevel == 0 then
            currentLevel = 0
        else
            currentLevel= model_treasureItemStrengthenMasterConfig:getMinLevel(currentStrengthenLevel)
        end
        typeToHighestLevel = model_treasureItemStrengthenMasterConfig:getHighestLevel()
        if typeToHighestLevel > currentLevel then
            miniLevel = model_treasureItemStrengthenMasterConfig:getMinLevel(currentStrengthenLevel+1)
        else
            miniLevel = typeToHighestLevel
        end
    end
    
--    if self.type == 1 then
--        miniLevel = model_equipmentStrengthenMasterConfig:getMinLevel(currentLevel+1)
--    else
--        miniLevel = model_treasureItemStrengthenMasterConfig:getMinLevel(currentLevel+1)
--    end
    self.Panel_Center:getChildByName("Text_Item_1"):setString(LANG("%s强化大师%s级",titleStr[self.type],currentStrengthenLevel))
    Panel_Item1:getChildByName("Text_DQSX"):setString(LANG("%s强化大师%s级",titleStr[self.type],currentStrengthenLevel))
    

    --是否玩家当前强化大师等级是0级
    Panel_Item1:getChildByName("Text_3"):setVisible(currentStrengthenLevel == 0)
    Panel_Item1:getChildByName("Panel_Level"):setVisible(currentStrengthenLevel ~= 0)
    if currentStrengthenLevel == 0 then
        Panel_Item1:getChildByName("Text_3"):setString(LANG("6件装备强化到%s级\n 可获得能力提升",self.needLevel))
    end
    
    Panel_Item2:getChildByName("Text_3"):setVisible(currentLevel == typeToHighestLevel)
    Panel_Item2:getChildByName("Panel_Level"):setVisible(currentLevel < typeToHighestLevel)
    Panel_Item2:getChildByName("Text_DQSX"):setVisible(currentLevel < typeToHighestLevel)
    if currentLevel == typeToHighestLevel then
        Panel_Item1:getChildByName("Text_3"):setString(LANG("%s强化大师已达到最高级",titleStr[self.type]))
    else
        Panel_Item2:getChildByName("Text_DQSX"):setString(LANG("%s强化大师%s级\n(所有%s强化到%s级)",titleStr[self.type],currentStrengthenLevel+1,titleStr[self.type],miniLevel))
    end
    
    --装备强化大师增加绝对值属性，珍宝强化大师增加百分比属性，最多4条属性
    local groupArr = {}
    local group1
    local group2
    if self.type == 1 then -- 装备
        if currentStrengthenLevel ~= 0 then
            group1 = model_equipmentStrengthenMasterConfig:getStrengthEffectGroup(currentStrengthenLevel)
        end
        if currentLevel <= model_equipmentStrengthenMasterConfig:getHighestLevel() then
            group2 = model_equipmentStrengthenMasterConfig:getStrengthEffectGroup(currentStrengthenLevel+1)
        end
        groupArr = {group1,group2}
    else -- 珍宝
        if currentStrengthenLevel ~= 0 then
            group1 = model_treasureItemStrengthenMasterConfig:getStrengthEffectGroup(currentStrengthenLevel)
        end
        if currentLevel <= model_treasureItemStrengthenMasterConfig:getHighestLevel() then
        group2 = model_treasureItemStrengthenMasterConfig:getStrengthEffectGroup(currentStrengthenLevel+1)
        end
        groupArr = {group1,group2}
    end
    --当前的属性
    local propertyNameArr = {}
    for k = 1,4 do
        if currentStrengthenLevel ~= 0 then
            local text = Panel_Item1:getChildByName("Panel_Level"):getChildByName("text_"..k)
--        local group = model_equipmentStrengthenMasterConfig:getStrengthEffectGroup(currentLevel)
            if k<= table.getn(groupArr[1]) then
                local value
                if self.type == 1 then
                    value = groupArr[1][k].propertyValue
                else
                    value = (groupArr[1][k].propertyValue - 1)*100 .. "%"
                end
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
                        text = Panel_Item2:getChildByName("Panel_Level"):getChildByName("text_"..n)
                    end
                end
           else
                text = Panel_Item2:getChildByName("Panel_Level"):getChildByName("text_"..m)
           end

--            local group = model_equipmentStrengthenMasterConfig:getStrengthEffectGroup(currentLevel + 1)
            if m<= table.getn(groupArr[2]) then
                local value
                if self.type == 1 then
                    value = groupArr[2][m].propertyValue 
                else
                    value = (groupArr[2][m].propertyValue - 1)*100 .. "%"
                end
                text:setString(LANG("%s+%s",porterty.data[groupArr[2][m].propertyName],value))
            end
            text:setVisible(m<= table.getn(groupArr[2]))
    	end
    end
end

function ReinforcementScene:initPanel(type)
    if type == 2 and self.isTreasuresEnough == false then
        PopMsg.getInstance():flashShow(LANG("穿齐4件珍宝即可开启珍宝强化大师。"))  
        return
    end
    self.type = type
    self:setIndex(type)
    
    self.equipStrengthenLevel = model_equipmentStrengthenMasterConfig:getStrengthLevel(self.data.id) --当前装备强化等级
    if self.isTreasuresEnough then
        self.treasureItemStrengthenLevel = model_treasureItemStrengthenMasterConfig:getStrengthLevel(self.data.id) --当前珍宝强化等级
    end
    
    for i = 1,6 do
        local Panel_Item = self.Panel_Bottom:getChildByName("Panel_Item"..i)
        Panel_Item:getChildByName("Button_Bg"):setTag(i)
        Panel_Item:getChildByName("ProjectNode_1"):getChildByName("Panel_Object"):setTouchEnabled(false)
          
        if type == 1 then --装备强化
            Panel_Item:setVisible(true)
            self:setItemInfo(Panel_Item,self.data.equipment[i],1)
        elseif type == 2 then --珍宝强化
            Panel_Item:setVisible(i ~= 1 and i ~= 6)
            if i ~= 1 and i ~= 6 then
                self:setItemInfo(Panel_Item,self.data.treasureItems[i-1],2)
            end
        end
        
        Panel_Item:getChildByName("Button_Bg"):addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                if self.type == 1 then -- 装备强化界面
                    self:showDlg("hero/EquipUpgradeScene",self.data.equipment[sender:getTag()],self.data,function(data)
                        self:setCallback()
                    end)
                else --珍宝
                    local treasureItem = model_treasureItemStorage:getNode(self.data.treasureItems[sender:getTag()-1].inventoryTreasureItemId)
                    self:showDlg("treasure/TreasureStrongPanel",treasureItem,function()
                        self:setCallback()
                    end) 
                end
            end
        end)
    end
    
    self:initTopHeroInfo()
end

function ReinforcementScene:setItemInfo(item,info,type)
    local ProjectNode_1 = item:getChildByName("ProjectNode_1")
    local Panel_Object = ProjectNode_1:getChildByName("Panel_Object")
    local LoadingBar_1 = item:getChildByName("LoadingBar_1")
    local Text_1 = item:getChildByName("Text_1")
    local Text_2 = item:getChildByName("Text_2")
    self.needLevel = 0 --获得需要达到等级
    local currentLevel
    Panel_Object:getChildByName("Image_1"):loadTexture("Share/ui/item_comon.png")
    if type == 1 then
        Panel_Object:getChildByName("Image_1"):loadTexture(model_equipment:getIcon(info["id"],info["quality"]))
        Panel_Object:getChildByName("Image_2"):loadTexture(model_equipment:getFrameByQuality(info["quality"]))
        Text_1:setString(LANG(model_equipment:getName(info.id)))
        local maxLevel = model_equipmentStrengthenMasterConfig:getHighestLevel()
        if self.equipStrengthenLevel == 0 then
        	currentLevel = 0
        else
            currentLevel= model_equipmentStrengthenMasterConfig:getMinLevel(self.equipStrengthenLevel)
        end
         
        if currentLevel < maxLevel then
            self.needLevel = model_equipmentStrengthenMasterConfig:getMinLevel(self.equipStrengthenLevel + 1)
        else
            self.needLevel = maxLevel
        end
        LoadingBar_1:setPercent(100*info.level/self.needLevel)
        Text_2:setString(info.level .. "/" .. self.needLevel) 
    else
        self:initItemCacha(ProjectNode_1:getChildByName("Panel_Object"),{id = info.treasureItemId,type = pbCommon.ObjectType.TreasureItem})
        Panel_Object:getChildByName("Image_1"):setTouchEnabled(false)
        local maxLevel = model_treasureItemStrengthenMasterConfig:getHighestLevel()
        local currentLevel = model_treasureItemStrengthenMasterConfig:getMinLevel(self.treasureItemStrengthenLevel)
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
        Text_2:setString(treasureItem.level .. "/" .. self.needLevel)
        Text_1:setString(LANG(static_model_treasureItem:getName(info.treasureItemId)))
    end
    
end

function ReinforcementScene:setCallback()
    local ownHeros = model_monsterStorage:getAllActiveMonsters()
    for i = 1,table.getn(ownHeros) do
        if self.data.id == ownHeros[i].id then
            self.data = ownHeros[i]
        end
    end
    self:initPanel(self.type)
end
return ReinforcementScene
