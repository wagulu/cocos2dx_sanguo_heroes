
local CommonPopScene = class("CommonPopScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
CommonPopScene.RESOURCE_FILENAME = "Scene/RechargePopupScene.csb"


function CommonPopScene:onCreate()
    local data = self.args[1]
    local confirmText = data.confirmStr
    local cancelStr = data.cancelStr or ""
    local text = data.text
    local isSigleBtn = data.isSigleBtn or false -- 单个确定按钮
    
    local RechargePopup = self:getResourceNode():getChildByName("RechargePopup")
    local Panel_Center = RechargePopup:getChildByName("Panel_Center")
    local Panel_Bg = RechargePopup:getChildByName("Panel_Bg")
    local Popup = Panel_Center:getChildByName("Popup")
    local Text_1 = Popup:getChildByName("Text_1")
    local Button_skip = Popup:getChildByName("Button_skip")
    local Button_cancel = Popup:getChildByName("Button_cancel")
    Button_skip:setTitleText(confirmText)
    Button_cancel:setTitleText(cancelStr)
    Button_skip:setTitleText(confirmText)
    Text_1:setString(text)
    RechargePopup:setTouchEnabled(true)
    Button_skip:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            if data.fun then
            	data.fun({cmd="confirm"})
                self:removeDlgNoAction()
            end
            --dlgCallBack
        end

    end
    )
    Button_cancel:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            if data.fun then
                data.fun({cmd="cancel"})
                self:removeDlgNoAction()
            end
        end

    end
    )
    if isSigleBtn then
       Button_cancel:hide()
       local offSetX = Button_skip:getPositionX() + (Button_cancel:getPositionX() - Button_skip:getPositionX()) * 0.5
       Button_skip:setPositionX(offSetX)
    end
    
    SCREEN_SCALE_BG(Panel_Bg)
end


return CommonPopScene
