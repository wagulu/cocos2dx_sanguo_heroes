--购买确认界面
local StorePopupScene = class("StorePopupScene", cc.load("mvc").ViewBase)
StorePopupScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE("pbCommon.lua")
local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
StorePopupScene.STOPFRAME = 35
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua")
local model_user = _REQUIRE("model/model_user.lua")

StorePopupScene.RESOURCE_FILENAME = "Scene/PurchasePopupScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function StorePopupScene:onCreate()
    self.shopItem = self.args[1].shopItem
    self.callback = self.args[1].callback
    self.prize = self.shopItem.goodsPrize
    self:initUi()
end

function StorePopupScene:initUi()
    local StorePopupScene = self:getResourceNode():getChildByName("PurchasePopupScene")
    StorePopupScene:setTouchEnabled(true)
    local Panel_Center = StorePopupScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    local Panel_Top = StorePopupScene:getChildByName("Panel_Top")
    local Panel_Bg = StorePopupScene:getChildByName("Panel_Bg")
    self.Panel_Bottom = StorePopupScene:getChildByName("Panel_B"):getChildByName("Panel_Bottom")
    self.Panel_Center = Panel_Center
    local Button_1 = Panel_Center:getChildByName("Button_1")
    local Button_2 = Panel_Center:getChildByName("Button_2")
    local buttonMax = Panel_Center:getChildByName("Button_4")
    local buttonMin = Panel_Center:getChildByName("Button_4_0")
    local Node_1 = Panel_Center:getChildByName("Node_1")--购买物品的图标
    --SCREEN_TOP(Panel_Top)
    --SCREEN_SCALE_BG(Panel_Bg)
    
    local icon_iv = Panel_Center:getChildByName("Node_1")
    local name_tv = Panel_Center:getChildByName("Text_1")
    local haveCount_tv = Panel_Center:getChildByName("Text_4")
    local des_tv = Panel_Center:getChildByName("Text_5")
    local buyCount_tv = Panel_Center:getChildByName("Text_7")
    
    self.price = 0  --单件价格
    self.textTotal = Panel_Center:getChildByName("Text_8")
    self.textTotal2 = Panel_Center:getChildByName("Text_7")
    self.addInterval = 0;
    self.goodsCount = 1;
    self.totalInterval = 0;
    self.timerId = 0;
    self.count = self:getPrizeMaxCount();   --总共可买数量
    self.max_count = self.count
    if self.count == 0 then
        self.max_count = 1
    end
    self.min_count = 1;
    if self.max_count == 0 then
        self.min_count = 1;
    end
    
    if name_tv then
        cclog("name_tv")
    else
        cclog("name_tv = nil")
    end
    name_tv:setString(LANG(model_static_library:getName(self.prize.value, self.prize.type)))
    haveCount_tv:setString(LANG(self:getPrizeCount(self.prize.type,self.prize.value)))
    des_tv:setString(LANG(model_static_library:getDesc(self.prize.value, self.prize.type)))
    
    local Panel_Object = Panel_Center:getChildByName("Node_1"):getChildByName("Panel_Object")
    Panel_Object:getChildByName("Image_1"):loadTexture(model_static_library:getAsset(self.prize.value, self.prize.type))
    Panel_Object:getChildByName("Image_2"):loadTexture(model_static_library:getFrame(self.prize.value, self.prize.type))
    Panel_Object:getChildByName("Text_1"):setString(LANG("%s",self.prize.content))
    Panel_Object:getChildByName("Text_1"):setVisible(true)
    
    --花费总数
    self.Panel_Center:getChildByName("Text_Cost"):setString(self.goodsCount * self.price)
    if self.shopItem.priceType == "Cash" then   
        --Panel_Bottom:getChildByName("Text_Cost"):setString(LANG(model_user.cash))
        self.Panel_Center:getChildByName("Image_Icon_Cash"):loadTexture("Share/icon/Icon_Gold.png")
    elseif self.shopItem.priceType == "Coin" then
        --Panel_Bottom:getChildByName("Text_Cost"):setString(LANG(model_user.coin))
        self.Panel_Center:getChildByName("Image_Icon_Cash"):loadTexture("Share/icon/Icon_Cash1.png")
    end
   
    --购买按钮
    local Button1_10 = self.Panel_Center:getChildByName("Button1_10")
    Button1_10:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            if self:getPrizeCount(self.prize.type,self.prize.value) < 999 then
                if self.shopItem.buyLimited == 1 and self.shopItem.quota <= 0 then
                    PopMsg.getInstance():flashShow(LANG("物品已售罄"))
                    return
                end
                if self.goodsCount == 1 and self.goodsCount > self.count then
                    --PopMsg.getInstance():flashShow(LANG("请选择购买数量"))
                    return
                end
                ctlShopBuyItemRequest:sendRequest(handler(self,self.onShopBuyItem), 1, self.prize.id, self.goodsCount);
            else
                PopMsg.getInstance():flashShow(LANG("物品已满"))
            end
        end
    end
    )

    local Button1_11=Panel_Center:getChildByName("Button1_11")
    Button1_11:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:removeDlgNoAction()
            --SWITSCENE("arena/StoreScene")
        end
    end
    )
    
    self:updateGoodsText()
    
    self:addBtnTouchEventListener(Button_1, function(sender,eventType)
        cclog("Touch-----".. eventType)
        if eventType==0 then
            self.timerId = Timer:start(handler(self,self.goodsTimer), 0.2, nil, "remove")
            cclog("down-----".. self.goodsCount)
        end
        --        handler(self,self.addItem)
        if eventType == 2  or eventType == 3 then
            Timer:kill(self.timerId)
            self.addInterval = 0;
            self.totalInterval = 0;
            self:updateGoodsNum("remove", 1)
            self:updateGoodsText()
            cclog("up-----".. self.goodsCount)
        end
    end)

    self:addBtnTouchEventListener(Button_2, function(sender,eventType)
        cclog("Touch-----".. eventType)
        if eventType==0 then
            self.timerId = Timer:start(handler(self,self.goodsTimer), 0.2, nil, "add")
            cclog("down-----".. self.goodsCount)
        end
        --        handler(self,self.addItem)
        if eventType == 2 or eventType == 3 then
            Timer:kill(self.timerId)
            self.addInterval = 0;
            self.totalInterval = 0;
            self:updateGoodsNum("add", 1)
            self:updateGoodsText()
            cclog("up-----".. self.goodsCount)
        end
    end)
    
    self:addBtnTouchEventListener(buttonMax, function(sender,eventType)
        if eventType == 2 then
            self.goodsCount = self.max_count;
            self:updateGoodsText()
        end
    end)
    
    self:addBtnTouchEventListener(buttonMin, function(sender,eventType)
        if eventType == 2 then
            self.goodsCount = self.min_count;
            self:updateGoodsText()
        end
    end)
    
