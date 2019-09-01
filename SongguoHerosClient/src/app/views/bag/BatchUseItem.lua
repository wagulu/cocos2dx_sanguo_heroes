-- 背包 批量使用物品页面
local BatchUseItem = class("BatchUseItem", cc.load("mvc").ViewBase)

local model_item = _REQUIRE("model/static/model_item.lua")
local model_material = _REQUIRE("model/static/model_material.lua")
local model_treasureBox = _REQUIRE("model/static/model_treasureBox.lua")

BatchUseItem.RESOURCE_FILENAME = "Scene/SellPop2.csb" 

function BatchUseItem:onCreate() 
    self.selectItemData_ = self.args[1]
    self.parentPanel_ = self.args[2]
    self:initUseItemPanel()
    self:updateUseItemPanel()
end

-- 初始化
function BatchUseItem:initUseItemPanel()

    self.useItemPanel_ = self:getResourceNode()
    local useItem_Bg = self.useItemPanel_:getChildByName("Panel_BattleResults"):getChildByName("Panel_B")
    SCREEN_SCALE_BG(useItem_Bg)
    useItem_Bg:setTouchEnabled(true)

    local Panel_Scene = self.useItemPanel_:getChildByName("Panel_BattleResults")
    local Panel_Center = Panel_Scene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    local numLabel = Panel_Center:getChildByName("Text_SellNumber")
    self.useItemLabel_ = numLabel  -- 用户选择使用物品label
    self.selectItemCurCnt_ = 1

    self.addInterval = 0
    self.totalInterval = 0
    self.timerId_ = 0

    -- max  
    local Button_Max=Panel_Center:getChildByName("Button_Max")
    self:addBtnTouchEventListener(Button_Max, function(sender,eventType)
        if eventType==2 then
            numLabel:setString(LANG(self.selectItemMaxCnt_)) 
            self.selectItemCurCnt_ = self.selectItemMaxCnt_
        end
    end) 

    -- 减小
    local Button_Minus=Panel_Center:getChildByName("Button_Minus")
    self:addBtnTouchEventListener(Button_Minus, function(sender,eventType)

            if eventType == 0 then
                self.timerId_ = Timer:start(handler(self,self.goodsTimer), 0.2, nil, "remove")
            elseif eventType==2 then
                Timer:kill(self.timerId_)
                self.addInterval = 0;
                self.totalInterval = 0;
                self:updateGoodsNum("remove", 1)
                self:updateGoodsText()
            elseif eventType==3 then
                Timer:kill(self.timerId_)
                self.addInterval = 0;
                self.totalInterval = 0;
            end
    end) 

    -- 增加
    local Button_Plus=Panel_Center:getChildByName("Button_Plus")
    self:addBtnTouchEventListener(Button_Plus, function(sender,eventType)
        if eventType==0 then
            self.timerId_ = Timer:start(handler(self,self.goodsTimer), 0.2, nil, "add")
        elseif eventType==2 then
            Timer:kill(self.timerId_)
            self.addInterval = 0
            self.totalInterval = 0
            self:updateGoodsNum("add", 1)
            self:updateGoodsText() 
        elseif eventType==3 then
            Timer:kill(self.timerId_)
            self.addInterval = 0
            self.totalInterval = 0
        end
    end)    

    -- 确定
    local  Button_Confirm=Panel_Center:getChildByName("Button1")
    local  item
    self:addBtnTouchEventListener(Button_Confirm, function(sender,eventType)
        if eventType==2 then
            --cclog("使用道具 确定回调")    
            -- treasureBox
            if self.selectItemCurCnt_ > 0 then
                item = model_treasureBox:getNode(self.selectItemID_)
                ctlPlayerUseTreasureBoxRequest:sendRequest(handler(self, self.openRewardPanel), item.groupId, self.selectItemCurCnt_)
                cclog("ctlPlayerUseTreasureBoxRequest groupId "..item.groupId.."  selectItemCurCnt_ "..self.selectItemCurCnt_)
            else
                PopMsg.getInstance():flashShow(LANG("选择的数量不足"))
            end   
        end
    end) 
    
    -- 关闭
