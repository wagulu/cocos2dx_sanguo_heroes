
local TreasureDetailsPanel = class("TreasureDetailsPanel", cc.load("mvc").ViewBase)
TreasureDetailsPanel.RESOURCE_FILENAME = "Scene/TreasureDetailsPanel.csb"
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
local model_extension_treasure = _REQUIRE "model/extension/model_extension_treasure.lua"
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local model_treasureItem =  _REQUIRE "model/static/model_treasureItem.lua"
local static_model_monster =  _REQUIRE "model/static/model_monster.lua"
local model_extension_relationship_treasure = _REQUIRE "model/extension/model_extension_relationship_treasure.lua"
local porterty = _REQUIRE "common/AttributeName.lua"
local pbCommon = _REQUIRE "pbCommon.lua"

function TreasureDetailsPanel:onCreate()
    local Panel_TreasureDetails = self:getResourceNode():getChildByName("Panel_TreasureDetails")
    Panel_TreasureDetails:setTouchEnabled(true)
    local Panel_Bg = Panel_TreasureDetails:getChildByName("Panel_Bg")
    local Panel_C = Panel_TreasureDetails:getChildByName("Panel_C")
    local Panel_Center = Panel_C:getChildByName("Panel_Center")
    --local Panel_Top = Panel_TreasureDetails:getChildByName("Panel_Top")
    local scrollView = Panel_Center:getChildByName("ScrollView_1")
    --local callBack = self.args[1]
    local data = self.args[1]
    local monsterId =  self.args[2]
    self.dlgCallBack = self.args[3]
    self.type = self.args[4]
    self.from = self.args[5]
    --SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    --  点击空白关闭
    Panel_TreasureDetails:setTouchEnabled(true)
    self:addBtnTouchEventListener(Panel_TreasureDetails,function(sender,eventType)
        if eventType == 2 then
            self:sceneBack(function()
                self:removeDlgNoAction()
            end)
            
        end
    end)
--    local Button_Close = Panel_Center:getChildByName("Button_Close_0")
--    Button_Close:addTouchEventListener(function(sender,eventType)
--        if eventType ==2 then
--            self:removeDlgNoAction()
--        end
--
--    end
--    )
    --    inventoryTreasureItemId     背包中的唯一ID，相同史记不同ID
    --    slotId                      珍宝所在的位置
    --    treasureItemId              静态表中的ID
    local treasureItem = model_treasureItemStorage:getNode(data.inventoryTreasureItemId)

    local Image_Bg_Brown =  scrollView:getChildByName("Image_Bg_Brown")
    local Node_Treasure_Match =  scrollView:getChildByName("Node_Treasure_Match")
    local Text_Title_1 =  scrollView:getChildByName("Text_Title_1")
    local Text_Attribute =  scrollView:getChildByName("Text_Attribute")
    local Button_SXXL_0 =  Image_Bg_Brown:getChildByName("Button_SXXL_0")
    local Button_SXXL =  Image_Bg_Brown:getChildByName("Button_SXXL")
    local Node_Treasure_Icon =  Image_Bg_Brown:getChildByName("Node_Treasure_Icon")         --珍宝图标
    local Panel_Treasure_Icon =  Node_Treasure_Icon:getChildByName("Panel_Treasure_Icon")
