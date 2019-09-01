--商城界面
local StoreScene = class("StoreScene", cc.load("mvc").ViewBase)
StoreScene.NEXTSCENE = "battle/ChapterScene"

StoreScene.RESOURCE_FILENAME = "Scene/ShopListPop.csb"
function StoreScene:onCreate()
    self:initUi()
end

local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
local model_user = _REQUIRE("model/model_user")

function StoreScene:initUi()
    local ShopListPop = self:getResourceNode():getChildByName("ShopListPop")
    self.Panel_Center = ShopListPop:getChildByName("Panel_Center")
    self.Panel_Top = ShopListPop:getChildByName("Panel_T"):getChildByName("Panel_Top")
    local Panel_Bg = ShopListPop:getChildByName("Panel_Bg")
    SCREEN_TOP(ShopListPop:getChildByName("Panel_T"))
    SCREEN_SCALE_BG(Panel_Bg)
    local Node_Top=self.Panel_Top:getChildByName("Node_Top")
    self:initTopBar(Node_Top)
    self.shopData = {}
    self.shopData = self.args[1]
    self.scrollView = self.Panel_Center:getChildByName("HeroScrollView_1")
    self:addScrollViewBar(self.scrollView)
    local node
    node = self:createNode("Node/Node_Item_2.csb")
    self.item = node:getChildByName("Panel_Item_2")
    
    self.timerArray = {}
    self.items = {}
    self.space = 3
    
    --关闭按钮
    local  Button_Back=self.Panel_Top:getChildByName("Button_Back")
    self:addBtnTouchEventListener(Button_Back,function(sender,eventType)
        if eventType==2 then
            self:sceneBack(function()
                if GLOBAL_MY_APP.isSwitch == true then
                    GLOBAL_MY_APP:setSwitch(false)
                else
                    SWITSCENE("home/MainCityScene")
                end
            end,true)
            
        end
    end
    )
    --标题
    self.Panel_Center:getChildByName("Panel_TitleBg_2"):getChildByName("Text_Title"):setString(LANG("商城"))
     
    --刷新按钮
    local  Button_1=self.Panel_Center:getChildByName("Button_1")
    Button_1:setVisible(false)
    --代币
    self.Panel_Center:getChildByName("Text_2"):setString("代币：999999999")
    self.Panel_Center:getChildByName("Text_2"):setVisible(false)
    self.Panel_Center:getChildByName("Text_1"):setVisible(false)
    self.Panel_Center:getChildByName("Text_2"):setVisible(false)
    
    --self:initTab()
    
    --获取商品信息
    self:getShopList()
end

function StoreScene:initTab()
    local node,item
    node=self:createNode("Node/Node_tab.csb")
    item = node:getChildByName("Panel_tab")
    local contentSize =item:getContentSize()
    local Image_BgFrame = self.Panel_Center:getChildByName("Image_Bg")
    self.buttons = {}
    for i = 1,4 do
        local newItem = item:clone()
        newItem:setCascadeOpacityEnabled(true)
        newItem:getChildByName("Button_Select_1"):addTouchEventListener(function(sender)
            self:setIndex(sender:getTag())
        end
        )
        local index = i-1
        self.buttons[i] = newItem:getChildByName("Button_Select_1")
        newItem:getChildByName("Button_Select_1"):setTag(index)
        self:addChild(newItem)

        local x=Image_BgFrame:getPositionX() - contentSize.width/2 + 30
        local y=Image_BgFrame:getPositionY() - contentSize.height/2 - (contentSize.height * index) - 10
        newItem:setPosition(cc.p(x,y))

    end
end
--获取商品列表
function StoreScene:getShopList()
    self:storeSceneInit(self.shopData)
end

