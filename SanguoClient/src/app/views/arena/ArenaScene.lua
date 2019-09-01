--竞技场主界面
local ArenaScene = class("ArenaScene", cc.load("mvc").ViewBase)
ArenaScene.NEXTSCENE = "battle/ChapterScene"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_monster = _REQUIRE("model/static/model_monster.lua")
local helper_pvp = _REQUIRE("model/helper/helper_pvp.lua")
--_REQUIRE "src/packages/packet/Packet_Regester"

ArenaScene.RESOURCE_FILENAME = "Scene/ArenaScene.csb"
ArenaScene.STOPFRAME = 45
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function ArenaScene:onCreate()
    self.data = self.args[1]
    self.nodeActionIndex = 0
    self.nodeActionSecondStepIndex = 0
    self.rankItemIndex = 0
    self.scrollViewProgress = 0 --scrollView进度
    self.moveDisX = 0
    self.moveDisY = 0
    self:initUi()
end

function ArenaScene:initUi()
    local Panel_ArenaScene = self:getResourceNode():getChildByName("Panel_ArenaScene")
    self.Panel_Center = Panel_ArenaScene:getChildByName("Panel_Center")
    self.Panel_Left = self.Panel_Center:getChildByName("Panel_Left")
    local Panel_Right = self.Panel_Center:getChildByName("Panel_Right")
    local Panel_Top = Panel_ArenaScene:getChildByName("Panel_T"):getChildByName("Panel_Top")
    local Panel_Bg = Panel_ArenaScene:getChildByName("Panel_Bg")
--    self.panel_fight = self.Panel_Center:getChildByName("panel_fight"):getChildByName("ProjectNode_figh")
    SCREEN_TOP(Panel_ArenaScene:getChildByName("Panel_T"))
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_CENTER(Panel_ArenaScene)
    self.Panel_Left:setLocalZOrder(1000)
    local Node_Top=Panel_Top:getChildByName("Node_Top")
    self:initTopBar(Node_Top)
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            SWITSCENE("home/MainCityScene")
        end
    end
    )
    
    Panel_ArenaScene:setTouchEnabled(true)
    Panel_ArenaScene:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.currentIndex = -1
            self:setFightNodeStatus(false)
        end
    end
    )
    
    --玩家列表
    self.scrollView = self.Panel_Center:getChildByName("ScrollView_1")
--    self.scrollView:setTouchEnabled(false)
--    self.scrollView:setInertiaScrollEnabled(true)
    self.scrollViewPreWidth = self.scrollView:getContentSize().width
    self.scrollViewX = self.scrollView:getPositionX()
    local node,item
    node=self:createNode("Node/Panel_Arena_Anim.csb")
    item = node:getChildByName("Panel_ArenaSelect")
    
    local contentSize =item:getContentSize()

    self.heroItems = {}
    self.newItemArr = {}
    self.space = 10
    local num = table.getn(self.data.ranks) + 1--对手数量
    --将自己和敌方武将放在一起，并且按排名排序
    self.heroArr = {}
    local indexNum = 0
    local bool = false
    for u = 1,num do
        indexNum = indexNum + 1
        if indexNum == num  then
            self.heroArr[u] = self.data.self
            self.selfNumber = u --记录数组中第几个是自己
        else
            if self.data.ranks[indexNum].rank < self.data.self.rank then 
                self.heroArr[u] = self.data.ranks[indexNum]
            else
                if bool == false then
                    self.heroArr[u] = self.data.self
                    indexNum = indexNum - 1
                    bool = true
                    self.selfNumber = u --记录数组中第几个是自己
                else
                    self.heroArr[u] = self.data.ranks[indexNum]
                end 
            end
        end
    end
    
--    local totalNum = contentSize.height*(num-1)*5/9+contentSize.height+70
    local totalNum = (num - 1)*160 + contentSize.height *6/5 --math.floor(num/2)*320 + contentSize.height---
    local sSize=cc.size(self.scrollView:getInnerContainerSize().width,math.max(self.scrollView:getContentSize().height,totalNum))
    self.scrollView:setInnerContainerSize(sSize)
    --滚动条的高度设置和屏幕高度一样高
    
    self.scrollView:setContentSize(cc.size(self.scrollView:getContentSize().width,CC_SCREEN_SIZE.height))
    self.nodeActionArr = {}
    self.x = 0
    for i=1,num do