--    local Image_Bg =  Panel_Treasure_Icon:getChildByName("Image_Bg")
--    local Image_Icon =  Panel_Treasure_Icon:getChildByName("Image_Icon")
--    local Text_2 =  Panel_Treasure_Icon:getChildByName("Text_2")
    local Text_TreasureName =  Image_Bg_Brown:getChildByName("Text_TreasureName")           --珍宝名字
    local Imag_bg = scrollView:getChildByName("Imag_bg")
    Image_Bg_Brown:setVisible(false)


    Image_Bg_Brown:removeSelf()
    Node_Treasure_Match:removeSelf()
    Text_Title_1:removeSelf()
    Text_Attribute:removeSelf()
    Button_SXXL_0:removeSelf()
    Button_SXXL:removeSelf()
    Node_Treasure_Icon:removeSelf()
    Text_TreasureName:removeSelf()
    Imag_bg:removeSelf()

    local originSize = Imag_bg:getContentSize()
    local bgWidth = scrollView:getInnerContainerSize().width - 16
    local space = 20
    local buttonSize = Button_SXXL_0:getContentSize()
    local treasureName = model_static_library:getName(data.treasureItemId,pbCommon.ObjectType.TreasureItem)
    local titles = {treasureName, "强化LV"..data.level, "精炼"..data.quality.."阶", "珍宝套装", "珍宝配缘"}

    local attributesLevel = model_extension_treasure:getAttributesLevel(treasureItem)
    local attributesQui = model_extension_treasure:getAttributesQuality(treasureItem)

    local levelLen = 0
    local quaLen = 0
    local suitLen = 0

    for key,value in pairs(attributesLevel) do
        levelLen = levelLen + 1
    end

    for key,value in pairs(attributesQui) do
        quaLen = quaLen + 1
    end
    local suitId = model_treasureItem:getSuitId(treasureItem.treasureItemId)
    local totalTreasureSuits = model_treasureItem:getSuits(suitId)
    local treasureSuit = {}
    local totalSuit
    if self.from ~= nil and self.from == 1 then
        totalSuit = model_extension_treasure:getSuitPropNormal(treasureItem.treasureItemId)
    else
        treasureSuit,totalSuit = model_extension_treasure:getSuitProp(monsterId,treasureItem.treasureItemId)  --套装效果
    end

    local currentSuitLen = #treasureSuit
    local totalSuitLen = #totalTreasureSuits
    for key,value in pairs(totalSuit) do
        suitLen = suitLen + 1
    end

    local relationTreasure = model_extension_relationship_treasure:getRelationshipMonster(treasureItem.treasureItemId)
    local matchLen = #relationTreasure
    local txtSpace = 40
    self.size2 = (levelLen+1)*txtSpace +40

    self.size3 = (quaLen+1)*txtSpace +40 + 40
    if totalSuitLen > 0 then
        self.size4 = (suitLen+1)*txtSpace + Panel_Treasure_Icon:getContentSize().height +20
    else
        self.size4 = (suitLen+1)*txtSpace
    end
    self.size5 = txtSpace + (Panel_Treasure_Icon:getContentSize().height)*math.ceil(matchLen/4) +60



    local txtDesc = Text_Attribute:clone()
    txtDesc:setString(model_treasureItem:getDesc(treasureItem.treasureItemId))
    local strLen = txtDesc:getStringLength()
    local size = txtDesc:getVirtualRendererSize()
    local txtDesWidth = bgWidth/2 + 40
    local txtHeight = math.ceil(size.width/(txtDesWidth)) * size.height
    txtDesc:setContentSize(cc.size(txtDesWidth, txtHeight))
    self.size1 = math.max((txtHeight + 75), (Panel_Treasure_Icon:getContentSize().height + 40))

    local innerHeight = 0
    for i=1,5 do
        innerHeight = innerHeight+space +self["size"..i]
    end
    innerHeight = innerHeight + #(titles) * txtSpace

    local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,innerHeight));
    local currentY = sSize.height
    scrollView:setInnerContainerSize(sSize)
    local bg = Imag_bg:clone()
    scrollView:addChild(bg)
    bg:setContentSize(cc.size(scrollView:getInnerContainerSize().width,innerHeight))
    bg:setPosition(cc.p(originSize.width/2,currentY))

    for i =1,#titles do
        if i == 1 then
            currentY = currentY - space - space
        else
            currentY = currentY - self["size"..(i-1)] - space
        end
        --标题
        local title = Text_Title_1:clone()
        title:setString(titles[i])
        title:ignoreContentAdaptWithSize(true)
        title:setAnchorPoint(cc.p(0.5,0.5))
        title:setPosition(cc.p(sSize.width/2,currentY))
        title:setVisible(true)
        scrollView:addChild(title)

        currentY = currentY - space

        self["bg"..i] = Imag_bg:clone()
        scrollView:addChild(self["bg"..i])
        self["bg"..i]:setVisible(true)
        self["bg"..i]:setContentSize(cc.size(bgWidth,self["size"..i]))
        self["bg"..i]:setPosition(cc.p(originSize.width/2,currentY))
    end

    --珍宝信息

    local Node_Treasure = Panel_Treasure_Icon:clone()
    Node_Treasure:getChildByName("Image_Icon"):loadTexture(model_static_library:getAsset(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
    Node_Treasure:getChildByName("Image_Bg"):loadTexture(model_static_library:getFrame(data.treasureItemId,pbCommon.ObjectType.TreasureItem))
    Node_Treasure:getChildByName("Text_2"):setString(LANG(model_static_library:getName(data.treasureItemId,pbCommon.ObjectType.TreasureItem)))
    self.bg1:addChild(Node_Treasure)
    local bg1Size = self.bg1:getContentSize()
    local treasureSize = Node_Treasure:getContentSize()
    Node_Treasure:setVisible(true)
    Node_Treasure:setPosition(treasureSize.width/2 + 20, bg1Size.height - treasureSize.height/2 - 20)
    self.bg1:addChild(txtDesc)
    self.bg1:setVisible(true)
    local desSize = txtDesc:getContentSize()
    txtDesc:setPosition(bg1Size.width - desSize.width - 40, bg1Size.height - desSize.height - 20)

    Button_SXXL_0:setVisible(self.type == nil) --type不传才显示
    Button_SXXL:setVisible(self.type == nil)
    self.bg1:addChild(Button_SXXL_0)
    self.bg1:addChild(Button_SXXL)
    Button_SXXL_0:setPosition(bg1Size.width - desSize.width - 40 + buttonSize.width/2, bg1Size.height - desSize.height - 20 - buttonSize.height)
    Button_SXXL:setPosition(bg1Size.width - buttonSize.width/2 - 20, bg1Size.height - desSize.height - 20 - buttonSize.height)

    Button_SXXL_0:addTouchEventListener(function(sender,eventType)--更换
        if  eventType == 2 then
            self:showDlg("treasure/TreasureListPanel",{1,data.slotId,monsterId,self.dlgCallBack})--选择珍宝
            self:removeDlgNoAction()
    end
    end)
    Button_SXXL:addTouchEventListener(function(sender,eventType)--卸下
        if  eventType == 2 then
            ctlMonstersChangeTreasureItemRequest:sendRequest(
                function(sucess,data)
                    if sucess then
                        self.dlgCallBack()
                        self:removeDlgNoAction()
                    end  
                end, monsterId, data.slotId, 0);

    end
    end)

    --强化属性
    local Button_qianghua =  Button_SXXL:clone()
    Button_qianghua:setTitleText(LANG("强化"))
    Button_qianghua:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            self:showDlg("treasure/TreasureStrongPanel",treasureItem,self.dlgCallBack)
            self:removeDlgNoAction()
        end
    end)
    local bg2Size = self.bg2:getContentSize()
    Button_qianghua:setPosition(cc.p(bg2Size.width-80,(bg2Size.height/2)))
    self.bg2:addChild(Button_qianghua)
    local numLevel =0
    for key,value in pairs(attributesLevel) do
        numLevel = numLevel + 1
        local txt = Text_Attribute:clone()
        txt:setString(porterty.data[key].."+"..value)
        --txt:setPosition(cc.p(sSize.width/2,self.size1-txtSpace*(i+1)))
        txt:setPosition(cc.p(40,self.size2-txtSpace*(numLevel)))
        self.bg2:addChild(txt)

    end

    --精炼属性
    local Button_jinlian =  Button_SXXL:clone()
    local bg3Size = self.bg3:getContentSize()
    Button_jinlian:setTitleText(LANG("精炼"))
    Button_jinlian:setPosition(cc.p(bg3Size.width-80,(bg3Size.height/2)))
    self.bg3:addChild(Button_jinlian)
    Button_jinlian:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            self:showDlg("treasure/TreasureRefinePanel",treasureItem,self.dlgCallBack)
            self:removeDlgNoAction()
        end
    end)
    local numQuality =0
    for key,value in pairs(attributesQui) do
        numQuality = numQuality + 1
        local txt = Text_Attribute:clone()
        txt:setString(porterty.data[key].."+"..value)
        txt:setPosition(cc.p(40,self.size2-txtSpace*(numQuality)))
        self.bg3:addChild(txt)

    end

    --珍宝套装
    local bg4Size = self.bg4:getContentSize()
    local iconNum =3
    local iconSpace = 0
    local startSpace = 65
    local scale =0.75
    for i=1,totalSuitLen do
        local suit = totalTreasureSuits[i]
        local Node_Treasure_IconClone = Panel_Treasure_Icon:clone()
        Node_Treasure_IconClone:setScale(scale)
        self.bg4:addChild(Node_Treasure_IconClone)
        local Image_Bg =  Node_Treasure_IconClone:getChildByName("Image_Bg")
        local Image_Icon =  Node_Treasure_IconClone:getChildByName("Image_Icon")
        Image_Icon:loadTexture(model_static_library:getAsset(suit.id,pbCommon.ObjectType.TreasureItem))
        Image_Bg:loadTexture(model_static_library:getFrame(suit.id,pbCommon.ObjectType.TreasureItem))
        local Text_2 =  Node_Treasure_IconClone:getChildByName("Text_2")
        Text_2:setString(suit.name_zhCN)
        Node_Treasure_IconClone:setPosition(cc.p(startSpace+(i-1)*(Node_Treasure_IconClone:getContentSize().width*scale+iconSpace),
            bg4Size.height -Node_Treasure_IconClone:getContentSize().height/2-10))
        local isHave = false
        for k=1,#treasureSuit do
            if treasureSuit[k].treasureItemId ==suit.id  then
                isHave = true
            end
        end
        if isHave == true then
            Text_2:setColor(cc.c3b(255,0,1))
        else
            Text_2:setColor(cc.c3b(255,255,255))
        end
    end
    local count =1
    for key,value in pairs(totalSuit) do
        local txt = Text_Attribute:clone()
        txt:setVisible(true)
        count = count +1
        txt:setString(LANG("%s件效果：%s",key,porterty.data[value[1].propertyName].."+"..value[1].propertyValue))
        --        if #treasureSuit > 0 then
        --            txt:setPosition(cc.p(50,self.size4-txtSpace*count-Panel_Treasure_Icon:getContentSize().height*scale))
        --        else
        --            txt:setPosition(cc.p(50,self.size4-txtSpace*count-20))
        --        end
        txt:setPosition(cc.p(50,self.size4-txtSpace*count-Panel_Treasure_Icon:getContentSize().height*scale))

        if key <= #treasureSuit then
            txt:setColor(cc.c3b(255,0,1))
        else
            txt:setColor(cc.c3b(255,255,255))
        end
        self.bg4:addChild(txt)
    end

    --珍宝配缘
    local bg5Size = self.bg5:getContentSize()
    local Panel_Treasure_Match = Node_Treasure_Match:getChildByName("Node_Team_Item")
    local row = math.ceil(matchLen / 4)
    local index =0
    for i = 1,row do
        for j = 1,4 do
            index = index + 1
            if index <= matchLen then
                local Node_Treasure_MatchClone = Panel_Treasure_Match:clone()
                self.bg5:addChild(Node_Treasure_MatchClone)
                local Image_Head_bg = Node_Treasure_MatchClone:getChildByName("Image_Head_bg")
                local Image_Hero = Node_Treasure_MatchClone:getChildByName("Image_Hero")
                Image_Hero:loadTexture(static_model_monster:getIcon(relationTreasure[index]))
                Image_Head_bg:loadTexture(static_model_monster:getFrame(relationTreasure[index]))
                Node_Treasure_MatchClone:setScale(scale)
                Node_Treasure_MatchClone:setPosition(cc.p(startSpace+(j-1)*(Node_Treasure_MatchClone:getContentSize().width*scale + iconSpace),
                    bg5Size.height -Node_Treasure_MatchClone:getContentSize().height/2 - 10 -(i-1)*(Node_Treasure_MatchClone:getContentSize().height)))
                local Text_2 = Node_Treasure_MatchClone:getChildByName("Text_2")
                Text_2:setString(static_model_monster:getName(relationTreasure[index]))
                local Panel_Star = Node_Treasure_MatchClone:getChildByName("Panel_Star")
                local Image__Job = Node_Treasure_MatchClone:getChildByName("Image__Job")
                Panel_Star:setVisible(false)
                Image__Job:setVisible(false)
            end

        end
    end

end

return TreasureDetailsPanel
