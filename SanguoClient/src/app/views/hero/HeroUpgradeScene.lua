
local HeroUpgradeScene = class("ReinforcementScene3", cc.load("mvc").ViewBase)
HeroUpgradeScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
HeroUpgradeScene.STOPFRAME = 45
HeroUpgradeScene.RESOURCE_FILENAME = "Scene/ReinforcementScene3.csb"
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
--local pbCommon = _REQUIRE "pb/pbCommon.lua"
local model_itemStorage = _REQUIRE "model/model_itemStorage.lua"
local static_model_item = _REQUIRE "model/static/model_item.lua"
local static_model_monsterLevel = _REQUIRE "model/static/model_monsterLevel.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_monster = _REQUIRE "model/static/model_monster.lua"
local itemsId={}
function HeroUpgradeScene:onCreate()
    self.data = self.args[1]
    if self.args[2] then
        self.dlgCallBack = self.args[2]
    end

    --自己所有的武将列表
    self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
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

                elseif sender:getTag() == 4 then

                end
            end
        end)
    end
    self:setIndex(1)
    
    self.oldLevel = self.data.level
    self.Button_1 = self.Panel_Center:getChildByName("ProjectNode_4"):getChildByName("Panel_1"):getChildByName("Button_1")
    self.Button_1:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.heroIndex = self.heroIndex - 1
            self:setArrowShow()
            self.currExp = self.data.exp
            self.currLevel = self.data.level
            self.maxExp = static_model_monsterLevel:getExp(self.data.level)
            self:initHeroUI()
        end
    end)

    self.Button_2 = self.Panel_Center:getChildByName("ProjectNode_3"):getChildByName("Panel_1"):getChildByName("Button_1")
    self.Button_2:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.heroIndex = self.heroIndex + 1
            self:setArrowShow()
            self.currExp = self.data.exp
            self.currLevel = self.data.level
            self.maxExp = static_model_monsterLevel:getExp(self.data.level)
            self:initHeroUI()
        end
    end)
    
    
    local levelUpType
    
--    local ProjectNode_6 = self.Panel_Bottom:getChildByName("ProjectNode_6")
--    ProjectNode_6:setVisible(false)
    
    local arr = static_model_item:getSameTypeTable("ExpItem")
    for k = 1,table.getn(arr) do
        itemsId[k] = arr[k].id
    end
    self:setArrowShow()

    self.currExp = self.data.exp
    self.currLevel = self.data.level
    self.maxExp = static_model_monsterLevel:getExp(self.data.level)
    
    self.num =0
    local txtIndex =0
    local actiosNames={"Panel_Left_Light_2","Panel_Left_Light_1","Panel_Right_Light_2","Panel_Right_Light_1"}
    
    for i=1,#itemsId do
        local id = itemsId[i]
        local panelItem = self.Panel_Bottom:getChildByName("Panel_2"):getChildByName("Panel_Item"..i)
        local Button_Bg_1 = panelItem:getChildByName("Button_Bg_"..i)
        Button_Bg_1:setTag(id)
        panelItem:setTag(i)
        local ProjectNode_1 = panelItem:getChildByName("ProjectNode_"..i)
        local Panel_Object = ProjectNode_1:getChildByName("Panel_Object")
        local Image_1 = Panel_Object:getChildByName("Image_1")--图片
        local Image_2 = Panel_Object:getChildByName("Image_2")--背景
        Panel_Object:setTouchEnabled(false)
        
        local str = pbCommon.ObjectType.Item_ExpItem
        Image_1:loadTexture(model_static_library:getAsset(id,str))
        Image_2:loadTexture(model_static_library:getFrame(id,str))
        local count = model_itemStorage:getCount(id,str)
        local increment = static_model_item:getIncrement(id)
        txtIndex= txtIndex+1
        local Text_1 = panelItem:getChildByName("Text_"..txtIndex)
        local Text_2 = panelItem:getChildByName("ProjectNode_"..i):getChildByName("Panel_Object"):getChildByName("Text_1")
        Text_2:setVisible(true)
        Text_1:setString(LANG("经验+%s",increment))
        Text_2:setString(count)
        self:initImgColor(Image_1,count)

        Button_Bg_1:addTouchEventListener(function(sender,eventType)
            local id = sender:getTag()
            if eventType == 0 then
                self.num =0
                self.tiemId1 = Timer:runOnce(function(dt, data, timerId)
                    self.count = model_itemStorage:getCount(id,pbCommon.ObjectType.Item_ExpItem)
                    self.tiemId2 = Timer:start(
                        function(dt, data, timerId)
                            local increment = static_model_item:getIncrement(id)
                            if self.count == 0 then
                                Timer:kill(self.tiemId2)
                                return
                            end
                            self.currExp = self.currExp + increment
                            self.num = self.num + 1
                            self.count = self.count - 1

                            if self.currExp >= self.maxExp then --升级了
                                local obj = static_model_monsterLevel:getTotleToLevel(self.currExp + static_model_monsterLevel:getLevelToTotleExp(self.currLevel-1))
                                if model_user.level < obj[1] then
                                    Timer:kill(self.tiemId2)
                                    self.currLevel = model_user.level - 1
                                    self.currExp = static_model_monsterLevel:getExp(model_user.level - 1)
                                    self.maxExp = static_model_monsterLevel:getExp(self.currLevel)
                                else
                                    self.currLevel = obj[1]
                                    self.currExp = obj[2]
                                    self.maxExp = static_model_monsterLevel:getExp(self.currLevel)
                                end
                            end
                            self:initUpdateItem(id,sender:getParent():getTag())
                        end,0.1)
                end,.2)
            elseif eventType == 2 or eventType == 3 then
                Timer:kill(self.tiemId1)
                Timer:kill(self.tiemId2)
                --如果是武将升级界面，道具不足的项，点击则进入到掉落界面
                if model_itemStorage:getCount(id,pbCommon.ObjectType.Item_ExpItem) == 0 then
                    local nodeData = static_model_item:getNodeItem(id)
                    --self:showDlg("hero/ChipDropScene", static_model_item:getNodeItem(id), pbCommon.ObjectType.Item_ExpItem)
                    self:showDlg("hero/ChipDropScene",{id = id,type =  pbCommon.ObjectType.Item_ExpItem,
                    needNum = 1})
                    return
                end
                
                ctlMonstersUseExpItemRequest:sendRequest(function(success,data)
                    if success then
                        PopMsg.getInstance():flashShow(LANG("获得经验%s",increment*math.max(1,self.num)))
                        model_monsterStorage:updateMonster(data.monster);
                        for i = 1,table.getn(data.item) do
                            model_itemStorage:setCount(data.item[i].itemId, data.item[i].type, data.item[i].count)
                        end

                        self.data = data.monster
                        self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
                        self.currExp = self.data.exp
                        self.currLevel = self.data.level
                        self.maxExp = static_model_monsterLevel:getExp(self.data.level)
                        self.count = model_itemStorage:getCount(id,pbCommon.ObjectType.Item_ExpItem)
                        self:initUpdateItem(id,sender:getParent():getTag())
                    end
                end, self.data.id, id, math.max(1,self.num));
                
            end
        end
        )
    end

    self:initHeroUI()