--    Panel_Scene:setTouchEnabled(true)
--    Panel_Scene:addTouchEventListener(function(sender, eventType)
--        if eventType == 2 then
--            self:removeDlgNoAction()
--        end
--    end)
    

    -- 特殊处理 修改提示文本
    local tmpLabel = Panel_Center:getChildByName("Text_StoreNumber")
    tmpLabel:setString("选择可使用物品")
end

-- 持续按住

function BatchUseItem:goodsTimer(dt, data, timerId)
    self.addInterval = self.addInterval + dt;
    self.totalInterval = self.totalInterval + dt;
    if self.totalInterval < 2 and self.addInterval >= 0.2 then
        self:updateGoodsNum(data,1);
    elseif self.totalInterval > 2 and self.totalInterval < 4 and self.addInterval >= 0.2 then 
        self:updateGoodsNum(data,5);
    elseif self.totalInterval > 4 and self.addInterval >= 0.2 then
        self:updateGoodsNum(data,10);
    end

    self:updateGoodsText()
end

function BatchUseItem:updateGoodsNum(pattern, num)
    if pattern == "add" then
        local tempNum = self.selectItemCurCnt_ + num
        if tempNum > self.selectItemMaxCnt_ then
            self.selectItemCurCnt_ = self.selectItemMaxCnt_
        else
            self.selectItemCurCnt_ = tempNum;
        end
    end

    if pattern == "remove" then
        local tempNum = self.selectItemCurCnt_ - num
        if tempNum < 1 then
            self.selectItemCurCnt_ = 1;
        else
            self.selectItemCurCnt_ = tempNum;
        end
    end
end

function BatchUseItem:updateGoodsText()
    self.useItemLabel_:setString(""..self.selectItemCurCnt_)
end

-- 更新
function BatchUseItem:updateUseItemPanel()
    local Panel_Scene = self.useItemPanel_:getChildByName("Panel_BattleResults")
    local Panel_Center = Panel_Scene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    -- 数据
    self.selectItemMaxCnt_ = self.selectItemData_["count"] -- 剩余数量
    local itemID = self.selectItemData_["itemId"]
    self.selectItemID_ = itemID
    local item = self.selectItemData_
    local sType =  item.type
    local remainCnt = item.count
    local model,quality,asset,sName
    if sType ==  "Material" then  -- 装备进阶材料
        model = model_material
    elseif sType == "TreasureBox" then -- 宝箱
        model = model_treasureBox
    else 
        model = model_item
    end  
    quality = model:getQuality(itemID) -- 品质
    sName = LANG(model:getName(itemID)) -- 名称
    asset = model:getAsset(itemID)

    --cclog("-------------------updateUseItemPanel "..sType)

    -- icon
    local itemIcon_ = Panel_Center:getChildByName("ProjectNode_Item"):getChildByName("Panel_Object")
    local icon = itemIcon_:getChildByName("Image_1") -- 图标
    local frame = itemIcon_:getChildByName("Image_2") -- 品质背景框
    local textCnt = itemIcon_:getChildByName("Text_1") -- 隐藏标签
    if textCnt then
        textCnt:setVisible(false)
    end
    --todo asset 暂时为1
    asset = "1"
    icon:loadTexture(LANG("items/"..sType.."/"..sType.."_"..asset..".png")) 
    frame:loadTexture(LANG("items/frame/frame_%d.png",quality)) 

    -- 名称
    local name = Panel_Center:getChildByName("Text_1_0")
    name:setString(sName)

    -- 数量
    local cnt = Panel_Center:getChildByName("Text_SellNumber")
    cnt:setString("1") 
    self.selectItemCurCnt_ = 1 -- 默认为1
end

-- 获取奖励页面打开回调 
function BatchUseItem:openRewardPanel(success, data, code, msg)
    if(success) then
   
        self.parentPanel_:showDlg("bag/BagRewardPanel", data) -- 显示物品使用页面
        self:removeDlgNoAction()  -- 关闭使用物品页面    
    else
        self:removeDlgNoAction()  -- 关闭使用物品页面   
        release_print("背包 批量使用treasureBox 请求失败")
    end   
end


return BatchUseItem