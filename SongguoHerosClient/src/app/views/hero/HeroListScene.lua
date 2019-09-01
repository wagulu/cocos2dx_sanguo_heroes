
local HeroListScene = class("HeroListScene", cc.load("mvc").ViewBase)
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local helper_monster = _REQUIRE("model/helper/helper_monster.lua")
local static_model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua");
local model_equipment = _REQUIRE("model/static/model_equipment.lua")
HeroListScene.NEXTSCENE = "battle/ChapterScene"
HeroListScene.STOPFRAME = 30
--_REQUIRE "src/packages/packet/Packet_Regester"

--HeroListScene.RESOURCE_FILENAME = "Scene/HeroListScene.csb"
HeroListScene.RESOURCE_FILENAME = "Scene/SelectHeroPop.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function HeroListScene:onCreate()
    self.upArrowArr = {}
    self.totalPartsArr = {}
    self:initUi()
end

function HeroListScene:initUi()
    local Panel_HeroListScene = self:getResourceNode():getChildByName("Panel_SelectHeroPop")
    self.Panel_Center = Panel_HeroListScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    local Panel_Top = Panel_HeroListScene:getChildByName("Panel_Top")
    Panel_HeroListScene:setTouchEnabled(true)
--    local  Button_Close=Panel_Top:getChildByName("Button_Close")
--    Button_Close:setVisible(false)
    
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_HeroListScene:getChildByName("Panel_Bg"))
    SCREEN_CENTER(Panel_HeroListScene)
    Panel_HeroListScene:getChildByName("Panel_Bg"):setTouchEnabled(true)
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender, eventType)
        --SWITSCENE("home/HomeScene")
        self:sceneBack(function()
            if eventType == 2 then
                SWITSCENE("home/MainCityScene") 
            end
        end,true)
        
    end
    )
    
    self.itemsArr = {}
    self:updateListInfo(0)
    
--    local items_count = table.getn(listView:getItems())
--    
--    for i = 1,items_count do
--        --cclog("..........."..i)
--        local item = listView:getItem(i - 1)
----        for i=1,2 do
----            local panel = item:getChildByName("panel_"..i)
----            local Panel_Item_1 = panel:getChildByName("Panel_Item_1")
----            local Panel_Item_2 = panel:getChildByName("Panel_Item_2")
----            local Image_Head = Panel_Item_1:getChildByName("Image_Head")
----            local Image_Head_Off =Panel_Item_1:getChildByName("Image_Head_Off")
----            Image_Head_Off:setOpacity(0)
----            --Image_Head:loadTexture("Assets/win_panel/head_role.png")
----            Panel_Item_1:setVisible(true)
----            Panel_Item_2:setVisible(false)
----        end
--        
----        local index = listView:getIndex(item)
----        print("index is ", index)
----        button:setTitleText(array[index + 1])
--    end
    --print("items_count", items_count)
    
    -- create model
--    local default_button = ccui.Button:create("Assets/ChapterScene_panel/BackBtn_Bg1.png", "Assets/ChapterScene_panel/BackBtn_Bg1.png")
--    default_button:setName("Title Button")
--
--    local default_item = ccui.Layout:create()
--    default_item:setTouchEnabled(true)
--    default_item:setContentSize(default_button:getContentSize())
--    default_button:setPosition(cc.p(default_item:getContentSize().width / 2.0, default_item:getContentSize().height / 2.0))
--    default_item:addChild(default_button)
--
--    --set model
--    ListView_1:setItemModel(default_item)
--
--    --add default item
--    --local count = table.getn(array)
--    for i = 1,4 do
--        ListView_1:pushBackDefaultItem()
--    end
    
    self.buttons = {}
    self.pageTitle_ = {"全部","砍将","法将","统将"}
    for i=1,4 do
        local btn = self.Panel_Center:getChildByName("ProjectNode_"..i):getChildByName("Panel_tab"):getChildByName("Button_Select_1")
        btn:setTag(i)
        self.buttons[i] = btn
        btn:setTitleText(LANG(self.pageTitle_[i]))
        btn:addTouchEventListener(function(sender,eventType)
            if eventType==2 then
                self:setIndex(sender:getTag())
                self:updateListInfo(sender:getTag() - 1)
            end
        end)
    end
    
    self:setIndex(1)   
