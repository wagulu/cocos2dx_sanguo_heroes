--声望商店界面
local PrestigeShop = class("PrestigeShop", cc.load("mvc").ViewBase)
PrestigeShop.NEXTSCENE = "battle/ChapterScene"

local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"

PrestigeShop.RESOURCE_FILENAME = "Scene/ShopListPop.csb"
function PrestigeShop:onCreate()
    self.data = self.args[1]
    self.dlData = self.args[2]
    self.fun = self.args[3]
    self:initUi()
end

function PrestigeShop:initUi()
    local ShopListPop = self:getResourceNode():getChildByName("ShopListPop")
    self.Panel_Center = ShopListPop:getChildByName("Panel_Center")
    self.Panel_Top = ShopListPop:getChildByName("Panel_T"):getChildByName("Panel_Top")
    local Panel_Bg = ShopListPop:getChildByName("Panel_Bg")
    SCREEN_TOP(ShopListPop:getChildByName("Panel_T"))
    SCREEN_SCALE_BG(Panel_Bg)
    local Node_Top=self.Panel_Top:getChildByName("Node_Top")
    self:initTopBar(Node_Top)
    self.items = {}
    self.timerId = nil
    self.space = 3
    self.shop = {}
    self.textTime = self.Panel_Center:getChildByName("Text_1")
    
    self.scrollView = self.Panel_Center:getChildByName("HeroScrollView_1")
    self:addScrollViewBar(self.scrollView)
    local node
    node = self:createNode("Node/Node_Item_2.csb")
    self.item = node:getChildByName("Panel_Item_2")
    
    --关闭按钮
    local  Button_Back=self.Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self.fun()
            self:removeDlgNoAction()
        end
    end
    )
    --标题
    self.Panel_Center:getChildByName("Panel_TitleBg_2"):getChildByName("Text_Title"):setString(LANG("声望商城"))

    --刷新按钮
    local  Button_1=self.Panel_Center:getChildByName("Button_1")
    Button_1:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:showDlg("common/CommonPopScene",{confirmStr=LANG("刷新"),cancelStr=LANG("取消"),text=LANG("今日已刷新声望商城%s次，本次刷新消耗%s声望",self.shop.refreshTimes, self.shop.refreshPrestige),
            fun=function(data)
                if data.cmd=="confirm" then
                        if self.shop.prestige < self.shop.refreshPrestige then
                            PopMsg.getInstance():flashShow(LANG("声望不足"))
                            return
                        end
                    ctlHighladderRefreshShopRequest:sendRequest(function(success, data, code, msg)
                        if success then
                            self:storeSceneInit(data.shop)
                        end
                    end) 
                elseif data.cmd =="cancel" then 

                end
            end})
          end 
    end)

    --获取商品信息
    self:getShopList()
end

function PrestigeShop:getShopList()
    ctlHighladderGetShopRequest:sendRequest(function(sucess,data, code, msg)
        self:storeSceneInit(data.shop)
    end);
end

function PrestigeShop:storeSceneInit(data)
    self.shop = data
    local time = model_user:getCurrentServerTimestamp()
    --声望代币
    self.Panel_Center:getChildByName("Text_2"):setString(LANG("声望:%s", data.prestige))
    --刷新时间
    if self.timerId == nil then
        if model_user:getCurrentServerTimestamp() < data.resetTime then
            self.timerId = Timer:start(function()
                local leftTime = (data.resetTime - model_user:getCurrentServerTimestamp())
                if leftTime >= 0 then
                    local second = (math.floor(leftTime % 60) < 10) and ("0"..math.floor(leftTime % 60)) or math.floor(leftTime % 60);
                    local minute = (math.floor((leftTime - second) % 3600 / 60) < 10) and ("0"..math.floor((leftTime - second) % 3600 / 60)) or math.floor((leftTime - second) % 3600 / 60);
                    local hour = (math.floor((leftTime - second - minute) / 3600) < 10) and ("0"..math.floor((leftTime - second - minute) / 3600)) or math.floor((leftTime - second - minute) / 3600);
                    self.textTime:setString(LANG("距离下次刷新%s:%s:%s", hour, minute, second))
                else
                    self.textTime:setString(LANG("00:00:00"))
                    Timer:kill(self.timerId)
                    self.timerId = nil
                    --刷新数据
                    self:getShopList()
                end
            end)
        end
    end
    
    
    local contentSize = self.item:getContentSize()
    local num = #(data.items)
    local sSize = cc.size(self.scrollView:getInnerContainerSize().width, 
        math.max(self.scrollView:getContentSize().height, (contentSize.height + self.space)*math.ceil(num/2)));
    self.scrollView:setInnerContainerSize(sSize)
    self.scrollView:removeAllChildren()
    
    for i=1,num do
        local newItem
        if self.items[i] == nil then
            newItem = self.item:clone()
            newItem:retain()
            newItem:setTag(i)
            self.items[i] = newItem
        else
            newItem = self.items[i]
        end
        
        local index = i - 1
        local x = (sSize.width/2) / 2.0 + sSize.width/2 * (index%2) - 10;
        local y = sSize.height - (contentSize.height + self.space) * math.floor(index / 2) - contentSize.height/2
        newItem:setPosition(cc.p(x, y))
        self.scrollView:addChild(newItem)
        
        local shopItem = data.items[i]
        
        self:updateItemView(newItem,shopItem)
     end
