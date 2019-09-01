
local SuccinctPopScene = class("SuccinctPopScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
SuccinctPopScene.RESOURCE_FILENAME = "Scene/SuccinctPop.csb"


function SuccinctPopScene:onCreate()
    local args = self.args
    local Panel_SuccinctPop = self:getResourceNode():getChildByName("Panel_SuccinctPop")
    Panel_SuccinctPop:setTouchEnabled(true)
    local Panel_Bg = Panel_SuccinctPop:getChildByName("Panel_Bg")
    Panel_Bg:setTouchEnabled(true)
    local Panel_Center = Panel_SuccinctPop:getChildByName("Panel_Center")
    local Panel_Top = Panel_SuccinctPop:getChildByName("Panel_Top")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    local pageView = Panel_Center:getChildByName("PageView_1")
    local Panel_Item = Panel_Center:getChildByName("Panel_Item")
    local Button_TH = Panel_Center:getChildByName("Button_TH")
    Button_TH:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.args[1]()
            self:removeDlgNoAction()
            
        end

    end
    )
    Panel_Item:setVisible(false)
    pageView:removeAllPages()
    pageView:setTouchEnabled(true)
    pageView:setCustomScrollThreshold(20)

    for i=1,3 do
        local item = Panel_Item:clone()
        item:setVisible(true)
        pageView:addPage(item)
    
    end
    --pageView:setPositionX(436 -Panel_Item:getContentSize().width)
    --    for k,page in ipairs(pageView:getPages())  do
    --        page:setPosition(cc.p((k-4)*Panel_Item:getContentSize().width,
    --            0));
    --    end
    local function pageViewEvent(sender, eventType)
        --ccui.PageViewEventType.turning
        -- cclog(eventType)
        if eventType == 0 then
            local pageView = sender
            local pageInfo = string.format("page %d " , pageView:getCurPageIndex() + 1)
            local totalPage = 3
            if pageView:getCurPageIndex() + 1 ==totalPage then
            --                local firstPage = pageView:getPage(0)
            --                pageView:removePage(firstPage)
            --                local item = Panel_Item:clone()
            --                item:setVisible(true)
            --pageView:insertPage(item,4)
            end
            --            self.pageIndex = pageView:getCurPageIndex() + 1
            cclog(pageInfo)
            --            --self._displayValueLabel:setString(pageInfo)
        end
    end 

    pageView:addEventListener(pageViewEvent)

    local Button_Close = Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end

    end
    )
end



function SuccinctPopScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")

    end
end
return SuccinctPopScene
