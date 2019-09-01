--寻宝战斗界面
local SearchScene = class("SearchScene", cc.load("mvc").ViewBase)
SearchScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"

SearchScene.RESOURCE_FILENAME = "Scene/SearchScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function SearchScene:onCreate()
    self:initUi()
end

function SearchScene:initUi()
    local Panel_SearchScene = self:getResourceNode():getChildByName("Panel_SearchScene")
    local Panel_Center = Panel_SearchScene:getChildByName("Panel_Center")
    local Panel_Top = Panel_SearchScene:getChildByName("Panel_Top")
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    local Panel_Bg = Panel_SearchResultScene:getChildByName("Panel_Bg")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            SWITSCENE("search/SearchTreasureScene")
        end
    end
    )
    --搜寻打5次按钮
    local  Button_Search=Panel_Top:getChildByName("Button_Search")
    Button_Search:addTouchEventListener(function()
        self:onSearchBtn()
    end
    )
    
    --详情按钮
    local detailBtn = Panel_Center:getChildByName("detailBtn")
    detailBtn:addTouchEventListener(function()
        self:onDetailBtn()
    end)
    
    --石头剪子布，单打一次
    for i=1,3 do
        local  Button_Battle=Panel_Center:getChildByName("Button_Battle_"..i)
        Button_Battle:setTag(i)
        Button_Battle:setTouchEnabled(true);
        Button_Battle:addTouchEventListener(function(sender)
            self:onOnceBattleHandle(sender:getTag())
        end
        )
    end
end

function onSearchBtn()
    --这里应该向服务器发送请求，搜寻打5次
    SWITSCENE("search/SearchResultScene")
end

function onOnceBattleHandle(index)
    --这里应该向服务器发送请求，打1次
    self:showDlg("search/SearchOnceResultScene",1)
end

function SearchScene:onDetailBtn()
    --策划说可以展示技能详情的那个弹窗
--    local boo = self.detailPanel:getVisible()
--    if boo == true then
--        self.detailPanel:setVisible(true)
--    else
--        self.detailPanel:setVisible(false)
--    end
    
end

return SearchScene
