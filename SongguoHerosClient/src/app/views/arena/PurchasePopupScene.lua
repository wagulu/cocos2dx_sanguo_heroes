--购买确认界面
local PurchasePopupScene = class("PurchasePopupScene", cc.load("mvc").ViewBase)
PurchasePopupScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE("pbCommon.lua")
local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
PurchasePopupScene.STOPFRAME = 35
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua")
local model_treasureItemStorage = _REQUIRE("model/model_treasureItemStorage.lua")
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua")
local model_user = _REQUIRE("model/model_user.lua")
local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"

PurchasePopupScene.RESOURCE_FILENAME = "Scene/PurchasePopupScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function PurchasePopupScene:onCreate()
    self.shopItem = self.args[1].shopItem
    self.prize = self.shopItem.prize
    self.callback = self.args[1].callback
    self:initUi()
end

function PurchasePopupScene:initUi()
    local StorePopupScene = self:getResourceNode():getChildByName("PurchasePopupScene")
    self.Panel_Center = StorePopupScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    local Panel_Top = StorePopupScene:getChildByName("Panel_Top")
    local Panel_Bg = StorePopupScene:getChildByName("Panel_Bg")
    self.Panel_Bottom = StorePopupScene:getChildByName("Panel_B"):getChildByName("Panel_Bottom")
    local Image_Item_Bg = self.Panel_Center:getChildByName("Image_Item_Bg")
    local Button_1 = self.Panel_Center:getChildByName("Button_1")
    local Button_2 = self.Panel_Center:getChildByName("Button_2")
    local buttonMax = self.Panel_Center:getChildByName("Button_4")
    local buttonMin = self.Panel_Center:getChildByName("Button_4_0")
    local Node_1 = self.Panel_Center:getChildByName("Node_1")--购买物品的图标
    SCREEN_TOP(Panel_Top)
    SCREEN_BOTTOM(self.Panel_Bottom)
    SCREEN_SCALE_BG(Panel_Bg)
    
    StorePopupScene:setTouchEnabled(true)
    local icon_iv = self.Panel_Center:getChildByName("Node_1")
    local name_tv = self.Panel_Center:getChildByName("Text_1")
    local haveCount_tv = self.Panel_Center:getChildByName("Text_4")
    local des_tv = self.Panel_Center:getChildByName("Text_5")
    local buyCount_tv = self.Panel_Center:getChildByName("Text_7")
    local Button_1 = self.Panel_Center:getChildByName("Button_1")
    local Button_2 = self.Panel_Center:getChildByName("Button_2")
    local buttonMax = self.Panel_Center:getChildByName("Button_4")
    local buttonMin = self.Panel_Center:getChildByName("Button_4_0")
    Button_1:setVisible(false)
    Button_2:setVisible(false)
    buttonMax:setVisible(false)
    buttonMin:setVisible(false)

    self.textTotal = self.Panel_Center:getChildByName("Text_8")
    self.textTotal2 = self.Panel_Center:getChildByName("Text_7")
    self.timerId = 0;
    self.count = self:getPrizeMaxCount();
    self.goodsCount = 1;
    
    self:updateGoodsText()

    --购买按钮
    local Button_3 = self.Panel_Bottom:getChildByName("Button_3")
    Button_3:addTouchEventListener(function(sender,eventType)
        if eventType==2 then

            if self:getPrizeCount(self.prize.type,self.prize.value) <= 999 then
                if self.count == 0 then
                    PopMsg.getInstance():flashShow(LANG("声望值不够！"))
                    return
                end
                ctlHighladderBuyItemRequest:sendRequest(handler(self,self.onHighladderBuyItem), self.shopItem.pos);
            else
                PopMsg.getInstance():flashShow(LANG("物品已满"))
            end
        end
    end
    )

    local Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:removeDlgNoAction()
        end
    end
    )
 
    self:updateUI()

end
function PurchasePopupScene:updateUI()
    local name_tv = self.Panel_Center:getChildByName("Text_1")
    local haveCount_tv = self.Panel_Center:getChildByName("Text_4")
    local des_tv = self.Panel_Center:getChildByName("Text_5")
    --价格 是否打折
    if self.shopItem.prestige == self.shopItem.discount then 
        self.Panel_Bottom:getChildByName("Text_Cost"):setString(LANG("%s", self.shopItem.discount))
    else
        self.Panel_Bottom:getChildByName("Text_Cost"):setString(LANG("%s", self.shopItem.prestige))
    end
    
    self.Panel_Bottom:getChildByName("Image_Icon_Cash"):loadTexture("Share/icon/icon_popularity.png")

    name_tv:setString(LANG(model_static_library:getName(self.prize.value, self.prize.type)))
    haveCount_tv:setString(LANG(self:getPrizeCount(self.prize.type,self.prize.value)))
    des_tv:setString(LANG(model_static_library:getDesc(self.prize.value, self.prize.type)))
    
    local Panel_Object = self.Panel_Center:getChildByName("Node_1"):getChildByName("Panel_Object")
    Panel_Object:getChildByName("Image_1"):loadTexture(model_static_library:getAsset(self.prize.value, self.prize.type))
    Panel_Object:getChildByName("Image_2"):loadTexture(model_static_library:getFrame(self.prize.value, self.prize.type))
    Panel_Object:getChildByName("Text_1"):setString(LANG("%s",self.prize.content))
    Panel_Object:getChildByName("Text_1"):setVisible(true)
end

function PurchasePopupScene:onHighladderBuyItem(success, data, code, msg)
    if success then
       
        PopMsg.getInstance():flashShow(LANG("购买成功！"))
        self.prize = data.prize
        self.callback(data.shop)
        self:removeDlgNoAction()
    else
    --WARNING(msg);
    end
end


function PurchasePopupScene:updateGoodsText()
    self.textTotal:setString(" " .. self.goodsCount)
    self.textTotal2:setString(" " .. self.goodsCount)
end

function PurchasePopupScene:getPrizeCount(type, id)
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

function PurchasePopupScene:getPrizeMaxCount()
    local maxCount
    local shopItem = self.shopItem
    local price        --单间价格
    local total = 0       --货币总数
    local currentCount --当前拥有数量(单个物品)

    currentCount = self:getPrizeCount(self.prize.type,self.prize.value)

    --价格 是否打折
    if shopItem.prestige == shopItem.discount then 
        price = shopItem.discount
    else
        price = shopItem.prestige
    end

    --货币总数
    total = model_user.prestige

    local count = math.floor(total/price)
    if count > 0 then
        count = 1
    end

    --购买够的单件数量是否超过999
    local buyedCount = count * self.prize.content + currentCount
    if buyedCount > 999 then
        maxCount = 0
    else
        maxCount = count
    end

    return maxCount
end

return PurchasePopupScene