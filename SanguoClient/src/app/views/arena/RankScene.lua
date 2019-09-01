--排行榜界面
local RankScene = class("RankScene", cc.load("mvc").ViewBase)
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
RankScene.NEXTSCENE = "battle/ChapterScene"

RankScene.RESOURCE_FILENAME = "Scene/ArenaRankPop.csb"
function RankScene:onCreate()
    self.data = self.args[1]
    self:initUi()
end

function RankScene:initUi()
    local ArenaRankPop = self:getResourceNode():getChildByName("ArenaRankPop")
    self.Panel_Center = ArenaRankPop:getChildByName("Panel_C"):getChildByName("Panel_Center")
    local Panel_Top = ArenaRankPop:getChildByName("Panel_Top")
    local Panel_Bg = ArenaRankPop:getChildByName("Panel_Bg")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    --关闭按钮
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:removeDlgNoAction()

--            SWITSCENE("arena/ArenaScene")
--            ctlHighladderGetListRequest:sendRequest(handler(self,self.cutArenaPanel));
        end
    end
    )
    
    --详细按钮，弹出排行奖励界面
    local  Button_3=self.Panel_Center:getChildByName("Button_3")
    Button_3:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            ctlHighladderGetRankingRewardRequest:sendRequest(handler(self,self.onGetRankingReward));
        end
    end
    )
 
    --排行榜
    self:rankInit()
    
    self.buttons = {}
    for i=1,4 do
        local btn = self.Panel_Center:getChildByName("ProjectNode_"..i):getChildByName("Panel_tab"):getChildByName("Button_Select_1")
        btn:setVisible(false)
        self.buttons[i] = btn
        btn:setTag(i-1);
        btn:addTouchEventListener(function(sender)
            self:setIndex(sender:getTag())
        end)
    end
    self:setIndex(1)
end
--排行榜
function RankScene:rankInit()
    local Image_BgFrame = self.Panel_Center:getChildByName("Image_BgFrame")
    
    local node,item
    node=self:createNode("Node/Node_tab.csb")
    item = node:getChildByName("Panel_tab")
    local contentSize =item:getContentSize()
    
    --排行信息
    self:rankInfo()
end