end

function HeroListScene:setListInfo(newItem,index)
    local info = self.allActiveMonsters[index+1]
    
    local Image_Head = newItem:getChildByName("Panel_Bg"):getChildByName("Image_Head")
    local Image_Head_bg = newItem:getChildByName("Panel_Bg"):getChildByName("Image_Head_Bg")
    local Image_Icon_Job = newItem:getChildByName("Panel_Bg"):getChildByName("Image_Icon_Job")
    Image_Head:loadTexture(model_monster:getIcon(info["id"]))
--    Image_Head:setColor(cc.c3b(100,100,100))
    Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(info["quality"]))
    Image_Head_bg:setCascadeOpacityEnabled(true)
    Image_Icon_Job:loadTexture("Job/Icon_job"..model_monster:getType(info["id"])..".png")
    
    newItem:getChildByName("Panel_Item_1"):getChildByName("Text_Level"):setString("LV"..info.level)
    newItem:getChildByName("Panel_Item_1"):getChildByName("Text_Name"):setString(LANG("%s",model_monster:getName(info.id)))
    --星级显示
    for i = 1,5 do
        local star = newItem:getChildByName("Panel_Item_1"):getChildByName("Image_Star_"..i)
        star:setVisible(info.star>=i)
    end
    --装备显示
    for j =1,6 do
        local equipItem = newItem:getChildByName("Panel_Item_1"):getChildByName("Image_ItemS_"..j)
        equipItem:setCascadeOpacityEnabled(true)
        local equipIcon = equipItem:getChildByName("Image_Icon_"..j)
        equipItem:setVisible(table.getn(info.equipment)>=j)
        --装备图标和框
        equipIcon:loadTexture(model_equipment:getIcon(info.equipment[j].id,info.equipment[j].quality))
        equipItem:loadTexture(model_equipment:getFrameByQuality(info.equipment[j].quality))
        if equipItem:getChildByTag(1001) ~= nil then
            equipItem:removeChild(equipItem:getChildByTag(1001))
        end
        
        --给装备上面添加可进化动画
        if helper_monster:canEquipmentQualityUp(info.id,info.equipment[j].id) == true then
            local node,nodeAction=self:createNode("Node/Node_UpArrow.csb")
            local Panel_UpArrow = node:getChildByName("Panel_UpArrow"):getChildByName("UpArrow0007_1")
            equipItem:addChild(node)
            equipItem:setTag(1000)
            node:setTag(1001)
            self.upArrowArr[table.getn(self.upArrowArr)+1] = {equipItem,node}
            nodeAction:gotoFrameAndPlay(0,true) 
            local x =  equipItem:getContentSize().width -Panel_UpArrow:getContentSize().width*3/5
            local y =  equipItem:getContentSize().height-Panel_UpArrow:getContentSize().height/2       
            node:setPosition(cc.p(x ,y))   
        end
    end
    --装备是否可进阶，武将是否可进阶和升星(有一个可以则显示小红点)
    newItem:getChildByName("Panel_Item_1"):getChildByName("Image_1"):setVisible(self:isShowRedDot(info))
end

function HeroListScene:setNotOwnHeroListInfo(newItem,index,num)
    local info
	if num == 1 then --表示可以合成的武将
        info = self.allInactiveCanMergeMonsters[index+1]
    else--表示不能合成的武将
        info = self.allInactiveNotMergeMonsters[index+1]
	end
    if info == nil then
		return
	end
    newItem:getChildByName("Panel_Item_2"):getChildByName("Text_Name"):setString(LANG("%s",model_monster:getName(info.id)))
    local Image_Head = newItem:getChildByName("Panel_Item_2"):getChildByName("Image_Head")
--    local Image_Head_bg = newItem:getChildByName("Panel_Bg"):getChildByName("Image_Head_Bg")
    Image_Head:loadTexture(model_monster:getIcon(info.id))
