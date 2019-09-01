
local ChipDropScene = class("ChipDropScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStar = _REQUIRE("model/static/model_monsterStar.lua")
local model_monsterPartStorage = _REQUIRE("model/model_monsterPartStorage.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
local model_item = _REQUIRE("model/static/model_item.lua")
local model_dropWay = _REQUIRE("model/static/model_dropWay.lua")
local model_area = _REQUIRE "model/static/model_area.lua"
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua")
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local model_treasureItemPartStorage = _REQUIRE("model/model_treasureItemPartStorage.lua")
local model_treasureBoxStorage = _REQUIRE("model/model_treasureBoxStorage.lua")
ChipDropScene.RESOURCE_FILENAME = "Scene/ChipDropScene.csb"


function ChipDropScene:onCreate()
    self:initUi()
end

function ChipDropScene:initUi()
    self.data = self.args[1]
    self.id = self.data.id
    self.type = self.data.type
    self.needNum = self.data.needNum
    --    self.index = self.args[3] -- index 为-1的时候表示的是武将列表中不能合成武将

    local Panel_ChipDropScene = self:getResourceNode():getChildByName("Panel_ChipDropScene")
    Panel_ChipDropScene:setTouchEnabled(true)
    local Panel_Bg = Panel_ChipDropScene:getChildByName("Panel_Bg")
    local Panel_Center = Panel_ChipDropScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    --    local Panel_Top = Panel_ChipDropScene:getChildByName("Panel_Top")
    --    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)

    Panel_ChipDropScene:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end
    end
    )

    --scrollView
    local scrollView = Panel_Center:getChildByName("ScrollView_1")
    local contentSize =cc.size(425,96)
    local node=self:createNode("Node/Node_ChapterSelect.csb")
    local item = node:getChildByName("Panel_EQ_Item")
    self.space = 1
    self.items = model_dropWay:getItems(self.id,self.type)
    local num = #self.items 
    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*num));
    scrollView:setInnerContainerSize(sSize)

    --    for i=1,num do
    --        local obj = {typeId = 1,islandId = 1, areaId = 1, stageId = 1, desc = "点点滴滴"}
    --        self.items[#self.items + 1] = obj
    --    end

    for i=1,num do
        local newItem = item:clone()
        local itemData = self.items[i]
        local Button_Go = newItem:getChildByName("Button_Go")
        local Text_JY = newItem:getChildByName("Text_JY")
        local Text_Chapter = newItem:getChildByName("Text_Chapter")
        local Text_Chapter_Name = newItem:getChildByName("Text_Chapter_Name")
        local Text_Chapter_Num1 = newItem:getChildByName("Text_Chapter_Num1")
        local Text_Chapter_Num2 = newItem:getChildByName("Text_Chapter_Num2")
        Text_JY:setString(LANG(""))
        if itemData.typeId == 1 then
            Text_JY:setString(LANG("普通"))
        elseif itemData.typeId == 2 then
            Text_JY:setString(LANG("精英"))
        end
        local chapter
        local chapterName
        local stageName 
        local type
        if itemData.funcName == "chanpter" then
            chapter = model_area:getName(itemData.areaId)

            if itemData.typeId == 1 then
                type = pbCommon.StageType.NORMAL
            else
                type = pbCommon.StageType.MASTER
            end
            local stageModel = model_areaStorage:getStage(itemData.stageId,type)
            chapterName = stageModel.name_zhCN

            if type == pbCommon.StageType.MASTER then
                Text_Chapter_Num1:setString(LANG("（%s/3）",model_user.selectEliteStageLeftBattleTimes))
                Text_Chapter_Num2:setString(model_user.selectEliteStageLeftBattleTimes)
                Text_Chapter_Num2:setVisible(model_user.selectEliteStageLeftBattleTimes < 3)
            else
                Text_Chapter_Num1:setString("")
                Text_Chapter_Num2:setString("")
            end
            Button_Go:setVisible(model_areaStorage:isLockStage(itemData.stageId, type))
        else
            chapter = pbCommon.dropWayType[itemData.funcName].name
            chapterName = itemData.desc
            Text_Chapter_Num1:setString("")
            Text_Chapter_Num2:setString("")
            Button_Go:setVisible(true)
        end
        Text_Chapter:setString(chapter)
        Text_Chapter_Name:setString(chapterName)
        --        newItem:setTouchEnabled(true);
        Button_Go:addTouchEventListener(function(sender,eventType)
            if eventType == 2 then
                if itemData.funcName == "chanpter" then
                    helper_pve:initStageById(itemData.islandId,itemData.areaId,itemData.stageId,type,function(sucess,data)
                        if sucess then
                            GLOBAL_MY_APP:setSwitch(true)
                            GLOBAL_MY_APP:showDlg(pbCommon.dropWayType[itemData.funcName].sceneName)
                        end
                    end)
                else
                    ctlShopListRequest:sendRequest(function(sucess,data, code, msg)
                        GLOBAL_MY_APP:setSwitch(true)
                        SWITSCENE(pbCommon.dropWayType[itemData.funcName].sceneName,data)
                    end, pbCommon.ShopListType.ITEM);

                end

            end
        end
        )
        --确定哪个前往按钮不是隐藏的
        --        Button_Go:setVisible(false)
        --        if i == 3 then
        --            Button_Go:setVisible(true)
        --        end
        
        local index = i-1
        newItem:setTag(index);
        scrollView:addChild(newItem)
        local x=contentSize.width/2
        --local t = math.floor(index/2)
        local y=sSize.height-(contentSize.height)*index-contentSize.height/2
        newItem:setPosition(cc.p(x,y))
    end

    local heroIcon = Panel_Center:getChildByName("ProjectNode_1"):getChildByName("Panel_RisingStar_Head")
    local Image_Hero = heroIcon:getChildByName("Image_Hero")
    local Image_Head_bg = heroIcon:getChildByName("Image_Head_bg")
    local Image__Job = heroIcon:getChildByName("Image__Job")
    Image__Job:setVisible(false)

    heroIcon:getChildByName("Panel_Star"):setVisible(false)

    --需要碎片数量
    local needParts = self.needNum
    --当前碎片数量
    local currentParts
    local partName
    local model_things
    if self.type == pbCommon.ObjectType.Material then --材料
        model_things = model_materialStorage
    elseif self.type == pbCommon.ObjectType.TreasureItem then --宝物
        model_things = model_treasureItemStorage
    elseif self.type == pbCommon.ObjectType.MonsterPart then --武将碎片
        model_things = model_monsterPartStorage
    elseif self.type == pbCommon.ObjectType.TreasureBox then --TreasureBox
        model_things = model_treasureBoxStorage
    elseif self.type == pbCommon.ObjectType.TreasureItemPart then --宝物碎片
        model_things = model_treasureItemPartStorage
    else --if self.type == pbCommon.ObjectType.Item then--物品
        model_things = model_itemStorage
    end
    currentParts = model_things:getCount(self.data.id);
    --        needParts = 
    partName = model_static_library:getName(self.data["id"],self.type)
    Image_Hero:loadTexture(model_static_library:getAsset(self.data["id"],self.type))
    Image_Head_bg:loadTexture(model_static_library:getFrame(self.data["id"],self.type))
    Panel_Center:getChildByName("Text_Chapter_Num1"):setString("("..currentParts.."/"..needParts..")")
    Panel_Center:getChildByName("Text_Chapter_Num2"):setVisible(false)
    Panel_Center:getChildByName("Text_Title_JSMC"):setString(partName)
end

function ChipDropScene:refreshPage()
    self:initUi()
end

function ChipDropScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")
    end
end
return ChipDropScene
