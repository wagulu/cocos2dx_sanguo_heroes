--装备强化和装备进化界面
local EquipUpgradeScene = class("EquipUpgradeScene", cc.load("mvc").ViewBase)
EquipUpgradeScene.NEXTSCENE = "battle/ChapterScene"
local helper_monster = _REQUIRE("model/helper/helper_monster.lua")
local model_equipment = _REQUIRE("model/static/model_equipment.lua")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua");
local model_material = _REQUIRE("model/static/model_material.lua")
local model_extension_equipment = _REQUIRE("model/extension/model_extension_equipment.lua")
local porterty = _REQUIRE "common/AttributeName.lua"
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
--_REQUIRE "src/packages/packet/Packet_Regester"

EquipUpgradeScene.RESOURCE_FILENAME = "Scene/EquipUpgradeScene.csb"
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"
function EquipUpgradeScene:onCreate()
    self.data = self.args[1]
    self.heroInfo = self.args[2]
    self.fun = self.args[3]
    self.topEquipArr = {}--上面装备数组
    self.underEquipArr = {}--下面装备数组
    self.topEquipArr[1] = self.data
    self.chuiZiDH = false--小锤子动画
    self.successDH = false--强化成功动画
    self:initUi()
end

function EquipUpgradeScene:initUi()
    --记录此时上面显示装备个数
    self.equipNum = 1;--点击下面进化材料的时候每点击一次加1，每回点击一次上面的装备图标，则减1
    self.isRefreshPage = false
    local Panel_EquipUpgradeScene = self:getResourceNode():getChildByName("Panel_EquipUpgradeScene")
    local Panel_T = Panel_EquipUpgradeScene:getChildByName("Panel_T")
    local Panel_Bg = Panel_EquipUpgradeScene:getChildByName("Panel_Bg")
    self.Panel_Forging = Panel_EquipUpgradeScene:getChildByName("Panel_Forging")
    local ProjectNode_2 = self.Panel_Forging:getChildByName("ProjectNode_2")
    self.black_15 = self.Panel_Forging:getChildByName("black_15")
    self.Panel_Center = Panel_EquipUpgradeScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    self.Panel_Strong = self.Panel_Center:getChildByName("Panel_Strong")
    self.Panel_Node_Icon = self.Panel_Center:getChildByName("Panel_Node_Icon")
    self.Panel_equipList1 = self.Panel_Center:getChildByName("Panel_equipList1")
    self.Panel_equipList2 = self.Panel_Center:getChildByName("Panel_equipList2")
    local Panel_Zhuangbei = self.Panel_Strong:getChildByName("Node_Icon"):getChildByName("Panel_Zhuangbei")
    Panel_Zhuangbei:getChildByName("Text_1"):setVisible(false)
    Panel_Zhuangbei:getChildByName("Text_2"):setVisible(false)
    Panel_Zhuangbei:getChildByName("Text_3"):setVisible(false)
    Panel_EquipUpgradeScene:setTouchEnabled(true)
--    self.Panel_Forging:removeChild(ProjectNode_2)
    ProjectNode_2:setVisible(false)
    self.black_15:setVisible(false)

    SCREEN_TOP(Panel_T)
    SCREEN_SCALE_BG(Panel_Bg)
    local Node_Top=Panel_T:getChildByName("Panel_Top_8"):getChildByName("Node_Top_1")
    self:initTopBar(Node_Top)
    self.Panel_16 = Panel_T:getChildByName("Panel_Top_8"):getChildByName("Panel_16")
    local ProjectNode_1 = self.Panel_16:getChildByName("ProjectNode_1") 
--    self.Panel_16:removeChild(ProjectNode_1)
    ProjectNode_1:setVisible(false)
    local Button_Back=Panel_T:getChildByName("Panel_Top_8"):getChildByName("Button_Back_6")
    Button_Back:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self.fun({cmd="MonstersEquipmentQualityUpSuccess"})
            self:removeDlgNoAction()
        end
