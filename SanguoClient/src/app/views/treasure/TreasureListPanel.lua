
local TreasureListPanel = class("TreasureListPanel", cc.load("mvc").ViewBase)
TreasureListPanel.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
TreasureListPanel.STOPFRAME = 30
TreasureListPanel.RESOURCE_FILENAME = "Scene/ArenaRankPop.csb"
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local model_treasureItemPartStorage = _REQUIRE "model/model_treasureItemPartStorage.lua"
local model_treasureItem =  _REQUIRE "model/static/model_treasureItem.lua"
local model_treasureItemPart =  _REQUIRE "model/static/model_treasureItemPart.lua"
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
local model_extension_treasure = _REQUIRE "model/extension/model_extension_treasure.lua"
local porterty = _REQUIRE "common/AttributeName.lua"
--local pbCommon = _REQUIRE "pb/pbCommon.lua"
local helper_treasure = _REQUIRE "model/helper/helper_treasure.lua"
--cclog(porterty.data["HP"])
function TreasureListPanel:onCreate()
    local ArenaRankPop = self:getResourceNode():getChildByName("ArenaRankPop")
    local Panel_C = ArenaRankPop:getChildByName("Panel_C")
    self.Panel_Center = Panel_C:getChildByName("Panel_Center")
    local Panel_Top = ArenaRankPop:getChildByName("Panel_Top")
    local Panel_Bg = ArenaRankPop:getChildByName("Panel_Bg")
    ArenaRankPop:setTouchEnabled(true)
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    Panel_Bg:setTouchEnabled(true)
    local data = self.args[1]
    local type
    if data ~= nil then
        type =data[1] --是否是选择珍宝
        self.slot = data[2]
        self.monsterId = data[3]
        self.dlgCallBack = data[4]
    end
    
    local Button_3 = self.Panel_Center:getChildByName("Button_3")
    local Text_2 = self.Panel_Center:getChildByName("Text_2")
    Button_3:setVisible(false)
    Text_2:setVisible(false)
    self.isSelect = (type == 1) --是否选择珍宝
--    --关闭按钮
--    local  Button_Close=Panel_Top:getChildByName("Button_Close")
--    Button_Close:setVisible(false)
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
--    local  Panel_TitleBg=Panel_Top:getChildByName("Panel_TitleBg")
--    Panel_TitleBg:setTouchEnabled(false)
    Button_Back:addTouchEventListener(handler(self,self.onClickHandler))
    --local  Panel_TitleBg=Panel_Top:getChildByName("Panel_TitleBg")
    local  Text_1=self.Panel_Center:getChildByName("Text_1")
    Text_1:setString("宝物")
    self.buttons = {}
    local tabNames ={"宝物","碎片"}
    
    for i=1,4 do
        local ProjectNode =self.Panel_Center:getChildByName("ProjectNode_"..i)
        ProjectNode:setVisible(false)
        if i<= #tabNames and (self.isSelect == false) then
            ProjectNode:setVisible(true)
            local btn = ProjectNode:getChildByName("Panel_tab"):getChildByName("Button_Select_1")
            self.buttons[i] = btn
            btn:setTitleText(tabNames[i])
            btn:addTouchEventListener(function(sender,eventType)
                if eventType==2 then
                    self:setIndex(sender:getTag())
                    if sender:getTag()==1 then
                        self:initTreasureItem()
                    elseif sender:getTag()==2 then
                        self:initTreasureItemPart()
                    end
                end
                
            end)
        end
        
    end
    self:setIndex(1)
    
    
    self:initTreasureItem()
    
    self:initTabRedSpot()
end

function TreasureListPanel:initTabRedSpot()
    local ProjectNode2 =self.Panel_Center:getChildByName("ProjectNode_"..2)
    local Image_1 = ProjectNode2:getChildByName("Panel_tab"):getChildByName("Image_1")
    Image_1:setVisible(self:canMergeTreasure())
end

