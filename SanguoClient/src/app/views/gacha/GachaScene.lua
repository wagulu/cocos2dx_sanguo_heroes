
local GachaScene = class("GachaScene", cc.load("mvc").ViewBase)
GachaScene.RESOURCE_FILENAME = "Scene/GachaScene.csb"
local model_gachaStorage = _REQUIRE("model/model_gachaStorage")
local model_itemStorage = _REQUIRE("model/model_itemStorage")
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE "model/model_monsterStorage.lua"
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
local static_model_gachaToken = _REQUIRE "model/static/model_gachaToken.lua"
local dateHelper = _REQUIRE("common/dateHelper.lua")
GachaScene.STOPFRAME = 40
function GachaScene:onCreate()
    local GachaScene = self:getResourceNode():getChildByName("PubScene");
    local Panel_B = GachaScene:getChildByName("Panel_B")
    self.Panel_Bg = Panel_B:getChildByName("Panel_Bg")
    
    --local Panel_Center = GachaScene:getChildByName("Panel_Center")
    
    local Panel_T = GachaScene:getChildByName("Panel_T")
    self.Panel_Top = Panel_T:getChildByName("Panel_TopBack")
    
    --local Panel_C = GachaScene:getChildByName("Panel_C")
    self.Panel_Center = GachaScene:getChildByName("Panel_Center")
    SCREEN_CENTER(GachaScene)
    --SCREEN_SCALE_BG(self.Panel_Bg)
    SCREEN_TOP(Panel_T)
    local Button_Back = self.Panel_Top:getChildByName("Button_Back")
    self.tag =0
    self:addBtnTouchEventListener(Button_Back,function(sender,eventType)
        if eventType==2 then
            if self.tag ==1 then
                self:playStart()
                self.tag =0
            else
                SWITSCENE("home/MainCityScene")
            end
            
        end
    end)
    local Button_Shop = self.Panel_Center:getChildByName("Button_Shop")
    Button_Shop:setVisible(false)
    
    local Button_Shop_VIP = self.Panel_Center:getChildByName("Button_Shop_VIP")
    Button_Shop_VIP:setVisible(false)
    
--    local Panel_TopBack = self.Panel_Center:getChildByName("Panel_TopBack")
--    local Button_Back_9 = Panel_TopBack:getChildByName("Button_Back_9")
--    self:addBtnTouchEventListener(Button_Back_9,function(sender,eventType)
--        if eventType==2 then
--            self.actionNode_:gotoFrameAndPlay(0,false)
--        end
--    end)
    self.isCommon =true
    self:initUi()
end