--        SWITSCENE("hero/HeroInfoScene")
    end
    )
    --强化一次按钮
    local Button_QH = self.Panel_Strong:getChildByName("Button_QH")
    Button_QH:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:setEquipmentLevelUp(1)
        end
    end
    )
    --强化五次按钮
    local Button_QH5 = self.Panel_Strong:getChildByName("Button_QH5")
    Button_QH5:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            local heroLevel = self.heroInfo.level
            local equipLevel = self.data.level
            if heroLevel - equipLevel < 5 then
                self:setEquipmentLevelUp(heroLevel - equipLevel)
            else
                self:setEquipmentLevelUp(5)
            end
        end
    end
    )
    --装备合成/进阶按钮
    self.Button_HC = self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Button_HC")
    self.Button_HC:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            --装备合成
            if self.statusIndex == 0 then
                --self.mergeId   当前要合成的装备的id值
                ctlMonstersEquipmentMaterialMergeRequest:sendRequest(handler(self,self.onMonstersEquipmentMaterialMerge),self.mergeId);
            else--装备进阶
                if self.isEnough == true then
                    ctlMonstersEquipmentQualityUpRequest:sendRequest(handler(self,self.onMonstersEquipmentQualityUp),self.heroInfo.id,self.data.id);
                else
                    local isCanHC = helper_monster:hasEnoughEquipmentDependence(self.heroInfo.id,self.data.id,true)
                    if isCanHC == true then
                        PopMsg.getInstance():flashShow(LANG("材料尚未合成，请备齐材料后重试"))
                    else
                        PopMsg.getInstance():flashShow(LANG("材料不足，请集齐材料后重试"))
                    end
                end
                
            end
        end
    end
    )
    
    --装备合成按钮
    local Button_HC_0 = self.Panel_Center:getChildByName("Panel_Evolvement_1"):getChildByName("Button_HC_0")
    Button_HC_0:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            ctlMonstersEquipmentMaterialMergeRequest:sendRequest(handler(self,self.onMonstersEquipmentMaterialMerge),self.mergeId);
        end
    end
    )
    
    self.buttons = {}
    for i=1,2 do
        local btn = self.Panel_Center:getChildByName("Button_Select_"..i)
        self.buttons[i] = btn
    end
    
    self.Panel_equipList1:setVisible(false)
    self.Panel_equipList2:setVisible(false)
    
    --如果可进阶，则显示进阶的tab页内容,否则是强化的tab页
    local bool = helper_monster:canEquipmentQualityUp(self.heroInfo.id,self.data.id)
    if bool==true then 
        self:setIndex(2)
    else
        self:setIndex(1)
    end

    --添加侦听事件
    self:setAddEventListener()
end
--装备强化
function EquipUpgradeScene:setEquipmentLevelUp(index)
    if self.data.level == self.heroInfo.level then
        PopMsg.getInstance():flashShow(LANG("强化等级已达上限，请提升武将等级"))
    else
        if self.data.nextLevelCoin > model_user.coin then
            self:showDlg("common/CommonPopScene",{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text=LANG("银两不足"),fun=function(data)
                if data.cmd=="confirm" then
                elseif data.cmd =="cancel" then 
                end
            end})
        else
            self.chuiZiDH = true
            self.black_15:setVisible(true)
            --增加小锤子动画
            if not self.chuiZiNodeAction then
                local node,nodeAction=self:createNode("Node/Node_Forging.csb")
                self.chuiZiNodeAction = nodeAction
                self.chuiZiNode = node
                self.chuiZiNodeAction:gotoFrameAndPlay(0,false)
                self.Panel_Forging:addChild(node)
                node:setPosition(cc.p(106.04,98.67))
            else 
                self.chuiZiNode:setVisible(true)
                self.chuiZiNodeAction:gotoFrameAndPlay(0,false)
            end
            self.currentEquipmentLevel = self.data.level
            self.currentEquipmentIndex = index
--            ctlMonstersEquipmentLevelUpRequest:sendRequest(handler(self,self.onMonstersEquipmentLevelUp),self.heroInfo.id,self.data.id,index);
        end
    end
    
end

function EquipUpgradeScene:update()
    self:updateHandler()
    if self.chuiZiDH == true and self.chuiZiNodeAction:isPlaying() == false then
        self.chuiZiDH = false
        self.black_15:setVisible(false)
        self.chuiZiNode:setVisible(false)
        ctlMonstersEquipmentLevelUpRequest:sendRequest(handler(self,self.onMonstersEquipmentLevelUp),self.heroInfo.id,self.data.id,self.currentEquipmentIndex);
    elseif self.successDH == true and self.successNodeAction:isPlaying() == false then
        
    end
