--竞技场武将信息界面
local ArenaHeroInfo = class("ArenaHeroInfo", cc.load("mvc").ViewBase)
ArenaHeroInfo.NEXTSCENE = "battle/ChapterScene"
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local model_treasureItem =  _REQUIRE "model/static/model_treasureItem.lua"
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_equipment = _REQUIRE("model/static/model_equipment.lua")

ArenaHeroInfo.RESOURCE_FILENAME = "Scene/ArenaHeroInfo.csb"
function ArenaHeroInfo:onCreate()
    self.data = self.args[1]
    self.dlData = self.args[2]--回调数据
    self.index = 1;
    self:initUi()
end

function ArenaHeroInfo:initUi()
    local Panel_ArenaHeroInfo = self:getResourceNode():getChildByName("Panel_ArenaHeroInfo")
    local Panel_Center = Panel_ArenaHeroInfo:getChildByName("Panel_Center")
    local Panel_Left = Panel_Center:getChildByName("Panel_Left")
    self.Panel_Right = Panel_Center:getChildByName("Panel_Right")
    local Panel_Top = Panel_ArenaHeroInfo:getChildByName("Panel_Top")
    local Panel_Bg = Panel_ArenaHeroInfo:getChildByName("Panel_Bg")
--    self.panel_fight = Panel_Center:getChildByName("panel_fight")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    --返回按钮
    local  Button_Back=Panel_Top:getChildByName("Button_Back")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
--            SWITSCENE("arena/RankScene",self.dlData)
            self:removeDlgNoAction()
        end
    end
    )
    
    --私聊按钮
    local Button_Chat = self.Panel_Right:getChildByName("Button_Chat")
    Button_Chat:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            release_print("aaaaaaaaaaaaaaaa")
        end
    end
    )
    
    --珍宝列表
    local scrollView = Panel_Left:getChildByName("ScrollView_1")
    local node,item
    node=self:createNode("Node/Node_item_cacha.csb")
    item = node:getChildByName("Panel_Object")
    local contentSize =item:getContentSize()

    self.treasureItems = {}
    self.space = 10
    local num = table.getn(self.data.monsters[self.index].treasureItems)--4
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*math.ceil(num/2)));
    scrollView:setInnerContainerSize(sSize)
    for i=1,num do
        local newItem = item:clone()
        --newItem:setAnchorPoint(cc.p(0,0))
--        newItem:setTouchEnabled(true);
--        newItem:addTouchEventListener(function(sender,eventType)
--            if eventType ==2 then
--                
--            end
--        end
--        )
        local index = i-1
        newItem:setTag(index);
        scrollView:addChild(newItem)
        local x=(sSize.width/2)/2.0+sSize.width/2*(index%2);
        local y=sSize.height-(contentSize.height+self.space)*math.floor(index/2)-contentSize.height/2
        newItem:setPosition(cc.p(x,y))
        self.treasureItems[i] = newItem
    end
    
    --武将列表
    self:roleListInfo()
    self:refreshPanel()
end

function ArenaHeroInfo:roleListInfo()
    local scrollView2 = self.Panel_Right:getChildByName("ScrollView_2")
    local node2,item2
    node2=self:createNode("Node/Node_RisingStar_Head.csb")
    item2 = node2:getChildByName("Panel_RisingStar_Head")
    local contentSize2 =item2:getContentSize()

    self.heroItems = {}
    self.heroSpace = 2
    local heroNum = table.getn(self.data.monsters)
    local sSize1=cc.size(math.max(scrollView2:getInnerContainerSize().width,(contentSize2.width+self.heroSpace)*heroNum),scrollView2:getContentSize().height);
    scrollView2:setInnerContainerSize(sSize1)
    for j=1,heroNum do
        local newItem2 = item2:clone()
        --点击此人物，上面的人物进行替换，并且信息也进行替换
        newItem2:setTouchEnabled(true);
        newItem2:addTouchEventListener(function(sender,eventType)
            if eventType == 2 then 
                self.index = sender:getTag()+1
                self:refreshPanel()
            end
        end
        )
        local index = j-1
        newItem2:setTag(index);
        scrollView2:addChild(newItem2)
        local x2= self.heroSpace+contentSize2.width/2+(self.heroSpace+contentSize2.width)*index
        local y2=sSize1.height/2 --- 15
        newItem2:setPosition(cc.p(x2,y2))
        self.heroItems[j] = newItem2
        
        --武将信息显示
        local itemData = self.data.monsters[j]
        local Image_Hero = newItem2:getChildByName("Image_Hero")
        local Image_Head_bg = newItem2:getChildByName("Image_Head_bg")
        local Panel_Star = newItem2:getChildByName("Panel_Star")
        Image_Hero:loadTexture(model_monster:getIcon(itemData["id"]))
