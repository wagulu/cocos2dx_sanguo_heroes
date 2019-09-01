
local AdditionPopScene = class("AdditionPopScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
AdditionPopScene.RESOURCE_FILENAME = "Scene/AdditionPop.csb"


function AdditionPopScene:onCreate()
    local Panel_AdditionPop = self:getResourceNode():getChildByName("Panel_AdditionPop")
    Panel_AdditionPop:setTouchEnabled(true)
    local Panel_Bg = Panel_AdditionPop:getChildByName("Panel_Bg")
    Panel_Bg:setTouchEnabled(true)
    local Panel_Center = Panel_AdditionPop:getChildByName("Panel_Center")
    local Panel_Top = Panel_AdditionPop:getChildByName("Panel_Top")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    local Button_Close = Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end

    end
    )
end



function AdditionPopScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")

    end
end
return AdditionPopScene