function TreasureListPanel:initTreasureItemPart()
    self.scrollView = self.Panel_Center:getChildByName("HeroScrollView_1")
    self.scrollView:removeAllChildren()
    self.treasurePartList = model_treasureItemPartStorage:getItems()
    local num =#self.treasurePartList
    self.node=self:createNode("Node/Node_Item_1.csb")
    --local panel_Item_1 = self.node:getChildByName("panel_Item_1")
    
    self.item = self.node:getChildByName("panel_Item_1")
    self.contentSize =self.item:getContentSize()
    self.sSize=cc.size(self.scrollView:getInnerContainerSize().width,math.max(self.scrollView:getContentSize().height,(self.contentSize.height+self.space)*math.ceil(num/2)));
    self.scrollView:setInnerContainerSize(self.sSize)
    table.sort(self.treasurePartList,function(item1,item2)
        local staticQuality1 = model_treasureItemPart:getQuality(item1.treasureItemPartId)
        local staticQuality2 = model_treasureItemPart:getQuality(item2.treasureItemPartId)
        local needCount1 = model_treasureItemPart:getNeedNumber(item1.treasureItemPartId)
        local needCount2 = model_treasureItemPart:getNeedNumber(item2.treasureItemPartId)
        
        if (item1.count >= needCount1 and item2.count < needCount2)  then
            return true
        elseif (item1.count < needCount1 and item2.count >= needCount2) then
            return false
        elseif staticQuality1 ~= staticQuality2 then
             return staticQuality1 > staticQuality2
        else
            return item1.treasureItemId>item2.treasureItemId 
        end
    end)
   
    --self.itemsArr = {}
    local count = 10
    self:initTreasureItemPartRange(1,math.min(num,count)) 
    if num-count>0 then
        Timer:runOnce(function()
            self:initTreasureItemPartRange(count+1,num)
        end,0.2)
    else
    end
    if num > 8 then
        self.scrollView:jumpToTop()
        self:addScrollViewBar(self.scrollView,5)
    else
        self:hideScrollViewBar(self.scrollView)
    end
end

function TreasureListPanel:canMergeTreasure()
    local itemParts =  model_treasureItemPartStorage:getItems()
    for i= 1,#itemParts do
        local needCount = model_treasureItemPart:getNeedNumber(itemParts[i].treasureItemPartId)
        local currentCount = itemParts[i].count
        if currentCount >= needCount then
        	return true
        end
    end
    return false
end

function TreasureListPanel:initTreasureItemPartRange(start,endIndex)
    self.space = 0  
    for i=start,endIndex do
        local newItem = self.item:clone()
        newItem:setCascadeOpacityEnabled(true)
        local data = self.treasurePartList[i]
        local index = i-1
        newItem:setTag(i);
        self.scrollView:addChild(newItem)
        local x=self.contentSize.width/2-15+(self.contentSize.width-15)*(index%2);
        --local t = math.floor(index/2)
        local y=self.sSize.height-(self.contentSize.height+self.space)*math.floor(index/2)-self.contentSize.height/2
        newItem:setPosition(cc.p(x,y))
--        local Panel_Team = newItem:getChildByName("Panel_Team")
--        local Panel_Item_1 = newItem:getChildByName("Panel_Item_1")
--        Panel_Team:setVisible(false)
--        Panel_Item_1:setVisible(false)
        local Panel_Item_1 = newItem:getChildByName("Panel_Item_1")
        Panel_Item_1:setVisible(false)
        local Panel_Item_2 = newItem:getChildByName("Panel_Item_2")
        local Panel_Bg = newItem:getChildByName("Panel_Bg")
        Panel_Item_2:setCascadeOpacityEnabled(true)
        Panel_Bg:setCascadeOpacityEnabled(true)
        Panel_Bg:getChildByName("Image_Head_Bg"):setCascadeOpacityEnabled(true)
