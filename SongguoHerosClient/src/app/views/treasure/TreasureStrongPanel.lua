--武将升级界面
local TreasureStrongPanel = class("TreasureStrongPanel", cc.load("mvc").ViewBase)
TreasureStrongPanel.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
TreasureStrongPanel.STOPFRAME = 45
TreasureStrongPanel.RESOURCE_FILENAME = "Scene/TreasureStrongPanel.csb"
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
--local pbCommon = _REQUIRE "pb/pbCommon.lua"
local model_itemStorage = _REQUIRE "model/model_itemStorage.lua"
local static_model_item = _REQUIRE "model/static/model_item.lua"
local static_model_treasureItemLevelType = _REQUIRE "model/static/model_treasureItemLevelType.lua"
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local helper_treasure = _REQUIRE "model/helper/helper_treasure.lua"
local static_model_treasureItem =  _REQUIRE "model/static/model_treasureItem.lua"
local model_extension_treasure = _REQUIRE "model/extension/model_extension_treasure.lua"
local static_model_monsterLevel = _REQUIRE "model/static/model_monsterLevel.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_monster = _REQUIRE "model/static/model_monster.lua"
local porterty = _REQUIRE "common/AttributeName.lua"
local itemsId={10,11,12,13}
function TreasureStrongPanel:onCreate()
    release_print("========= WinPopupScene create");
    self.data = self.args[1]
    if self.args[2] then
        self.dlgCallBack = self.args[2]
    end
    --self.index：等于1表示武将升级
    self.index = self.args[3]
    if self.args[3] == 1 then
        --自己所有的武将列表
        self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
        for t = 1,table.getn(self.allActiveMonsters) do
            if self.data.id == self.allActiveMonsters[t].id then
                self.heroIndex = t
            end
        end
    end
    
    local TreasureStrongPanel = self:getResourceNode():getChildByName("TreasureStrongPanel")
    TreasureStrongPanel:setTouchEnabled(true)
    local Panel_Bg=TreasureStrongPanel:getChildByName("Panel_Bg")
    local Panel_C = TreasureStrongPanel:getChildByName("Panel_C")
    self.Panel_Center_TreasureStrongPanel=Panel_C:getChildByName("Panel_Center_TreasureStrongPanel")
--    local treasureItem = model_treasureItemStorage:getNode(self.data.inventoryTreasureItemId)
    local Button_Cut2 = self.Panel_Center_TreasureStrongPanel:getChildByName("Button_Cut2")
    local Button_Cut1 = self.Panel_Center_TreasureStrongPanel:getChildByName("Button_Cut1")
    self.buttons = {}
    self.buttons[1] = Button_Cut1
    self.buttons[2] = Button_Cut2
    self:setIndex(1)
--    Button_Cut2:addTouchEventListener(function(sender,eventType)
--        if  eventType == 2 then
--            self:showDlg("treasure/TreasureRefinePanel",treasureItem,self.dlgCallBack)
--            self:removeDlgNoAction()
--        end
--    end)
    
    self.Panel_Center_Hero=Panel_C:getChildByName("Panel_Center_Hero")
    local Panel_T=TreasureStrongPanel:getChildByName("Panel_T")
    local Panel_Top=Panel_T:getChildByName("Panel_Top")
    local Panel_B=TreasureStrongPanel:getChildByName("Panel_B")
    self.Panel_Bottom=Panel_B:getChildByName("Panel_Bottom")
    SCREEN_TOP(Panel_T)
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_BOTTOM(Panel_B)
    local Node_Top = Panel_Top:getChildByName("Node_Top")
    self:initTopBar(Node_Top)
    local Button_Close = Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            if self.args[3] == 1 then -- 武将升级
                self.dlgCallBack({cmd = "LevelUp"})
            else -- 珍宝强化
                self.dlgCallBack({cmd = "strong",index = self.index})
            end
            self:removeDlgNoAction()
        end
    end
    )
    self.oldLevel = self.data.level
    self.Button_1 = self.Panel_Center_Hero:getChildByName("ProjectNode_8"):getChildByName("Panel_1"):getChildByName("Button_1")
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

    self.Button_2 = self.Panel_Center_Hero:getChildByName("ProjectNode_7"):getChildByName("Panel_1"):getChildByName("Button_1")
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
    local ProjectNode_5 = self.Panel_Center_TreasureStrongPanel:getChildByName("ProjectNode_5")
    
    local ProjectNode_6 = self.Panel_Bottom:getChildByName("ProjectNode_6")
    ProjectNode_6:setVisible(false)
    if self.args[3] == 1 then
        local arr = static_model_item:getSameTypeTable("ExpItem")
        for k = 1,table.getn(arr) do
            itemsId[k] = arr[k].id
        end
        self:setArrowShow()

        self.Panel_Center_TreasureStrongPanel:setVisible(false)
        self.Panel_Center_Hero:setVisible(true)
        self.currExp = self.data.exp
        self.currLevel = self.data.level
        self.maxExp = static_model_monsterLevel:getExp(self.data.level)
