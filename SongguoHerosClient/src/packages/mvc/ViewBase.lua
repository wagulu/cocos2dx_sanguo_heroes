
local ViewBase = class("ViewBase", cc.Node)
local model_monster =_REQUIRE("model/static/model_monster.lua")
local model_monsterStorage =_REQUIRE("model/model_monsterStorage.lua")
function ViewBase:ctor(app, name,...)
    self:enableNodeEvents()
    self.app_ = app
    self.name_ = name
    self.args = {...} or {}
    -- check CSB resource file
    self.dlgCallBack=self.args[1]
    table.remove(self.args,1)
    local res = rawget(self.class, "RESOURCE_FILENAME")
    if name == "home/MainCityScene" then
        local time1 =   model_user:getCurrentServerTimestamp()
        local tab1 = os.date("*t", time1)
        if(tab1.hour >= 6 and tab1.hour < 18) then    -- 白天
            res = "Scene/MainScene.csb"
        else  -- 夜晚
            res = "Scene/MainScene_N.csb"
        end
    end
    if res then
        self:createResoueceNode(res)
    end

    local binding = rawget(self.class, "RESOURCE_BINDING")
    if res and binding then
        self:createResoueceBinding(binding)
    end
    
    if self.onCreate then self:onCreate() end
end

function ViewBase:getApp()
    return self.app_
end

function ViewBase:getDialog()
    return self.app_.dialog
end

function ViewBase:getName()
    return self.name_
end

function ViewBase:getActionNode()
    return self.actionNode_
end

function ViewBase:getResourceNode()
    return self.resourceNode_
end

function ViewBase:createResoueceNode(resourceFilename)
    if self.resourceNode_ then
        self.resourceNode_:removeSelf()
        self.resourceNode_ = nil
    end
    
    self.resourceNode_ = cc.CSLoader:createNode(resourceFilename)
    if(self.resourceNode_ == nil) then
        return;
    end
    self.startScene = false
    self.endScene = false
    SCREEN_DO_LAYOUT(self.resourceNode_)
    assert(self.resourceNode_, string.format("ViewBase:createResoueceNode() - load resouce node from file \"%s\" failed", "Scene/ChapterScene.csb"))
    self:addChild(self.resourceNode_)
    self.resourceFilename = resourceFilename
    self.actionNode_ = cc.CSLoader:createTimeline(resourceFilename)
    self.resourceNode_:runAction(self.actionNode_)
    NotificationCenter_addObserver(self, "REFRESH_PAGE",self.refreshPage)
    self:scheduleUpdateWithPriorityLua(handler(self,self.update), 0)  
end

function ViewBase:playBtnEffect()
--    SanGuoSoundLib.playSoundEffect("effect1")    
end

function ViewBase:update()
    self:updateHandler()
end

function ViewBase:updateHandler()
    if self.endScene == true and self.actionNode_:isPlaying() == false then
        self.endScene = false
        if self.callBack then
            self.callBack()
        end
    elseif self.startScene == true and self.actionNode_:isPlaying() == false then
        self.startScene = false
        if self.frameStopCallBack then
            self.frameStopCallBack(self)
        end
    end 
end
function ViewBase:onEnter()
    self:playStart()
end

function ViewBase:onExit()
   self:onExitHandler()
end

function ViewBase:onExitHandler()
    self:removeTopBarObservers() 
    local eventDispatcher = self:getEventDispatcher()
    eventDispatcher:removeEventListener(self._listener)
    if self.timerId ~= 0 and self.timerId ~=nil then
        Timer:kill(self.timerId)
    end
    cc.Director:getInstance():getTextureCache():removeUnusedTextures()
    self:unscheduleUpdate()
    --cclog("dddd:"..self.resourceNode_:retainCount())
end

function ViewBase:playStart()
    if self.actionNode_ ==nil  then
        return
    end
    self.actionNode_:play("start",false)
    self.startScene = true
end

function ViewBase:playEnd()
    if self.actionNode_ ==nil  then
        return
    end
    self.actionNode_:play("end",false)
    self.endScene = true