end

function EquipUpgradeScene:setIndex(index)
    --进阶tab上面的小红点的位置
    local image_1 = self.Panel_Center:getChildByName("Image_1")
    local changeX = 0
    if index == 2 then --进阶
        changeX = image_1:getPositionX() - 10
        image_1:setPosition(changeX,image_1:getPositionY())
    else
        changeX = image_1:getPositionX() + 10
        image_1:setPosition(changeX,image_1:getPositionY())
    end
    
    self.index = index
    if index == 2 then
        self.equipNum = 1
    elseif index == 3 then
        self.equipNum = 2
    else
    end

    --根据选择tab显示界面
    self:setPanelData(index)
    for i = 1,#self.buttons do
        local  btn = self.buttons[i] 
        btn:setTag(i)
        btn:setBright(i~=index)
       
        btn:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                self:setIndex(sender:getTag())
            end
        end
        )
    end
    
end
--index:1为强化界面，2为进阶界面，3为合成界面，4为掉落界面
--（3和4在点击最下面的材料的时候判断并且调用）
function EquipUpgradeScene:setPanelData(index)
    self.Panel_equipList1:setVisible(false)
    self.Panel_equipList2:setVisible(false)
    if index == 1 then
        for i=0,2 do 
            self:setImageVisible(false)
            self.Panel_Center:getChildByName("Panel_Evolvement_"..i):setVisible(false)
        end 
        self.Panel_Strong:getChildByName("Text_Name_Zhuangbei"):setString(LANG(model_equipment:getName(self.data.id)))
        self.Panel_Strong:getChildByName("Text_Level_1"):setString("LV "..self.data.level)
        self.Panel_Strong:getChildByName("Text_Level_2"):setString("LV "..(self.data.level+1))
        self.Panel_Strong:getChildByName("Button_QH"):getChildByName("Text_Cost_0"):setString(self.data.nextLevelCoin)
        
        local equipIcon = self.Panel_Strong:getChildByName("Node_Icon"):getChildByName("Panel_Zhuangbei")
        local Image_Icon = equipIcon:getChildByName("Image_Icon")
        local Image_Frame = equipIcon:getChildByName("Image_Frame")
        Image_Icon:loadTexture(model_equipment:getIcon(self.data["id"],self.data["quality"]))
        Image_Frame:loadTexture(model_equipment:getFrameByQuality(self.data["quality"]))
        --强化属性显示
        local currentIfo = model_extension_equipment:getAttributes(self.data);
        local nextLevelInfo = model_extension_equipment:getAttributesNextLevel(self.data)
--        local nextQualityInfo = model_extension_equipment:getAttributesNextQuality(self.data)
        local infoNum = 1;
        for key, value in pairs(nextLevelInfo) do
--            if currentIfo[key] then
--            	self.Panel_Strong:getChildByName("Text_Shuxing_"..infoNum):setString(LANG(key..":"..currentIfo[key]))
--            else
--                self.Panel_Strong:getChildByName("Text_Shuxing_"..infoNum):setString(LANG(key..":0"))
--            end
            if currentIfo[key] then
                self.Panel_Strong:getChildByName("Text_Shuxing_"..infoNum):setString(LANG("%s%s",porterty.data[key],currentIfo[key]))
            else
                self.Panel_Strong:getChildByName("Text_Shuxing_"..infoNum):setString(LANG("%s0",porterty.data[key]))
            end
            self.Panel_Strong:getChildByName("Text_Shuxing_"..(infoNum+6)):setString(LANG("%s%s",porterty.data[key],value))
            infoNum = infoNum+1;
        end
        
        for p = 1,6 do
            self.Panel_Strong:getChildByName("Text_Shuxing_"..p):setVisible(p<infoNum)
            self.Panel_Strong:getChildByName("Text_Shuxing_"..(p+6)):setVisible(p<infoNum)
            self.Panel_Strong:getChildByName("Sprite_Arrow_"..p):setVisible(p<infoNum)

        end
    else
        if index == 4 then