--初始化界面
function StoreScene:storeSceneInit(data)
--    cclog("len-------"..#(data.goodsList))
--    cclog("price-------"..data.goodsList[1].price)
--    cclog("priceType-------"..data.goodsList[1].priceType)
--    cclog("mark-------"..data.goodsList[1].mark)
--    cclog("originalPrice-------"..data.goodsList[1].originalPrice)
--    cclog("vipLevel-------"..data.goodsList[1].vipLevel)
--    cclog("quota-------"..data.goodsList[1].quota)
--    cclog("startTime-------"..data.goodsList[1].startTime)
--    cclog("endTime-------"..data.goodsList[1].endTime)
    
    --Todo 关闭所有定时器
    self:killAllTimer()
    --Todo 隐藏所有item
    self:setVisibleAllItem(false)
    
    
    local contentSize = self.item:getContentSize()
    
    local point = cc.p(self.scrollView:getPosition())
    local scrollSize = self.scrollView:getContentSize()
    self.scrollView:setContentSize(cc.size(scrollSize.width,scrollSize.height + 40))
    self.scrollView:setPositionY(self.scrollView:getPositionY() + 40)
    
    local num = #(data.goodsList)
    local sSize = cc.size(self.scrollView:getInnerContainerSize().width, 
        math.max(self.scrollView:getContentSize().height, (contentSize.height + self.space)*math.ceil(num/2)));
    self.scrollView:setInnerContainerSize(sSize)
    self.scrollView:removeAllChildren()
    --添加item
    for i=1,num do
        local newItem
        if self.items[i] == nil then
            newItem = self.item:clone()
            newItem:setCascadeOpacityEnabled(true)
            newItem:getChildByName("cost_1"):setCascadeOpacityEnabled(true)
            newItem:getChildByName("cost_2"):setCascadeOpacityEnabled(true)
            newItem:retain()
            self.items[i] = newItem
        else
            newItem = self.items[i]
        end
        
        local index = i - 1
        newItem:setTag(i)
        self.scrollView:addChild(newItem)
        local x = (sSize.width/2) / 2.0 + sSize.width/2 * (index%2) - 10;
        local y = sSize.height - (contentSize.height + self.space) * math.floor(index / 2) - contentSize.height/2
        newItem:setPosition(cc.p(x, y))
        
        local prize = data.goodsList[i]
        
        self:updateItemView(newItem, prize);
        
    end
end

function StoreScene:updateItemView(newItem, prize)
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
    local buyButton_bt = newItem:getChildByName("Button_buy")

    local iconTab_iv = newItem:getChildByName("Image_13")

    iconTab_iv:setVisible(false)
    newItem:getChildByName("Text_1"):setString(LANG("%s", prize.goodsPrize.content))
    newItem:getChildByName("Text_Time"):setString(LANG(""))
    name_tv:setString(LANG(model_static_library:getName(prize.goodsPrize.value, prize.goodsPrize.type)))
    if model_static_library:getColor(prize.goodsPrize.value, prize.goodsPrize.type) then
        name_tv:setColor(model_static_library:getColor(prize.goodsPrize.value, prize.goodsPrize.type))
    end
    local des = LANG(model_static_library:getDesc(prize.goodsPrize.value, prize.goodsPrize.type))
    cclog("des1 = "..des)
    local len = self:utfstrlen(des)
    if len > 18 then    --18
        des = self:GetShortName(des, 34)
    end
    des_tv:setString(LANG("%s", des))
    icon_iv:loadTexture(model_static_library:getAsset(prize.goodsPrize.value, prize.goodsPrize.type))
    icon_iv:setTouchEnabled(true)
    icon_iv:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:showDlg("common/PropsPopScene", {id = prize.goodsPrize.value, type = prize.goodsPrize.type})
        end
    end)
    newItem:getChildByName("Image_Head_Bg"):loadTexture(model_static_library:getFrame(prize.goodsPrize.value, prize.goodsPrize.type))

    --标签
    --        if prize.mark > 0 then
    --            iconTab_iv:setVisible(true)
    --            iconTab_iv:loadTexture(prize.mark..".png")      --标签
    --        end
    
    --物品是否打折
    local steps = 1;
    if prize.originalPrice == 0 then                --没有打折，originalPrice=0
        cost_2_gp:setVisible(false)
        priceCost_tv:setString(LANG(prize.price))
        priceText_tv:setString(LANG("价格:"))
    else                                            --打折，originalPrice是原价，price是折扣后的价格
        cost_2_gp:setVisible(true)
        priceCost_tv:setString(LANG(""..prize.price))
        priceText_tv:setString(LANG("折扣价:"))
        orPriceCost_tv:setString(LANG(""..prize.originalPrice))
        orPriceText_tv:setString(LANG("原价:"))
    end

    --货币类型
    if prize.priceType == "Cash" then
        newItem:getChildByName("cost_1"):getChildByName("Image_Cash"):loadTexture("Share/icon/Icon_Gold.png")
        newItem:getChildByName("cost_2"):getChildByName("Image_Cash"):loadTexture("Share/icon/Icon_Gold.png")
    elseif prize.priceType == "Coin" then
        newItem:getChildByName("cost_1"):getChildByName("Image_Cash"):loadTexture("Share/icon/Icon_Cash1.png")
        newItem:getChildByName("cost_2"):getChildByName("Image_Cash"):loadTexture("Share/icon/Icon_Cash1.png")
    end
    
    self:updateListUI(newItem, prize);
