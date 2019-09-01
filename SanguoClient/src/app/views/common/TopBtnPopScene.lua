
local TopBtnPopScene = class("TopBtnPopScene", cc.load("mvc").ViewBase)

_REQUIRE "cocos.cocos2d.json"
TopBtnPopScene.RESOURCE_FILENAME = "Scene/RechargePopup2.csb"


function TopBtnPopScene:onCreate()
    local data = self.args[1]
    local confirmText = data.confirmStr
    local cancelStr = data.cancelStr
    local titleStr = data.titleStr
    local text = data.text
    
    local RechargePopup = self:getResourceNode():getChildByName("RechargePopup2")
    local Panel_Center = RechargePopup:getChildByName("Panel_Center")
    local Panel_Bg = RechargePopup:getChildByName("Panel_Bg")
    SCREEN_SCALE_BG(Panel_Bg)
    local Popup = Panel_Center:getChildByName("Popup")
    local Text_1 = Popup:getChildByName("Text_1")
    local Text_Title = Popup:getChildByName("Text_Title")
    local Button_skip = Popup:getChildByName("Button_skip")
    local Button_cancel = Popup:getChildByName("Button_cancel")
    Button_skip:setTitleText(confirmText)
    Button_cancel:setTitleText(cancelStr)
    Button_skip:setTitleText(confirmText)
    Text_1:setString(text)
    Text_Title:setString(titleStr)
    Panel_Bg:setTouchEnabled(true)
    self:addBtnTouchEventListener(Button_skip,function(sender,eventType)
        if eventType ==2 then
            if data.fun then
                data.fun({cmd="confirm"})
                self:removeDlgNoAction()
            end
            --dlgCallBack
        end
    end
    )
    self:addBtnTouchEventListener(Button_cancel,function(sender,eventType)
        if eventType ==2 then
            if data.fun then
                data.fun({cmd="cancel"})
                self:removeDlgNoAction()
            end
        end
    end
    )
    SCREEN_SCALE_BG(Panel_Bg)
end


return TopBtnPopScene
