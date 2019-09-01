--寻宝结果界面
local SearchOnceResultScene = class("SearchOnceResultScene", cc.load("mvc").ViewBase)
SearchOnceResultScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"

SearchOnceResultScene.RESOURCE_FILENAME = "Scene/SearchOnceResultScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function SearchOnceResultScene:onCreate()
    self:initUi()
end

function SearchOnceResultScene:initUi()
    local Panel_SearchOnceResultScene = self:getResourceNode():getChildByName("Panel_SearchOnceResultScene")
    local Panel_Center = Panel_SearchOnceResultScene:getChildByName("Panel_Center")
    local Panel_Top = Panel_SearchOnceResultScene:getChildByName("Panel_Top")
    local Panel_Bg = Panel_SearchOnceResultScene:getChildByName("Panel_Bg")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    --关闭按钮
    local Button_Close = Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end

    end
    )
end

return SearchOnceResultScene