--            self:setImageVisible(false)
        else
            self:setImageVisible(true)
            --装备或者材料的名字
            if self.equipNum == 1 then
                self.Panel_Center:getChildByName("Text_ZBMC"):setString(model_equipment:getName(self.topEquipArr[self.equipNum].id))
            else
                self.Panel_Center:getChildByName("Text_ZBMC"):setString(model_material:getName(self.topEquipArr[self.equipNum].needMaterialId))
            end
        end
        
        for j=0,2 do--上面装备数据
            self.Panel_Center:getChildByName("Panel_Evolvement_"..j):setVisible(j == (index-2))
            self.Panel_Node_Icon:getChildByName("Node_Icon_"..j):setVisible(j <= (self.equipNum-1))
            self.Panel_Node_Icon:getChildByName("Node_Icon_"..j):getChildByName("Sprite_Arrow_Brown"..j):setVisible(j == (self.equipNum-1))
               
            if self.equipNum > j then
                local equipIcon = self.Panel_Node_Icon:getChildByName("Node_Icon_"..j):getChildByName("Panel_Zhuangbei")
                local Image_Icon = equipIcon:getChildByName("Image_Icon")
                local Image_Frame = equipIcon:getChildByName("Image_Frame")
                if j == 0 then--只有第一个是装备，所以只有第一个显示颜色框（此处材料图片不显示颜色框和图片）
                    Image_Icon:loadTexture(model_equipment:getIcon(self.topEquipArr[j+1]["id"],self.topEquipArr[j+1]["quality"]))
                    Image_Frame:loadTexture(model_equipment:getFrameByQuality(self.topEquipArr[j+1]["quality"]))
                else
                    local str
                    if j == 1 then
                    	str = "needMaterialId"
                    else
                        str = "id"
                    end
                    local quality = model_material:getQuality(self.topEquipArr[j+1][str])
                    local asset = model_material:getAsset(self.topEquipArr[j+1][str])
                    Image_Icon:loadTexture(LANG("items/Material/Material_"..asset..".png")) 
                    Image_Frame:loadTexture(LANG("items/frame/frame_%d.png",quality))
                end
                
            end 
        end
        self.isEnough = true
        local Panel_Evolvement_0 = self.Panel_Center:getChildByName("Panel_Evolvement_0")
        if index == 2 then--进阶界面
            self:recoverIcon()
            self.Panel_Center:getChildByName("Text_CL"):setString(LANG("进阶材料"))
            self.mergeId = self.data.id--合成id
            local num = table.getn(self.data.nextQualityMaterial)
            self:setPage(num)--判断是否有进阶或者合成材料
            self.Panel_equipList1:setVisible(num <= 2)
            self.Panel_equipList2:setVisible(num > 2)
            local index
            local panel_equip
            if num <=2 then
                panel_equip = self.Panel_equipList1
                index = 2
            else
                panel_equip = self.Panel_equipList2
                index = 4
            end
            panel_equip:setVisible(num ~= 0)
            for t = 1,num do
                panel_equip:getChildByName("Node_Icon_"..(index+t)):setVisible(num > t-1)
            end
            for k = 1,num do
                self.underEquipArr[k] = self.data.nextQualityMaterial[k]
                local icon = panel_equip:getChildByName("Node_Icon_"..(k+index))
                
                local quality = model_material:getQuality(self.data.nextQualityMaterial[k].needMaterialId)
                local asset = model_material:getAsset(self.data.nextQualityMaterial[k].needMaterialId)
                local Image_Icon = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Image_Icon")
                local Image_Frame = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Image_Frame")
                Image_Icon:loadTexture(LANG("items/Material/Material_"..asset..".png")) 
                Image_Frame:loadTexture(LANG("items/frame/frame_%d.png",quality))
                
                local countCurrent = model_materialStorage:getCount(self.data.nextQualityMaterial[k].needMaterialId);
                local Text_1 = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_1")
                local Text_2 = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_2")
                local Text_3 = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_3")
                Text_1:setString("/" .. self.data.nextQualityMaterial[k].needMaterialCount)
                Text_2:setVisible(countCurrent >= self.data.nextQualityMaterial[k].needMaterialCount)
                Text_3:setVisible(countCurrent < self.data.nextQualityMaterial[k].needMaterialCount)
                if countCurrent >= self.data.nextQualityMaterial[k].needMaterialCount then
                    Text_2:setString(countCurrent)
                else
                    Text_3:setString(countCurrent)
                end
                
                if self.isEnough then
                    if countCurrent < self.data.nextQualityMaterial[k].needMaterialCount then
                        self.isEnough = false
                    end
                end
            end 

            self:setHcOrSj()
            --升阶属性显示
            local quiltyIndex = 1;
            local nextQualityInfo = model_extension_equipment:getAttributesNextQuality(self.data)
            for key, value in pairs(nextQualityInfo) do
                self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Panel_Text_"..quiltyIndex):getChildByName("Text_"..quiltyIndex.."_1"):setString(LANG("%s",porterty.data[key]))
                self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Panel_Text_"..quiltyIndex):getChildByName("Text_"..quiltyIndex.."_2"):setString(LANG(value))
                quiltyIndex = quiltyIndex +1
            end
            for n = 1,8 do
                self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Panel_Text_"..n):setVisible(n<quiltyIndex)
            end
        elseif index == 3 then--合成界面
            self:recoverIcon()
            self.Panel_equipList1:setVisible(false)
            self.Panel_equipList2:setVisible(true)
            self.Panel_Center:getChildByName("Text_CL"):setString(LANG("合成材料"))
            self.Panel_Center:getChildByName("Panel_Evolvement_1"):getChildByName("Text_ZBMS"):setString(model_material:getDesc(self.topEquipArr[self.equipNum].needMaterialId))
            --取合成装备的材料
            local synthesis = static_model_materialSynthesis:getSynthesis(self.mergeId);
            for m = 1,3 do
                self.Panel_equipList2:getChildByName("Node_Icon_"..(m+4)):setVisible(false)
            end
            
            if synthesis then
                self.Panel_equipList2:getChildByName("Node_Icon_6"):setVisible(true)-----貌似就一个，问问策划
                self.underEquipArr[1] = synthesis--------暂时这块只有一个
                local icon = self.Panel_equipList2:getChildByName("Node_Icon_6")
                local requireMaterialCountCurrent = model_materialStorage:getCount(synthesis.id);
                icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_2"):setString(requireMaterialCountCurrent)
                icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_1"):setString("/" .. synthesis.count)
                
                local quality = model_material:getQuality(synthesis.id)
                local asset = model_material:getAsset(synthesis.id)
                local Image_Icon = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Image_Icon")
                local Image_Frame = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Image_Frame")
                Image_Icon:loadTexture(LANG("items/Material/Material_"..asset..".png")) 
                Image_Frame:loadTexture(LANG("items/frame/frame_%d.png",quality))
                
                local Text_1 = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_1")
                local Text_2 = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_2")
                local Text_3 = icon:getChildByName("Panel_Zhuangbei"):getChildByName("Text_3")
                Text_1:setString("/" .. synthesis.count)
                Text_2:setVisible(requireMaterialCountCurrent >= synthesis.count)
                Text_3:setVisible(requireMaterialCountCurrent < synthesis.count)
                if requireMaterialCountCurrent >= synthesis.count then
                    Text_2:setString(requireMaterialCountCurrent)
                else
                    Text_3:setString(requireMaterialCountCurrent)
                end
            else
                if self.isRefreshPage == false then
                    self:setPage(0)--判断是否有进阶或者合成材料
                end
                self.isRefreshPage = false
            end
            
            --合成花费
            self.Panel_Center:getChildByName("Panel_Evolvement_1"):getChildByName("Text_Cost2_0"):setString(model_material:getMergeCoin(self.mergeId))
        elseif index == 4 then
            --添加掉落界面的具体信息