--        ProjectNode_5:getChildByName("Panel_Object"):getChildByName("Image_1"):loadTexture(model_monster:getIcon(self.data.id))
    else
        local treasureItem = model_treasureItemStorage:getNode(self.data.inventoryTreasureItemId)
        Button_Cut2:addTouchEventListener(function(sender,eventType)
            if  eventType == 2 then
                self:showDlg("treasure/TreasureRefinePanel",treasureItem,self.dlgCallBack)
                self:removeDlgNoAction()
            end
        end)
        itemsId={10,11,12,13}
        self.Panel_Center_TreasureStrongPanel:setVisible(true)
        self.Panel_Center_Hero:setVisible(false)
        levelUpType = static_model_treasureItem:getLevelUpType(self.data.treasureItemId)
        self.currExp = model_treasureItemStorage:getExp(self.data.inventoryTreasureItemId)
        self.currLevel = model_treasureItemStorage:getLevel(self.data.inventoryTreasureItemId)
        self.maxExp = static_model_treasureItemLevelType:getExp(levelUpType,self.currLevel)
        self:initItemCacha(ProjectNode_5:getChildByName("Panel_Object"),{id = self.data.treasureItemId,type = pbCommon.ObjectType.TreasureItem})
    end
    self.num =0
    local txtIndex =0
    local actiosNames={"Panel_Left_Light_2","Panel_Left_Light_1","Panel_Right_Light_2","Panel_Right_Light_1"}
    
    for i=1,#itemsId do
        local id = itemsId[i]
        local panelItem = self.Panel_Bottom:getChildByName("Panel_Item"..i)
        local Button_Bg_1 = panelItem:getChildByName("Button_Bg_"..i)
        Button_Bg_1:setTag(id)
        panelItem:setTag(i)
        local ProjectNode_1 = panelItem:getChildByName("ProjectNode_"..i)
        local Panel_Object = ProjectNode_1:getChildByName("Panel_Object")
        local Image_1 = Panel_Object:getChildByName("Image_1")--图片
        local Image_2 = Panel_Object:getChildByName("Image_2")--背景
        Panel_Object:setTouchEnabled(false)
        
        local str
        if self.args[3] == 1 then
            str = pbCommon.ObjectType.Item_ExpItem
        else
            str = pbCommon.ObjectType.Item_TreasureItemExpItem
        end
        Image_1:loadTexture(model_static_library:getAsset(id,str))
        Image_2:loadTexture(model_static_library:getFrame(id,str))
        local count = model_itemStorage:getCount(id,str)
        local increment = static_model_item:getIncrement(id)
        txtIndex= txtIndex+1
        local Text_1 = panelItem:getChildByName("Text_"..txtIndex)
        txtIndex= txtIndex+1
        local Text_2 = panelItem:getChildByName("ProjectNode_"..i):getChildByName("Panel_Object"):getChildByName("Text_1")
        Text_2:setVisible(true)
        Text_1:setString(LANG("经验+%s",increment))
        Text_2:setString(count)
        self:initImgColor(Image_1,count)
        --如果是武将升级界面，道具不足的项，点击则进入到掉落界面
