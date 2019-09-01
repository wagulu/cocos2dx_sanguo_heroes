-- 背包 获取奖励页面
local BagRewardPanel = class("BagRewardPanel", cc.load("mvc").ViewBase)

local model_item = _REQUIRE("model/static/model_item.lua")
local model_material = _REQUIRE("model/static/model_material.lua")
local model_treasureBox = _REQUIRE("model/static/model_treasureBox.lua")

local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"

BagRewardPanel.RESOURCE_FILENAME = "Scene/GetRewardPanel.csb" 

function BagRewardPanel:onCreate() 
    local data = self.args[1]
    self:initRewardPanel()
    self:updaterewardPanel(data)    
end

-- 初始化
function BagRewardPanel:initRewardPanel()

    self.rewardPanel_ = self:getResourceNode()
    local reward_Bg = self.rewardPanel_:getChildByName("Panel_BattleResults"):getChildByName("Panel_B")
    SCREEN_SCALE_BG(reward_Bg)
    reward_Bg:setTouchEnabled(true)   

    local Panel_Scene = self.rewardPanel_:getChildByName("Panel_BattleResults")
    local Panel_Center = Panel_Scene:getChildByName("Panel_C"):getChildByName("Panel_Center")

    self.scRewardView_ = Panel_Center:getChildByName("ScrollView_1")
    -- 确定
    local  Button_Confirm=Panel_Center:getChildByName("Button_1")
    self:addBtnTouchEventListener(Button_Confirm, function(sender,eventType)
        if eventType==2 then  
            NotificationCenter_postNotification({msg = "UPDATE_BAG_REWARD"});
            self:removeDlgNoAction()  
        end
    end) 
    
    -- 取消
    local  Button_Cancel=Panel_Center:getChildByName("Button_Cancel")
    self:addBtnTouchEventListener(Button_Cancel, function(sender,eventType)
        if eventType==2 then  
            self:removeDlgNoAction()  
        end
    end) 
    
    
end

-- 更新
function BagRewardPanel:updaterewardPanel(data)
    --cclog("--------------获得奖励更新")
    -- 数据
    -- 拥有的道具列表
    local itemPBs = {}
    -- 掉落物品 
    for k,v in pairs(data.prizes) do
        --cclog("---------PrizePB "..k..v.value)
        v.itemId = v.value  -- 物品id
        v.count = v.content -- 物品数量
        v.sType = v.type -- 自定义type
        itemPBs[#itemPBs+1] = v
    end

    -- 获得奖励log
    for k,v in pairs(itemPBs) do
        cclog("获得奖励 id "..v.itemId.." 类型 "..v.type.. " 数量 "..v.count)      
    end

    -- UI
    self.scRewardView_:removeAllChildren() -- 删除scView

    local node,item,itemBG
    node = self:createNode("Node/Node_item_cacha.csb")
    item = node:getChildByName("Panel_Object")
    local contentSize = item:getContentSize()

    self.items = {}
    local spaceW = 17  --间隔 
    local spaceH = 13
    local num = table.getn(itemPBs)
    local column = 4 -- 每行显示4个
    local row = math.ceil(num / column)     
    local offSetX = -((self.scRewardView_:getContentSize().width - column * contentSize.width - (column-1) * spaceW) * 0.5)
    local sSize=cc.size(self.scRewardView_:getInnerContainerSize().width,math.max(self.scRewardView_:getContentSize().height,(contentSize.height)*row + (row -1)*spaceH));
    self.scRewardView_:setInnerContainerSize(sSize)
    local itemList = {}

    -- icon刷新
    for i=1,num do
        local newItem = item:clone()
      --  newItem:setEnabled(false)
        self.scRewardView_:addChild(newItem)
        newItem:setTag(i)      
        itemList[i] = newItem
        -- icon刷新
        self:updateRewardItemIcon(itemPBs[i],newItem)    
               
    end
    -- 位置
    for i=1, row do
        for j=1, column do
            local tmpIdx = (i-1)*column + j
            if tmpIdx > num then 
                break
            end
            local newItem = itemList[tmpIdx]           
            newItem:setPosition(cc.p(j*contentSize.width + (j-1)*spaceW + offSetX,sSize.height-i*contentSize.height- (i-1)*spaceH + contentSize.height * 0.5))     
        end
    end

    -- 添加滚动条    
    if(row > 2) then
        self:addScrollViewBar(self.scRewardView_,17)
    else 
        self:hideScrollViewBar(self.scRewardView_)
    end
end

-- 更新获奖图标icon

function BagRewardPanel:updateRewardItemIcon(item, itemNode)
    --todo 获取奖励 需要添加特定数据类型 
    local sType =  item.sType
    local itemID = item.itemId
    local remainCnt = item.count -- 获得数量

    local icon = itemNode:getChildByName("Image_1") -- 图标
    local frame = itemNode:getChildByName("Image_2") -- 品质背景框
    local remainText = itemNode:getChildByName("Text_1")  -- 剩余数量

    icon:loadTexture(model_static_library:getAsset(itemID, sType))
    frame:loadTexture(model_static_library:getFrame(itemID, sType))
      
    if remainText then
        remainText:setString(remainCnt)
        remainText:setVisible(true) 
    end     
    
    -- 添加触控事件 显示物品详细信息
    itemNode:setTouchEnabled(true);
    self:addBtnTouchEventListener(itemNode, function(sender,eventType)
        if eventType==2 then  
            self:showDlg("common/PropsPopScene", {id = itemID, type = sType}) 
        end
    end)  
    
end


return BagRewardPanel