--    buttonMin:addTouchEventListener(function(sender,eventType)
--        if eventType == 2 then
--            self.goodsCount = self.min_count;
--            self:updateGoodsText()
--        end
--    end
--    )
    
end

function StorePopupScene:onShopBuyItem(success, data, code, msg)
    if success then
       self.shopItem = data.goods
       self.prize = data.prize
        --self:setPrizeCount(data.prize.content, data.prize.type, data.prize.id)
       PopMsg.getInstance():flashShow(LANG("购买成功！"))
       self:initUi()
       self.callback(self.shopItem)
    else
        --WARNING(msg);
    end
end

function StorePopupScene:goodsTimer(dt, data, timerId)
    --cclog("addItem-----".. data)
--    if self.addInterval == 0 then
--        self.goodsCount = self.goodsCount + 1
--    end
    
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
	
function StorePopupScene:updateGoodsNum(pattern, num)
	if pattern == "add" then
	   local tempNum = self.goodsCount + num;
        if tempNum > self.max_count then
            self.goodsCount = self.max_count
        else
            self.goodsCount = tempNum;
        end
	end
	
	if pattern == "remove" then
        local tempNum = self.goodsCount - num;
        if tempNum < self.min_count then
            self.goodsCount = self.min_count;
        else
            self.goodsCount = tempNum;
        end
	end
end

function StorePopupScene:updateGoodsText()
    self.textTotal:setString(" " .. self.goodsCount)
    self.textTotal2:setString(" " .. self.goodsCount)
    self.Panel_Center:getChildByName("Text_Cost"):setString(self.goodsCount * self.price)
end

function StorePopupScene:setPrizeCount(count, type, id)
    local libray
    if(type == pbCommon.ObjectType.MonsterPart) then
        libray = model_monsterPartStorage;
        libray:setCount(id, count)
    elseif(type == pbCommon.ObjectType.TreasureItem) then
        libray = model_treasureItemStorage;
    elseif(type == pbCommon.ObjectType.TreasureItemPart) then
        libray = model_treasureItemPartStorage;
        libray:setCount(id, count)
    elseif(type == pbCommon.ObjectType.Material) then
        libray = model_materialStorage;
        libray:setCount(id, count)
    elseif(type == pbCommon.ObjectType.Item) then
        libray = model_itemStorage;
        libray:setCount(id, type, count)
    end
    
end

function StorePopupScene:getPrizeCount(type, id)
    local libray
    local count
    if(type == pbCommon.ObjectType.MonsterPart) then
        libray = model_monsterPartStorage;
        count = libray:getCount(id)
    elseif(type == pbCommon.ObjectType.TreasureItem) then
        libray = model_treasureItemStorage;
    elseif(type == pbCommon.ObjectType.TreasureItemPart) then
        libray = model_treasureItemPartStorage;
        count = libray:getCount(id)
    elseif(type == pbCommon.ObjectType.Material) then
        libray = model_materialStorage;
        count = libray:getCount(id)
    elseif(type == pbCommon.ObjectType.Item) then
        libray = model_itemStorage;
        count = libray:getCount(id, type)
    end
    if count then
        return count
    else
        return 0
    end
end

function StorePopupScene:getPrizeMaxCount()
	local maxCount
	local shopItem = self.shopItem
	local price        --单间价格
	local total = 0       --货币总数
	local currentCount --当前拥有数量(单个物品)
	
    currentCount = self:getPrizeCount(self.prize.type,self.prize.value)
	
	--价格 是否打折
	if self.shopItem.originalPrice == 0 then 
	   price = self.shopItem.price
	else
        price = self.shopItem.price
	end
	self.price = price
	
	--货币总数
	if self.shopItem.priceType == "Cash" then
        total = model_user.cash
	elseif self.shopItem.priceType == "Coin" then
        total = model_user.coin
	end
	
    cclog("shopItem.priceType = "..shopItem.priceType)
    cclog("total = "..total)
	
    local count = math.floor(total/price)
	if count > 999 then
	   count = 999
	end
	
	--是否限购
	if self.shopItem.buyLimited == 1 and count > self.shopItem.quota then
        count = self.shopItem.quota
    end
    
    --购买够的单件数量是否超过999
    local buyedCount = count * self.prize.content + currentCount
    if buyedCount > 999 then
        maxCount = math.floor((999 - currentCount) / self.prize.content)
    else
        maxCount = count
    end
	
	return maxCount
end

return StorePopupScene