--        if count == 0 and self.args[3] == 1 then
--            self:showDlg("hero/ChipDropScene",self.data,pbCommon.ObjectType.Monster) 
--        else
--            
--        end
        local ProjectNode__Animation_TreasureUp2_End = self.Panel_Center_TreasureStrongPanel:getParent():getChildByName("ProjectNode__Animation_TreasureUp2_End")
        ProjectNode__Animation_TreasureUp2_End:setVisible(false)
        Button_Bg_1:addTouchEventListener(function(sender,eventType)
            local id = sender:getTag()
            if eventType == 0 then
                self.num =0
                self.tiemId1 = Timer:runOnce(function(dt, data, timerId)
                    if self.args[3] == 1 then
                        self.count = model_itemStorage:getCount(id,pbCommon.ObjectType.Item_ExpItem)
                    else
                        self.count = model_itemStorage:getCount(id,pbCommon.ObjectType.Item_TreasureItemExpItem)
                    end
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
                                if self.args[3] == 1 then
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
                            else
                                Timer:kill(self.tiemId2)
                                self.currExp = math.abs(self.maxExp -self.currExp)
                                self.currLevel = self.currLevel + 1
                            end
                            end
                            self:initUpdateItem(id,sender:getParent():getTag())
                        end,0.1)
                end,.2)
            elseif eventType == 2 or eventType == 3 then
                Timer:kill(self.tiemId1)
                Timer:kill(self.tiemId2)
                --如果是武将升级界面，道具不足的项，点击则进入到掉落界面
                if model_itemStorage:getCount(id,pbCommon.ObjectType.Item_ExpItem) == 0 and self.args[3] == 1 then
                    local nodeData = static_model_item:getNodeItem(id)
                    --self:showDlg("hero/ChipDropScene", static_model_item:getNodeItem(id), pbCommon.ObjectType.Item_ExpItem)
                    self:showDlg("hero/ChipDropScene",{id = id,type =  pbCommon.ObjectType.Item_ExpItem,
                    needNum = 1})
                    return
                end
                if self.args[3] == 1 then
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
                else
                    local str
                    if self.args[3] == 1 then
                        str = pbCommon.ObjectType.Item_ExpItem
                    else
                        str = pbCommon.ObjectType.Item_TreasureItemExpItem
                    end
                    local count = model_itemStorage:getCount(id,str)
                    if count == 0 then
                        --self:showDlg("hero/ChipDropScene",{id = id,quality = model_treasureItemStorage:getQuality(self.data.inventoryTreasureItemId)},pbCommon.ObjectType.Item_TreasureItemExpItem)
                        self:showDlg("hero/ChipDropScene",{id = id,type =  pbCommon.ObjectType.Item_TreasureItemExpItem,
                            needNum = 1})
                        return
                    end
                    model_user.selectTreasureItemUpgradeTargetInventoryId = self.data.inventoryTreasureItemId;
                    model_user.selectTreasureItemUpgradeConsumeId = id;
                    model_user.selectTreasureItemUpgradeConsumeCount = math.max(1,self.num);
                    helper_treasure:levelUp(function(success,data)
                        if success then
                            --PopMsg.getInstance():flashShow(LANG("获得经验%s",increment*math.max(1,self.num)))
                            if self.upNode == nil and self.upAction ==nil  then
                                local node, action = self:createNode("Node/Node_Animation_TreasureUp2_Sart.csb",false)
                                self.upNode = node
                                self.upAction = action
                                self.Panel_Bottom:addChild(self.upNode)
                            end
                            for j = 1, 4 do
                                self.upNode:getChildByName(actiosNames[j]):setVisible(false)
                            end
                            
                            local currentNode = self.upNode:getChildByName(actiosNames[i])
                            currentNode:setVisible(true)
                            self.upAction:gotoFrameAndPlay(55,66,false)
                            
                            self.upNode:setPosition(ProjectNode_6:getPosition())
                            
                            --local dis = self.upNode:getChildByName(actiosNames[2]):getPositionX() - self.upNode:getChildByName(actiosNames[1]):getPositionX()
                            local p = cc.p(73,327)
                            p.y = p.y -  (CC_DESIGN_RESOLUTION.height-CC_SCREEN_SIZE.height)/2
                            --local px,py = self.Panel_Top:getPosition()
                            Timer:runOnce(function()
                                currentNode:runAction(
                                    cc.Sequence:create(
                                        cc.EaseIn:create(cc.MoveTo:create(.5,cc.p(p.x,p.y)),5),
                                        cc.CallFunc:create(function()
                                            --currentNode:setVisible(false)
                                            self.upAction:gotoFrameAndPlay(66,false)
                                            if self.upNodeEnd == nil and self.upActionEnd ==nil  then
                                                local node, action = self:createNode("Node/Node_Animation_TreasureUp2_End.csb",false)
                                                self.upNodeEnd = node
                                                self.upActionEnd = action
                                                self.Panel_Center_TreasureStrongPanel:getParent():addChild(self.upNodeEnd)
                                                self.upNodeEnd:setPosition(ProjectNode__Animation_TreasureUp2_End:getPosition())
                                            end
                                            self.upActionEnd:gotoFrameAndPlay(90,false)
                                        end)
                                        ))
                            end,0.2)
                            self.currExp = model_treasureItemStorage:getExp(self.data.inventoryTreasureItemId)
                            self.currLevel = model_treasureItemStorage:getLevel(self.data.inventoryTreasureItemId)
                            self.maxExp = static_model_treasureItemLevelType:getExp(levelUpType,self.currLevel)
                            self.count = model_itemStorage:getCount(id,pbCommon.ObjectType.Item_TreasureItemExpItem)
                            self:initUpdateItem(id,sender:getParent():getTag())
                        end
                    end);
                end
            end
        end
        )
    end

    if self.args[3] == 1 then
        self:initHeroUI()
    else
        self:initUi()
    end

