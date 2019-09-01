local MailViewPop = class("MailViewPop", cc.load("mvc").ViewBase)
MailViewPop.RESOURCE_FILENAME = "Scene/MailContentPopup.csb"
MailViewPop.STOPFRAME = 45 --停在哪一帧
MailViewPop.ENDFRAME = 105
local dateHelper = _REQUIRE("common/dateHelper.lua")
_REQUIRE("pb.pbCommon")
function MailViewPop:onCreate()
    local MailContentPopup = self:getResourceNode():getChildByName("MailContentPopup")   
    local Panel_B = MailContentPopup:getChildByName("Panel_B") 
    local Panel_C = MailContentPopup:getChildByName("Panel_C") 
    MailContentPopup:setTouchEnabled(true)
    SCREEN_SCALE_BG(Panel_B)
    SCREEN_CENTER(Panel_C)
    local Panel_Center = Panel_C:getChildByName("Panel_Center")
    local Button_1 = Panel_Center:getChildByName("Button_1")
    local Button_2 = Panel_Center:getChildByName("Button_2")
    local Panel_Content = Panel_Center:getChildByName("Panel_Content")
    local Panel_Name = Panel_Center:getChildByName("Panel_Name")
    local ScrollView_1 = Panel_Content:getChildByName("ScrollView_1")
    local Text_Content1 = ScrollView_1:getChildByName("Text_Content1")
    local Panel_Cut = ScrollView_1:getChildByName("Panel_Cut")
    local Panel_Attachment1 = ScrollView_1:getChildByName("Panel_Attachment1")
    local Panel_Attachment2 = ScrollView_1:getChildByName("Panel_Attachment2")
    local Text_Num1 = Panel_Attachment1:getChildByName("Text_Num1")
    local Text_Num2 = Panel_Attachment2:getChildByName("Text_Num2")
    local innerHeight = 0
    
    local Text_Name1 = Panel_Name:getChildByName("Text_Name1")
    local Text_Name2 = Panel_Name:getChildByName("Text_Name2")
    local Text_Name3 = Panel_Name:getChildByName("Text_Name3")
    Text_Name3:hide()
    
    local data = self.args[1]
    local channel = self.args[2]
    self.dlgCallBack = self.args[3]
    
    Text_Name1:setString(LANG("发件人：%s",data.reason.from))
    local  dateStr = dateHelper:getDataStringFromeTimestamp(data.depositedAt)
    Text_Name2:setString(dateStr)
    local function receveMail(type)
        local key = data.key;
        if data.status == 1 then
--            self.dlgCallBack()
--            self:removeDlgNoAction()
            self:sceneBack(function()
                self.dlgCallBack()
                self:removeDlgNoAction()
            end,80,MailViewPop.ENDFRAME)
            return
        end
        ctlDepositReceiveRequest:sendRequest(function(sucess)
            if sucess then
                if type == 2 then
                	PopMsg.getInstance():flashShow(LANG("领取成功!"))
                end
                self:sceneBack(function()
                    self.dlgCallBack()
                    self:removeDlgNoAction()
                end,80,MailViewPop.ENDFRAME)
            end
        end, key, channel);
    end
    self:addBtnTouchEventListener(Button_1,function(sender, eventType)
        if eventType == 2 then
            receveMail(1)
        end
    end
    )
    Button_2:setVisible(data.actionType ~= 1 and data.status == 0)
    self:addBtnTouchEventListener(Button_2,function(sender, eventType)
        if eventType == 2 then
            receveMail(2)
        end
    end
    )
    local innerHeight = 0
    local contentWidth = 90
    local contentHeight = 100
    Text_Content1:setString(data.reason.content)
    --Text_Content1:setString("短发短发饿fads非发的fads发的发地方阿萨德飞发生的fad萨德发的身份大事发的身份阿")
    local strLen = Text_Content1:getStringLength()
    local size = Text_Content1:getVirtualRendererSize()
    local txtDesWidth = 594
    local txtHeight = math.ceil(size.width/(txtDesWidth)) * 26
    if txtHeight > 4*26 then
        Text_Content1:setContentSize(cc.size(txtDesWidth, txtHeight))
    end
    self.size1 = math.max(txtHeight,104)
    self.size2 = Panel_Cut:getContentSize().height
    self.size3 = Panel_Attachment1:getContentSize().height
    local prizes = data.prizeInfo
        
    local prizeRow = math.ceil(#prizes / 5)
    self.size4 = prizeRow * contentHeight
    
    for i=1,4 do
        innerHeight = innerHeight + self["size"..i]
    end
    local sSize=cc.size(ScrollView_1:getInnerContainerSize().width,math.max(ScrollView_1:getContentSize().height,innerHeight));
    local currentY = sSize.height
    
    ScrollView_1:setInnerContainerSize(sSize)
    Text_Content1:setPositionY(currentY)
    
    Panel_Cut:setPositionY(Text_Content1:getPositionY() - self.size1-  (self.size2 / 2))

    
    local coin = self:getCoin(prizes)
    local cash = self:getCash(prizes)
    Panel_Attachment1:setVisible(cash ~= 0)
    Panel_Attachment2:setVisible(coin ~= 0)
    Panel_Attachment1:setPosition(cc.p(0,Panel_Cut:getPositionY() - self.size2 - (self.size3 / 2)))
    Panel_Attachment2:setPosition(cc.p(284,Panel_Cut:getPositionY() - self.size2 - (self.size3 / 2)))
    if cash == 0 and coin ~= 0 then
        Panel_Attachment2:setPosition(cc.p(0,Panel_Cut:getPositionY() - self.size2 - (self.size3 / 2)))
    end
    local Text_Num1= Panel_Attachment1:getChildByName("Text_Num1")
    local Text_Num2= Panel_Attachment2:getChildByName("Text_Num2")
    Text_Num1:setString("X"..cash)
    Text_Num2:setString("X"..coin)
    
    local isCoin = (cash ~=0 or coin ~=0)
    local count = 0
    local col = 5
    local totalPrizeItemCount = 0
    local space = 10
    for i=1,#prizes do
        if prizes[i].type == pbCommon.ObjectType.coin or prizes[i].type == pbCommon.ObjectType.cash then
            return
        end
        totalPrizeItemCount = totalPrizeItemCount + 1
    end
    
    local startX = 0
    
    col = math.min(col,totalPrizeItemCount)
    startX = (sSize.width - (contentWidth + space) * col)/2
    
    for i=1,#prizes do
        local prize = prizes[i]
        if prize.type == pbCommon.ObjectType.coin or prize.type == pbCommon.ObjectType.cash then
        	return
        end
        
        count = count + 1
        local itemNode = self:createNode("Node/Node_item_cacha.csb")
        local Panel_Object = itemNode:getChildByName("Panel_Object")
        
        self:initItemCacha(Panel_Object,{id = prize.id, type = prize.type , num = prize.content})
        ScrollView_1:addChild(itemNode)
        
        local x = startX + (contentWidth + space)/2 + ((count-1) % col) * (contentWidth + space)
        cclog("x:"..x)
        local y
        if isCoin then
            y = Panel_Attachment1:getPositionY() - self.size3 - (contentHeight / 2) - math.floor((count-1)/col)*contentHeight + 20
        else
            y = Panel_Cut:getPositionY() - self.size2 - (contentHeight / 2) - math.floor((count-1)/col)*contentHeight + 20
        end
        itemNode:setPosition(cc.p(x,y))
    end
    
    
end


return MailViewPop