--    Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(info["quality"]))

    local Image_Icon_Job = newItem:getChildByName("Panel_Item_2"):getChildByName("Image_Icon_Job")
    Image_Icon_Job:loadTexture("Job/Icon_job"..model_monster:getType(info["id"])..".png")
end

function HeroListScene:updateListInfo(tab)
    for k = 1,table.getn(self.upArrowArr) do
        self.upArrowArr[k][1]:removeChild(self.upArrowArr[k][2])
    end
    self.upArrowArr = {}
    local scrollView = self.Panel_Center:getChildByName("HeroScrollView")
    local isNodeCuttinglineShow = false
    -- 清空武将列表信息
    scrollView:removeAllChildren()
    self.itemsArr = {}
    
    --自己所有的武将列表
    self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
    --不是自己拥有的武将列表
    local allInactiveMonsters = model_monsterStorage:getAllInactiveMonster()
    --可以合成的武将
    self.allInactiveCanMergeMonsters = {}
    --材料不足不能合成的武将
    self.allInactiveNotMergeMonsters = {}
    --不是自己的武将中筛选哪个是可以招募哪个不能招募
    for t = 1, table.getn(allInactiveMonsters) do
        local isMerge = helper_monster:canMergeMonster(allInactiveMonsters[t].id)
        if isMerge then
            self.allInactiveCanMergeMonsters[table.getn(self.allInactiveCanMergeMonsters)+1] = allInactiveMonsters[t]
        else
            self.allInactiveNotMergeMonsters[table.getn(self.allInactiveNotMergeMonsters)+1] = allInactiveMonsters[t]
        end
    end
    --筛选"全部","砍将","法将","统将"
    if tab == 0 then--全部
    elseif tab == 1 then--砍将
        self.allInactiveCanMergeMonsters = self:getHeroList(1,self.allInactiveCanMergeMonsters)
        self.allActiveMonsters = self:getHeroList(1,self.allActiveMonsters)
        self.allInactiveNotMergeMonsters = self:getHeroList(1,self.allInactiveNotMergeMonsters)
    elseif tab == 2 then--法将
        self.allInactiveCanMergeMonsters = self:getHeroList(2,self.allInactiveCanMergeMonsters)
        self.allActiveMonsters = self:getHeroList(2,self.allActiveMonsters)
        self.allInactiveNotMergeMonsters = self:getHeroList(2,self.allInactiveNotMergeMonsters)
    elseif tab == 3 then--统将
        self.allInactiveCanMergeMonsters = self:getHeroList(3,self.allInactiveCanMergeMonsters)
        self.allActiveMonsters = self:getHeroList(3,self.allActiveMonsters)
        self.allInactiveNotMergeMonsters = self:getHeroList(3,self.allInactiveNotMergeMonsters)
    end
    
    local canMergeNum = table.getn(self.allInactiveCanMergeMonsters)--可以合成武将的数量
    local ownMonstersNum = table.getn(self.allActiveMonsters)--自己拥有的武将的数量
    local notMergeNum = table.getn(self.allInactiveNotMergeMonsters)--不可合成的武将数量
    self.space = 0
    local num =  canMergeNum + ownMonstersNum + notMergeNum
--    local scrollView = self.Panel_Center:getChildByName("HeroScrollView")
    local contentSize =cc.size(435,138)
    local node=self:createNode("Node/Node_Item_1.csb")
    local item = node:getChildByName("panel_Item_1")
    local number = math.ceil((canMergeNum + ownMonstersNum)/2) + math.ceil(notMergeNum/2)