--        local Image_82 = Panel_Item_2:getChildByName("Image_82")
--        Image_82:setVisible(false)
        
        local LoadingBar_1 = Panel_Item_2:getChildByName("LoadingBar_1")
        local Text_Name = Panel_Item_2:getChildByName("Text_Name")
        local Image_Head_Bg = Panel_Item_2:getChildByName("Image_Head_Bg")
        local Image_Head_Off =Panel_Item_2:getChildByName("Image_Head_Off")
        local Image_Head = Panel_Item_2:getChildByName("Image_Head")
        local Image_Icon_Job = Panel_Item_2:getChildByName("Image_Icon_Job")
        Image_Icon_Job:setVisible(false)
        local Text_14 = Panel_Item_2:getChildByName("Text_14")
        Image_Head:loadTexture(model_static_library:getAsset(data.treasureItemPartId,pbCommon.ObjectType.TreasureItemPart))
        Image_Head_Bg:loadTexture(model_static_library:getFrame(data.treasureItemPartId,pbCommon.ObjectType.TreasureItemPart))
        Image_Head_Off:setOpacity(0)
        Text_Name:setString(model_treasureItemPart:getName(data.treasureItemPartId))
        Text_Name:setColor( model_static_library:getColor(data.treasureItemPartId,pbCommon.ObjectType.TreasureItemPart))
        local needCount = model_treasureItemPart:getNeedNumber(data.treasureItemPartId)
        local currentCount = data.count
        --Text_14:setVisible(currentCount>=needCount)
        LoadingBar_1:setPercent(currentCount/needCount*100)
--        local Image_1_0 = Panel_Item_2:getChildByName("Image_1_0")
--        Image_1_0:setVisible(currentCount >= needCount)
        local Image_82 = Panel_Item_2:getChildByName("Image_82")
        Image_82:setVisible(currentCount>=needCount)
        if currentCount>=needCount then
            Text_14:setString(LANG("可合成"))
            
        else
            Text_14:setString(currentCount.."/"..needCount)
        end
        newItem:setTouchEnabled(true)
        newItem:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
--                for j = 1,#self.treasurePartList do
--                    local newItem = self.scrollView:getChildByTag(j)
--                    local Panel_Item_2 = newItem:getChildByName("Panel_Item_2")
--                    local Image_82 = Panel_Item_2:getChildByName("Image_82")
--                    Image_82:setVisible(i == j)
--                end
                
                local item = self.treasurePartList[sender:getTag()]
                if currentCount < needCount then
                    self:showDlg("hero/ChipDropScene",{id = item.treasureItemPartId,type =  pbCommon.ObjectType.TreasureItemPart,needNum = needCount})
                else
                    
                    ctlMonstersTreasureItemMergeRequest:sendRequest(function(sucess,data)
                        if sucess then
                            PopMsg.getInstance():flashShow(LANG("合成成功!"))
                            self:initTreasureItemPart()
                            self:initTabRedSpot()
                        end
                    end, item.treasureItemPartId);
                end
                
            end
        end
        )
    end
end