--        Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(itemData["quality"]))
        --星级显示
        for k = 1,5 do
            Panel_Star:getChildByName("Image_Star_"..k):setVisible(k<= itemData.star)
        end
    end
end

--更新数据
function ArenaHeroInfo:refreshPanel()
    --右边武将信息显示
    self.Panel_Right:getChildByName("Image_Icon_Num"):setVisible(false)
    self.equipItems = {}
    local Text_1 = self.Panel_Right:getChildByName("Text_1")
    local showData = self.data.monsters[self.index]
    Text_1:setString("LV:".. showData.level .. "\n"..LANG("战力:" ))
    for t = 1,6 do
        local node_item = self.Panel_Right:getChildByName("Node_Item"..t)
        self.equipItems[t] = node_item
        if t<= table.getn(showData.equipment) then
            node_item:setVisible(true)
--            node_item:getChildByName("Panel_Object"):getChildByName("Image_1"):loadTexture(model_monster:getIcon(showData["id"]))
--            node_item:getChildByName("Panel_Object"):getChildByName("Image_2"):loadTexture(model_monsterStorage:getFrameByQuality(showData["quality"]))
            node_item:getChildByName("Panel_Object"):getChildByName("Image_1"):loadTexture(model_equipment:getIcon(showData["id"],showData["quality"]))
            node_item:getChildByName("Panel_Object"):getChildByName("Image_2"):loadTexture(model_equipment:getFrameByQuality(showData["quality"]))
        else
            node_item:setVisible(false)
        end
    end
    
    local color,splits = model_monsterStorage:getColor(showData.id)
    self.Panel_Right:getChildByName("Text_name_9"):setColor(color)
    if table.getn(splits) == 2 then
        self.Panel_Right:getChildByName("Text_name_9"):setString(LANG("%s",model_monster:getName(showData.id).."+"..splits[2]))
    else
        self.Panel_Right:getChildByName("Text_name_9"):setString(LANG("%s",model_monster:getName(showData.id)))
    end
    
    for m = 1,5 do
        local starItem = self.Panel_Right:getChildByName("Image_Star_"..m)
        starItem:setVisible(showData.star >= m)
    end
	
	--珍宝信息
    local itemData = self.data.monsters[self.index].treasureItems
    for h = 1,table.getn(self.treasureItems) do
        local Image_1 = self.treasureItems[h]:getChildByName("Image_1")
        local Image_2 = self.treasureItems[h]:getChildByName("Image_2")
        if itemData[h].inventoryTreasureItemId ~= 0 then
            local treasureItem = model_treasureItemStorage:getNode(itemData[h].inventoryTreasureItemId)
--            Text_TreasureName:setString(LANG("%sLV%s",model_treasureItem:getName(treasureItem.treasureItemId),treasureItem.level))--珍宝名字
--            Text_TreasureName:setColor( model_static_library:getColor(treasureItem.treasureItemId,pbCommon.ObjectType.TreasureItem))--珍宝名字颜色
            Image_1:setVisible(true)
            Image_1:loadTexture(model_static_library:getAsset(treasureItem.treasureItemId,pbCommon.ObjectType.TreasureItem))
            Image_2:loadTexture(model_static_library:getFrame(treasureItem.treasureItemId,pbCommon.ObjectType.TreasureItem))
        else
            Image_1:setVisible(false)
            Image_2:loadTexture(model_static_library:getFrame(0,pbCommon.ObjectType.TreasureItem))
        end
	end
end

return ArenaHeroInfo