--            local scrollView = self.Panel_Center:getChildByName("Panel_Evolvement_2"):getChildByName("ScrollView")
--            -- 清空掉落列表信息
--            scrollView:removeAllChildren()
--            
--            local contentSize =cc.size(425,96)
--            local node=self:createNode("Node/Node_ChapterSelect.csb")
--            local item = node:getChildByName("Panel_EQ_Item")
--            self.space = 1
--            local num = 6
--            local sSize=cc.size(scrollView:getInnerContainerSize().width,math.max(scrollView:getContentSize().height,(contentSize.height+self.space)*num));
--            scrollView:setInnerContainerSize(sSize)
--            for i=1,num do
--                local newItem = item:clone()
--                local Button_Go = newItem:getChildByName("Button_Go")
--                --newItem:setAnchorPoint(cc.p(0,0))
--                Button_Go:setTouchEnabled(true);
--                Button_Go:addTouchEventListener(function(sender,eventType)
--                    if eventType == 2 then
--                        helper_pve:initStage(function(sucess,data)
--                            if sucess then
--                                --SWITSCENE("battle/DefeatScene")
--                                --SWITSCENE("battle/ChapterScene")
--                                GLOBAL_MY_APP:setSwitch(true)
--                                GLOBAL_MY_APP:showDlg("battle/ChapterScene")
--
--                                --self:showDlg("hero/HeroListScene")
--                            end
--                        end)
--                    end
--                end
--                )
--                --确定哪个前往按钮不是隐藏的
--                Button_Go:setVisible(false)
--                if i == 3 then
--                    Button_Go:setVisible(true)
--                end
--                local index = i-1
--                newItem:setTag(index);
--                scrollView:addChild(newItem)
--                local x=contentSize.width/2
--                local y=sSize.height-(contentSize.height)*index-contentSize.height/2
--                newItem:setPosition(cc.p(x,y))
--            end
--            scrollView:scrollToPercentVertical(0,0.01,false)
        end
    end
    self.Panel_Strong:setVisible(index == 1)
    self.Panel_Node_Icon:setVisible(index ~= 1)

    --判断进阶tab上面的小红点是否显示（可进阶则显示小红点）
    self.Panel_Center:getChildByName("Image_1"):setVisible(helper_monster:canEquipmentQualityUp(self.heroInfo.id,self.data.id)==true)