end

function TreasureStrongPanel:setArrowShow()
    self.Button_1:setVisible(self.heroIndex ~= 1)
    self.Button_2:setVisible(self.heroIndex ~= table.getn(self.allActiveMonsters))
    self.data = self.allActiveMonsters[self.heroIndex]
end

function TreasureStrongPanel:initImgColor(Image_1,count)
    if count <= 0 then
        Image_1:setColor(cc.c3b(150,150,150))
    else
        Image_1:setColor(cc.c3b(255,255,255))
    end
end

function TreasureStrongPanel:initUpdateItem(id,index)
    local panelItem = self.Panel_Bottom:getChildByName("Panel_Item"..index)
    local ProjectNode = panelItem:getChildByName("ProjectNode_"..index)
    local Text_2 = ProjectNode:getChildByName("Panel_Object"):getChildByName("Text_1")
    local Image_1 = ProjectNode:getChildByName("Panel_Object"):getChildByName("Image_1")
    Text_2:setVisible(true)
    Text_2:setString(self.count)
    self:initImgColor(Image_1,self.count)
    if self.args[3] == 1 then
    	self:initHeroUI()
    else
        self:initUi()
    end
end

function TreasureStrongPanel:initUi()
    local Text_Title = self.Panel_Center_TreasureStrongPanel:getChildByName("Text_Title")
    Text_Title:setString(LANG("珍宝强化"))
    local Text_Num = self.Panel_Center_TreasureStrongPanel:getChildByName("Text_Num")
    local LoadingBar_1 = self.Panel_Center_TreasureStrongPanel:getChildByName("LoadingBar_1")
    local Text_Level_2 = self.Panel_Center_TreasureStrongPanel:getChildByName("Text_Level_2")
    local Text_Level_1 = self.Panel_Center_TreasureStrongPanel:getChildByName("Text_Level_1")
    local name = static_model_treasureItem:getName(self.data.treasureItemId)
    if self.maxExp ==nil then
        Text_Num:setString(self.currExp.."/".."Max")
        LoadingBar_1:setPercent(0)
    else
        LoadingBar_1:setPercent(self.currExp/self.maxExp*100)
        Text_Num:setString(self.currExp.."/"..self.maxExp)
    end
    Text_Level_2:setString(LANG("%slv%s",name,self.currLevel))
    Text_Level_1:setString(self.currLevel+1)
    local treasureItem = model_treasureItemStorage:getNode(self.data.inventoryTreasureItemId)
    local attributesLevel = model_extension_treasure:getAttributesLevel(treasureItem)
    local attributesNextLevel = model_extension_treasure:getAttributesNextLevel(treasureItem)
    
    for i=1,3 do
        local txt_current = self.Panel_Center_TreasureStrongPanel:getChildByName("current_"..i)
        local text_bg = self.Panel_Center_TreasureStrongPanel:getChildByName("text_bg_"..i)
        local txt_next = self.Panel_Center_TreasureStrongPanel:getChildByName("next_"..i)
        local Sprite_Arrow_3 = self.Panel_Center_TreasureStrongPanel:getChildByName("Sprite_Arrow_"..i)
        txt_current:setVisible(false)
        text_bg:setVisible(false)
        txt_next:setVisible(false)
        Sprite_Arrow_3:setVisible(false)
    end
    
    local strNum = 0
    if self.oldLevel ~= self.currLevel then
        self:getActionNode():gotoFrameAndPlay(245,310,false)
        self.oldLevel = self.currLevel
    end
    --self:getActionNode():gotoFrameAndPlay(245,310,false)
    for key,value in pairs(attributesLevel) do
        strNum= strNum + 1
        local attLevel = self.Panel_Center_TreasureStrongPanel:getChildByName("current_"..strNum)
        local text_bg = self.Panel_Center_TreasureStrongPanel:getChildByName("text_bg_"..strNum)
        text_bg:setVisible(true)
        attLevel:setVisible(true)
        attLevel:setString(porterty.data[key].."+"..value)
    end
    
    local strNextNum = 0
    if self.maxExp ~=nil then
        for key,value in pairs(attributesNextLevel) do
            strNextNum= strNextNum + 1
            local attLevel = self.Panel_Center_TreasureStrongPanel:getChildByName("next_"..strNextNum)
            local Sprite_Arrow_3 = self.Panel_Center_TreasureStrongPanel:getChildByName("Sprite_Arrow_"..strNextNum)
            attLevel:setVisible(true)
            Sprite_Arrow_3:setVisible(true)
            attLevel:setString(porterty.data[key].."+"..value)
        end
    end 
    