end

function ViewBase:sceneBack(callBack,notPlay)--1播放场景动画完毕在回调函数中做相应处理
    self.callBack = callBack
    if not notPlay then
        self:playEnd()
    else
        self.callBack()
    end
    
end

function ViewBase:refreshPage()

end

function ViewBase:addBtnTouchEventListener(btn,callBack)
    btn:addTouchEventListener(function(sender,eventType)
        if eventType == 0 then
            self:playBtnEffect()  
        end
        callBack(sender,eventType)
    end
    )   
end

--ͨ��csd ����node
function ViewBase:createNode(resourceFilename,isLoop)
    local node = cc.CSLoader:createNode(resourceFilename)
    SCREEN_DO_LAYOUT(node)
    assert(node, string.format("ViewBase:createResoueceNode() - load resouce node from file \"%s\" failed", resourceFilename))
    local actionNode = cc.CSLoader:createTimeline(resourceFilename)
    node:runAction(actionNode)
    actionNode:gotoFrameAndPlay(0,isLoop)
    return node,actionNode
end
function ViewBase:showDlg(sceneName,...)
    local arg ={...}
    table.insert(arg,1,handler(self,self.dlgCallBack))
    self.dialog = GLOBAL_MY_APP:showDlg(sceneName,unpack(arg, 1, table.maxn(arg)))
end

function ViewBase:dlgCallBack(data)
    
end



function ViewBase:setIndex(index)
    self.index = index
    for i = 1,#self.buttons do
        local  btn = self.buttons[i] 
        btn:setTag(i)
        btn:setBright(i~=index)
        btn:setEnabled(i~=index)
    end
end



--function ViewBase:onEnter()
--    self:getActionNode():play("stop",false)
--end

function ViewBase:initStar(star,panel_star)
    for i = 1, 5 do
        local image_star = panel_star:getChildByName("Image_Star_"..i)
        image_star:setVisible(i <= star)
    end
end


function ViewBase:initBag2(id,type,PanelIcon)
    local Image_Icon = PanelIcon:getChildByName("Image_Icon")
    local Image_Bg = PanelIcon:getChildByName("Image_Bg")
    local Text_2 = PanelIcon:getChildByName("Text_2")
    Image_Icon:loadTexture(model_static_library:getAsset(id,type,PanelIcon,type))
    Image_Bg:loadTexture(model_static_library:getFrame(id,type))
    cclog("model_static_library:getName(id,type):"..model_static_library:getName(id,type).."id:"..id.."type:"..type)
    Text_2:setString(model_static_library:getName(id,type))
end

function ViewBase:initPanel_RisingStar_Head(node,itemData,isNpc)
    local Panel_RisingStar_Head = node:getChildByName("Panel_RisingStar_Head")
    local Image_Hero = Panel_RisingStar_Head:getChildByName("Image_Hero")
    local Image_Head_bg = Panel_RisingStar_Head:getChildByName("Image_Head_bg")
    local Panel_Star = Panel_RisingStar_Head:getChildByName("Panel_Star")
    local Image__Job = Panel_RisingStar_Head:getChildByName("Image__Job")
    Panel_Star:setVisible(itemData ~= nil)
    Image_Hero:setVisible(itemData ~= nil)
    Image__Job:setVisible(itemData ~= nil)
    if itemData == nil then
    	return
    end
    
    if isNpc then
        Image_Head_bg:loadTexture(model_monster:getFrameByQuality(itemData["quality"]))
        Image_Hero:loadTexture(model_monster:getIconByAsset(itemData["asset"]))
    else
        Image_Head_bg:loadTexture(model_monsterStorage:getFrame(itemData["id"]))
        Image_Hero:loadTexture(model_monster:getIcon(itemData["id"]))
    end
    
    --星级显示
    for k = 1,5 do
        Panel_Star:getChildByName("Image_Star_"..k):setVisible(k<= itemData.star)
    end
    Image__Job:loadTexture("Job/Icon_job"..model_monster:getType(itemData["id"])..".png")
