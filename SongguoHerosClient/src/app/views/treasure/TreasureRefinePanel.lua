
local TreasureRefinePanel = class("TreasureRefinePanel", cc.load("mvc").ViewBase)
TreasureRefinePanel.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
TreasureRefinePanel.STOPFRAME = 60
TreasureRefinePanel.RESOURCE_FILENAME = "Scene/TreasureRefinePanel.csb"
local model_static_library = _REQUIRE "model/static/model_static_library.lua"
_REQUIRE("pb/pbCommon.lua")
local model_itemStorage = _REQUIRE "model/model_itemStorage.lua"
local static_model_item = _REQUIRE "model/static/model_item.lua"
local static_model_treasureItemLevelType = _REQUIRE "model/static/model_treasureItemLevelType.lua"
local model_treasureItemStorage = _REQUIRE "model/model_treasureItemStorage.lua"
local helper_treasure = _REQUIRE "model/helper/helper_treasure.lua"
local static_model_treasureItem =  _REQUIRE "model/static/model_treasureItem.lua"
local model_extension_treasure = _REQUIRE "model/extension/model_extension_treasure.lua"
local model_TreasureItemQualityCoinType = _REQUIRE("model/static/model_TreasureItemQualityCoinType.lua")
local model_TreasureItemQualityCoinType = _REQUIRE "model/static/model_TreasureItemQualityCoinType.lua"
local porterty = _REQUIRE "common/AttributeName.lua"
local qualityItemId = 14
function TreasureRefinePanel:onCreate()
    release_print("========= WinPopupScene create");
    local Panel_TreasureRefine = self:getResourceNode():getChildByName("Panel_TreasureRefine")
    Panel_TreasureRefine:setTouchEnabled(true)
    local Panel_Bg=Panel_TreasureRefine:getChildByName("Panel_Bg")
    local Panel_C=Panel_TreasureRefine:getChildByName("Panel_C")
    local Panel_T=Panel_TreasureRefine:getChildByName("Panel_T")
    local Panel_B=Panel_TreasureRefine:getChildByName("Panel_B")
    self.Panel_Center=Panel_C:getChildByName("Panel_Center")
    self.Panel_Top=Panel_T:getChildByName("Panel_Top")
    self.Panel_Bottom=Panel_B:getChildByName("Panel_Bottom")
    SCREEN_TOP(Panel_T)
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_BOTTOM(Panel_B)
    local Button_Close = self.Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:sceneBack(function()
                self.dlgCallBack({cmd = "refine"})
                self:removeDlgNoAction()
            end)
        end

    end
    )
    local ProjectNode_Animation_TreasureUp2_End = self.Panel_Center:getChildByName("ProjectNode_Animation_TreasureUp2_End")
    ProjectNode_Animation_TreasureUp2_End:setVisible(false)
    self:initUi()
    
--
--    local Panel_Bottom_Base=Panel_Win:getChildByName("Panel_Bottom_Base")
--    local panel_Bottom=Panel_Bottom_Base:getChildByName("Panel_Bottom")
--    local  button_Retry=panel_Bottom:getChildByName("Button_Retry")
--    button_Retry:addTouchEventListener(handler(self,self.btnBackCallback)) 

    --release_print(_REQUIRE("battle/ChapterSceneData").type);
end

function TreasureRefinePanel:initUi()
    self.data = self.args[1]
    if self.args[2] then
        self.dlgCallBack = self.args[2]
    end
    local Text_Name3=self.Panel_Center:getChildByName("Text_Name3_0")
    local name = static_model_treasureItem:getName(self.data.treasureItemId)
    Text_Name3:setString(name)
    
    local ProjectNode_6=self.Panel_Center:getChildByName("ProjectNode_6")
    
    self:initItemCacha(ProjectNode_6:getChildByName("Panel_Object"),{type = pbCommon.ObjectType.TreasureItem,id = self.data.treasureItemId})