--    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*math.ceil(num/2) + 50));
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height+10,(contentSize.height+self.space)*number + 50 +10));
    scrollView:setInnerContainerSize(sSize)
    local function pageViewEvent(sender, eventType)
        if eventType == 4 then
            local pageView = sender

            cclog("ddddddddddddd")
        end
    end 
    scrollView:addEventListener(pageViewEvent)
    
    -- 添加滚动条    
    scrollView:jumpToTop()
    if(num > 6) then
        self:addScrollViewBar(scrollView,6)
    else 
        self:hideScrollViewBar(scrollView)
    end
    
    local boo = false
    for i=1,num do
        local index = i-1
        if table.getn(self.itemsArr) <= num then
            local newItem = item:clone()
            --newItem:setAnchorPoint(cc.p(0,0))
            newItem:setTouchEnabled(true);
            self.itemsArr[i] = newItem
            newItem:setTag(index);
            scrollView:addChild(newItem)
            --scrollView:setCascadeOpacityEnabled(true)
            newItem:setCascadeOpacityEnabled(true)
            local Panel_Bg = newItem:getChildByName("Panel_Bg")
            Panel_Bg:setCascadeOpacityEnabled(true)
            
        end
        local x=(sSize.width/2)/2.0+sSize.width/2*(index%2);
        local y=sSize.height-(contentSize.height+self.space)*math.floor(index/2)-contentSize.height/2 - 10
        self.itemsArr[i]:setPosition(cc.p(x,y))
        local Panel_Item_1 = self.itemsArr[i]:getChildByName("Panel_Item_1")
        Panel_Item_1:setCascadeOpacityEnabled(true)
        local Panel_Item_2 = self.itemsArr[i]:getChildByName("Panel_Item_2")
        Panel_Item_2:setCascadeOpacityEnabled(true)
        local Panel_Bg = self.itemsArr[i]:getChildByName("Panel_Bg")
        local Panel_Bg = self.itemsArr[i]:getChildByName("Panel_Bg")
--        local Panel_Team = self.itemsArr[i]:getChildByName("Panel_Team")
        local Text_14 = Panel_Item_2:getChildByName("Text_14")
        local Image_82 = Panel_Item_2:getChildByName("Image_82")
--        Panel_Team:setVisible(false)
--        local Image_Head_Off_0 = Panel_Item_2:getChildByName("Image_Head_Off_0"):(70)--------设置透明度
        Panel_Item_1:setVisible(i>canMergeNum and i <= canMergeNum + ownMonstersNum)
        Panel_Item_2:setVisible(i>canMergeNum + ownMonstersNum or i <= canMergeNum)
        Panel_Bg:setVisible(i>canMergeNum and i <= canMergeNum + ownMonstersNum)
        local LoadingBar_1 = Panel_Item_2:getChildByName("LoadingBar_1")
        LoadingBar_1:setPercent(70)
        local Image_Head_Off =Panel_Bg:getChildByName("Image_Head_Off")
        Image_Head_Off:setOpacity(0)
        Panel_Item_2:getChildByName("Image_Head_Off"):setColor(cc.c3b(0,0,0))
        Panel_Item_2:getChildByName("Image_Head_Off"):setOpacity(170)
        
--        self.greenNodeActionArr = {}
        if i<=canMergeNum then
            local Image_Item_Bg1 = Panel_Item_2:getChildByName("Image_Item_Bg1")
            local node,nodeAction=self:createNode("Node/Node_Item_1_Select_GreenAnim.csb")
            Image_Item_Bg1:addChild(node)
            node:setLocalZOrder(-1)
            nodeAction:gotoFrameAndPlay(0,true)           
            node:setPosition(cc.p(Image_Item_Bg1:getContentSize().width/2,Image_Item_Bg1:getContentSize().height/2-6))