end
--item 按钮开始时间，提示文字出现位置等设置
function StoreScene:updateListUI(view, dataItem)
--    cclog("dataItem.startTime------"..dataItem.startTime)
--    cclog("dataItem.endTime------"..dataItem.endTime)
--    cclog("model_user:getCurrentServerTimestamp()------"..model_user:getCurrentServerTimestamp())
    
    local limitIndex = 1        --限制条件影响下文字出现的位置    1：按钮上方 2：按钮下方 3：标签 4：按钮上
    local textView = view:getChildByName("Text_limited")
    local buyButton_bt = view:getChildByName("Button_buy")
    local timerId;
    local isLimitTime = false;
    local isLimitVip = false;
    local isLimitCount = false;
    --限时
    --dataItem.startTime = model_user:getCurrentServerTimestamp() + 10
    --dataItem.endTime = model_user:getCurrentServerTimestamp() - 100
    --dataItem.startTime = dataItem.endTime - 10
	if dataItem.startTime ~= 0 then
	    isLimitTime = true
        limitIndex = limitIndex + 1
	   if model_user:getCurrentServerTimestamp() < dataItem.startTime then
            timerId = Timer:start(function()
	           local leftTime = (dataItem.startTime - model_user:getCurrentServerTimestamp())
	           if leftTime >= 0 then
                    local second = (math.floor(leftTime % 60) < 10) and ("0"..math.floor(leftTime % 60)) or math.floor(leftTime % 60);
                    local minute = (math.floor((leftTime - second) % 3600 / 60) < 10) and ("0"..math.floor((leftTime - second) % 3600 / 60)) or math.floor((leftTime - second) % 3600 / 60);
                    local hour = (math.floor((leftTime - second - minute) / 3600) < 10) and ("0"..math.floor((leftTime - second - minute) / 3600)) or math.floor((leftTime - second - minute) / 3600);
                    
                    textView:setString(LANG("%s:%s:%s后开启", hour, minute, second))
                else
                    textView:setString(LANG("00:00:00"))
                    Timer:kill(timerId)
                    self:printArr(self.timerArray)
                    self:removeFromTable(self.timerArray, timerId)
                    self:printArr(self.timerArray)
                    self:updateListUI(view, dataItem)
	           end
	       end, 1, nil)
	       --self.timerArray[#(self.timerArray) + 1] = timerId
            table.insert(self.timerArray,timerId)
            self:printArr(self.timerArray)
	       cclog("array len = "..#(self.timerArray))
        elseif dataItem.startTime < model_user:getCurrentServerTimestamp() and  model_user:getCurrentServerTimestamp() < dataItem.endTime then
            isLimitTime = false
            timerId = Timer:start(function()
                local leftTime = dataItem.endTime - model_user:getCurrentServerTimestamp()
                if leftTime >= 0 then
--                    local second = math.floor(leftTime % 60);
--                    local minute = math.floor((leftTime - second) % 3600 / 60);
--                    local hour = math.floor((leftTime - second - minute) / 3600);
                    local second = (math.floor(leftTime % 60) < 10) and ("0"..math.floor(leftTime % 60)) or math.floor(leftTime % 60);
                    local minute = (math.floor((leftTime - second) % 3600 / 60) < 10) and ("0"..math.floor((leftTime - second) % 3600 / 60)) or math.floor((leftTime - second) % 3600 / 60);
                    local hour = (math.floor((leftTime - second - minute) / 3600) < 10) and ("0"..math.floor((leftTime - second - minute) / 3600)) or math.floor((leftTime - second - minute) / 3600);
                    
                    textView:setString(LANG("%s:%s:%s后结束", hour, minute, second))
                else 
                     textView:setString(LANG("00:00:00"))
                     Timer:kill(timerId)
                    self:removeFromTable(self.timerArray, timerId)
                    self:printArr(self.timerArray)
                    self:updateListUI(view, dataItem)
                end
           end, 1, nil)
            --self.timerArray[#(self.timerArray) + 1] = timerId
            table.insert(self.timerArray,timerId)
         else
            isLimitTime = true
	     end
	    end
    --view:getChildByName("Text_Time"):setString(LANG(""))
    view:getChildByName("Text_limited"):setString(LANG(""))
	 
	 --限次
	 if dataItem.buyLimited == 1 then
        self:limitPlace(view, limitIndex, LANG("今日可买%s次", dataItem.quota))
        limitIndex = limitIndex + 1
        if dataItem.quota <= 0 then
            isLimitCount = true
        end
	 end
	 
	 --Vip
	 if dataItem.vipLevel > 0 then
        self:limitPlace(view, limitIndex, LANG("vip%s可购买", dataItem.vipLevel), "VIP")
        limitIndex = limitIndex + 1
        isLimitVip = true
	 end
	 --isLimitVip = false;
    --按钮限制
    if isLimitVip then
        buyButton_bt:setTitleText(LANG("未解锁"))
        buyButton_bt:setBright(false)
        buyButton_bt:addTouchEventListener(function(sender, eventType)
                if eventType == 2 then
                PopMsg.getInstance():flashShow(LANG("未到购买所需VIP等级"))
                end
            end)
    elseif isLimitTime then
        buyButton_bt:setTitleText(LANG("未解锁"))
        buyButton_bt:setBright(false)
        buyButton_bt:addTouchEventListener(function(sender, eventType)
            if eventType == 2 then
                PopMsg.getInstance():flashShow(LANG("出售时间未到"))
            end
        end)
    elseif isLimitCount then
        buyButton_bt:setTitleText(LANG("售罄"))
        buyButton_bt:setBright(false)
        buyButton_bt:addTouchEventListener(function(sender, eventType)
            if eventType == 2 then
                PopMsg.getInstance():flashShow(LANG("售罄")) 
            end
        end)
    else
        buyButton_bt:setBright(true)
        
        buyButton_bt:addTouchEventListener(function(sender, eventType)
            if eventType == 2 then
                self:showDlg("arena/StorePupopScene", {shopItem = dataItem, callback = function(prize)
                    --self:getShopList()
                    self.shopData.goodsList[view:getTag()] = prize
                    self:updateItemView(view, prize)
                end})
            end
        end)
        
--        buyButton_bt:addTouchEventListener(function(sender, eventType)
--            if eventType == 2 then
--                --SWITSCENE("arena/StorePupopScene", dataItem)
--                self:showDlg("arena/StorePupopScene", dataItem, function(data)
--                    --self:getShopList()
--                    self:storeSceneInit(data)
--                end)
--            end
--        end)
    end 
	 
end

function StoreScene:showDialog(text1, text2, description)
    self:showDlg("common/CommonPopScene",{confirmStr=text1,cancelStr=text2,text=description,fun=function(data)
        if data.cmd=="confirm" then
             if self.dialog then
                   self.dialog:removeDlgNoAction()
             end
        end
    end}) 
end

function StoreScene:removeFromTable(arr, v)
    cclog("befor remove".." v = "..v)
    local i = 1
    while i <= #arr do
        if arr[i] == v then
            cclog("remove i = "..i.." v = "..v)
            table.remove(arr,i)
            break
        else
            i = i + 1
        end
    end
end

--影响按钮的限制条件
function StoreScene:limitOnButton(isLimitVip, isLimitTime, isLimitCount)
    
end

function StoreScene:limitPlace(view, limitIndex, text, des)
    local textView = view:getChildByName("Text_limited")
    if limitIndex == 1 then      --按钮上方
        local textView = view:getChildByName("Text_limited")
        textView:setString(text)
    end
    
    if limitIndex == 2 then      --按钮下方
        local textView = view:getChildByName("Text_Time")
        textView:setString(text)
    end
    
    if limitIndex == 3 then      --标签
        if des ~=nil and des == "VIP" then
            local iconTab_iv = view:getChildByName("Image_13")
            iconTab_iv:setVisible(true)
            iconTab_iv:loadTexture("Assets/ShopScene/VIP.png")
        end
    end
    
    if limitIndex == 4 then      --按钮
        local buyButton_bt = view:getChildByName("Button_buy")
        buyButton_bt:setTitleText(text)
    end
end

function StoreScene:printArr(arr)
    for key, value in ipairs(arr) do      
        cclog("key="..key.."  value="..value)
    end
end

function StoreScene:onExit()
    --cclog("onExit")
    self:killAllTimer()
    self:releaseAllItem()
end

function StoreScene:killAllTimer()
    --self:printArr(self.timerArray)
    for key, value in ipairs(self.timerArray) do      
        if value then
            Timer:kill(value)
        end
    end
end

function StoreScene:releaseAllItem()
    for i=1,#(self.items) do
        self.items[i]:release()
    end
end

function StoreScene:setVisibleAllItem(isVisible)
    for key, value in pairs(self.items) do      
        if value then
            value:setVisible(isVisible)
        end
    end
end

function StoreScene:utfstrlen(str)
    local len = #str;
    local left = len;
    local cnt = 0;
    local arr={0,0xc0,0xe0,0xf0,0xf8,0xfc};
    while left ~= 0 do
        local tmp=string.byte(str,-left);
        local i=#arr;
        while arr[i] do
            if tmp>=arr[i] then left=left-i;break;end
            i=i-1;
        end
        cnt=cnt+1;
    end
    return cnt;
end

function StoreScene:GetShortName(sName,nMaxCount,nShowCount)
    if sName == nil or nMaxCount == nil then
        return
    end
    local sStr = sName
    local tCode = {}
    local tName = {}
    local nLenInByte = #sStr
    local nWidth = 0
    if nShowCount == nil then
        nShowCount = nMaxCount - 3
    end
    for i=1,nLenInByte do
        local curByte = string.byte(sStr, i)
        local byteCount = 0;
        if curByte>0 and curByte<=127 then
            byteCount = 1
        elseif curByte>=192 and curByte<223 then
            byteCount = 2
        elseif curByte>=224 and curByte<=239 then
            byteCount = 3
        elseif curByte>=240 and curByte<=247 then
            byteCount = 4
        end
        local char = nil
        if byteCount > 0 then
            char = string.sub(sStr, i, i+byteCount-1)
            i = i + byteCount -1
        end
        if byteCount == 1 then
            nWidth = nWidth + 1
            table.insert(tName,char)
            table.insert(tCode,1)

        elseif byteCount > 1 then
            nWidth = nWidth + 2
            table.insert(tName,char)
            table.insert(tCode,2)
        end
    end

    if nWidth > nMaxCount then
        local _sN = ""
        local _len = 0
        for i=1,#tName do
            _sN = _sN .. tName[i]
            _len = _len + tCode[i]
            if _len >= nShowCount then
                break
            end
        end
        sName = _sN .. "..."
    end
    return sName
end

return StoreScene