--        local newItem = item:clone()
        -----------------------------
        local node1,nodeAction=self:createNode("Node/Panel_Arena_Anim.csb")
        self.nodeActionArr[i] = nodeAction
        local Names = node1:getChildByName("Names")
--        nodeAction:setTag(i)
        local newItem = node1:getChildByName("Panel_ArenaSelect")
        nodeAction:gotoFrameAndPause(0)
        nodeAction:setFrameEventCallFunc(function(frame)
            if nil == frame then
                return
            end
            local str = frame:getEvent()
            if str == "Stop" then
                if self.nodeActionSecondStepIndex < table.getn(self.data.ranks) + 1 then
                    self:playNodeAction(2)
                end
            elseif str == "LoopOver" then  
            elseif str == "SelectStop" then
            elseif str == "SelectOver" then
                self.nodeActionArr[self.rankItemIndex]:gotoFrameAndPlay(110,411,true)
                self.rankItemIndex = 0
            end
        end);
        
        local default_item = ccui.Layout:create()
--        default_item:setTouchEnabled(true)
        default_item:setContentSize(newItem:getContentSize())
        default_item:addChild(node1)
        default_item:setLocalZOrder(i)--设置层级
        self.scrollView:addChild(default_item)
        node1:setPosition(cc.p(default_item:getContentSize().width/2,default_item:getContentSize().height/2))
        -----------------------------
        local Panel_Role = newItem:getChildByName("Panel_Role")
        Panel_Role:setTouchEnabled(false)
        newItem:setTouchEnabled(false)
        Names:setTouchEnabled(false)
        default_item:setTouchEnabled(false)
        default_item:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                if sender:getTag()+1 == self.selfNumber and self.isShowFightNode == false then
                    PopMsg.getInstance():flashShow(LANG("这是您自己哦！"))
                    self:setFightNodeStatus(false)
                    return
                end
                self:initPanel(sender:getTag()+1)
                self.rankItemIndex = sender:getTag()+1;     
            end 
        end
        )
        local index = i-1
        self.heroItems[i] = default_item--newItem
        self.newItemArr[i] = newItem
        newItem:setTag(index);
        default_item:setTag(index);
        
--        local x=3/5*contentSize.width +index%2*(contentSize.width+10) - default_item:getContentSize().width/2 - 20
--        local y=sSize.height - 160*index -contentSize.height*6/5
--        local x=3/5*contentSize.width +index%3*(contentSize.width-40) - default_item:getContentSize().width/2 - 20
--        local y=sSize.height - 160*index -contentSize.height*6/5
        if i < 4 then
            self.x=3/5*contentSize.width +index%3*(contentSize.width+10) - default_item:getContentSize().width/2 - 30
        elseif (i%4 == 0 or i%4 == 1) then
            self.x=self.x-(contentSize.width+10)
        else
            self.x=self.x+(contentSize.width+10)
        end
        local y=sSize.height - 160*index -contentSize.height*6/5
        
--        newItem:setPosition(cc.p(x,y))
        default_item:setPosition(cc.p(self.x,y))
        --锁链   
        newItem:getChildByName("Panel_ChainL_1"):setTouchEnabled(false)
        newItem:getChildByName("Panel_ChainL_2"):setTouchEnabled(false)
        newItem:getChildByName("Panel_ChainR_1"):setTouchEnabled(false)
        newItem:getChildByName("Panel_ChainR_2"):setTouchEnabled(false)
        
        --锁链的隐藏
        newItem:getChildByName("Panel_ChainL_1"):setVisible(false)
        newItem:getChildByName("Panel_ChainL_2"):setVisible(false)
        newItem:getChildByName("Panel_ChainR_1"):setVisible(false)
        newItem:getChildByName("Panel_ChainR_2"):setVisible(false)

        --数据部分
        local itemInfo = self.heroArr[i]
        --自己和对手的名字及名字颜色
        if i == self.selfNumber then
            newItem:getChildByName("Panel_Role"):getChildByName("user_name_2"):setString(LANG(model_user.userName))
            newItem:getChildByName("Panel_Role"):getChildByName("user_name_2"):setColor(cc.c3b(244, 194, 71))