--    local Image_Treasure=self.Panel_Center:getChildByName("Image_Treasure")
--    Image_Treasure:loadTexture(model_static_library:getAsset(self.data.treasureItemId,pbCommon.ObjectType.TreasureItem))
    
    local Text_Cost=self.Panel_Bottom:getChildByName("Text_Cost")
    local Button_JL=self.Panel_Bottom:getChildByName("Button_Upgrade")
    local item = model_treasureItemStorage:getNode(self.data.inventoryTreasureItemId);
    local effects = helper_treasure:getQualityTypeDefines(self.data.inventoryTreasureItemId);
    local Panel_Item_1=self.Panel_Bottom:getChildByName("Panel_Item_1")
    local Panel_Item_2=self.Panel_Bottom:getChildByName("Panel_Item_2")
    Panel_Item_1:setVisible(false)
    Panel_Item_2:setVisible(false)
    local isMaxLvl = (effects == nil)
    self.quality = 0
    if effects ~= nil then
        local function initItem(currentCount,needCount,Node_Material,name,id,type)
            local Panel_Zhuangbei = Node_Material:getChildByName("Panel_Zhuangbei")
            local Image_Frame = Panel_Zhuangbei:getChildByName("Image_Frame")--背景
            local Image_Icon = Panel_Zhuangbei:getChildByName("Image_Icon")--图片


            local Text_1 = Panel_Zhuangbei:getChildByName("Text_1")--
            local Text_2 = Panel_Zhuangbei:getChildByName("Text_2")--
            local Text_3 = Panel_Zhuangbei:getChildByName("Text_3")--
            
            
            
            Text_1:setString("/"..needCount)
            Text_2:setString(currentCount)
            Text_3:setString(currentCount)
            Text_3:setVisible(currentCount < needCount)
            Text_2:setVisible(currentCount >= needCount)
            --Text_1:setPositionY(16)
            --                local Text_56 = Panel_Zhuangbei:getChildByName("Text_56")--
            --                Text_56:setString(currentCount.."/"..needCount)
            Image_Icon:loadTexture(model_static_library:getAsset(
                id,
                type))
            Image_Frame:loadTexture(model_static_library:getFrame(
                id,
                type))
            
            Image_Icon:setTouchEnabled(true)
            Image_Icon:addTouchEventListener(function(sender,eventType)
                if eventType == 2 then
                    if currentCount < needCount then
                        self:showDlg("hero/ChipDropScene",{id = id,type =  type,currentNum = currentCount,needNum = needCount})
                	end
                end
            end)
        end
        for key, value in pairs(effects) do
            if(static_model_treasureItemQualityType:isRequirement_needSelf(value)) then
            
                Panel_Item_2:setVisible(true)
                local excludeInventoryIdList = {};
                excludeInventoryIdList[item.inventoryTreasureItemId] = 1;
                local freeItems = model_treasureItemStorage:collectItemWithTypeId(item.treasureItemId, true, excludeInventoryIdList);
                local needCount = static_model_treasureItemQualityType:getRequirementCount(value);
                local currentCount = #freeItems
                if #freeItems ~= 0 then
                    self.quality = freeItems[1].quality
                else
                    self.quality = 0
                end
                local Node_Material_2=Panel_Item_2:getChildByName("Node_Material_2")
                initItem(
                    currentCount,
                    needCount,
                    Node_Material_2,
                    static_model_treasureItem:getName(self.data.treasureItemId),
                    self.data.treasureItemId,
                    pbCommon.ObjectType.TreasureItem)
                local Text_Name2=Panel_Item_2:getChildByName("Text_Name2")
                Text_Name2:setString(static_model_treasureItem:getName(self.data.treasureItemId))
            end

            if(static_model_treasureItemQualityType:isRequirement_needMaterial(value)) then
                Panel_Item_1:setVisible(true)
                local needCount = static_model_treasureItemQualityType:getRequirementCount(value);
               
                local currentCount = model_itemStorage:getCount(
                    pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_ID,
                    pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_TYPE);
                local Node_Material_1=Panel_Item_1:getChildByName("Node_Material_1")
                initItem(
                currentCount,
                needCount,
                Node_Material_1,
                static_model_item:getName(pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_ID),
                    pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_ID,
                    pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_TYPE)
                local Text_Name1=Panel_Item_1:getChildByName("Text_Name1")
                Text_Name1:setString(static_model_item:getName(pbCommon.TREASURE_ITEM_QUALITY_UP_REQUIRE_MATERIAL_ID))
            end
        end
    end
    
    
    
    
    local treasureItem = model_treasureItemStorage:getNode(self.data.inventoryTreasureItemId)
    local attributesQuality = model_extension_treasure:getAttributesQuality(treasureItem)
    local attributesNextQuality = model_extension_treasure:getAttributesNextQuality(treasureItem)
    local currentQuality = treasureItem.quality--进阶数 与颜色框的quality不一样
    local index = 1
    for i=1,2 do
        local Panel_Item=self.Panel_Center:getChildByName("Panel_Item_a_"..i)
        local Text_Item_1=Panel_Item:getChildByName("Text_Item_"..i)
        local quality
        if i == 1 then
            quality = currentQuality
        else
            quality = currentQuality + 1
        end
        Text_Item_1:setString(LANG("精炼%s阶",quality))
        for i=1,3 do
            local txt = Panel_Item:getChildByName("text_"..index)
            local text_bg = Panel_Item:getChildByName("text_bg_"..index)
            index = index + 1
            txt:setVisible(false)
            if text_bg then
                text_bg:setVisible(false)
            end
            
        end
    end
    

    local strNum = 0
    local Panel_Item_a_1=self.Panel_Center:getChildByName("Panel_Item_a_1")
    for key,value in pairs(attributesQuality) do
        strNum= strNum + 1
        local attLevel = Panel_Item_a_1:getChildByName("text_"..strNum)
        local attLevelBg = Panel_Item_a_1:getChildByName("text_bg_"..strNum)
        attLevelBg:setVisible(true)
        attLevel:setVisible(true)
        attLevel:setString(porterty.data[key].."+"..value)
    end

    local strNextNum = 3
    local Panel_Item_a_2=self.Panel_Center:getChildByName("Panel_Item_a_2")
    if isMaxLvl == false then
        for key,value in pairs(attributesNextQuality) do
            strNextNum= strNextNum + 1
            local attLevel = Panel_Item_a_2:getChildByName("text_"..strNextNum)
            attLevel:setVisible(true)
            attLevel:setString(porterty.data[key].."+"..value)
        end
    end
    
    
    local Text_Cost=self.Panel_Bottom:getChildByName("Text_Cost")
    local Image_Icon_Cash=self.Panel_Bottom:getChildByName("Image_Icon_Cash")
    Text_Cost:setVisible(isMaxLvl == false)
    Image_Icon_Cash:setVisible(isMaxLvl == false)
    local levelUpType = static_model_treasureItem:getLevelUpType(self.data.treasureItemId)
    local coin = model_TreasureItemQualityCoinType:getCoin(levelUpType,currentQuality+1)
    if isMaxLvl == false then
        Text_Cost:setString(coin)
        if tonumber(model_user.coin) < tonumber(coin) then
            Text_Cost:setColor(cc.c3b(255,0,0))
        else
            Text_Cost:setColor(cc.c3b(255,255,255))
        end
    end
    
    local ProjectNode_7 = self.Panel_Bottom:getChildByName("ProjectNode_7")
    ProjectNode_7:setVisible(false)
     
    
    self:initTopBar(self.Panel_Top:getChildByName("Node_Top"))
    local Panel_Item_a_2=self.Panel_Center:getChildByName("Panel_Item_a_2")
    local text_5 = Panel_Item_a_2:getChildByName("text_5")
    local Text_Item_2=Panel_Item_a_2:getChildByName("Text_Item_2")
    Text_Item_2:setVisible(isMaxLvl == false)
    if isMaxLvl == true then
        text_5:setString(LANG("达到最高等级"))
        text_5:setPositionY(120)
        text_5:setVisible(true)
        --Button_JL:setTitleText(LANG("达到最高等级"))
    else
        text_5:setPositionY(102)
        --Button_JL:setTitleText(LANG("精炼"))
    end
    
    local Button_Cut1 = self.Panel_Center:getChildByName("Button_Cut1")
    local Button_Cut2 = self.Panel_Center:getChildByName("Button_Cut2")
    self.buttons = {}
    self.buttons[1] = Button_Cut2
    self.buttons[2] = Button_Cut1
    self:setIndex(1)
    Button_Cut1:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            self:sceneBack(function()
                self:showDlg("treasure/TreasureStrongPanel",treasureItem,self.dlgCallBack)
                self:removeDlgNoAction()
            end)
           
        end
    end)
    
    
    Button_JL:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
        
            local ProjectNode_Animation_TreasureUp2_End = self.Panel_Center:getChildByName("ProjectNode_Animation_TreasureUp2_End")
            if isMaxLvl == true then
                PopMsg.getInstance():flashShow(LANG("达到最高等级"))
                return
            end
            local canQualityUp , id ,type,needCount = helper_treasure:canQualityUp(self.data.inventoryTreasureItemId)
            if canQualityUp then
                if tonumber(model_user.coin) < tonumber(coin) then
                    PopMsg.getInstance():flashShow(LANG("银币不足"))
                    return
                end
                
                local function update()
                    ctlMonstersTreasureItemQualityUpRequest:sendRequest(
                        function(sucess,data)
                            if sucess then
                                if self.upNode == nil and self.upAction ==nil  then
                                    local node, action = self:createNode("Node/Node_Animation_TreasureUp1_Sart.csb",false)
                                    self.upNode = node
                                    self.upAction = action
                                    self.Panel_Bottom:addChild(self.upNode)
                                end
                                self.upAction:gotoFrameAndPlay(55,false)
                                self.upNode:setPosition(ProjectNode_7:getPosition())
                                local Panel_Left_Light = self.upNode:getChildByName("Panel_Left_Light")
                                local Panel_Right_Light = self.upNode:getChildByName("Panel_Right_Light")
                                Panel_Right_Light:setVisible(#effects > 1)
                                if self.timerId ~= 0 and self.timerId ~=nil then
                                    Timer:kill(self.timerId)
                                end
                                local p = cc.p(73,327)
                                p.y = p.y -  (CC_DESIGN_RESOLUTION.height-CC_SCREEN_SIZE.height)/2
                                --local px,py = self.Panel_Top:getPosition()
                                self.timerId = Timer:runOnce(function()
                                    Panel_Left_Light:runAction(
                                        cc.Sequence:create(
                                            cc.EaseIn:create(cc.MoveTo:create(.5,cc.p(p.x,p.y)),5),
                                            cc.CallFunc:create(function()
                                                --currentNode:setVisible(false)
                                                self.upAction:gotoFrameAndPlay(66,false)
                                                if self.upNodeEnd == nil and self.upActionEnd ==nil  then
                                                    local node, action = self:createNode("Node/Node_Animation_TreasureUp2_End.csb",false)
                                                    self.upNodeEnd = node
                                                    self.upActionEnd = action
                                                    self.Panel_Center:addChild(self.upNodeEnd)
                                                    self.upNodeEnd:setPosition(ProjectNode_Animation_TreasureUp2_End:getPosition())
                                                end
                                                self.upActionEnd:gotoFrameAndPlay(90,false)
                                                self:initUi()
                                            end)
                                        ))
                                    if #effects > 1 then
                                        Panel_Right_Light:runAction(
                                            cc.Sequence:create(
                                                cc.EaseIn:create(cc.MoveTo:create(.5,cc.p(p.x,p.y)),5)
                                            ))
                                    end

                                end,0.2)
                            end
                        end, self.data.inventoryTreasureItemId);
                end
                
--                if self.quality ~= 0 then
--                    self:showDlg("common/CommonPopScene",{confirmStr=LANG("继续"),cancelStr=LANG("取消"),text=LANG("材料中的宝物已经被精炼过，是否继续？"),
--                        fun=function(data)
--                            if data.cmd=="confirm" then
--                                update()
--                            elseif data.cmd =="cancel" then 
--
--                            end
--                        end})
--                    return 
--                end
                update()
               
            else
                self:showDlg("hero/ChipDropScene",{id = id,type =  type,needNum = needCount})
                --PopMsg.getInstance():flashShow(LANG("所需精炼材料不足"))	
            end
        end

    end
    )
end

return TreasureRefinePanel
