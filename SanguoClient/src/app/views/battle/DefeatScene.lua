
local DefeatScene = class("DefeatScene", cc.load("mvc").ViewBase)
DefeatScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
DefeatScene.STOPFRAME = 100
DefeatScene.RESOURCE_FILENAME = "Scene/DefeatPanel.csb"
local model_user = _REQUIRE("model.model_user")
local helper_pvp = _REQUIRE("model/helper/helper_pvp.lua")
function DefeatScene:onCreate()
    self.data = self.args[1]
    self.isDraw = false --是否翻牌
--    release_print("========= WinPopupScene create");
    local Panel_DefeatPanel=self:getResourceNode():getChildByName("Panel_DefeatPanel")
    local Panel_Bg=Panel_DefeatPanel:getChildByName("Panel_Bg")
    local Panel_Top=Panel_DefeatPanel:getChildByName("Panel_Top")
    local Panel_Center_Base=Panel_DefeatPanel:getChildByName("Panel_Center_Base")
    local Panel_Center=Panel_Center_Base:getChildByName("Panel_Center")
    local Text_Level=Panel_Center:getChildByName("Text_Level")
    local Text_Exp=Panel_Center:getChildByName("Text_Exp")
    local Text_Coins=Panel_Center:getChildByName("Text_Coins")
    local Button_InfoCheck=Panel_Center:getChildByName("Button_InfoCheck")
    Text_Level:setVisible(false)
    Text_Exp:setVisible(false)
    Text_Coins:setVisible(false)
    Button_InfoCheck:setVisible(false)
    
    local Panel_Bottom_Base=Panel_DefeatPanel:getChildByName("Panel_Bottom_Base")
    self.Panel_Bottom=Panel_Bottom_Base:getChildByName("Panel_Bottom")
    local button_Retry=self.Panel_Bottom:getChildByName("Button_Retry")
    self.Button_Next=self.Panel_Bottom:getChildByName("Button_Next")
    button_Retry:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            helper_pve:confirmStage(model_user.selectStageId, function(success, data, code, msg)
                if success then
                    self:sceneBack(function()
                        SWITSCENE(DefeatScene.NEXTSCENE,1)
                    end)
                end
            end);
            
        end
    end)
    
    self.Button_Next:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:sceneBack(function()
                if model_user.isPvp then
--                    helper_pvp:getEnemy(model_user.currentBattleRank, handler(self,self.onHighladderGetEnemy));
                    ctlHighladderGetListRequest:sendRequest(handler(self,self.cutArenaPanel));
                else
                    SWITSCENE(DefeatScene.NEXTSCENE)
                end 
            end)
        end
    end)
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    
    for i = 1,5 do
        self.Panel_Bottom:getChildByName("Panel_Item1"):getChildByName("Node_Item" .. i):setVisible(model_user.isPvp == false)
    end
    button_Retry:setVisible(model_user.isPvp == false)
    self.Button_Next:setVisible(model_user.isPvp == false)
    if model_user.isPvp == true then
        for i = 1,3 do
            local ProjectNode = self.Panel_Bottom:getChildByName("Panel_Item2"):getChildByName("ProjectNode_"..i):setVisible(false)
        end
    end
    
    local Panel_Item2 = self.Panel_Bottom:getChildByName("Panel_Item2")
    Panel_Item2:setVisible(model_user.isPvp == true)

--    Panel_Win:setTouchEnabled(true)
--    local Panel_Bottom_Base=Panel_Win:getChildByName("Panel_Bottom_Base")
--    local panel_Bottom=Panel_Bottom_Base:getChildByName("Panel_Bottom")
--    local  button_Retry=panel_Bottom:getChildByName("Button_Retry")
--    button_Retry:addTouchEventListener(handler(self,self.btnBackCallback)) 

    --release_print(_REQUIRE("battle/ChapterSceneData").type);
end

function DefeatScene:initItemInfo(Panel_Object,data)
    local Image_Icon = Panel_Object:getChildByName("Image_1")
    local Image_Bg = Panel_Object:getChildByName("Image_2")
    local Text_1 = Panel_Object:getChildByName("Text_1")
    Image_Icon:loadTexture(model_static_library:getAsset(data.id,data.type))
    Image_Bg:loadTexture(model_static_library:getFrame(data.id,data.type))