--            newItem:getChildByName("Panel_Role"):getChildByName("Image_Role_Name"):setString(LANG(model_user.userName))
--            newItem:getChildByName("Panel_Role"):getChildByName("Image_Role_Name"):setColor(cc.c3b(244, 194, 71))
        else
            newItem:getChildByName("Panel_Role"):getChildByName("user_name_2"):setString(LANG(itemInfo.name))
            newItem:getChildByName("Panel_Role"):getChildByName("user_name_2"):setColor(cc.c3b(255, 255, 255))
--            newItem:getChildByName("Panel_Role"):getChildByName("Image_Role_Name"):setString(LANG(itemInfo.name))
--            newItem:getChildByName("Panel_Role"):getChildByName("Image_Role_Name"):setColor(cc.c3b(255, 255, 255))
        end
        
        newItem:getChildByName("Panel_Role"):getChildByName("rank_number_4"):setString(LANG("排名：%s",itemInfo.rank))
--        newItem:getChildByName("rank_number_6"):setString(LANG("战力：%s",itemInfo.power))
        --记录自己排名所处的滚动条位置
        if itemInfo.rank == self.data.self.rank then
            local heighNum = math.max(self.scrollView:getContentSize().height,totalNum)
--            self.scrollViewProgress = 100 - (y+default_item:getContentSize().height/2)*100/heighNum
            self.scrollViewProgress = 100 - (y+contentSize.height/2)*100/heighNum
            self.scrollView:scrollToPercentVertical(self.scrollViewProgress,0.01,false)
        end
        --排名（前三名有图标）
        newItem:getChildByName("Panel_Role"):getChildByName("Image_Rank"):setVisible(itemInfo.rank < 4)
        if itemInfo.rank < 4 then
            newItem:getChildByName("Panel_Role"):getChildByName("Image_Rank"):loadTexture("Share/icon/pvp_title_" .. itemInfo.rank .. ".png")
        end
        
    end
    self:setSuoLian(0)

    --声望商店按钮
    local Button_shop = Panel_Right:getChildByName("Button_shop")
    Button_shop:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:setFightNodeStatus(false)
            self:showDlg("arena/PrestigeShop",nil,nil,function(data)
                self.data.self.prestige = model_user.prestige
            end)
        end
    end
    )
    
    --排行按钮
    local Button_rank = Panel_Right:getChildByName("Button_rank")
    Button_rank:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:setFightNodeStatus(false)
            ctlHighladderGetRankingRequest:sendRequest(handler(self,self.onHighladderGetRanking));
        end 
    end
    )
    
    --奖励按钮
    local Button_reward = Panel_Right:getChildByName("Button_reward")
    Button_reward:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:showDlg("arena/GetRewardPopupScene",self.data,function(data)
                self:setRewardPlay()
            end)
        end 
    end
    )
    
    local node2,nodeAction2=self:createNode("Node/Node_fight.csb")
    self.panel_fight_node = node2
    self.scrollView:addChild(self.panel_fight_node)
    nodeAction2:gotoFrameAndPause(0)
    self.panel_fight_node:getChildByName("Names"):setTouchEnabled(false)
    self.panel_fight_node:setLocalZOrder(100)
    
    self.nodeAction2 = nodeAction2
    self:setFightNodeStatus(false)
    
    --挑战按钮
    local challengeBtn = self.panel_fight_node:getChildByName("Panel_Fight"):getChildByName("Button_fight") --挑战按钮
    challengeBtn:setTouchEnabled(true)
    challengeBtn:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
--            if self.data.self.rank >200 and model_user.pvpRanks[self.rankItemIndex].rank < 13 then
            --首先判断精力值是否足够(写死，现在每场挑战需要的精力都是1)
            if model_user.vitality > 0 then
                if self.data.self.rank >200 and self.heroArr[self.rankItemIndex].rank < 13 then
                    PopMsg.getInstance():flashShow(LANG("您的排名太低，无法挑战该玩家！"))
                else
                    model_user.preRank = self.data.self.rank
                    model_user.currentBattleRank = self.heroArr[self.rankItemIndex].rank;
                    helper_pvp:getEnemy(self.heroArr[self.rankItemIndex].rank, handler(self,self.onHighladderGetEnemy));
                end
            else
                PopMsg.getInstance():flashShow(LANG("精力不足！"))
            end
        end
    end
    )
    
    self:setRewardPlay()