end

function ViewBase:initItemCacha(Panel_Object,data)
    local Image_Icon = Panel_Object:getChildByName("Image_1")
    local Image_Bg = Panel_Object:getChildByName("Image_2")
    local Text_1 = Panel_Object:getChildByName("Text_1")
    Image_Icon:loadTexture(model_static_library:getAsset(data.id,data.type))
    Image_Icon:setTouchEnabled(true)
    self:addBtnTouchEventListener(Image_Icon,function(sender,eventType)
        if  eventType == 2 then
            self:showDlg("common/PropsPopScene", {id = data.id, type = data.type})
        end
    end)
    
    Image_Bg:loadTexture(model_static_library:getFrame(data.id,data.type))
    cclog("model_static_library:getName(id,type):"..model_static_library:getName(data.id,data.type).."id:"..data.id.."type:"..data.type)
    if data.num then
        Text_1:setString(data.num)
    end
    Text_1:setVisible(data.num ~= nil and data.num > 1)
end

function ViewBase:initPanelTopButton(Panel_TopButton)
    for i=1 ,4 do
        local TopBtn_01 = Panel_TopButton:getChildByName("TopBtn_0"..i)
        local Text_TopBtn_01 = Panel_TopButton:getChildByName("Text_TopBtn_0"..i)
        TopBtn_01:setTag(1000+i)
        self:addBtnTouchEventListener(TopBtn_01,function(sender,eventType)
            if eventType == 2 then
                local tag = sender:getTag()
                if tag == 1001 then --邮件
                    _REQUIRE("model/helper/helper_mail.lua"):initMailList(function(sucess,data)
                        if sucess then
                            SWITSCENE("mail/MailListScene")
                        end
                    end)
                end
            end
        end)
    end
    local Tip_TopBtn_01 = Panel_TopButton:getChildByName("Tip_TopBtn_01")
    self.Tip_TopBtn_01 = Tip_TopBtn_01
    -- 添加邮件数量更新
    NotificationCenter_addObserver(self, "UPDATE_DEPOSIT_COUNT",self.updateMailCount)
    self:updateMailCount()
end

function ViewBase:updateMailCount()
    if self.Tip_TopBtn_01 then
        self.Tip_TopBtn_01:setVisible(model_user.deposit_count > 0)
    end
end