function GachaScene:initUi()
    local Panel_L = self.Panel_Center:getChildByName("Panel_L")
    local Panel_R = self.Panel_Center:getChildByName("Panel_R")
     
    local Text_2_1 = Panel_L:getChildByName("Text_2_1")
    local Text_3_1 = Panel_L:getChildByName("Text_3_1")
    local Text_4_1 = Panel_L:getChildByName("Text_4_1")
    local Text_5_1 = Panel_R:getChildByName("Text_5_1")
    local Text_6_1 = Panel_R:getChildByName("Text_6_1")
    local Button_1 = Panel_L:getChildByName("Button_1")
    local Button_2 = Panel_L:getChildByName("Button_2")
    local Button_3 = Panel_R:getChildByName("Button_3")
    local Button_4 = Panel_R:getChildByName("Button_4")
    local Text_2_2 = Panel_R:getChildByName("Text_2_2")
    local Panel_integral = self.Panel_Center:getChildByName("Panel_integral")
    local Text_Integral = Panel_integral:getChildByName("Text_Integral")
    Text_Integral:setString(LANG("当前积分：%s",model_user.gacha_token))
    local buyTimes1 = model_gachaStorage:getTotalBuyTimesWithType(pbCommon.ObjectType.Item_NormalGachaTicket)
    Text_3_1:setString(LANG("可获得%s积分",static_model_gachaToken:getNormal(buyTimes1+1)))
    
    local buyTimes2 = model_gachaStorage:getTotalBuyTimesWithType(pbCommon.ObjectType.Item_NormalGachaTicket)
    Text_4_1:setString(LANG("可获得%s积分",static_model_gachaToken:getTenNormal(buyTimes2+1)))
    
    local buyTimes3 = model_gachaStorage:getTotalBuyTimesWithType(pbCommon.ObjectType.cash)
    Text_5_1:setString(LANG("可获得%s积分",static_model_gachaToken:getRare(buyTimes3+1)))
    
    local buyTimes4 = model_gachaStorage:getTotalBuyTimesWithType(pbCommon.ObjectType.cash)
    Text_6_1:setString(LANG("可获得%s积分",static_model_gachaToken:getTenRare(buyTimes4+1)))
    local Image_Icon1 = Panel_L:getChildByName("Image_Icon1")
    local Image_Icon5 = Panel_R:getChildByName("Image_Icon5")
    Image_Icon1:setTouchEnabled(true)
    Image_Icon5:setTouchEnabled(true)
    self:addBtnTouchEventListener(Image_Icon1,function(sender,eventType)
        if eventType ==0 then
            sender:stopAllActions();
            sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) );
        elseif eventType==2 then
            SWITSCENE("gacha/GachaViewListScene",1)
        end
    end)
    
    self:addBtnTouchEventListener(Image_Icon5,function(sender,eventType)
        if eventType==0 then
            sender:stopAllActions();
            sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) );
        elseif eventType==2 then
            SWITSCENE("gacha/GachaViewListScene",2)
        end
    end)
    
    local function playOneAnimation(prizeItem)
        --self.actionNode_:gotoFrameAndPlay(80,453,false) --获得武将转碎片
        if prizeItem["type"] == pbCommon.ObjectType.Monster then
            if prizeItem["toParts"] and prizeItem["toParts"].partId then
                self.actionNode_:gotoFrameAndPlay(80,453,false) --获得武将转碎片
            else
                self.actionNode_:gotoFrameAndPlay(751,1113,false) --获得武将
            end
        else
            self.actionNode_:gotoFrameAndPlay(1114,1410,false) --获得武将碎片
        end
    end
    
     local function playTenAnimation(prizeItem)
        self.actionNode_:gotoFrameAndPlay(454,748,false) --十连抽
     end
    
    local function addMonster(id,parentNode,currentNode)
        currentNode:setVisible(false)
        local imageView = parentNode:getChildByName("imageView")
        if imageView then
            imageView:removeSelf()
            imageView = nil
        end
        imageView = ccui.ImageView:create(model_monster:getHalf(id))
        imageView:setTouchEnabled(false)
        parentNode:addChild(imageView)
        imageView:setName("imageView")
        imageView:setPosition(currentNode:getPosition())
        imageView:setScale(currentNode:getScale())
        currentNode:loadTexture(model_monster:getHalf(id))
    end
    
    local function initExtractOne(prizeItem,parentNode)
        self.tag = 1
        local Panel_extractOne_role = self.Panel_Bg:getChildByName("Panel_extractOne_role")
        local Image_extractOne_role = Panel_extractOne_role:getChildByName("Image_extractOne_role")
        --local ProjectNode_extractOne_head = self.Panel_Bg:getChildByName("ProjectNode_extractOne_head")
        parentNode:removeAllChildren()
        if prizeItem["type"] == pbCommon.ObjectType.Monster then
            if prizeItem["toParts"] and prizeItem["toParts"].partId ~=0 then --武将转碎片
                local node,nodeAction=self:createNode("Node/Node_item_cacha.csb")
                parentNode:addChild(node)
                local Panel_Object = node:getChildByName("Panel_Object")
                self:initItemCacha(Panel_Object,{type = pbCommon.ObjectType.MonsterPart,id = prizeItem["toParts"].partId,num = prizeItem.content})
            end
            addMonster(prizeItem.value,Panel_extractOne_role,Image_extractOne_role)
        else
            local node,nodeAction=self:createNode("Node/Node_item_cacha.csb")
            parentNode:addChild(node)
            local Panel_Object = node:getChildByName("Panel_Object")
            self:initItemCacha(Panel_Object,{type = prizeItem["type"],id = prizeItem.value,num = prizeItem.content})
        end
    end
    
    
    
    local function initExtractTen(prizes)
        local Panel_TenHead = self.Panel_Bg:getChildByName("Panel_TenHead")
        local arrMonsterIndex = {1,5,6,10}
        local arrItemIndex = {2,3,4,7,8,9}
        local arrMonsters = {}
        local arrItems = {}
        for i=1,#prizes do
            if prizes[i]["type"] == pbCommon.ObjectType.Monster then
                arrMonsters[#arrMonsters + 1] = prizes[i]
            else
                arrItems[#arrItems + 1] = prizes[i]
            end
        end
        
        for i=1,#arrMonsters do
            local prizeItem = arrMonsters[i]
            local parentNode = Panel_TenHead:getChildByName("ProjectNode_h"..arrMonsterIndex[i])
            parentNode:removeAllChildren()
            if prizeItem["toParts"] and prizeItem["toParts"].partId ~=0 then
                local node,nodeAction=self:createNode("Node/Node_RisingStar_RoleToHead.csb",false)
                parentNode:addChild(node)
                nodeAction:gotoFrameAndPlay(0,0,false)
                Timer:runOnce(function()
                    nodeAction:gotoFrameAndPlay(0,false)
                end,4)
                local Panel_All = node:getChildByName("Panel_All")
                local ProjectNode_Head = Panel_All:getChildByName("ProjectNode_Head")
                ProjectNode_Head:removeAllChildren()
                local nodeHead,nodeAction=self:createNode("Node/Node_item_cacha.csb")
                ProjectNode_Head:addChild(nodeHead)
                local Panel_Object = nodeHead:getChildByName("Panel_Object")
                self:initItemCacha(Panel_Object,{id = prizeItem["toParts"].partId,type = pbCommon.ObjectType.MonsterPart,num = prizeItem.content})
                
                local Panel_Role = Panel_All:getChildByName("Panel_Role")
                local Image_xtract_role = Panel_Role:getChildByName("Image_xtract_role")
                addMonster(prizeItem.value,Panel_Role,Image_xtract_role)
            else
                local node,nodeAction=self:createNode("Node/Node_RisingStar_Role.csb")
                parentNode:addChild(node)
                local Panel_Role =node:getChildByName("Panel_Role") 
                local Image_xtract_role = Panel_Role:getChildByName("Image_xtract_role")
                addMonster(prizeItem.value,Panel_Role,Image_xtract_role)
            end
        end
        
        for i=1,#arrItems do --先放道具槽里面
            if i<= #arrItemIndex then
                local node = Panel_TenHead:getChildByName("ProjectNode_h"..arrItemIndex[i])
                initExtractOne(arrItems[i],node)
            end
        end
        if #arrItemIndex < #arrItems then --道具放不完，在放武将对应的位置
            local count = #arrItems - #arrItemIndex
            local startIndex = #arrMonsters
        	for i = 1,count do
                local node = Panel_TenHead:getChildByName("ProjectNode_h"..arrMonsterIndex[startIndex + i])
                initExtractOne(arrItems[#arrItemIndex + 1],node)
        	end
        end
    end
    
    self:addBtnTouchEventListener(Button_1,function(sender,eventType)
        if eventType==2 then
        
        
        
--            self.isCommon =true
--            initExtractOne({type = "Monster",toParts ={partId = 0},content = 1,value = 2},self.Panel_Bg:getChildByName("ProjectNode_extractOne_head"))
--            self:initUi()
--            playOneAnimation({type = "Monster",toParts ={partId = 2},content = 1,value = 2})
--        
--            if(true) then
--                return
--            end
            ctlGachaRollGachaRequest:sendRequest(function(sucess,data)
                if sucess then
                    if #data.prizes ~= 0 then
                        self.isCommon =true
                        initExtractOne(data.prizes[1],self.Panel_Bg:getChildByName("ProjectNode_extractOne_head"))
                        self:initUi()
                        playOneAnimation(data.prizes[1])
--                        self.actionNode_:gotoFrameAndPlay(80,445,false)
                    end
                end
                
                
            end, 1);
            
        end
    end)
    self:addBtnTouchEventListener(Button_2,function(sender,eventType)
        if eventType==2 then
            ctlGachaRollGachaRequest:sendRequest(function(sucess,data)
                if sucess then
                    self.isCommon =true
                    initExtractTen(data.prizes)
                    self:initUi()
                    playTenAnimation()                   
                    --self.actionNode_:gotoFrameAndPlay(445,755,false)
                end
                    
            end, 2);
            
        end
    end)
    
    self:addBtnTouchEventListener(Button_3,function(sender,eventType)
        if eventType==2 then
            
            ctlGachaRollGachaRequest:sendRequest(function(sucess,data) 
                if sucess then
                    if #data.prizes ~= 0 then
                        self.isCommon =false
                        initExtractOne(data.prizes[1],self.Panel_Bg:getChildByName("ProjectNode_extractOne_head"))
                        self:initUi()
                        playOneAnimation(data.prizes[1])
                    end
                    
                end 
                
            end, 3);

        end
    end)
    self:addBtnTouchEventListener(Button_4,function(sender,eventType)
        if eventType==2 then
            ctlGachaRollGachaRequest:sendRequest(function(sucess,data)
                if sucess then
                    self.isCommon =false
                    initExtractTen(data.prizes)
                    self:initUi()
                    playTenAnimation() 
                end
                
            end, 4);

        end
    end)
    local Text_Scores_1 = Panel_L:getChildByName("Text_Scores_1")
    local Text_Scores_2_39 = Text_Scores_1:getChildByName("Text_Scores_2_39")
    local freeTimes = model_gachaStorage:getUsedFreeBuyTimes(1)
    local config1 = model_gachaStorage:getConfig(1)
    local freeTime = config1.freeTimes - freeTimes
    
    local nextResetNormalTimeStamp = model_gachaStorage:getResetTimestamp(1)
    local resetNormalTime =  nextResetNormalTimeStamp- model_user:getCurrentServerTimestamp()
    local function initResetTime(time)
        local  day, hour, minute, second = dateHelper:convertSecondToDayHourMinuteSecond(time)
        Text_2_1:setString(LANG("%s:%s:%s后免费", day*24+hour, minute, second))
    end
    if resetNormalTime >0 then
        initResetTime(resetNormalTime)
    else
        Text_2_1:setString(LANG("今日免费次数:%s",freeTime))
    end
    
    Text_2_1:setString(LANG("今日免费次数:%s",freeTime))
    if self.normalTimeId == nil and resetNormalTime > 0 then
        self.normalTimeId = Timer:start(function()
            local time = nextResetNormalTimeStamp- model_user:getCurrentServerTimestamp()
            if time > 0 then
                initResetTime(time)
            else
                Timer:kill(self.normalTimeId) 
                self.normalTimeId = nil
                Text_2_1:setString(LANG("今日免费次数:%s",freeTime))
            end
        end,1)
    end
    
    local haveCount = model_itemStorage:getCount(pbCommon.NORMALGACHATICKET_ID,pbCommon.ObjectType.Item_NormalGachaTicket)
    
    if (resetNormalTime > 0 or freeTime <= 0)then
        Text_Scores_1:setString(LANG("(%s/%s)",haveCount,config1.requiredPoint))
        Text_Scores_2_39:setString(haveCount)
        Text_Scores_2_39:setVisible(haveCount < config1.requiredPoint)
    else

        Text_Scores_1:setString(LANG("免费"))
        Text_Scores_2_39:setVisible(false)
    end
    
    
    local config2 = model_gachaStorage:getConfig(2)
    local Text_Scores_3 = Panel_L:getChildByName("Text_Scores_3")
    local Text_Scores_4_43 = Text_Scores_3:getChildByName("Text_Scores_4_43")
    Text_Scores_3:setString(LANG("(%s/%s)",haveCount,config2.requiredPoint))
    Text_Scores_4_43:setString(haveCount)
    Text_Scores_4_43:setVisible(haveCount < config2.requiredPoint)
    
    local config3 = model_gachaStorage:getConfig(3)
    local canFreeBuy = model_gachaStorage:canFreeBuy(3)
    local Text_Scores_5 = Panel_R:getChildByName("Text_Scores_5")
    local Text_Scores_5_R = Panel_R:getChildByName("Text_Scores_5_R")
    Text_Scores_5_R:setVisible(false)
    if canFreeBuy then
        Text_Scores_5:setString(LANG("免费"))
    else
        Text_Scores_5:setString(config3.requiredPoint)	
    end
    local nextResetTimeStamp = model_gachaStorage:getResetTimestamp(3)
    local resetTime =  nextResetTimeStamp- model_user:getCurrentServerTimestamp()
    if resetTime <=0 then
        Text_2_2:setString(LANG("免费"))
    end
    if self.timeId == nil and resetTime > 0 then
    	self.timeId = Timer:start(function()
            local time = nextResetTimeStamp- model_user:getCurrentServerTimestamp()
            if time > 0 then
                local  day, hour, minute, second = dateHelper:convertSecondToDayHourMinuteSecond(time)
                Text_2_2:setString(LANG("%s:%s:%s后免费", day*24+hour, minute, second))
            else
                Timer:kill(self.timeId)	
                self.timeId = nil
            end
    	end,1)
    end
    
    local config4 = model_gachaStorage:getConfig(4)
    local Text_Scores_6 = Panel_R:getChildByName("Text_Scores_6")
    local Text_Scores_6_R = Panel_R:getChildByName("Text_Scores_6_R")
    Text_Scores_6_R:setVisible(false)
    Text_Scores_6:setString(config4.requiredPoint)
    local topBar = self.Panel_Top:getChildByName("Node_Top")
    self:initTopBar(topBar)
    
    local Panel_ButtonR1 = self.Panel_Center:getChildByName("Panel_ButtonR1")
    local Panel_ButtonR2 = self.Panel_Center:getChildByName("Panel_ButtonR2")
    local Text_Scores1_12 = Panel_ButtonR1:getChildByName("Text_Scores1_12")
    local Text_Scores1_1_11 = Text_Scores1_12:getChildByName("Text_Scores1_1_11")
    local Button_1_11 = Panel_ButtonR1:getChildByName("Button_1_11")
    local Image_Icon1_54 = Panel_ButtonR1:getChildByName("Image_Icon1_54")
    local Text_1_14 = Panel_ButtonR1:getChildByName("Text_1_14")
    
    
    local Text_Scores2_18 = Panel_ButtonR2:getChildByName("Text_Scores2_18")
    local Text_Scores2_1_17 = Text_Scores2_18:getChildByName("Text_Scores2_1_17")
    local Button_2_13 = Panel_ButtonR2:getChildByName("Button_2_13")
    Button_2_13:setTitleText(LANG("点将10次"))
    local Image_Icon2_58 = Panel_ButtonR2:getChildByName("Image_Icon2_58")
    local Text_2_20 = Panel_ButtonR2:getChildByName("Text_2_20")
    
    if self.isCommon == true then
        Image_Icon1_54:loadTexture("Assets/GachaScene/gacha_icon1.png")
        Image_Icon2_58:loadTexture("Assets/GachaScene/gacha_icon1.png")
        
        Text_Scores1_12:setString(LANG("(%s/%s)",haveCount,config1.requiredPoint))
        Text_Scores1_1_11:setString(haveCount)
        Text_Scores1_1_11:setVisible(haveCount < config1.requiredPoint)
        self:addBtnTouchEventListener(Button_1_11,function(sender,eventType)
            if eventType==2 then
                ctlGachaRollGachaRequest:sendRequest(function(sucess,data)
                    if sucess then
                        if #data.prizes ~= 0 then
                            self.isCommon =true
                            initExtractOne(data.prizes[1],self.Panel_Bg:getChildByName("ProjectNode_extractOne_head"))
                            self:initUi()
                            playOneAnimation(data.prizes[1])
                        end
                    end
                end, 1);

            end
        end)
        --local buyTimes1 = model_gachaStorage:getTotalBuyTimes(1)
        --local buyTimes2 = model_gachaStorage:getTotalBuyTimes(2)
        Text_1_14:setString(LANG("可获得%s积分",static_model_gachaToken:getNormal(buyTimes1+1)))
        Text_2_20:setString(LANG("可获得%s积分",static_model_gachaToken:getTenNormal(buyTimes2+1)))
        
        Text_Scores2_18:setString(LANG("(%s/%s)",haveCount,config2.requiredPoint))
        Text_Scores2_1_17:setString(haveCount)
        Text_Scores2_1_17:setVisible(haveCount < config2.requiredPoint)
        self:addBtnTouchEventListener(Button_2_13,function(sender,eventType)
            if eventType==2 then
                ctlGachaRollGachaRequest:sendRequest(function(sucess,data)
                    if sucess then
                        self.isCommon =true
                        initExtractTen(data.prizes)
                        self:initUi()                   
                        playTenAnimation() 
                    end

                end, 2);

            end
        end)
    else
        Image_Icon1_54:loadTexture("Share/icon/Icon_Gold.png")
        Image_Icon2_58:loadTexture("Share/icon/Icon_Gold.png")
        
        
        Text_Scores1_1_11:setVisible(false)
        Text_Scores2_1_17:setVisible(false)
        if canFreeBuy then
            Text_Scores1_12:setString(LANG("免费"))
        else
            Text_Scores1_12:setString(config3.requiredPoint)  
        end
        Text_Scores2_18:setString(config4.requiredPoint)
        --local buyTimes3 = model_gachaStorage:getTotalBuyTimes(3)
        --local buyTimes4 = model_gachaStorage:getTotalBuyTimes(4)
        Text_1_14:setString(LANG("可获得%s积分",static_model_gachaToken:getRare(buyTimes3+1)))
        Text_2_20:setString(LANG("可获得%s积分",static_model_gachaToken:getTenRare(buyTimes4+1)))
        self:addBtnTouchEventListener(Button_1_11,function(sender,eventType)
            if eventType==2 then

                ctlGachaRollGachaRequest:sendRequest(function(sucess,data) 
                    if sucess then
                        if #data.prizes ~= 0 then
                            self.isCommon =false
                            initExtractOne(data.prizes[1],self.Panel_Bg:getChildByName("ProjectNode_extractOne_head"))
                            self:initUi()
                            playOneAnimation(data.prizes[1])
                        end

                    end 

                end, 3);

            end
        end)
        self:addBtnTouchEventListener(Button_2_13,function(sender,eventType)
            if eventType==2 then
                ctlGachaRollGachaRequest:sendRequest(function(sucess,data)
                    if sucess then
                        self.isCommon =false
                        initExtractTen(data.prizes)
                        self:initUi()
                        playTenAnimation() 
                    end

                end, 4);

            end
        end)
    end
end


function GachaScene:onExit()
    if self.timeId ~= nil then
        Timer:kill(self.timeId)
    end
    if self.normalTimeId ~= nil then
        Timer:kill(self.normalTimeId)
    end
    
end

return GachaScene