end

function EquipUpgradeScene:setHcOrSj()
--    local isCanHC = helper_monster:hasEnoughEquipmentDependence(self.heroInfo.id,self.data.id,true)
--    if isCanHC  and isEnough == false then
--        self.statusIndex = 0;--表示合成
--        self.Button_HC:setTitleText(LANG("合成"))
--        --合成所需金币
----        self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Text_Cost2"):setString()
--    else
        self.statusIndex = 1;--表示进阶
        if self.data.quality >= 10 then
        	self.Button_HC:setVisible(false)
            self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Text_Cost2"):setVisible(false)
        end
        self.Button_HC:setTitleText(LANG("进阶"))
        --进阶所需金币
        self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Button_HC"):getChildByName("Text_Cost2"):setString(self.data.nextQualityCoin)
--    end
end

function EquipUpgradeScene:setAddEventListener()
--    local Button_Back = self.Panel_Center:getChildByName("Panel_Evolvement_2"):getChildByName("Button_Back")
--    Button_Back:addTouchEventListener(function(sender,eventType)
--        if eventType ==2 then
--            self:setEquipIndex(self.equipNum - 2)
--        end  
--    end 
--    )
    --给上面一行的Panel_Node_Icon中的icon添加监听事件，最后一个不用添加侦听事件
    local num = 2 --最上面一行显示icon数量-1
    for j=0,num do
        local Panel_Zhuangbei = self.Panel_Node_Icon:getChildByName("Node_Icon_"..j):getChildByName("Panel_Zhuangbei")
        Panel_Zhuangbei:getChildByName("Text_1"):setVisible(false)
        Panel_Zhuangbei:getChildByName("Text_2"):setVisible(false)
        Panel_Zhuangbei:getChildByName("Text_3"):setVisible(false)
        local iconFrame = Panel_Zhuangbei:getChildByName("Image_Frame")
        iconFrame:setTag(j)
        iconFrame:setTouchEnabled(true);
        iconFrame:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                self:setEquipIndex(sender:getTag())
            end 
        end
        )
    end
    
    --装备进阶界面，进化材料icon的侦听事件
    for i=3,4 do
        local iconFrame1 = self.Panel_equipList1:getChildByName("Node_Icon_"..i):getChildByName("Panel_Zhuangbei"):getChildByName("Image_Frame")
        iconFrame1:setTag(i-3)
        iconFrame1:setTouchEnabled(true);
        iconFrame1:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                self:setMaterial(sender:getTag())
            end
        end
        )
    end
    
    --合成方式界面，材料的点击事件
    for k=5,7 do
        local iconFrame2 = self.Panel_equipList2:getChildByName("Node_Icon_"..k):getChildByName("Panel_Zhuangbei"):getChildByName("Image_Frame")
        iconFrame2:setTag(k-5)
        iconFrame2:setTouchEnabled(true);
        iconFrame2:addTouchEventListener(function(sender,eventType)
            if eventType ==2 then
                self:setMaterial(sender:getTag())
            end
            
        end
        )
    end
    --进阶属性
    for m=1,8 do
        local txt = self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Panel_Text_"..m):getChildByName("Text_"..m.."_3")
        local arrow = self.Panel_Center:getChildByName("Panel_Evolvement_0"):getChildByName("Panel_Text_"..m):getChildByName("Sprite_Arrow2_"..m)
        txt:setVisible(false)
        arrow:setVisible(false)
    end
