
local FavorabilityScene = class("FavorabilityScene", cc.load("mvc").ViewBase)
FavorabilityScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"

FavorabilityScene.RESOURCE_FILENAME = "Scene/FavorabilityScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function FavorabilityScene:onCreate()
    self.buttons = {}
    self.pageIndex =1
    self.Panel_FavorabilityScene = self:getResourceNode():getChildByName("Panel_FavorabilityScene")
    self.Panel_Center = self.Panel_FavorabilityScene:getChildByName("Panel_Center")
    
    local Panel_Top = self.Panel_FavorabilityScene:getChildByName("Panel_Top")
    local Panel_Bg = self.Panel_FavorabilityScene:getChildByName("Panel_Bg")
    Panel_Bg:setTouchEnabled(true)
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2  then
            cclog("ddddddddd")
            SWITSCENE("home/HomeScene")
        end
        
    end
    )
    for i=1,4 do
        local btn = self.Panel_Center:getChildByName("Button_Select_"..i)
        self.buttons[i] = btn
        btn:addTouchEventListener(function(sender,eventType)
        if eventType ==0 then
                sender:stopAllActions();
                sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
        elseif eventType ==2 then
                self:setIndex(sender:getTag())
                self:initUi(sender:getTag())
        end
            
        end)
    end
    self:setIndex(1)
    self:initUi(1)
end

function FavorabilityScene:initUi(type)
    for i=1,4 do
        local Panel_Item = self.Panel_Center:getChildByName("Panel_Item_"..i)
        Panel_Item:setVisible(i==type)
    end
    if type ==1  then
    	self:initPanel1()
    elseif type==2 then
    elseif type==3 then
    elseif type==4 then
    else
    
    end
    

    
end

function FavorabilityScene:initPanel1()
    local Panel_Item_1 = self.Panel_Center:getChildByName("Panel_Item_1")
    local  Panel_Right=Panel_Item_1:getChildByName("Panel_Right")
    local  Panel_Left=Panel_Item_1:getChildByName("Panel_Left")
    local  Button_SXXL=Panel_Left:getChildByName("Button_SXXL")
    Button_SXXL:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:showDlg("favor/SuccinctPopScene",handler(self,self.initPanel1))
        end
    end
    )
    local scrollView = Panel_Right:getChildByName("ScrollView_1")
    local Button_JCZL = Panel_Right:getChildByName("Button_JCZL")
    Button_JCZL:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:showDlg("favor/AdditionPopScene")
        end
    end
    )
    local contentSize =cc.size(90,104)
    local node=self:createNode("Node/Node_Favor_Icon.csb")
    local item = node:getChildByName("Panel_Zhangbei")
    self.space = 0
    local num = 10
    scrollView:setContentSize(cc.size(360, 125))
    local sSize=cc.size(math.max(scrollView:getContentSize().width,(contentSize.width+self.space)*num),scrollView:getInnerContainerSize().height);
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
        local y=sSize.height-contentSize.height/2
        local x=sSize.width-(contentSize.width+self.space)*math.floor(index)-contentSize.width/2
        newItem:setPosition(cc.p(x,y))
    end
    local pageView = Panel_Right:getChildByName("PageView_1")
    local Panel_role = Panel_Right:getChildByName("Panel_role")
    pageView:setContentSize(cc.size(Panel_role:getContentSize().width+200,Panel_role:getContentSize().height+100))
    Panel_role:setVisible(false)
    pageView:setTouchEnabled(true)
    pageView:setCustomScrollThreshold(20)
    --pageView:setBounceable(true)
    for i=1,5 do
        local item = Panel_role:clone()
        item:setVisible(true)
        pageView:addPage(item)
    end
    local function pageViewEvent(sender, eventType)
    --ccui.PageViewEventType.turning
       -- cclog(eventType)
        if eventType == 0 then
            local pageView = sender
            local pageInfo = string.format("page %d " , pageView:getCurPageIndex() + 1)
            self.pageIndex = pageView:getCurPageIndex() + 1
            cclog(pageInfo)
            --self._displayValueLabel:setString(pageInfo)
        end
    end 

    pageView:addEventListener(pageViewEvent)
    local function switchPage(tag)
        
        local page = self.pageIndex+tag
        page =math.min(math.max(1,page),5)
        if page ~=self.pageIndex then
            self.pageIndex = page
            pageView:scrollToPage(self.pageIndex-1)
        end
    end
    local Button_Arrow_Left = Panel_Right:getChildByName("Button_Arrow_Left")
    local Button_Arrow_Right = Panel_Right:getChildByName("Button_Arrow_Right")
    Button_Arrow_Left:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            switchPage(-1)
        end
    end
    )
    
    Button_Arrow_Right:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            switchPage(1)
        end
        
    end
    )
    
    local Image_Btn_QMCJ = Panel_Right:getChildByName("Image_Btn_QMCJ")
    local Image_Btn_QHWJ = Panel_Right:getChildByName("Image_Btn_QHWJ")
    Image_Btn_QMCJ:setTouchEnabled(true)
    Image_Btn_QHWJ:setTouchEnabled(true)

    Image_Btn_QMCJ:addTouchEventListener(function(sender,eventType)--Achievement
        if eventType ==0 then
            sender:stopAllActions();
            sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
        elseif eventType ==2 then
        self:showDlg("favor/AchievementPopScene")
        end

    end
    )
    
    Image_Btn_QHWJ:addTouchEventListener(function(sender,eventType)
        if eventType ==0 then
            sender:stopAllActions();
            sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
        elseif eventType ==2 then
            self:showDlg("favor/SelectHeroPopScene",{type=1})
        end

    end
    )
end

function FavorabilityScene:dlgCallBack(data)
    
end


function FavorabilityScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        if (sender:getTag()==140) then
            self:sceneNext()
            _REQUIRE("battle/ChapterSceneData").type=1
            --self:getActionNode():gotoFrameAndPlay(self:getActionNode():getCurrentFrame(),false)
        elseif (sender:getTag()==142) then

        end 
    end
end
return FavorabilityScene
