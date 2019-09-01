
local MopResultScene = class("MopResultScene", cc.load("mvc").ViewBase)
MopResultScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
MopResultScene.STOPFRAME = 100
MopResultScene.RESOURCE_FILENAME = "Scene/Mop-UpResultsScene.csb"
function MopResultScene:onCreate()
    --    release_print("========= WinPopupScene create");
    local data = self.args[1]
    local PanelMopUpResults=self:getResourceNode():getChildByName("Panel_Mop-UpResults")
    PanelMopUpResults:setTouchEnabled(true)
    local Panel_Black_Bg=PanelMopUpResults:getChildByName("Panel_Black_Bg")
    local Panel_Bg_Base=PanelMopUpResults:getChildByName("Panel_Bg_Base")
    local Panel_Center_Base=PanelMopUpResults:getChildByName("Panel_Center_Base")
    local Panel_Bottom_Base=PanelMopUpResults:getChildByName("Panel_Bottom_Base")
    local Panel_Bottom=Panel_Bottom_Base:getChildByName("Panel_Bottom")
    Panel_Bottom:setTouchEnabled(false)
    local Panel_Item=Panel_Bottom_Base:getChildByName("Panel_Item")
    local scrollView=Panel_Item:getChildByName("ScrollView_1")
    local Image_Bg=Panel_Center_Base:getChildByName("Image_Bg")
    local Text_Level=Image_Bg:getChildByName("Text_Level")
    local Text_Exp=Image_Bg:getChildByName("Text_Exp")
    local Text_Coins=Image_Bg:getChildByName("Text_Coins")
    Text_Level:setString(LANG("LV:%s",model_user.level))
    Text_Exp:setString(LANG("exp+%s",data.exp))
    Text_Coins:setString(LANG("coins+%s",data.coin))
    local panel_1 = scrollView:getChildByName("Panel_1")
    
    --local item =  node:getChildByName("Panel_Object")
    local item =  panel_1
    item:setTouchEnabled(true)
    local contentSize =item:getContentSize()
    item:setVisible(false)
    local moppingTreasureList =data.moppingTreasureList
--    for key, value in pairs(data.moppingTreasureList) do
--        for key1, value1 in pairs(value.moppingTreasures.treasures) do
--            moppingTreasureList[#moppingTreasureList + 1] = value1
--        end
--    end
    local num = #moppingTreasureList
    --local num = 10
    
    local col =5
    self.space = 0
    --scrollView:scrollToPercentVertical(0,.1,false)
    
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*num));
    cclog("sssss"..sSize.width)
    local percent =100/(num-2)
    scrollView:setInnerContainerSize(sSize)
    local Button_Next=Panel_Bottom:getChildByName("Button_Next")
    Button_Next:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:showDlg("battle/SpotPopupScene",model_user.selectStageId)
            self:removeDlgNoAction()
        end
    end)
    local pSequence
    local arr ={}
    local count = 0
    scrollView:setTouchEnabled(false)
    local btnDisplayTime = 0
    for i=1,num do
        local newItem = item:clone()
       
        --newItem:setAnchorPoint(cc.p(0,0))
        newItem:setVisible(true)
        newItem:setTouchEnabled(false);
        local index = i-1
        newItem:setTag(index);
        newItem:setOpacity(0)
        scrollView:addChild(newItem)
        --local x=(sSize.width/col)/2+sSize.width/col*(index%col);
        local x = 0
        --local y = newItem:getPositionY()
        
        local y=sSize.height-(contentSize.height+self.space)*index
--        --local y=sSize.height-(contentSize.height+self.space)*math.floor(index/col)-contentSize.height/2
        newItem:setPosition(cc.p(x,y))
       
         local function callBack()
            if (sSize.height - y + (contentSize.height+self.space)) > scrollView:getContentSize().height then
                count = count + 1
                scrollView:scrollToPercentVertical(percent*count,1,false)
            end
        end
        ----先滚动-------------------------------------------
        
        local treasures = moppingTreasureList[i].moppingTreasures.treasures
        
        ------------- panel出现消失sequence  
        local function addNode()
            local node=self:createNode("Nodes/Node_Animation_Mop-Up.csb")
            newItem:addChild(node)
            local Panel_1 = node:getChildByName("Panel_1")
            Panel_1:setTouchEnabled(false)
            local Text_1 = Panel_1:getChildByName("Text_1")
            Text_1:setString(LANG("第%s次扫荡",i))
            for k = 1, col do
                local projectNode = Panel_1:getChildByName("ProjectNode_"..k)
                local Panel_Object = projectNode:getChildByName("Panel_Object")
                projectNode:setVisible(k <= #treasures)
                Panel_Object:setVisible(k <= #treasures)
                if k <= #treasures then
                    self:initItemCacha(Panel_Object,{id = treasures[k].value,type = treasures[k].type,num = treasures[k].content})
                    Panel_Object:setTouchEnabled(true);
                    Panel_Object:addTouchEventListener(function(sender,eventType)
                        if eventType ==2 then
                            self:showDlg("common/PropsPopScene", {id = treasures[k]["value"], type = treasures[k]["type"]})
                        end
                    end
                    )
                end
                
            end
            node:setPosition(cc.p(239,94))
        end
        
        local function scrollPanel()
            local arr = {}
            arr[#arr+1] = cc.CallFunc:create(callBack)
            arr[#arr+1] = cc.DelayTime:create(1*count)
            pSequence = cc.Sequence:create(arr)
            scrollView:runAction(cc.Sequence:create(pSequence))
            scrollView:setTouchEnabled(i == num)
        end
        local panelArr ={}
        if i ~= 1 then
            panelArr[#panelArr+1] = cc.DelayTime:create(1.5*index)--时间延迟
        end
        
        panelArr[#panelArr+1] = cc.FadeIn:create(1) --先淡入
        panelArr[#panelArr+1] = cc.CallFunc:create(addNode)--增加进来
        panelArr[#panelArr+1] = cc.CallFunc:create(scrollPanel)
        
        local qunce = cc.Sequence:create(panelArr)
        newItem:runAction(qunce)
    end
    btnDisplayTime =   1.5*num + 1
    Timer:runOnce(function()
        self:getActionNode():gotoFrameAndPlay(300,320,false)
        end,btnDisplayTime)
end

function MopResultScene:initUi()
    local panel_Bottom=self:getResourceNode():getChildByName("Panel_Bottom")
    for i=1, 5 do
        local item=panel_Bottom:getChildByName("Node_Item1_"..i)

    end
end

function MopResultScene:frameStopCallBack()
    --玩家等级升级弹窗
    if model_user.preMonsterLevel < model_user.level then
        self:showDlg("battle/LevelUpPopup")
    end
end

return MopResultScene