end

function EquipUpgradeScene:setEquipIndex(index)
    if index==0 then
        self.mergeId = self.topEquipArr[1].id
        self.equipNum = 1
        self.index  = 2
        self:setPanelData(self.index)
    elseif index==1 then
        self.mergeId = self.topEquipArr[2].needMaterialId
        self.equipNum = 2
        self.index = 3
        self:setPanelData(self.index)
    elseif index==2 then
        self.mergeId = self.topEquipArr[3].id
        self.equipNum = 3
        self.index = 3
        self:setPanelData(self.index)
    end
end

function EquipUpgradeScene:setMaterial(index)
    self.maxDrop = false
    local num = index +1
    if self.equipNum >=2 then
    	num = 1
        if self.mergeId ~= self.underEquipArr[num].id then
            self.preMaterialId = self.mergeId
            self.mergeId = self.underEquipArr[num].id
        end
    else
        if self.mergeId ~= self.underEquipArr[num].needMaterialId then
            self.preMaterialId = self.mergeId
            self.mergeId = self.underEquipArr[num].needMaterialId
        end
        
    end
    
    self.topEquipArr[self.equipNum + 1] = self.underEquipArr[num]  
    if self.equipNum == 1 then
		self.equipNum = 2
        self.index = 3
        self:setPanelData(3)
	elseif self.equipNum >= 2 then
        if self.equipNum == 3 then
            self.maxDrop = true
        end
        self.equipNum = 3
        self.index = 3
        self:setPanelData(3)
	end
end
--判断是否没有合成或者进阶材料，那么就到掉落界面
function EquipUpgradeScene:setPage(num)
    if num == 0 and self.equipNum == 2 then
        local needNumber = 0
        local needId = 0
        local countCurrent = 0
        for i = 1,table.getn(self.underEquipArr) do
            if self.underEquipArr[i].needMaterialId == self.mergeId then
                needNumber = self.underEquipArr[i].needMaterialCount
                needId = self.underEquipArr[i].needMaterialId
                countCurrent = model_materialStorage:getCount(self.underEquipArr[i].needMaterialId);
        	end
        end
        self:showDlg("hero/ChipDropScene",{id = needId,type = pbCommon.ObjectType.Material,currentNum = countCurrent, needNum = needNumber})
        self.equipNum = 1
        self.index = 2
        self:setPanelData(2)
    elseif num == 0 and self.equipNum == 3 then
        self.mergeId = self.preMaterialId
        local countCurrent = model_materialStorage:getCount(self.underEquipArr[1].id);
        self:showDlg("hero/ChipDropScene",{id = self.underEquipArr[1].id,type = pbCommon.ObjectType.Material,currentNum = countCurrent, needNum = self.underEquipArr[1].count})
        if self.maxDrop then
            self.equipNum = 3
        else
            self.equipNum = 2
        end
        
        self.index = 3
        self:setPanelData(3)
    end
end

function EquipUpgradeScene:setImageVisible(boo)
    self.Panel_Center:getChildByName("Image_Item_Bg"):setVisible(boo)
    self.Panel_Center:getChildByName("Text_ZBMC"):setVisible(boo)
    self.Panel_Center:getChildByName("Text_CL"):setVisible(boo)
end

function EquipUpgradeScene:recoverIcon()
	for i = 3,4 do
        local icon1 = self.Panel_equipList1:getChildByName("Node_Icon_"..i):getChildByName("Panel_Zhuangbei"):getChildByName("Image_Icon")
        icon1:loadTexture(LANG("ui/Share/icon/rewardItem.png")) 
	end
	
	for j = 5,7 do
        local icon2 = self.Panel_equipList2:getChildByName("Node_Icon_"..j):getChildByName("Panel_Zhuangbei"):getChildByName("Image_Icon")
        icon2:loadTexture(LANG("ui/Share/icon/rewardItem.png")) 
	end