end

function DefeatScene:onHighladderGetEnemy(success, data, code, msg)
    if(success) then
        SWITSCENE("battle/VersusScene",data,1)
    else
    end
end

function DefeatScene:cutArenaPanel(success, data, code, msg)
    if(success) then
        SWITSCENE("arena/ArenaScene",data)
    else
    end
end

function DefeatScene:initUi()
    local panel_Bottom=self:getResourceNode():getChildByName("Panel_Bottom")
    for i=1, 5 do
        local item=panel_Bottom:getChildByName("Node_Item1_"..i)

    end
end

function DefeatScene:frameStopCallBack()
    if model_user.isPvp then
        --翻牌特效
        local Panel_Item2 = self.Panel_Bottom:getChildByName("Panel_Item2")
        self.nodeActionArr = {}
        self.nodeArr = {}
        self.fanpaiBtns = {}
        for j = 1,3 do
            local node,nodeAction=self:createNode("Node/Node_Animation_Draw.csb")
            nodeAction:gotoFrameAndPause(0)
            node:getChildByName("Panel_Icon"):getChildByName("Button_1"):setTag(j)
            self.nodeActionArr[j] = nodeAction
            self.nodeArr[j] = node

            local ProjectNode = Panel_Item2:getChildByName("ProjectNode_"..j)
            ProjectNode:setVisible(false)
            Panel_Item2:addChild(node)
            node:setPosition(cc.p(ProjectNode:getPositionX(),ProjectNode:getPositionY()))
            local Button_1 = node:getChildByName("Panel_Icon"):getChildByName("Button_1")
            node:getChildByName("Panel_Icon"):getChildByName("Panel_Object"):setTouchEnabled(false)
            local Button_1 = node:getChildByName("Panel_Icon"):getChildByName("Button_1")
            self.fanpaiBtns[j] = Button_1
            Button_1:addTouchEventListener(function(sender,eventType)
                if eventType==2 then
                    local node0,nodeAction0=self:createNode("Node/Node_Animation_Draw_Select.csb")
                    self.fanpaiBtns[sender:getTag()]:addChild(node0)
                    nodeAction0:gotoFrameAndPlay(0,true)           
                    node0:setPosition(cc.p(self.fanpaiBtns[sender:getTag()]:getContentSize().width/2,self.fanpaiBtns[sender:getTag()]:getContentSize().height/2))
                    
                    
                    self:initItemInfo(self.nodeArr[sender:getTag()]:getChildByName("Panel_Icon"):getChildByName("Panel_Object"),self.data.prize)
                    self:setDraw(sender:getTag())
                end
            end)
        end
        
        --在玩家无操作5秒后，自动播放翻牌动画，随机翻开一个，0.5秒后翻开其他两个，并出现继续按钮
        Timer:runOnce(function()
            if self.isDraw == false then
                local num = math.random(1,3)
                self:setDraw(num)
            end
        end,5)
    end
end

function DefeatScene:setDraw(index)
    for m = 1,3 do
        self.nodeArr[m]:getChildByName("Panel_Icon"):getChildByName("Button_1"):setTouchEnabled(false)
    end
    self.isDraw = true
    self.nodeActionArr[index]:gotoFrameAndPlay(0,90,false)
    
    --翻牌边框
    local button_1 = self.nodeArr[index]:getChildByName("Panel_Icon"):getChildByName("Button_1")
    local node,nodeAction=self:createNode("Node/Node_Animation_Draw_Select.csb")
    button_1:addChild(node)
    nodeAction:gotoFrameAndPlay(0,true)           
    node:setPosition(cc.p(button_1:getContentSize().width/2,button_1:getContentSize().height/2))
    
    Timer:runOnce(function()
        local num = 0
        for k = 1,3 do
            num = num+1
            if k ~= index then
                self:initItemInfo(self.nodeArr[k]:getChildByName("Panel_Icon"):getChildByName("Panel_Object"),self.data.displayPrizes[num])
                self.nodeActionArr[k]:gotoFrameAndPlay(0,90,false)
            else
                num = num - 1
            end
        end
    end,1)
    self.Button_Next:setVisible(true)
end

return DefeatScene