--            self.greenNodeActionArr[i] = nodeAction
            
            Image_82:setVisible(false)
            LoadingBar_1:setPercent(100)
            self:setNotOwnHeroListInfo(self.itemsArr[i],index,1)
            self.itemsArr[i]:addTouchEventListener(function(sender,eventType)
                if eventType ==2 then
                    ctlMonstersMergeRequest:sendRequest(handler(self,self.onMonsterMerge),self.allInactiveCanMergeMonsters[sender:getTag()+1].id);
                end
            end)
        elseif i<=canMergeNum + ownMonstersNum then
            self:setListInfo(self.itemsArr[i],index-canMergeNum)
            self.itemsArr[i]:addTouchEventListener(function(sender,eventType)
                if eventType ==2 then
                    cclog("touchtouche"..sender:getTag())
                    local data = self.allActiveMonsters[sender:getTag() + 1 -canMergeNum]
                    self:showDlg("hero/HeroInfoScene",data,"mine","",function()
                        self:callBack()
                    end)  
                end
            end)
        else
            local monster = self.allInactiveNotMergeMonsters[i - canMergeNum - ownMonstersNum]
            local totalParts = static_model_monsterStar:getTotalStar(monster.star);
            self.totalPartsArr[i - canMergeNum - ownMonstersNum] = totalParts
            local currentParts = model_monsterPartStorage:getCount(monster.id);
            LoadingBar_1:setPercent(currentParts/totalParts)
            Text_14:setString(LANG("%s/%s",currentParts,totalParts))
            Image_82:setVisible(false)
            if boo == false then
                if i%2 == 0 and num ~= table.getn(self.allInactiveNotMergeMonsters) then--如果是不可合成武将，那么要新起一行
                    index = index + 1
            	end
                boo = true
            end

            local x=(sSize.width/2)/2.0+sSize.width/2*(index%2);
            local y=sSize.height-(contentSize.height+self.space)*math.floor(index/2)-contentSize.height/2 - 50
            self.itemsArr[i]:setPosition(cc.p(x,y))

            self:setNotOwnHeroListInfo(self.itemsArr[i],i-canMergeNum -ownMonstersNum-1 ,2)
            self.itemsArr[i]:addTouchEventListener(function(sender,eventType)
                if eventType ==2 then
                    cclog("touchtouche"..sender:getTag())
                    local data1 = self.allInactiveNotMergeMonsters[sender:getTag() + 1 -canMergeNum - ownMonstersNum]
                    self:showDlg("hero/ChipDropScene",{id = data1.id,type = pbCommon.ObjectType.MonsterPart, needNum = self.totalPartsArr[sender:getTag() + 1 - canMergeNum - ownMonstersNum]})
                end
            end
            )
            
            --放置与未获得武将UI隔开的隔开条
            if isNodeCuttinglineShow == false then
                isNodeCuttinglineShow = true
                local node=self:createNode("Node/Node_Cuttingline.csb")
                local item = node:getChildByName("Node_Cuttingline") 
                scrollView:addChild(item)
                item:setPosition(cc.p(scrollView:getInnerContainerSize().width/2,y + 100))
            end
        end
    end
end
--index:0-获取全部武将，1-砍将，2-法将，3-统将
function HeroListScene:getHeroList(index,info)
    local arr = {}
	for i = 1,table.getn(info) do
        if model_monster:getType(info[i]["id"]) == index then
			arr[table.getn(arr)+1] = info[i]
		end
	end
	return arr
end

--装备进阶，武将进阶和升星(显示小红点)
function HeroListScene:isShowRedDot(data)
    local canUp = false
    --装备是否可以进阶
    for j = 1,table.getn(data.equipment) do
        if canUp == false then
            canUp = helper_monster:canEquipmentQualityUp(data.id,data.equipment[j].id)
        end
        if canUp then
        	return true
        end
    end
    
    --武将是否可以进阶
    canUp = helper_monster:canMonsterQualityUp(data.id)
--    if canUp ~= 1 and canUp ~= 2 then
    if canUp.id == 0 then
    	return true
    end
    
    --武将是否可以升星
    canUp = helper_monster:canMonsterStarUp(data.id)
    if canUp then
        return true
    end
    
    return false
end

function HeroListScene:callBack()
    --进阶成功回调/升星成功回调
--    if data.cmd =="MonstersQualityUpSuccess" or "MonstersStarUpSuccess" then
        --自己所有的武将列表
        self.allActiveMonsters = model_monsterStorage:getAllActiveMonsters()
        for i = 1,table.getn(self.allActiveMonsters) do
            self:setListInfo(self.itemsArr[table.getn(self.allInactiveCanMergeMonsters) + i],i-1)
        end
--    end
end

function HeroListScene:onMonsterMerge(success, data, code, msg)
    if(success) then
        model_monsterPartStorage:setCount(data.monster.id,data.monsterPart.count)
        self:updateListInfo(0)
    else
    end
end


return HeroListScene