function TreasureListPanel:initTreasureItem()
    self.scrollView = self.Panel_Center:getChildByName("HeroScrollView_1")
    self.scrollView:removeAllChildren()
    self.node=self:createNode("Node/Node_SelectTreasurePop_Item.csb")
    self.item = self.node:getChildByName("Panel_SelectTreasurePop_Item")
    self.item:setTouchEnabled(true)
    
    self.contentSize =self.item:getContentSize()
    if  self.isSelect == true then
        self.treasureList = {}
        local treaList = model_treasureItemStorage:getItems()
        local selectIndex =0
        for i = 1,#treaList do
            if treaList[i].quippedType == self.slot then
                if treaList[i].useMonster == self.monsterId then
                    selectIndex = #self.treasureList + 1
                end
                self.treasureList[#self.treasureList + 1] = treaList[i]
            end
        end
        if selectIndex ~= 0  and #self.treasureList ~= 0 then
            local t = self.treasureList[1]
            self.treasureList[1] = self.treasureList[selectIndex]
            self.treasureList[selectIndex] = t
        end
    else
        self.treasureList = model_treasureItemStorage:getItems()
    end
    
    
    self.space = 10
    local num = #self.treasureList
    table.sort(self.treasureList,function(item1,item2)
        local staticQuality1 = model_treasureItem:getQuality(item1.treasureItemId)
        local staticQuality2 = model_treasureItem:getQuality(item2.treasureItemId)
        cclog(item1.inventoryTreasureItemId)
        cclog(item2.inventoryTreasureItemId)
        if item1.useMonster ~= item2.useMonster then
            if self.isSelect == true then
                if item1.useMonster == self.monsterId and item2.useMonster ~= self.monsterId then
                	return true
                elseif item1.useMonster ~= self.monsterId and item2.useMonster == self.monsterId then
                    return false
                else
                    return item1.useMonster > item2.useMonster
                end
            else
                return item1.useMonster > item2.useMonster
            end
            
        elseif staticQuality1 ~= staticQuality2 then
            return staticQuality1 > staticQuality2
        elseif item1.quality~= item2.quality then
            return item1.quality > item2.quality
        elseif item1.level~= item2.level then
            return item1.level>item2.level
        elseif item1.exp ~= item2.exp then
            return item1.exp>item2.exp 
        else
            return item1.treasureItemId>item2.treasureItemId 
        end
    end)
    self.sSize=cc.size(self.scrollView:getInnerContainerSize().width,math.max(self.scrollView:getContentSize().height,(self.contentSize.height+self.space)*math.ceil(num)));
    --cclog("sssss"..sSize.width)
    self.scrollView:setInnerContainerSize(self.sSize)
    local count = 5
    self:initTreasureItemRange(1,math.min(num,count)) 

    if num-count>0 then
        self.scrollView:jumpToTop()
        self:addScrollViewBar(self.scrollView, -10)
        local timerId = Timer:runOnce(function(dt, data, timerId)
            self:initTreasureItemRange(count+1,num)
        end,0.2)
    else
        self:hideScrollViewBar(self.scrollView)   
    end
end

function TreasureListPanel:dlgCallBack(data)
    self.treasureList = model_treasureItemStorage:getItems()
    if data.cmd == "strong" then
        self:initTreasureItem()
    elseif data.cmd == "refine" then
        self:initTreasureItem()
    end
end

function TreasureListPanel:initTreasureItemRange(start,endIndex)
    --cclog("timeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee")
    for i=start,endIndex do
        local newItem = self.scrollView:getChildByTag(i)
        if newItem == nil then
            --local parentItem = ccui.Layout:create()
            newItem = self.item:clone()
            --parentItem:setContentSize(newItem:getContentSize())
            --parentItem:setAnchorPoint(newItem:getAnchorPoint())
            --parentItem:addChild(newItem)
            newItem:setCascadeOpacityEnabled(true)
            self.scrollView:addChild(newItem)
        end
        local data = self.treasureList[i]
        local index = i-1
        newItem:setTag(i);
        local Image_Item_Light = newItem:getChildByName("Image_Item_Light")
        Image_Item_Light:setVisible(false)
        
        local x=self.contentSize.width/2+15
        --local t = math.floor(index/2)
        local y=self.sSize.height-(self.contentSize.height+self.space)*math.floor(index)-self.contentSize.height/2
        newItem:setPosition(cc.p(x,y))
        local Text_Refine2 = newItem:getChildByName("Text_Refine2")
        Text_Refine2:setVisible(false)
        local Button_stren = newItem:getChildByName("Button_2")
        local Button_jinglian = newItem:getChildByName("Button_1")
        Button_stren:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                local  id = sender:getParent():getTag()
                
                self:showDlg("treasure/TreasureStrongPanel",self.treasureList[sender:getParent():getTag()],function()
                    self:initTreasureItem()
                end)
            end
        end
        )
        Button_jinglian:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                if self.isSelect == false then
                    self:showDlg("treasure/TreasureRefinePanel",self.treasureList[sender:getParent():getTag()],function()
                        self:initTreasureItem()
                    end)
                else --装备宝物
                    helper_treasure:equipTreasure(self.treasureList[sender:getParent():getTag()].inventoryTreasureItemId, self.monsterId, function(sucess,data)
                        if sucess then
                            self.dlgCallBack({cmd="select"})
                            self:removeDlgNoAction()
                        end
                    end)
