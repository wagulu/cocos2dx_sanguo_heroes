local MailScene = class("MailScene", cc.load("mvc").ViewBase)
MailScene.RESOURCE_FILENAME = "Scene/ArenaRankPop.csb"
local model_mailStorage = _REQUIRE("model/model_mailStorage.lua")
local dateHelper = _REQUIRE("common/dateHelper.lua")
MailScene.STOPFRAME = 31 --停在哪一帧
MailScene.ENDFRAME = 101 --结束在哪一帧
function MailScene:onCreate()
    local MailPanel = self:getResourceNode():getChildByName("ArenaRankPop")
    local Panel_Bg = MailPanel:getChildByName("Panel_Bg")    
    local Panel_C = MailPanel:getChildByName("Panel_C") 
    local Panel_Top = MailPanel:getChildByName("Panel_Top") 
   
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_TOP(Panel_Top)
    SCREEN_CENTER(MailPanel)
    self.Panel_Center = Panel_C:getChildByName("Panel_Center")
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    self:addBtnTouchEventListener(Button_Back,function(sender, eventType)
        if eventType == 2 then
            self:sceneBack(function()
                SWITSCENE("home/MainCityScene.lua") 
            end,true)
            
        end
    end
    )
    self.scrollView = self.Panel_Center:getChildByName("HeroScrollView_1")
    local node = self:createNode("Node/Node_Mail_Item.csb")
    local panelItem  = node:getChildByName("Panel_Item_3")
    self.item = panelItem
    self.item:setTouchEnabled(true)
    self.contentSize =self.item:getContentSize()
    self.buttons = {}
    local tabNames ={"系统"}
    local Button_3 = self.Panel_Center:getChildByName("Button_3")
    local Text_2 = self.Panel_Center:getChildByName("Text_2")
    Button_3:setVisible(false)
    Text_2:setVisible(false)
    local  Text_1=self.Panel_Center:getChildByName("Text_1")
    Text_1:setString("邮件")
    for i=1,4 do
        local ProjectNode =self.Panel_Center:getChildByName("ProjectNode_"..i)
        ProjectNode:setVisible(false)
        if i<= #tabNames then
            ProjectNode:setVisible(true)
            local btn = ProjectNode:getChildByName("Panel_tab"):getChildByName("Button_Select_1")
            self.buttons[i] = btn
            btn:setTitleText(tabNames[i])
            btn:addTouchEventListener(function(sender,eventType)
                if eventType==2 then
                    self:setIndex(sender:getTag())
                    if sender:getTag()==1 then
                        self:initMailItem(1)
                    elseif sender:getTag()==2 then
                        self:initMailItem(2)
                    end
                end

            end)
        end

    end
    self:setIndex(1)
    self:initMailItem(1)
end

function MailScene:initMailItem(type)
    self.type = type
    local chanel
    if type == 1 then
        chanel = DEPOSIT_CHANNEL.SYS
    else
        chanel = DEPOSIT_CHANNEL.GUILD
    end
    self.chanel = chanel
    self.scrollView:removeAllChildren()
    self.mailList = model_mailStorage:collectMail(chanel)
    table.sort(self.mailList,function(team1,team2)
        if team1.status ~= team2.status then
            return team1.status < team2.status
        else
            return team1.depositedAt > team2.depositedAt
        end
    end)
    self.space = 10
    local num = #self.mailList
    self.sSize=cc.size(self.scrollView:getInnerContainerSize().width,math.max(self.scrollView:getContentSize().height,(self.contentSize.height+self.space)*math.ceil(num)));
    --cclog("sssss"..sSize.width)
    self.scrollView:setInnerContainerSize(self.sSize)
    local count = 5
    --self:initMailItemRange(1,math.min(num,count)) 
    self:initMailItemRange(1,num) 
    if num-count>0 then
        --self.scrollView:jumpToTop()
        self:addScrollViewBar(self.scrollView, 18)
--        local timerId = Timer:runOnce(function(dt, data, timerId)
--            self:initMailItemRange(count+1,num)
--        end,0.2)
    else
        self:hideScrollViewBar(self.scrollView)   
    end
end

function MailScene:initMailItemRange(start,endIndex)
    for i=start,endIndex do
        local newItem = self.scrollView:getChildByTag(i)
        if newItem == nil then
            newItem = self.item:clone()
            newItem:setCascadeOpacityEnabled(true)
            self.scrollView:addChild(newItem)
        end
        local data = self.mailList[i]
        local index = i-1
        newItem:setTag(i);
        local x=self.contentSize.width/2+15
        --local t = math.floor(index/2)
        local y=self.sSize.height-(self.contentSize.height+self.space)*math.floor(index)-self.contentSize.height/2
        newItem:setPosition(cc.p(x,y))
        local Text_1 = newItem:getChildByName("Text_1")
        local Text_2 = newItem:getChildByName("Text_2")
        local Text_3 = newItem:getChildByName("Text_3")
        local Image_Get = newItem:getChildByName("Image_Get")
        Image_Get:setVisible(data.actionType ~= 1 and data.status == 1)
        Text_2:setColor(cc.c3b(255,206,74))
        Text_3:setColor(cc.c3b(255,206,74))
        local reason = data.reason
        Text_1:setString(reason.title)
        Text_2:setString(LANG("发件人：%s",reason.from))
        Text_3:setString(dateHelper:getTimeString(model_user:getCurrentServerTimestamp() - data.depositedAt))
        local Image_ItemLight = newItem:getChildByName("Image_ItemLight")
        Image_ItemLight:setVisible(data.status == 0)
        newItem:setTouchEnabled(true)
        newItem:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                self:showDlg("mail/MailViewPop",data,self.chanel,function()
                    self:initMailItem(self.type)
                end)
            end
        end
        )
    end
end

function MailScene:onExit()
    --self.item:release()
end

return MailScene