end

function HeroUpgradeScene:setArrowShow()
    self.Button_1:setVisible(self.heroIndex ~= 1)
    self.Button_2:setVisible(self.heroIndex ~= table.getn(self.allActiveMonsters))
    self.data = self.allActiveMonsters[self.heroIndex]
end

function HeroUpgradeScene:initImgColor(Image_1,count)
    if count <= 0 then
        Image_1:setColor(cc.c3b(150,150,150))
    else
        Image_1:setColor(cc.c3b(255,255,255))
    end
end

function HeroUpgradeScene:initUpdateItem(id,index)
    local panelItem = self.Panel_Bottom:getChildByName("Panel_Item"..index)
    local ProjectNode = panelItem:getChildByName("ProjectNode_"..index)
    local Text_2 = ProjectNode:getChildByName("Panel_Object"):getChildByName("Text_1")
    local Image_1 = ProjectNode:getChildByName("Panel_Object"):getChildByName("Image_1")
    Text_2:setVisible(true)
    Text_2:setString(self.count)
    self:initImgColor(Image_1,self.count)
    self:initHeroUI()
end

function HeroUpgradeScene:initHeroUI()
    local Text_Title = self.Panel_Center:getChildByName("Text_Title")
    Text_Title:setString(LANG("武将升级"))
    local Panel_Item3 = self.Panel_Center:getChildByName("Panel_Item3")
    local Panel_2 = self.Panel_Center:getChildByName("Panel_2")
    --人物头像
--    local Hero_Frame = Panel_Cut:getChildByName("Hero_Frame")
--    Hero_Frame:getChildByName("Panel_RisingStar_Head"):getChildByName("Image_Hero"):loadTexture(model_monster:getIcon(self.data.id))
    --星级显示
    for i = 1,5 do
        local star = Panel_2:getChildByName("Panel_Star"):getChildByName("Star_"..i)
        star:setVisible(self.data.star>=i)
    end
    
    local Text_Num_1 = Panel_Item3:getChildByName("Text_Num_1")
    local LoadingBar_1 = Panel_Item3:getChildByName("LoadingBar_1")
    local Text_Hint_2 = Panel_Item3:getChildByName("Text_Hint_2")
    local Text_Hint_3 = Panel_Item3:getChildByName("Text_Hint_3")
    local Text_level = Panel_2:getChildByName("Text_level")
    local Text_heroname = Panel_2:getChildByName("Text_heroname")
    local Sprite_Quality_Num = Panel_2:getChildByName("Sprite_Quality_Num")
    local Job = Panel_2:getChildByName("Panel_Star"):getChildByName("Job")
    Text_Hint_2:setString("武将等级:LV"..self.currLevel)
    Text_Hint_3:setString("LV"..self.currLevel+1)
    Text_level:setString("LV"..self.currLevel)

    Text_Num_1:setString(self.currExp.."/"..self.maxExp)
    LoadingBar_1:setPercent(self.currExp/self.maxExp*100)
    
    --名字
    local color,splits = model_monsterStorage:getColor(self.data.id)
    Text_heroname:setColor(color)
    Sprite_Quality_Num:setColor(color)
--    Text_heroname:getVirtualRenderer():enableOutline(cc.c4b(44, 25, 3, 255),1)
    Job:loadTexture("Job/Icon_job"..model_monster:getType(self.data.id)..".png")
    if table.getn(splits) == 2 then
        Text_heroname:setString(LANG("%s",model_monster:getName(self.data.id)))
        Sprite_Quality_Num:setString(LANG("+"..splits[2]))
    else
        Text_heroname:setString(LANG("%s",model_monster:getName(self.data.id)))
        Sprite_Quality_Num:setString(LANG(""))
    end
    --人物头像
    self.ImageView:loadTexture(model_monster:getHalf(self.data.id))
end

function HeroUpgradeScene:refreshPage()
    self:onCreate()
end

return HeroUpgradeScene