end

function ArenaScene:setRewardPlay()
    local boo = false
    if self.data.self.prestige2Draw ~=0 or self.data.self.cash2Draw ~=0 then
        boo = true
    end
    --奖励特效是否显示
    self.Panel_Center:getChildByName("Panel_Right"):getChildByName("Node_CircleBtnReady"):setVisible(boo)
end

function ArenaScene:initImgColor(Image_1,count)
    if count <= 0 then
        Image_1:setColor(cc.c3b(150,150,150))
    else
        Image_1:setColor(cc.c3b(255,255,255))
    end
end

function ArenaScene:initPanel(index)
--    if index == self.currentIndex and self.isShowFightNode then
--    	return
--    end
    if self.isShowFightNode then
        self:setFightNodeStatus(false)
        return 
    end
    
    for t = 1,table.getn(self.nodeActionArr) do
        if t ~= index then
            self:initImgColor(self.newItemArr[t]:getChildByName("Image_Role"),0)
        else
            self.nodeActionArr[t]:gotoFrameAndPlay(420,461,false)
            self.heroItems[t]:setLocalZOrder(table.getn(self.heroItems) + 1)
        end
    end
    
    self.currentIndex = index
--    local x = (index - 1)%2*50 +300
    local x = self.heroItems[index]:getPositionX() +self.heroItems[index]:getContentSize().width*3/5 + (self.scrollViewX - self.scrollView:getContentSize().width/2)*(index%2)

    self:setFightNodeStatus(true)
    self.panel_fight_node:setPosition(x,self.heroItems[index]:getPositionY() +self.heroItems[index]:getContentSize().width/2)
    
    local info = self.heroArr[index].monsters
    for k = 1,3 do
        self.panel_fight_node:getChildByName("Panel_Fight"):getChildByName("hero"..k):setVisible(k <= table.getn(info))
    end
    
    for i = 1,table.getn(info) do
        --星级的显示
    	for j = 1,5 do
            local star = self.panel_fight_node:getChildByName("Panel_Fight"):getChildByName("hero"..i):getChildByName("Panel_RisingStar_Head"):getChildByName("Panel_Star"):getChildByName("Image_Star_"..j)
            star:setVisible(j <= info[i].star)
    	end
    	--头像及颜色框的显示
        local Image_Hero = self.panel_fight_node:getChildByName("Panel_Fight"):getChildByName("hero"..i):getChildByName("Panel_RisingStar_Head"):getChildByName("Image_Hero")
        local Image_Head_bg = self.panel_fight_node:getChildByName("Panel_Fight"):getChildByName("hero"..i):getChildByName("Panel_RisingStar_Head"):getChildByName("Image_Head_bg")
        Image_Hero:loadTexture(model_monster:getIcon(info[i]["id"]))
        Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(info[i]["quality"]))
        
    end
    
    local challengeBtn = self.panel_fight_node:getChildByName("Panel_Fight"):getChildByName("Button_fight") --挑战按钮
    if self.data.self.rank >200 and self.heroArr[index].rank < 13 then
        challengeBtn:setBright(false)
    else
        challengeBtn:setBright(true)
    end
    self.nodeAction2:gotoFrameAndPlay(0,30,false)
    

    --滚动区域最下方位置相对于scrollView描点位置和点击到的对象相对于滚动区别最下方位置的和
    self.moveDisY = self.scrollView:getContentSize().height/2 - (self.heroItems[index]:getPositionY() + self.scrollView:getInnerContainer():getPositionY() + self.newItemArr[1]:getContentSize().height/2)             
    local disX = self.scrollView:getContentSize().width/2 - self.heroItems[index]:getPositionX()
    self.moveDisX = disX - self.newItemArr[1]:getContentSize().width/2

    --里面的单项进行移动
    for i = 1, table.getn(self.heroItems) do
        local move = cc.MoveTo:create(0.1, cc.p(self.heroItems[i]:getPositionX()+self.moveDisX,self.heroItems[i]:getPositionY()+self.moveDisY))
        self.heroItems[i]:runAction(cc.Sequence:create(move,nil))
    end
    local move1 = cc.MoveTo:create(0.1, cc.p(self.scrollView:getPositionX(),self.panel_fight_node:getPositionY()+self.moveDisY))
    self.panel_fight_node:runAction(cc.Sequence:create(move1,nil))
    
    --锁链的显示隐藏
    self:setSuoLian(index)  

    --挑战需要花费精力值
    --    self.panel_fight:getChildByName("Panel_Fight"):getChildByName("power_number"):setString()-------------------和服务器说一下，缺少这个值