end

function TreasureStrongPanel:initHeroUI()
    local Text_Title_2 = self.Panel_Center_Hero:getChildByName("Text_Title_2")
    Text_Title_2:setString(LANG("武将升级"))
    self.Panel_Center_Hero:getChildByName("ScrollView_2"):setTouchEnabled(false)
    local Panel_Cut = self.Panel_Center_Hero:getChildByName("ScrollView_2"):getChildByName("Panel_Cut")
    local Hero_Frame = Panel_Cut:getChildByName("Hero_Frame")
    Hero_Frame:getChildByName("Panel_RisingStar_Head"):getChildByName("Image_Hero"):loadTexture(model_monster:getIcon(self.data.id))
    --星级显示
    for i = 1,5 do
        local star = Hero_Frame:getChildByName("Panel_RisingStar_Head"):getChildByName("Panel_Star"):getChildByName("Image_Star_"..i)
        star:setVisible(self.data.star>=i)
    end
    
    local Text_Num_1 = Panel_Cut:getChildByName("Text_Num_1")
    local LoadingBar_1_1 = Panel_Cut:getChildByName("LoadingBar_1_1")
    local current_1_1 = Panel_Cut:getChildByName("current_1_1")
    current_1_1:setString("LV"..self.currLevel.."->LV"..self.currLevel+1)

    Text_Num_1:setString(self.currExp.."/"..self.maxExp)
    LoadingBar_1_1:setPercent(self.currExp/self.maxExp*100)
end

function TreasureStrongPanel:refreshPage()
    self:onCreate()
end

return TreasureStrongPanel
