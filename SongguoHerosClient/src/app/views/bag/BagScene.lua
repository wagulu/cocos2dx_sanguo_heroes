--背包界面

local BagScene = class("BagScene", cc.load("mvc").ViewBase)
local model_item = _REQUIRE("model/static/model_item.lua")
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_material = _REQUIRE("model/static/model_material.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_treasureBox = _REQUIRE("model/static/model_treasureBox.lua")
local model_treasureBoxStorage = _REQUIRE("model/model_treasureBoxStorage.lua")

BagScene.RESOURCE_FILENAME = "Scene/storage.csb" 

function BagScene:onCreate() 
    self:initUi()
end

function BagScene:initUi() 
    -- 场景根节点
    self.Panel_BagScene_ = self:getResourceNode():getChildByName("ArenaRankPop")
    local panelBg = self.Panel_BagScene_:getChildByName("Panel_Bg")
    SCREEN_SCALE_BG(panelBg)
    
    local Panel_Center = self.Panel_BagScene_:getChildByName("Panel_C"):getChildByName("Panel_Center")
    local Panel_T = self.Panel_BagScene_:getChildByName("Panel_T")
    local Panel_Top = Panel_T:getChildByName("Panel_Top")
    SCREEN_TOP(Panel_T)
    local node_hud_top = Panel_Top:getChildByName("Node_HUD_Top")
    self:initTopBar(node_hud_top)
    self.Panel_BagScene_:setTouchEnabled(true)
    -- scView
    self.scView_  = Panel_Center:getChildByName("HeroScrollView_1")
    -- 物品名称
    self.itemName_ = Panel_Center:getChildByName("Text_2")
    -- 物品描述
    self.itemDesc_ = Panel_Center:getChildByName("Text_1")
    -- 物品icon
    self.itemIcon_ = Panel_Center:getChildByName("ProjectNode_6_6"):getChildByName("Panel_Object")
    local textCnt = self.itemIcon_:getChildByName("Text_1")
    if textCnt then
        textCnt:setVisible(false)
    end
    
    -- 物品列表容器
    self.itemList_ = {}
    -- 页签按钮容器
    self.pageBtns_ = {}
    -- 出售按钮
    self.sellBtn_ = Panel_Center:getChildByName("Button_sell") 
    -- 使用按钮
    self.useBtn_ = Panel_Center:getChildByName("Button_use")
    -- 分类数据
    self.classifyData_ = {}
    self.classifyData_ = {
        all = {},
        consume = {},
        chip = {},
        other = {}
    }  
    -- 当前选中页签 默认为1
    self.selectPageID_ = 1
    -- 选择当前物品数据
    self.selectItemID_ = -1 -- 当前选择的物品id
    self.selectItemData_ = 0
    self.selectItemMaxCnt_ = 0
    self.selectItemCurCnt_ = 1
        
    -- 页签回调
    --self.titieText_ = Panel_Center:getChildByName("Panel_Title"):getChildByName("Text_23")
    self.pageTitle_ = {"全部","消耗","碎片","其他"}
    for i=0,3 do
        local Btn_Page = Panel_Center:getChildByName("Button_1_"..i) 
        Btn_Page:setTitleText(LANG(self.pageTitle_[i+1]));      
        Btn_Page:setTag(i+1)
        self.pageBtns_[i+1] = Btn_Page
        self:addBtnTouchEventListener(Btn_Page, function(sender,eventType)
         if eventType==2 then
            self.selectPageID_ = sender:getTag()
            self.selectItemID_ = -1 -- 重置选中id 在使用时保存该变量
            self:transitionPages(self.selectPageID_)     
         end
        end)
    end
    
    -- 出售回调 todo
    self.sellBtn_:addTouchEventListener(function(sender, eventType)
        if eventType==2 then
            --cclog("出售回调") 
        end    
    end)
    
    -- 使用回调
        self:addBtnTouchEventListener(self.useBtn_, function(sender, eventType)
        if eventType==2 then
            local tag = sender:getTag()
            if tag == 1 then -- 单个使用
                ctlPlayerUseItemRequest:sendRequest(handler(self, self.useSinigleItemCB),self.selectItemID_,1)
            elseif tag == 2 then -- 批量使用
                self:showDlg("bag/BatchUseItem", self.selectItemData_,self)
            end
        end    
    end)
     
    -- 关闭
    local  Button_Closed=Panel_Top:getChildByName("Button_Close")
    self:addBtnTouchEventListener(Button_Closed, function(sender,eventType)
        if eventType==2 then
            self:sceneBack(function()
                NotificationCenter_removeObserverByName("UPDATE_BAG_REWARD")
                SWITSCENE("home/MainCityScene")
            end,true)
            
        end
    end
    )   

    -- 物品数据分类
    self:classifyData()
    -- 默认显示全部页面
    self:transitionPages(self.selectPageID_)
    
    -- todo 待删除UI
    local unuseBtn_Page = Panel_Center:getChildByName("Button_1_4")
    if unuseBtn_Page then
        unuseBtn_Page:setVisible(false)
    end  
    -- todo 隐藏出售按钮
    self.sellBtn_:setVisible(false)
    -- todo 文本描述客户端修改
    Panel_Center:getChildByName("Text_3"):setString("道具最多叠加999个")
    -- 添加获取奖励后 更新回调
    NotificationCenter_addObserver(self, "UPDATE_BAG_REWARD",self.updateBagReward) 
     
end

-- 获取奖励后通知背包页面UI更新
function BagScene:updateBagReward()
    -- 物品数据分类
    self:classifyData()
    -- 更新页签
    self:transitionPages(self.selectPageID_)
end

-- 单个使用物品回调
function BagScene:useSinigleItemCB(success, data, code, msg)
    if(success) then
        -- 物品数据分类
        self:classifyData()
        -- 刷新选中页签
        self:transitionPages(self.selectPageID_,true)   
        -- 提示
        PopMsg.getInstance():flashShow(LANG("使用了%s",model_item:getName(self.selectItemID_)))
    else
        release_print("背包 单次使用物品请求失败")
    end 

end

-- 使用按钮状态切换
function BagScene:updateUseItemBtn(itemId , sType)
    -- 根据类型判断
  --  local sType = model_item:getType(itemID) -- 类型   
    --cclog("-----------"..sType.." id ".. itemId )
    self.selectItemID_ = itemId
    self.useBtn_:setTitleText("使用")  
    self.useBtn_:setTag(1)
    self.useBtn_:setVisible(false)
    if (sType == "FavorabilityItem") then
        --todo 跳转
        self.useBtn_:setVisible(false)
    elseif (sType == "TreasureBox") then
        self.useBtn_:setTitleText("批量使用") 
        self.useBtn_:setTag(2)
        self.useBtn_:setVisible(true)
    elseif (sType == "Material") then
         self.useBtn_:setVisible(false)
    elseif (sType == "ExpItem") then
        --todo 跳转
        self.useBtn_:setVisible(false)
    elseif (sType == "EnergyItem") then
        self.useBtn_:setVisible(true)   
    elseif (sType == "VitalityItem") then
        self.useBtn_:setVisible(true)
    elseif (sType == "MoppingTicketItem") then
        self.useBtn_:setVisible(false)   
    elseif (sType == "TreasureItemExpItem") then
        --todo 跳转
        self.useBtn_:setVisible(false)
    elseif (sType == "TreasureItemQualityItem") then
        --todo 跳转
        self.useBtn_:setVisible(false)
    elseif (sType == "BreakthroughItem") then
        self.useBtn_:setVisible(false)
    end   
end

-- 数据分类背包物品分类
function BagScene:classifyData()
    self.classifyData_ = {}
    self.classifyData_ = {
        all = {},
        consume = {},
        chip = {},
        other = {}
    }
    -- 消耗
    local consumeTab = {    
        "FavorabilityItem",
        "TreasureBox",
        "ExpItem",    
        "EnergyItem",      
        "VitalityItem",       
        "TreasureItemExpItem",     
        "TreasureItemQualityItem"  
    } 
    -- 碎片
    local chipTab = {
        "Material",
        "BreakthroughItem"
    }
    -- 其他
    local otherTab = {
        "MoppingTicketItem"
    }
    local itemPBs = {}
    -- 获取item数据
    for key, value in pairs(model_itemStorage.map) do
        for key, value in pairs(value) do
            if value.count > 0 then
                itemPBs[#itemPBs+1] = value
            end
            --cclog("itemStorage---------%d",#itemPBs)
        end
    end
    -- 获取装备进阶材料数据
    for key, value in pairs(model_materialStorage.map) do
        value.type = "Material"
        value.itemId = value.id
        if value.count > 0 then
            itemPBs[#itemPBs+1] = value
        end 
        --cclog("materialStorage---------%d",#itemPBs)
    end
    
    -- 获取宝箱数据
    local treasure =  model_treasureBoxStorage:getBoxes() 
    --cclog("TreasureBox count %d id %d",#treasure)
    for key, value in pairs(treasure) do
        value.type = "TreasureBox"
        value.itemId = value.treasureBoxId
        value.groupId = value.groupId -- groupId 
        if value.count > 0 then
            itemPBs[#itemPBs+1] = value
        end 
        --cclog("TreasureBox---------%d",#itemPBs)
    end
    
    local num = table.getn(itemPBs)    
    for i=1,num do
        local type = itemPBs[i].type
        if(self:tableContainValue(consumeTab, type) == true) then
            self.classifyData_.all[#(self.classifyData_.all)+1] = itemPBs[i]
            self.classifyData_.consume[#(self.classifyData_.consume)+1] = itemPBs[i]
        elseif (self:tableContainValue(chipTab, type) == true) then
            self.classifyData_.all[#(self.classifyData_.all)+1] = itemPBs[i]
            self.classifyData_.chip[#(self.classifyData_.chip)+1] = itemPBs[i] 
        elseif (self:tableContainValue(otherTab, type) == true) then
            self.classifyData_.all[#(self.classifyData_.all)+1] = itemPBs[i]
            self.classifyData_.other[#(self.classifyData_.other)+1] = itemPBs[i]         
        end    
    end   
end

-- 判定table是否包含指定Value
function BagScene:tableContainValue(tab, val)
    local num = table.getn(tab)
    for i=1,num do
        if val == tab[i] then
            return true
         end
    end
	return false
end

-- 刷新背包item
function BagScene:refreshBagList(data, isRefesh)
    -- 拥有的道具列表
    local itemPBs = data
   
    local node,item,itemBG
    node = self:createNode("Node/Node_Store_Item.csb")
    item = node:getChildByName("Panel_Object")
    local contentSize = item:getContentSize()

    self.items = {}
    local spaceW = 13  --间隔 
    local spaceH = 8
    local num = table.getn(itemPBs)   
    local column = 4 -- 每行显示4个
 
    -- item位置移动
    local offSetX = -((self.scView_:getContentSize().width - column * contentSize.width - (column-1) * spaceW) * 0.5)
    local row = math.ceil(num / column)  
    local realRow = row -- 保存含有物品真是行数
    row = math.max(4,row) -- 强制显示4行 补齐空白
    local sSize=cc.size(self.scView_:getInnerContainerSize().width,math.max(self.scView_:getContentSize().height,(contentSize.height)*row + (row -1)*spaceH));
    self.scView_:setInnerContainerSize(sSize)
    self.itemList_ = {}
       
    -- 触摸回调 icon刷新
    for i=1,num do
        local newItem = item:clone()
        newItem:setTouchEnabled(true);
        self.scView_:addChild(newItem)
        newItem:setCascadeOpacityEnabled(true)
        newItem:setTag(i);       
        self.itemList_[i] = newItem
        -- icon刷新
        self:updateItemIcon(itemPBs[i],newItem)
        -- 点击icon回调
        self:addBtnTouchEventListener(newItem,function(sender,eventType)
        if eventType == 2 then -- 更新物品详情  
            self:updateItemDesc(itemPBs[sender:getTag()],i)
            self.selectItemData_ = itemPBs[sender:getTag()] -- 设置当前选中物品数据信息  
         end
        end)
        
        -- 记录上次请求前对应的node 设置item选中状态
        if(self.selectItemID_ == itemPBs[i]["itemId"]) then
            self:updateItemDesc(itemPBs[i],i) 
        end
        
     end
         
     -- 默认显示第一项
     if(self.selectItemID_ == -1) then
        self:updateItemDesc(itemPBs[1],1) 
        self.selectItemID_ = itemPBs[1]["itemId"]
     end
     
    -- 位置
    for i=1, row do
     	for j=1, column do
            local tmpIdx = (i-1)*column + j
            local newItem 
            if tmpIdx > num then -- 补空格
               newItem = item:clone()
               newItem:getChildByName("Image_1"):setVisible(false)
               newItem:getChildByName("Image_2"):setVisible(false)
               newItem:getChildByName("Image_3"):setVisible(false)
               newItem:getChildByName("Image_4"):setVisible(false)
               newItem:getChildByName("Text_3"):setVisible(false)
               newItem:setEnabled(false)
               self.scView_:addChild(newItem)
            	else 
            	   newItem = self.itemList_[tmpIdx] 
            end
            newItem:setPosition(cc.p(j*contentSize.width + (j-1)*spaceW + offSetX,sSize.height-i*contentSize.height- (i-1)*spaceH + contentSize.height * 0.5))      
        end
     end
     
    -- 添加分割线UI
    local lineNum = math.floor(num / column)
    for i = 1, lineNum do
        local imgLine = ccui.ImageView:create("ui/Assets/HeroListScene/cuttingline.png")
        self.scView_:addChild(imgLine)
        imgLine:setPositionX(imgLine:getContentSize().width * 0.5)
        imgLine:setPositionY(sSize.height - i*contentSize.height - (i-1)*spaceH - spaceH * 0.5)
    end
 
     -- 添加滚动条    
     local refesh = isRefesh or false  -- 刷新数据时不改变滚动条 scrollView 监听闭包会记录位置   	
     if (refesh == false) then
        self.scView_:jumpToTop()
        if(realRow > 3) then
            self:addScrollViewBar(self.scView_)
        else 
            self:hideScrollViewBar(self.scView_)
        end
     end
     
end

-- 更新道具item
function BagScene:updateItemIcon(item, itemNode)
     local sType =  item.type
     local itemID = item.itemId
     local remainCnt = item.count
     local asset
     local quality
     if sType ==  "Material" then  -- 装备进阶材料
           quality = model_material:getQuality(itemID)
           asset = model_material:getAsset(itemID)
     elseif sType == "TreasureBox" then -- 宝箱
           quality = model_treasureBox:getQuality(itemID)
           asset = model_treasureBox:getAsset(itemID)
     else 
        quality = model_item:getQuality(itemID)
        asset = model_item:getAsset(itemID)
     end
 
    local icon = itemNode:getChildByName("Image_1") -- 图标
    local frame = itemNode:getChildByName("Image_2") -- 品质背景框
    local select = itemNode:getChildByName("Image_3")  -- 选中状态
    local select2 = itemNode:getChildByName("Image_4")  -- 选中状态
    local remainText = itemNode:getChildByName("Text_3")  -- 剩余数量
    self:setLabelEffect(remainText)
    --todo 特殊处理
    if(sType ~=  "Material") then
        asset = 1
    end
    icon:loadTexture(LANG("items/"..sType.."/"..sType.."_"..asset..".png")) 
    frame:loadTexture(LANG("items/frame/frame_%d.png",quality))
    remainText:setString(remainCnt)
    select:setVisible(false)  
    select2:setVisible(false) 
    --cclog("背包道具id "..itemID.." 类型 "..sType.." 品质 "..quality.." 剩余数量 "..remainCnt);
    
end

-- 设置阴影和描边
function BagScene:setLabelEffect(UIText)
    if UIText then
        --UIText:enableShadow(display.COLOR_BLACK,cc.size(4,-4))
        UIText:enableOutline(display.COLOR_BLACK,3)
    end
end

-- 更新物品详情
function BagScene:updateItemDesc(item,selectIndex)
    local sType =  item.type
    local itemID = item.itemId
    local remainCnt = item.count
    local model,quality,asset
    if sType ==  "Material" then  -- 装备进阶材料
        model = model_material
    elseif sType == "TreasureBox" then -- 宝箱
        model = model_treasureBox
    else 
        model = model_item
    end
    quality = model:getQuality(itemID)
    asset = model:getAsset(itemID)
     
    self.itemName_:setString(LANG(model:getName(itemID))) 
    -- 描述  
    self.itemDesc_:setString(LANG(model:getDesc(itemID))) 
    -- icon
    local icon = self.itemIcon_:getChildByName("Image_1") -- 图标
    local frame = self.itemIcon_:getChildByName("Image_2") -- 品质背景框
    --todo 特殊处理
    if(sType ~=  "Material") then
        asset = "1"
    end
    icon:loadTexture(LANG("items/"..sType.."/"..sType.."_"..asset..".png")) 
    frame:loadTexture(LANG("items/frame/frame_%d.png",quality)) 
    
    -- 显示详情UI
    self.itemDesc_:setVisible(true)
    self.itemName_:setVisible(true)
    self.itemIcon_:setVisible(true) 
    self.useBtn_:setVisible(true)        
    -- 更新item选中状态
    self:updateBtnState(selectIndex)  
    -- 更新使用按钮状态
    self:updateUseItemBtn(itemID, sType)
end

-- 更新item按钮选中状态
function BagScene:updateBtnState(selectIdx)
    local num = table.getn(self.itemList_)
    for i=1,num do
    	   local item = self.itemList_[i]
    	   local select = item:getChildByName("Image_3")  
    	   select:setVisible(false)
        local select2 = item:getChildByName("Image_4")  
        select2:setVisible(false)
        if selectIdx == i then
    	   	 select:setVisible(true)
         select2:setVisible(true)
    	   end     
    end	 
end

-- 更新页签按钮状态
function BagScene:updatePageBtnState(selectIdx)
   local num = table.getn(self.pageBtns_)
    for i=1,num do
       local pageBtn = self.pageBtns_[i]
       pageBtn:loadTextureNormal("ui/Share/button/Btn_HeroInfo_2.png")
       if selectIdx == i then 
            pageBtn:loadTextureNormal("ui/Share/button/Btn_HeroInfo_1.png")  
       end     
    end 
end

-- 页签切换回调
function BagScene:transitionPages(tag,isRefesh)
    -- 设置标题文本
    --self.titieText_:setString(LANG(self.pageTitle_[tag]));
    -- 刷新UI选中状态
    self:updatePageBtnState(tag)
    -- 清空背包内列表
    self.scView_:removeAllChildren()
    -- 详情页面隐藏
    self.itemDesc_:setVisible(false)
    self.itemName_:setVisible(false)
    self.itemIcon_:setVisible(false) 
    self.sellBtn_:setVisible(false) 
    self.useBtn_:setVisible(false) 
    
    if tag == 1 then
        self:refreshBagList(self.classifyData_.all, isRefesh)
    elseif tag == 2 then
        self:refreshBagList(self.classifyData_.consume, isRefesh)  
    elseif tag == 3 then
        self:refreshBagList(self.classifyData_.chip, isRefesh)
    elseif tag == 4 then
        self:refreshBagList(self.classifyData_.other, isRefesh) 
    end
end

return BagScene