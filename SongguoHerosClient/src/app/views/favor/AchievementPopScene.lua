
local AchievementPopScene = class("AchievementPopScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
AchievementPopScene.RESOURCE_FILENAME = "Scene/AchievementPop.csb"


function AchievementPopScene:onCreate()
    local Panel_AchievementPop =  self:getResourceNode():getChildByName("Panel_AchievementPop")
    Panel_AchievementPop:setTouchEnabled(true)
    local Panel_Bg = Panel_AchievementPop:getChildByName("Panel_Bg")
    Panel_Bg:setTouchEnabled(true)
    local Panel_Center = Panel_AchievementPop:getChildByName("Panel_Center")
    local Panel_Top = Panel_AchievementPop:getChildByName("Panel_Top")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    local scrollView = Panel_Center:getChildByName("ScrollView_2")

    local node=self:createNode("Node/Node_AchievementPop_Item.csb")
    local item = node:getChildByName("Panel_AchievementPop_Item")
    local contentSize =item:getContentSize()
    self.space = 0
    local num = 10
    --scrollView:setContentSize(cc.size(419, 452))
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*math.ceil(num/2)));
    cclog("sssss"..sSize.width)
    scrollView:setInnerContainerSize(sSize)
    for i=1,num do
        local newItem = item:clone()
        --newItem:setAnchorPoint(cc.p(0,0))
        newItem:setTouchEnabled(true);
        newItem:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                cclog("touchtouche"..sender:getTag())
                --SWITSCENE("hero/HeroInfoScene")
            end
        end
        )

        local index = i-1
        newItem:setTag(index);
        scrollView:addChild(newItem)

        local x=(sSize.width/2)
        --local t = math.floor(index/2)
        local y=sSize.height-(contentSize.height+self.space)*math.floor(index/2)-contentSize.height/2
        newItem:setPosition(cc.p(x,y))
    end
    local Button_Close_2 = Panel_Top:getChildByName("Button_Close_2")
    Button_Close_2:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end

    end
    )
end



function AchievementPopScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")

    end
end
return AchievementPopScene