--                    ctlMonstersChangeTreasureItemRequest:sendRequest(function(sucess,data)
--                        if sucess then
--                            self.dlgCallBack({cmd="select"})
--                            self:removeDlgNoAction()
--                        end
--                    end, self.monsterId, self.slot, self.treasureList[sender:getParent():getTag()].inventoryTreasureItemId);
                end
                
            end
        end
        )
        Button_stren:setVisible(self.isSelect == false)
        Button_jinglian:setBright(true)
        Button_jinglian:setEnabled(true)
        if self.isSelect == true then
            if data.useMonster == self.monsterId then
                Button_jinglian:setTitleText(LANG("已装备"))
                Button_jinglian:setBright(false)
                Button_jinglian:setEnabled(false)
            else
                Button_jinglian:setTitleText(LANG("装备"))
            end
            
        else
            Button_jinglian:setTitleText(LANG("精炼"))
        end
        local Text_TreasureName = newItem:getChildByName("Text_TreasureName")
        Text_TreasureName:setString(model_treasureItem:getName(data.treasureItemId))
        
        Text_TreasureName:setColor( model_static_library:getColor(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
        local Image_Bg = newItem:getChildByName("Image_Bg")
        local Image_Icon = newItem:getChildByName("Image_Icon")
        local Text_Match = newItem:getChildByName("Text_Match")
        local Text_Refine = newItem:getChildByName("Text_Refine")
        local Text_Name = newItem:getChildByName("Text_Name")
        local Image_Name = newItem:getChildByName("Image_Name")
        local Image_Name_Bg = newItem:getChildByName("Image_Name_Bg")
        local stren_name = newItem:getChildByName("stren_name")
        local refine_name = newItem:getChildByName("refine_name")

        Image_Name:setVisible(data.useMonster~=0)
        Image_Name_Bg:setVisible(data.useMonster~=0)
        Text_Match:setVisible(false)
        Text_Refine:setVisible(false)
        --Image_Icon:setTouchEnabled(true)
        if data.useMonster~=0 then
            local name = model_monster:getName(data.useMonster)
            local monster = model_monsterStorage:getMonster(data.useMonster);
            Text_Name:setString(name)
            
        else 
            Text_Name:setString("")
        end
        
        newItem:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                for j = 1,#self.treasureList do
                    local newItem = self.scrollView:getChildByTag(j)
                    local Image_Item_Light = newItem:getChildByName("Image_Item_Light")
                    Image_Item_Light:setVisible(i == j)
                end
                local Image_Item_Light = sender:getChildByName("Image_Item_Light")
                Image_Item_Light:setVisible(true)
                
                if data.useMonster~=0 then
                    self:showDlg("treasure/TreasureDetailsPanel",data,data.useMonster,function(data)
                        --self:initPanel(2)
                        end,2)
                else
                    self:showDlg("treasure/TreasureDetailsPanel",data,data.useMonster,function(data)
                        --self:initPanel(2)
                        end,2,1)   
                end
            end
        end
        )

        stren_name:setString(LANG("强化lv%s:",data.level))
        refine_name:setString(LANG("精炼%s阶:",data.quality))
        refine_name:setVisible(data.quality ~= 0)
        Image_Icon:loadTexture(model_static_library:getAsset(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
        Image_Bg:loadTexture(model_static_library:getFrame(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
        local treasureItem = model_treasureItemStorage:getNode(data.inventoryTreasureItemId)
        local attributesLevel = model_extension_treasure:getAttributesLevel(treasureItem)
        local attributesQui = model_extension_treasure:getAttributesQuality(treasureItem)
        
        local stren_1 = newItem:getChildByName("stren_1")
        local stren_2 = newItem:getChildByName("stren_2")
        stren_1:setVisible(false)
        stren_2:setVisible(false)
        local refine_1 = newItem:getChildByName("refine_1")
        local refine_2 = newItem:getChildByName("refine_2")
        refine_1:setVisible(false)
        refine_2:setVisible(false)
        
        local strNum = 0
        for key,value in pairs(attributesLevel) do
            strNum= strNum + 1
            local attLevel = newItem:getChildByName("stren_"..strNum)
            attLevel:setVisible(true)
            newItem:getChildByName("stren_"..strNum):setString(porterty.data[key].."+"..value)
        end
        
        local quaNum = 0
        for key,value in pairs(attributesQui) do
            quaNum= quaNum + 1
            local txtQua = newItem:getChildByName("refine_"..strNum)
            txtQua:setVisible(true)
            txtQua:setString(porterty.data[key].."+"..value)
        end
    end
end

function TreasureListPanel:initUi()
    local panel_Bottom=self:getResourceNode():getChildByName("Panel_Bottom")
    for i=1, 5 do
        local item=panel_Bottom:getChildByName("Node_Item1_"..i)

    end
end

function TreasureListPanel:onClickHandler(sender, eventType)
    if eventType == 2 then
        self:sceneBack(function()
            if self.isSelect == true then
                self:removeDlgNoAction()
            else
                SWITSCENE("home/MainCityScene")
            end
        end,true)
        
    end
    
end

return TreasureListPanel
