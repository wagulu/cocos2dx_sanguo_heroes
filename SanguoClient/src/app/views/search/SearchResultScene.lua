--寻宝5次结果界面
local SearchResultScene = class("SearchResultScene", cc.load("mvc").ViewBase)
SearchResultScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"

SearchResultScene.RESOURCE_FILENAME = "Scene/SearchResultsPanel.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function SearchResultScene:onCreate()
    self:initUi()
end

function SearchResultScene:initUi()
    local Panel_SearchResultScene = self:getResourceNode():getChildByName("Panel_SearchResults")
--    local Panel_Center = Panel_SearchResultScene:getChildByName("Panel_Center")
--    local Panel_Top = Panel_SearchResultScene:getChildByName("Panel_Top")
--    local Panel_Bg = Panel_SearchResultScene:getChildByName("Panel_Bg")
--    SCREEN_TOP(Panel_Top)
--    SCREEN_SCALE_BG(Panel_Bg)
    --继续按钮
--    local  Button_Continue=Panel_Center:getChildByName("Button_Continue")
--    Button_Continue:addTouchEventListener(function()
--        self:onContinueBtn()
--    end
--    )
    --搜索结果列表
--    local scrollView = Panel_Center:getChildByName("ScrollView")
--    local contentSize =cc.size(435,138)
--    local node=self:createNode("Node/Node_Item_1.csb")
--    local item = node:getChildByName("panel")
--    self.space = 10
--    local num = 20
--    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*num));
--    scrollView:setInnerContainerSize(sSize)
--    for i=1,num do
--        local newItem = item:clone()
--        local index = i-1
--        newItem:setTag(index);
--        scrollView:addChild(newItem)
--        local x=sSize.width/2
--        local y=sSize.height-(contentSize.height+self.space)*index-contentSize.height/2
--        newItem:setPosition(cc.p(x,y))
--    end
    
end

function SearchResultScene:onContinueBtn()
--    if exp then --碎片足够
--    	SWITSCENE("search/SearchTreasureScene")
--    else        --碎片不足
--      SWITSCENE("search/SearchScene")
--    end
end

return SearchResultScene