function ViewBase:initTopBar(node_hud_top)
    local node = node_hud_top:getChildByName("Panel_HUD_Top")
    node:setTouchEnabled(false)
    local EVERY_BUY_ENERGY = 120  -- 每次可购买体力
    local EVERY_BUY_VITALITY = 50 -- 每次可购买精力  
    local helper_player = _REQUIRE ("model/helper/helper_player.lua")
    -- 文本提示位置
    local offX = CC_SCREEN_SIZE.width - CC_DESIGN_RESOLUTION.width
    local offY = CC_SCREEN_SIZE.height - CC_DESIGN_RESOLUTION.height
    local tipPos = {
        cc.p(366.708771 + offX, 575.422974 + offY),
        cc.p(577.732178 + offX, 575.422974 + offY),
        cc.p(785.732910 + offX,  575.422974 + offY),
        cc.p(992.733154 + offX, 575.422974 + offY)    
    }
    -- 按钮1
    self.label_Energy_L_ = node:getChildByName("Panel_HUD_Item_1"):getChildByName("Text_1_l")
    self.label_Energy_R_ = node:getChildByName("Panel_HUD_Item_1"):getChildByName("Text_1_r")
    self:updateEnergyPoint()
 
    node:getChildByName("Panel_HUD_Item_1"):setTouchEnabled(false)
    local btn1 =  node:getChildByName("Panel_HUD_Item_1"):getChildByName("Button_Icon")  
    self:addBtnTouchEventListener(btn1, function(sender,eventType)
        if eventType==2 then
            self:showDlg("common/TopBtnPopScene",{titleStr = LANG("购买体力"), 
                confirmStr=LANG("确定"),cancelStr=LANG("取消"),
                text=LANG("是否花费%d元宝购买%d体力? \n(今日已购买%d次)",
                    helper_player:getBuyEnergyCost(), 
                    EVERY_BUY_ENERGY,
                    model_user:getDailyBuyEnergyTimes()),                                             
                fun=function(data)
                    if data.cmd=="confirm" then
                        ctlPlayerBuyEnergyRequest:sendRequest(function(sucess,data)
                            if sucess then
                                self:updateEnergyPoint()
                            end
                        end)
                    elseif data.cmd =="cancel" then 
                        cclog("Panel_HUD_Item_1 cancel")
                    end
                end})
        end 
    end)

    -- 文字提示
    local imgBg1 =  node:getChildByName("Panel_HUD_Item_1"):getChildByName("Image_Bg")  
    local pos = imgBg1:getParent():convertToWorldSpace(cc.p(imgBg1:getPosition()))
    imgBg1:setTouchEnabled(true)
    self:addBtnTouchEventListener(imgBg1, function(sender,eventType)
        if eventType==2 then
            self:showDlg("common/DescPopScene",{pos = tipPos[1], text1 = LANG("恢复间隔"),t_type = 1,   
                label_Energy_L = self.label_Energy_L_,   
                label_Energy_R = self.label_Energy_R_,
                text2 = LANG("距下次恢复还有"),
                text3 = LANG("距恢复满还有"),
                text4 = LANG("已购买体力%d次",model_user:getDailyBuyEnergyTimes())})
        end 
    end)

    -- 添加体力更新
    NotificationCenter_addObserver(self, "UPDATE_ENERGY_POINT",self.updateEnergyPoint)

    -- 按钮2
    self.label_Vitality_L_ = node:getChildByName("Panel_HUD_Item_4"):getChildByName("Text_4_l")
    self.label_Vitality_R_ = node:getChildByName("Panel_HUD_Item_4"):getChildByName("Text_4_r")
    self:updateVitalityPoint()

    local btn2 =  node:getChildByName("Panel_HUD_Item_4"):getChildByName("Button_Icon")  
    self:addBtnTouchEventListener(btn2, function(sender,eventType)
        if eventType==2 then
            self:showDlg("common/TopBtnPopScene",{titleStr = LANG("购买精力"), 
                confirmStr=LANG("确定"),cancelStr=LANG("取消"),
                text=LANG("是否花费%d元宝购买%d精力? \n(今日已购买%d次)",
                    helper_player:getBuyVitalityCost(), 
                    EVERY_BUY_VITALITY,
                    model_user:getDailyBuyVitalityTimes()),                                                                                                 
                   fun=function(data)
                    if data.cmd=="confirm" then
                        ctlPlayerBuyVitalityRequest:sendRequest(function(success, data)
                            if success then
                                self:updateVitalityPoint()
                            end
                        end) 
                    elseif data.cmd == "cancel" then 
                        cclog("Panel_HUD_Item_4 cancel")
                    end
                end})
        end 
    end)

    -- 文字提示
    local imgBg2 = node:getChildByName("Panel_HUD_Item_4"):getChildByName("Image_Bg")  
    local pos2 = imgBg2:getParent():convertToWorldSpace(cc.p(imgBg2:getPosition()))
    imgBg2:setTouchEnabled(true)
    self:addBtnTouchEventListener(imgBg2, function(sender,eventType)
        if eventType == 2 then
            self:showDlg("common/DescPopScene",{pos = tipPos[2], text1 = LANG("恢复间隔"),t_type = 2,
                label_Vitality_L = self.label_Vitality_L_, 
                label_Vitality_R = self.label_Vitality_R_,
                text2 = LANG("距下次恢复还有"),
                text3 = LANG("距恢复满还有"),
                text4 = LANG("已购买精力%d次",model_user:getDailyBuyVitalityTimes())})
        end 
    end)

    -- 添加精力更新
    NotificationCenter_addObserver(self, "UPDATE_VITALITY_POINT",self.updateVitalityPoint)

    -- 金币
    local btn3 =  node:getChildByName("Panel_HUD_Item_2"):getChildByName("Button_Icon")  
    local label_3 = node:getChildByName("Panel_HUD_Item_2"):getChildByName("Text_1")  
    label_3:setString(LANG(model_user.coin))
    self.label_Coin_= label_3
    self:addBtnTouchEventListener(btn3, function(sender,eventType)
        if eventType==2 then
            PopMsg.getInstance():flashShow(LANG("充值功能暂未开启"))
        end
    end)
    
    -- 添加金币更新
    NotificationCenter_addObserver(self, "UPDATE_COIN_POINT",self.updateCoinPoint)
    
    -- 元宝
    local btn4 =  node:getChildByName("Panel_HUD_Item_3"):getChildByName("Button_Icon")  
    local label_4 = node:getChildByName("Panel_HUD_Item_3"):getChildByName("Text_1") 
    label_4:setString(LANG(model_user.cash))
    self.label_Cash_ = label_4
    self:addBtnTouchEventListener(btn4, function(sender,eventType)
        if eventType==2 then
            PopMsg.getInstance():flashShow(LANG("充值功能暂未开启"))
        end
    end)  
    
    -- 添加元宝更新
    NotificationCenter_addObserver(self, "UPDATE_CASH_POINT",self.updateCashPoint) 
    