end
--锁链的隐藏和显示
function ArenaScene:setSuoLian(index) 
--    if index ~= 0 then
--        if index%2 == 1 then -- 左边的被点击
--            if index ~= 1 then--不是第一个
--                self.newItemArr[index - 1]:getChildByName("Panel_ChainL_1"):setVisible(false)
--            end
--            if index ~= table.getn(self.newItemArr) then -- 不是最后一个
--            self.newItemArr[index + 1]:getChildByName("Panel_ChainL_2"):setVisible(false)
--            end
--        else -- 右边的被点击
--            self.newItemArr[index - 1]:getChildByName("Panel_ChainR_1"):setVisible(false)
--            if index ~= table.getn(self.newItemArr) then
--                self.newItemArr[index + 1]:getChildByName("Panel_ChainR_2"):setVisible(false)
--            end
--        end
--    else
--        local num = table.getn(self.newItemArr)
--        for i = 1,num do
--            local newItem = self.newItemArr[i]
--            newItem:getChildByName("Panel_ChainL_1"):setVisible(i%2 ~= 1 and i ~= num)
--            newItem:getChildByName("Panel_ChainL_2"):setVisible(i%2 ~= 1)
--            newItem:getChildByName("Panel_ChainR_1"):setVisible(i%2 == 1 and i ~= num)
--            newItem:getChildByName("Panel_ChainR_2"):setVisible(i%2 == 1 and i ~= 1)
--        end
--    end
    
    
    if index ~= 0 then
        if index%4 == 0 then
            self.newItemArr[index - 1]:getChildByName("Panel_ChainL_1"):setVisible(false)
            if index ~= table.getn(self.newItemArr) then
                self.newItemArr[index + 1]:getChildByName("Panel_ChainR_2"):setVisible(false)
            end
        elseif index%4 == 1 then
            if index ~= 1 then--不是第一个
                self.newItemArr[index - 1]:getChildByName("Panel_ChainL_1"):setVisible(false)
            end
            if index ~= table.getn(self.newItemArr) then -- 不是最后一个
                self.newItemArr[index + 1]:getChildByName("Panel_ChainL_2"):setVisible(false)
            end
        elseif index%4 == 2 then
            self.newItemArr[index - 1]:getChildByName("Panel_ChainR_1"):setVisible(false)
            if index ~= table.getn(self.newItemArr) then
                self.newItemArr[index + 1]:getChildByName("Panel_ChainL_2"):setVisible(false)
            end
        elseif index%4 == 3 then
            self.newItemArr[index - 1]:getChildByName("Panel_ChainR_1"):setVisible(false)
            if index ~= table.getn(self.newItemArr) then
                self.newItemArr[index + 1]:getChildByName("Panel_ChainR_2"):setVisible(false)
            end
        end
        
    else
        local num = table.getn(self.newItemArr)
        for i = 1,num do
            local newItem = self.newItemArr[i]
            newItem:getChildByName("Panel_ChainL_1"):setVisible(i%4 ~= 1 and i%4 ~= 2 and i ~= num)
            newItem:getChildByName("Panel_ChainL_2"):setVisible(i%4 ~= 0 and i%4 ~= 1)
            newItem:getChildByName("Panel_ChainR_1"):setVisible(i%4 ~= 0 and i%4 ~= 3 and i ~= num) 
            newItem:getChildByName("Panel_ChainR_2"):setVisible(i%4 ~= 2 and i%4 ~= 3 and i ~= 1)
        end
    end
end