end

function PrestigeShop:updateItemView(newItem, shopItem)
        --声望代币
        self.Panel_Center:getChildByName("Text_2"):setString(LANG("声望:%s", self.shop.prestige))

        local limitTab_tv = newItem:getChildByName("Text_1")
        local limitTab_iv = newItem:getChildByName("Image_1")
        local icon_iv = newItem:getChildByName("Image_Head")
        local name_tv = newItem:getChildByName("Text_Name")
        local des_tv = newItem:getChildByName("Text_Level")
        --价格1
        local priceCost_tv = newItem:getChildByName("cost_1"):getChildByName("Text_cash_1")
        local priceText_tv = newItem:getChildByName("cost_1"):getChildByName("Text_cash_2")
        --价格2
        local cost_2_gp = newItem:getChildByName("cost_2")
        local orPriceCost_tv = newItem:getChildByName("cost_2"):getChildByName("Text_cash_1")
        local orPriceText_tv = newItem:getChildByName("cost_2"):getChildByName("Text_cash_2")

        local limitText_tv = newItem:getChildByName("Text_limited")
        limitText_tv:setVisible(false)
        local Text_Time = newItem:getChildByName("Text_Time")
        Text_Time:setVisible(false)

        local buyButton_bt = newItem:getChildByName("Button_buy")

        local iconTab_iv = newItem:getChildByName("Image_13")
        iconTab_iv:setVisible(false)
        
        newItem:getChildByName("cost_1"):getChildByName("Image_Cash"):loadTexture("Share/icon/icon_popularity.png")
        newItem:getChildByName("cost_2"):getChildByName("Image_Cash"):loadTexture("Share/icon/icon_popularity.png")

        newItem:getChildByName("Text_1"):setString(LANG("%s", shopItem.prize.content))
        local Button_buy = newItem:getChildByName("Button_buy")
        Button_buy:addTouchEventListener(function(sender, eventType)
            if eventType == 2 then
            self:showDlg("arena/PurchasePopupScene", {shopItem = shopItem, callback = function(shop)
                    --self:getShopList()
                self.shop.items[newItem:getTag()] = shop.items[1]
                shopItem = shop.items[1]
                self.shop.prestige = shop.prestige
                self:updateItemView(newItem, shopItem)
                end})
            end
        end)

        if shopItem.buyed == 1 then
            Button_buy:setBright(false)
            Button_buy:setEnabled(false)
            Button_buy:setTitleText(LANG("已购买"))
        else
            Button_buy:setBright(true)
            Button_buy:setEnabled(true)
            Button_buy:setTitleText(LANG("购买"))
        end

        if shopItem.prestige == shopItem.discount then  --原价
            cost_2_gp:setVisible(false)
            priceCost_tv:setString(LANG(shopItem.discount))
            priceText_tv:setString(LANG("价格:"))
        else
            cost_2_gp:setVisible(true)
            priceCost_tv:setString(LANG(""..shopItem.prestige))
            priceText_tv:setString(LANG("折扣价:"))
            orPriceCost_tv:setString(LANG(""..shopItem.discount))
            orPriceText_tv:setString(LANG("原价:"))
        end

        name_tv:setString(LANG(model_static_library:getName(shopItem.prize.value, shopItem.prize.type)))
        if model_static_library:getColor(shopItem.prize.value, shopItem.prize.type) then
            name_tv:setColor(model_static_library:getColor(shopItem.prize.value, shopItem.prize.type))
        end
        des_tv:setString(LANG(model_static_library:getDesc(shopItem.prize.value, shopItem.prize.type)))
        icon_iv:loadTexture(model_static_library:getAsset(shopItem.prize.value, shopItem.prize.type))
        icon_iv:setTouchEnabled(true)
        icon_iv:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                self:showDlg("common/PropsPopScene", {id = shopItem.prize.value, type = shopItem.prize.type})
            end
        end)
    newItem:getChildByName("Image_Head_Bg"):loadTexture(model_static_library:getFrame(shopItem.prize.value, shopItem.prize.type))
end

function PrestigeShop:onExit()
    if self.timerId then
        Timer:kill(self.timerId)
    end
    
    for i=1,#(self.items) do
        self.items[i]:release()
    end
end

return PrestigeShop