end

function ViewBase:removeTopBarObservers()
    NotificationCenter_removeObserverFromTargetByName(self,"UPDATE_ENERGY_POINT")
    NotificationCenter_removeObserverFromTargetByName(self,"UPDATE_VITALITY_POINT")
    NotificationCenter_removeObserverFromTargetByName(self,"UPDATE_COIN_POINT")
    NotificationCenter_removeObserverFromTargetByName(self,"UPDATE_CASH_POINT")
    NotificationCenter_removeObserverFromTargetByName(self, "REFRESH_PAGE")
    NotificationCenter_removeObserverFromTargetByName(self, "UPDATE_DEPOSIT_COUNT")
    
    self.label_Vitality_L_ = nil
    self.label_Vitality_R_ = nil
    self.label_Energy_L_ = nil
    self.label_Energy_R_ = nil
    self.label_Cash_ = nil
    self.label_Coin_ = nil
end

function ViewBase:updateEnergyPoint()
    if self.label_Energy_L_ then
        self.label_Energy_L_:setString(model_user.energy)
        if(model_user:isEnergyFull()) then
            self.label_Energy_L_:setColor(cc.c3b(0, 240, 255))
        end
    end
    if self.label_Energy_R_ then
        self.label_Energy_R_:setString(model_user.energyMax) 
    end
end

function ViewBase:updateVitalityPoint()
    if self.label_Vitality_L_ then
        self.label_Vitality_L_:setString(model_user.vitality)
        if(model_user:isVitalityFull()) then
            self.label_Vitality_L_:setColor(cc.c3b(0, 240, 255))
         end
    end
    if self.label_Vitality_R_ then
        self.label_Vitality_R_:setString(model_user.vitalityMax) 
    end
end

function ViewBase:updateCashPoint()
    if  self.label_Cash_ then
        self.label_Cash_:setString(LANG(model_user.cash))
    end
end

function ViewBase:updateCoinPoint()
    if self.label_Coin_ then
        self.label_Coin_:setString(LANG(model_user.coin))
    end
end

--function ViewBase:sceneNext(callBack)--2下一场景
--    self.returnType = 2
--    --nodeAction:play("animationEnd",false)
--    self:getActionNode():gotoFrameAndPlay(self:getActionNode():getCurrentFrame(),false)
--end
--function ViewBase:removeDlg(callBack) --3移除对话框
--    cclog("................................"..self:getActionNode():getCurrentFrame()..self.name_)
--    self.returnType = 3
--    self:getActionNode():gotoFrameAndPlay(self:getActionNode():getCurrentFrame(),false)
--end

