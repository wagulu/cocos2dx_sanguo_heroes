
local GetRewardPopupScene = class("GetRewardPopupScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"

GetRewardPopupScene.RESOURCE_FILENAME = "Scene/GetRewardPopupScene.csb"
GetRewardPopupScene.STOPFRAME = 25


function GetRewardPopupScene:onCreate()
    self.data = self.args[1]
    self.fun = self.args[2]
    self:initUi()
end

function GetRewardPopupScene:initUi()
    local Panel_GetRewardPopupScene = self:getResourceNode():getChildByName("SkipPopup")
    Panel_GetRewardPopupScene:setTouchEnabled(true)
    local Panel_Bg = Panel_GetRewardPopupScene:getChildByName("Panel_Bg")
    local Panel_Center = Panel_GetRewardPopupScene:getChildByName("Panel_Center")
    self.Popup = Panel_Center:getChildByName("Popup")
    --    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    --领取按钮
    local Button_skip = self.Popup:getChildByName("Button_skip")
    Button_skip:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            ctlHighladderDrawRequest:sendRequest(handler(self,self.onHighladderDraw));
        end
    end
    )

    --取消按钮
    local Button_cancel = self.Popup:getChildByName("Button_cancel")
    Button_cancel:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.fun()
            self:removeDlgNoAction()
        end
    end
    )
    
    Panel_GetRewardPopupScene:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
--            self.fun()
            self:removeDlgNoAction()
        end
    end
    )
    
    self:setData()
end

function GetRewardPopupScene:setData()
    local Panel_JL = self.Popup:getChildByName("Panel_JL")
    --排行
    Panel_JL:getChildByName("Text7"):setString(self.data.self.rank)
    --声望
    Panel_JL:getChildByName("Text5"):setString(self.data.self.prestige)
    --声望和元宝
    Panel_JL:getChildByName("Text1_12"):setString(self.data.self.rankPrestige)
    Panel_JL:getChildByName("Text2_14"):setString(self.data.self.rankCash)
    Panel_JL:getChildByName("Text3_18"):setString(self.data.self.prestige2Draw)
    Panel_JL:getChildByName("Text4_20"):setString(self.data.self.cash2Draw)
end

function GetRewardPopupScene:onHighladderDraw(success, data, code, msg)
    if(success) then
        local haveCash = self.data.self.cash2Draw - data.cash
        if haveCash < 0 then
            haveCash = 0
        end

        local havePrestige = self.data.self.prestige2Draw - data.prestige
        if havePrestige < 0 then
            havePrestige = 0
        end

        self.data.self.cash2Draw = haveCash
        self.data.self.prestige2Draw = havePrestige
        self.data.self.prestige = self.data.self.prestige + data.prestige
        self:setData()
        PopMsg.getInstance():flashShow(LANG("领取成功！"))
        self.fun()
    else
    end
end

function GetRewardPopupScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")
    end
end
return GetRewardPopupScene