function RankScene:rankInfo()
    local scrollView = self.Panel_Center:getChildByName("HeroScrollView_1")
    local node,item
    node=self:createNode("Node/Node_Item_3.csb")
    item = node:getChildByName("Panel_Item_3")
    local contentSize =item:getContentSize()

    self.items = {}
    self.space = 0
    local num = table.getn(self.data.rankings) + 1 
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*num));
    scrollView:setInnerContainerSize(sSize)
    
    local nodeHero,itemHero
    nodeHero=self:createNode("Node/Node_RisingStar_Head.csb")
    itemHero = nodeHero:getChildByName("Panel_RisingStar_Head")
    
    local itemRank
    for i=1,num do
        local newItem = item:clone()
        newItem:setTouchEnabled(true);
        newItem:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                if sender:getTag() == 0 then
                    itemRank = self.data.self.rank
                else
                    itemRank = self.data.rankings[sender:getTag()].rank
                end
                --弹出查看信息面板
                ctlHighladderGetOthersMonstersRequest:sendRequest(handler(self,self.onHighladderGetOthersMonsters), itemRank);
            end
        end
        )
        local Image_Rank = newItem:getChildByName("Image_Rank")
        local Text_1 = newItem:getChildByName("Text_1")
        local Text_2 = newItem:getChildByName("Text_2")
        if i ~= 2 and i ~= 3 and i ~= 4 then
            Image_Rank:setVisible(false)
            Text_1:setVisible(i == 1)
            Text_2:setVisible(true)
        else
            Image_Rank:setVisible(true)
            Text_1:setVisible(false)
            Text_2:setVisible(false)
            Image_Rank:loadTexture("Assets/HeroListScene/rank_"..(i-1)..".png")
        end
        --人物头像部分
        self.newHeroItems = {}
        for j = 1,4 do
            local newHeroItem = itemHero:clone()
            local x1
            local y1
            local Image_Hero = newHeroItem:getChildByName("Image_Hero")
            local Image_Head_bg = newHeroItem:getChildByName("Image_Head_bg")
            --信息显示
            if i == 1 then
            	if j == 1 then
                    Image_Hero:loadTexture(model_monster:getIcon(self.data.self.showMonster.id))
                    Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(self.data.self.showMonster.quality))
                    self:setStarAndJobInfo(newHeroItem,self.data.self.showMonster)
            	else
                    if (j - 1) <= table.getn(self.data.self.monsters) then
                        newHeroItem:setVisible(true)
                        Image_Hero:loadTexture(model_monster:getIcon(self.data.self.monsters[j-1].id))
                        Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(self.data.self.monsters[j-1].quality))
                        self:setStarAndJobInfo(newHeroItem,self.data.self.monsters[j-1])
                    else
                        newHeroItem:setVisible(false)
                    end
            	end
            else
                if j == 1 then
                    Image_Hero:loadTexture(model_monster:getIcon(self.data.rankings[i-1].showMonster.id))
                    Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(self.data.rankings[i-1].showMonster.quality))
                    self:setStarAndJobInfo(newHeroItem,self.data.rankings[i-1].showMonster)
                else
                    if (j - 1) <= table.getn(self.data.rankings[i-1].monsters) then
                        newHeroItem:setVisible(true)
                        Image_Hero:loadTexture(model_monster:getIcon(self.data.rankings[i-1].monsters[j-1].id))
                        Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(self.data.rankings[i-1].monsters[j-1].quality))
                        self:setStarAndJobInfo(newHeroItem,self.data.rankings[i-1].monsters[j-1])
                    else
                        newHeroItem:setVisible(false)
                    end
                end
            end
                    
            --控制武将图标的坐标
            if j == 1 then
                newHeroItem:setScale(0.9)
                x1 = 178
                y1 = 63
            else
                newHeroItem:setScale(0.435)
                x1 = 592 +65*(j-2)
                y1 = 42--46+48*(j-2) 
            end
            newItem:addChild(newHeroItem)
            newHeroItem:setPosition(cc.p(x1,y1))
            self.newHeroItems[j] = newHeroItem
        end

        local index = i-1
        self.items[i] = newItem
        newItem:setTag(index);
        scrollView:addChild(newItem)
        local x=contentSize.width/2
        local y=sSize.height-(contentSize.height)*index-contentSize.height/2
        newItem:setPosition(cc.p(x,y))
        
        --信息部分
        local Text_4 = newItem:getChildByName("Text_4")
        local Text_3 = newItem:getChildByName("Text_3")
        if i == 1 then--自己的信息
            Text_2:setString(self.data.self.rank)
            Text_3:setString(LANG("战力:%s",self.data.self.power))
            Text_4:setString(LANG("%s lv%s" ,self.data.self.name,self.data.self.level))
        else--排行信息
            Text_2:setString(self.data.rankings[i-1].rank)
            Text_3:setString(LANG("战力:%s",self.data.rankings[i-1].power))
            Text_4:setString(LANG("%s lv%s" ,self.data.rankings[i-1].name,self.data.rankings[i-1].level))
        end
    end
end 

--星级及职业信息
function RankScene:setStarAndJobInfo(item,info)
    local Image__Job = item:getChildByName("Image__Job")
    local Panel_Star = item:getChildByName("Panel_Star")
    --星级
    for i = 1,5 do
        local star = Panel_Star:getChildByName("Image_Star_"..i)
        star:setVisible(info.star >= i)
    end
    --职业
    Image__Job:loadTexture("Job/Icon_job"..model_monster:getType(info["id"])..".png")
end

function RankScene:onGetRankingReward(success, data, code, msg)
    self:showDlg("arena/ArenaAwardPopup",data)
end

--function RankScene:cutArenaPanel(success, data, code, msg)
--    if(success) then
--        SWITSCENE("arena/ArenaScene.lua",data)
--    else
--    end
--end

function RankScene:onHighladderGetOthersMonsters(success, data, code, msg)
    if(success) then
--        self:showDlg("arena/ArenaHeroInfo",data,self.data)

        self:showDlg("hero/HeroInfoScene",data.monsters[1],"others",data.monsters)
    else
    end

end

return RankScene