function ViewBase:removeDlgNoAction()
    self:removeSelf()
end

function ViewBase:getArgs()
    return self.args
end



function ViewBase:createResoueceBinding(binding)
    assert(self.resourceNode_, "ViewBase:createResoueceBinding() - not load resource node")
    for nodeName, nodeBinding in pairs(binding) do
        local node = self.resourceNode_:getChildByName(nodeName)
        if nodeBinding.varname then
            self[nodeBinding.varname] = node
        end
        for _, event in ipairs(nodeBinding.events or {}) do
            if event.event == "touch" then
                node:onTouch(handler(self, self[event.method]))
            end
        end
    end
end

function ViewBase:showWithScene(transition, time, more)
    self:setVisible(true)
    local scene = display.newScene(self.name_)
    scene:addChild(self)
    display.runScene(scene, transition, time, more)
    return self
end


-- 添加滚动条
function ViewBase:addScrollViewBar(scView, offsetX)
    local scPosX, scPosY= scView:getPosition()
    local scOrigin = scView:getContentSize() --  获取原始scView滑动区域
    local scInnerY = scView:getInnerContainer():getPositionY() -- 获取内部容器Y值
    local scInnerSizeH = scView:getInnerContainerSize().height -- 获取容器更改后高度
    local scBar = scView:getParent():getChildByTag(999)
    if scBar == nil then
        scBar = ccui.Scale9Sprite:create("ui/roll.png")
        scBar = self:createNode("Node/Node_ScrollPanel.csb"):getChildByName("Panel_Scroll")        
        scView:getParent():addChild(scBar)
        scBar:setTag(999) -- 设置特殊tag
    end
    scBar:show()
    local scBarSize = scBar:getContentSize() 
    -- 设置滑动条位置
    local scBarPosX, scBarPosY
    local offset = offsetX or 0 --滚动条偏移量
    local scViewAp = scView:getAnchorPoint()
    if scViewAp.x == 0.0 then
        scBarPosX = scOrigin.width + scPosX + offset
    	elseif scViewAp.x == 0.5 then
        scBarPosX = scOrigin.width * 0.5 + scPosX + offset
    	elseif scViewAp.x == 1.0 then
        scBarPosX = scPosX + offset
    end  
    if scViewAp.y == 0.0 then
        scBarPosY = scOrigin.height + scPosY
    elseif scViewAp.y == 0.5 then
        scBarPosY = scOrigin.height * 0.5 + scPosY
    elseif scViewAp.y == 1.0 then
        scBarPosY = scPosY
    end
    
    scBar:setPosition(cc.p(scBarPosX, scBarPosY)) 
    scBar:setScaleY( scOrigin.height / scBarSize.height * scOrigin.height / scInnerSizeH)
    local curInnerY = scView:getInnerContainer():getPositionY() -- 当前容器Y值
    local lastInnerY = curInnerY
       
    scView:addEventListener(function(sender,eventType) 
    if eventType == 4 then -- 滑动事件
        curInnerY = scView:getInnerContainer():getPositionY()
        if(curInnerY > scInnerY and curInnerY < 0 ) then
            local offsetY = (curInnerY - lastInnerY) * (scOrigin.height / scInnerSizeH)                   
                scBar:setPositionY(scBar:getPositionY() - offsetY)
                lastInnerY = scView:getInnerContainer():getPositionY()       
            end
        end
    end) 
end


function ViewBase:hideScrollViewBar(scView)
    local scBar = scView:getParent():getChildByTag(999)
    if scBar ~= nil then
        scBar:hide()
    end
end

function ViewBase:getCoin(prizes)
    for i=1,#prizes do
        if prizes[i].type == pbCommon.ObjectType.coin then
            return prizes[i].content
        end
    end
    return 0
end

function ViewBase:getCash(prizes)
    for i=1,#prizes do
        if prizes[i].type == pbCommon.ObjectType.cash then
            return prizes[i].content
        end
    end
    return 0
end

return ViewBase