end

--装备强化
function EquipUpgradeScene:onMonstersEquipmentLevelUp(success, data, code, msg)
    if(success) then
        --增加强化成功动画
        self.successDH = true
        if not self.successNodeAction then
            local node,nodeAction=self:createNode("Node/Node_Animation_TreasureUp2.csb")
            self.successNodeAction = nodeAction
            self.successNode = node
            self.successNodeAction:gotoFrameAndPlay(0,false)
            self.Panel_16:addChild(node)
            node:setPosition(cc.p(101.03,86.82))
        else 
            self.successNodeAction:gotoFrameAndPlay(0,false)
        end
        
        --强化总共升级了多少级
        local levelUpNum = data.level - self.currentEquipmentLevel
        if table.getn(data.turns)>1 then
            local baojiNum = 0;--暴击次数
            for j = 1,table.getn(data.turns) do
                if data.turns[j].isCrit == 1 then
                    baojiNum = baojiNum+1
                end
            end
            if data.level == self.heroInfo.level then
                PopMsg.getInstance():flashShow(LANG("已达到等级上限，强化%s次，暴击%s次，强化等级+%s",table.getn(data.turns),baojiNum,levelUpNum))
            elseif data.level < self.heroInfo.level and table.getn(data.turns) < 5 then
                PopMsg.getInstance():flashShow(LANG("银两不足，强化%s次，暴击%s次，强化等级+%s",table.getn(data.turns),baojiNum,levelUpNum))
            elseif baojiNum == 0 then
                PopMsg.getInstance():flashShow(LANG("强化%s次，强化等级+%s",table.getn(data.turns),levelUpNum))
            else
                PopMsg.getInstance():flashShow(LANG("强化%s次，暴击%s次，强化等级+%s",table.getn(data.turns),baojiNum,levelUpNum))
            end
        else
            if data.turns[1].isCrit then
                PopMsg.getInstance():flashShow(LANG("暴击！强化等级+%s",levelUpNum))
            else
                PopMsg.getInstance():flashShow(LANG("强化等级+%s",levelUpNum))
            end
        end
        --强化成功，扣除玩家银两，浮动文字提示玩家：强化等级+X，上方对应的装备属性发生变化
        self.heroInfo = data.monster 
        for i = 1,table.getn(data.monster.equipment) do
            if data.monster.equipment[i].id == self.data.id then
                self.data = data.monster.equipment[i]
            end
        end
        self:setPanelData(self.index)
        self.fun({cmd="MonstersEquipmentLevelUpSuccess"})
    else
    end
end
--装备材料合成
function EquipUpgradeScene:onMonstersEquipmentMaterialMerge(success, data, code, msg)
	if(success) then
        self:setPanelData(self.index)------
        PopMsg.getInstance():flashShow(LANG("合成成功!"))
	else
	end
end
--装备进阶
function EquipUpgradeScene:onMonstersEquipmentQualityUp(success, data, code, msg)
    if(success) then
        self.heroInfo = data.monster
        for i = 1,table.getn(data.monster.equipment) do
            if data.monster.equipment[i].id == self.data.id then
                self.data = data.monster.equipment[i]
            end
        end
        self.topEquipArr[1] = self.data
        
        for i=1,table.getn(data.monster.equipment) do
        	if data.monster.equipment[i].id == self.data.id then
                self:setPanelData(self.index)
        	end
        end
        self.fun({cmd="MonstersEquipmentQualityUpSuccess"})
        PopMsg.getInstance():flashShow(LANG("进阶成功!"))
    else
--        WARNING(msg);
    end
end

function EquipUpgradeScene:refreshPage()
    self.isRefreshPage = true
    local ownHeros = model_monsterStorage:getAllActiveMonsters()
    for i = 1,table.getn(ownHeros) do
        if self.heroInfo.id == ownHeros[i].id then
            self.heroInfo = ownHeros[i]
            for j = 1,table.getn(self.heroInfo.equipment) do
                if self.heroInfo.equipment[j].id == self.data.id then
                    self.data = self.heroInfo.equipment[j]
                    self.topEquipArr[1] = self.data
            	end
            end
    	end
    end
    self:setPanelData(self.index)
end

return EquipUpgradeScene