function ArenaScene:setFightNodeStatus(boo)
    self.isShowFightNode = boo
    if self.isShowFightNode == true then
        self.scrollView:setTouchEnabled(false)
    else
        self.scrollView:setTouchEnabled(true)
    end
    self.panel_fight_node:setVisible(boo);
    self.panel_fight_node:getChildByName("Panel_Fight"):setTouchEnabled(boo)
    if boo == false then
        for q = 1,table.getn(self.newItemArr) do
            self:initImgColor(self.newItemArr[q]:getChildByName("Image_Role"),1)
            self.heroItems[q]:setLocalZOrder(q)--设置层级
        end 
        if self.rankItemIndex > 0 then
            self.nodeActionArr[self.rankItemIndex]:gotoFrameAndPlay(470,507,false)
            --            self.rankItemIndex = 0
        end
--        self.scrollView:setContentSize(cc.size(self.scrollViewPreWidth,self.scrollView:getContentSize().height))
--        self.scrollView:scrollToPercentVertical(self.scrollViewProgress,0.01,false) 
        self:setSuoLian(0)
    else
--        self.scrollView:setContentSize(cc.size(CC_SCREEN_SIZE.width,self.scrollView:getContentSize().height)) 
    end

    if self.moveDisX ~= 0 then
        for i = 1, table.getn(self.heroItems) do
            local move = cc.MoveTo:create(0.1, cc.p(self.heroItems[i]:getPositionX()-self.moveDisX,self.heroItems[i]:getPositionY()-self.moveDisY))
            self.heroItems[i]:runAction(cc.Sequence:create(move,nil))
        end
        self.moveDisX = 0
        self.moveDisY = 0
    end 
end

function ArenaScene:onHighladderDraw(success, data, code, msg)
    if(success) then
        local haveCash = self.data.self.cash2Draw - data.cash
        if haveCash < 0 then
        	haveCash = 0
        end
        
        local havePrestige = self.data.self.prestige2Draw - data.prestige
        if havePrestige < 0 then
            havePrestige = 0
        end
        
        self.data.self.cash2Draw = haveCash
        self.data.self.prestige2Draw = havePrestige
        self.data.self.prestige = self.data.self.prestige + data.prestige
        self:setOwnData()
        PopMsg.getInstance():flashShow(LANG("领取成功！"))
    else
    end
end

function ArenaScene:onHighladderGetRanking(success, data, code, msg)
    if(success) then
         self:showDlg("arena/RankScene",data)
    else
    end
end

function ArenaScene:onHighladderGetShop(success, data, code, msg)
    if(success) then
        SWITSCENE("arena/PrestigeShop", data, self.data)
    else
    end
end

function ArenaScene:onHighladderGetEnemy(success, data, code, msg)
	if(success) then
        --self:showDlg("battle/VersusScene",data,1)
        SWITSCENE("battle/VersusScene",data,1)
    else
    end
end

function ArenaScene:playNodeAction(index)
    if index == 1 then
        self.nodeActionIndex = self.nodeActionIndex + 1
        self.nodeActionArr[self.nodeActionIndex]:gotoFrameAndPlay(0,101,false)
        if self.nodeActionIndex == table.getn(self.nodeActionArr) then
            for i = 1,table.getn(self.heroItems) do
                self.heroItems[i]:setTouchEnabled(true)
        	end
            Timer:kill(self.timerId)
        end
    elseif index == 2 then
        self.nodeActionSecondStepIndex = self.nodeActionSecondStepIndex + 1
        self.nodeActionArr[self.nodeActionSecondStepIndex]:gotoFrameAndPlay(110,411,true)
    end
end

function ArenaScene:onEnter()
    self:playNodeAction(1)
    if self.selfNumber > 6 then
        self.nodeActionIndex = self.selfNumber - 6
        for i = 1,self.selfNumber - 6 do
            self.nodeActionArr[i]:gotoFrameAndPlay(0,101,false)
        end
    end
    self.timerId = Timer:start(function()
        if self.nodeActionIndex < table.getn(self.data.ranks) + 1 then
            self:playNodeAction(1)
        end
    end,0.2)
    
    self:playStart()
--    self:getActionNode():gotoFrameAndPlay(30,200,true)
--    for i = 1,table.getn(self.nodeActionArr) do
--        self.nodeActionArr[i]:gotoFrameAndPlay(0,27,false)
--    end
end

return ArenaScene
