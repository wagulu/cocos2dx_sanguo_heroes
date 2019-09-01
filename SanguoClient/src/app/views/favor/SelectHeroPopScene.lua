
local SelectHeroPopScene = class("SelectHeroPopScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
SelectHeroPopScene.RESOURCE_FILENAME = "Scene/SelectHeroPop.csb"


function SelectHeroPopScene:onCreate()
    local Panel_SelectHeroPop = self:getResourceNode():getChildByName("Panel_SelectHeroPop")
    Panel_SelectHeroPop:setTouchEnabled(true)
    local Panel_Bg = Panel_SelectHeroPop:getChildByName("Panel_Bg")
    Panel_Bg:setTouchEnabled(true)
    local Panel_Center = Panel_SelectHeroPop:getChildByName("Panel_Center")
    local Panel_Top = Panel_SelectHeroPop:getChildByName("Panel_Top")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    local Button_Close_15 = Panel_Top:getChildByName("Button_Close_15")
    Button_Close_15:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end
    end
    )
    local scrollView = Panel_Center:getChildByName("HeroScrollView_1")
    local node,item,type
    type = self.args[1].type
    if type==1 then
        node=self:createNode("Node/Node_SelectHeroPop_Item.csb")
        item = node:getChildByName("Panel_SelectHeroPop_Item")
    elseif type ==2 then
        node=self:createNode("Node/Node_SelectTreasurePop_Item.csb")
        item = node:getChildByName("Panel_SelectTreasurePop_Item")
    end
    
    local contentSize =item:getContentSize()
    self.space = 10
    local num = 20
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
                self:removeDlgNoAction()
            end
        end
        )
        local index = i-1
        newItem:setTag(index);
        scrollView:addChild(newItem)
        local x=(sSize.width/2)/2.0+sSize.width/2*(index%2);
        --local t = math.floor(index/2)
        local y=sSize.height-(contentSize.height+self.space)*math.floor(index/2)-contentSize.height/2
        newItem:setPosition(cc.p(x,y))
        if type==2 then
            self:initTreasureItem(newItem)
        end
    end
end

function SelectHeroPopScene:initTreasureItem(item)
    local Button_Equipment = item:getChildByName("Button_Equipment")
    Button_Equipment:addTouchEventListener(function(sender,eventType)--卸下
        if  eventType == 2 then
            self.dlgCallBack({cmd="select",typeId = self.args[1].typeId})
            self:removeDlgNoAction()
    end
    end)
end

function SelectHeroPopScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")

    end
end
return SelectHeroPopScene
