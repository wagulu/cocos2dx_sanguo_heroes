local MainCityScene = class("MainCityScene", cc.load("mvc").ViewBase)

local monster_Notification = _REQUIRE("model/helper/helper_monsterNotification.lua")
local model_user = _REQUIRE("model.model_user")

_REQUIRE "cocos.cocos2d.json"
MainCityScene.RESOURCE_FILENAME = "Scene/MainScene.csb"

--local time1 =   model_user:getCurrentServerTimestamp()
--local tab1 = os.date("*t", time1)
--if(tab1.hour >= 6 and tab1.hour < 18) then    -- 白天
--    MainCityScene.RESOURCE_FILENAME = "Scene/MainScene.csb"
--else  -- 夜晚
--    MainCityScene.RESOURCE_FILENAME = "Scene/MainScene_N.csb"
--end

--cclog("getCurrentServerTimestamp "..time1.. " hour "..tab1.hour)

function MainCityScene:onCreate()
    local panel_MainScene = self:getResourceNode():getChildByName("Panel_MainScene")
    
    local panel_Top = panel_MainScene:getChildByName("Panel_Top")
    local panel_Bottom = panel_MainScene:getChildByName("Panel_Bottom")
    local Panel_Right = panel_MainScene:getChildByName("Panel_Right")  
    Panel_Right:setTouchEnabled(false)
    local Panel_Bg = panel_MainScene:getChildByName("Panel_Bg")    
    local Panel_Front_BG = panel_MainScene:getChildByName("Panel_Front")
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_TOP(panel_Top)
    SCREEN_BOTTOM(panel_Bottom)   
    SCREEN_BOTTOM(Panel_Front_BG)
 
    SanGuoAnimationLib.freeAllAdvancedAnimation();
    SanGuoAnimationLib.freeAllAdvancedAnimationAsset();
    -- 背景图片 包含滚动层  
  
    local panel_Center = panel_MainScene:getChildByName("Panel_Bottom")
    local panel_BG = panel_Center:getChildByName("Panel_Picture_Bg")
    local img_BG = panel_BG:getChildByName("Image_1")   
    local img_MidBG = panel_BG:getChildByName("Image_17")
    local Panel_Front = panel_MainScene:getChildByName("Panel_Front"):getChildByName("Image_45")
    -- 远景位置
    if model_user.NEARBYBG_POSX == nil then
        model_user.NEARBYBG_POSX = img_BG:getPositionX()
    end 
    -- 近景位置
    if model_user.FARBG_POSX == nil then
        model_user.FARBG_POSX = img_MidBG:getPositionX()
    end  
    -- 前景位置
    if model_user.FRONTBG_POSX == nil then
        model_user.FRONTBG_POSX = Panel_Front:getPositionX()
    end  
    
    
    panel_Center:setTouchEnabled(true)
    self:addScrollBgListener(panel_Center)   
    img_BG:setPositionX(model_user.NEARBYBG_POSX)
    img_MidBG:setPositionX(model_user.FARBG_POSX)
    Panel_Front:setTouchEnabled(false)
    Panel_Front:setPositionX(model_user.FRONTBG_POSX)
    
    
    -- 主角头像信息
    local item_playerInfo = panel_Top:getChildByName("Panel_PlayerInfo"):getChildByName("ProjectNode_PlayerImage"):getChildByName("Panel_RisingStar_Head")
    item_playerInfo:setTouchEnabled(true)
    item_playerInfo:setEnabled(true)
    -- 点击任务头像回调
    self:addBtnTouchEventListener(item_playerInfo,function(sender,eventType)
        if eventType == 2 then
           self:showDlg("home/PlayerInfoPopPanel")
        end
    end)  
     
    -- 添加右侧边栏弹出    
    local Panel_Right = panel_MainScene:getChildByName("Panel_Right")  
    SCREEN_TOP(Panel_Right)
    Panel_Right:setTouchEnabled(false)
    local ProjectNode_1 =  Panel_Right:getChildByName("ProjectNode_1") 
    
    -- 记录右侧滑动条状态 默认显示
    if model_user.MAINCITYSCENE_SLIDERBAR == nil then
        model_user.MAINCITYSCENE_SLIDERBAR = true
    end 
    -- 添加右侧边栏事件
    self:addRightSlideBar(ProjectNode_1,  model_user.MAINCITYSCENE_SLIDERBAR)
    -- 更新右侧边栏红点
    self:updateSliderBarRedPoint(ProjectNode_1)
    
    -- 添加top hud 回调
    local node_hud_top = panel_Top:getChildByName("Node_HUD_Top")
    self:initTopBar(node_hud_top)
    
    -- 上方活动按钮 暂时隐藏
    
    local Panel_TopButton = panel_Top:getChildByName("Panel_TopButton")
    self:initPanelTopButton(Panel_TopButton)
end

-- 记录页面跳转后 背景移动位置
function MainCityScene:recordBGPosX()
    local panel_MainScene = self:getResourceNode():getChildByName("Panel_MainScene")
    local panel_Center = panel_MainScene:getChildByName("Panel_Bottom")
    local panel_BG = panel_Center:getChildByName("Panel_Picture_Bg")
    local img_BG = panel_BG:getChildByName("Image_1")   
    local img_MidBG = panel_BG:getChildByName("Image_17")
    local Panel_Front = panel_MainScene:getChildByName("Panel_Front"):getChildByName("Image_45")
    model_user.FRONTBG_POSX = Panel_Front:getPositionX()
    model_user.NEARBYBG_POSX = img_BG:getPositionX()
    model_user.FARBG_POSX = img_MidBG:getPositionX()  
end

-- 添加背景滑动层监听
function MainCityScene:addScrollBgListener(panelCenter)
    local panel_BG = panelCenter:getChildByName("Panel_Picture_Bg")
    local img_BG = panel_BG:getChildByName("Image_1")   
    local img_MidBG = panel_BG:getChildByName("Image_17")
    local panel_MainScene = self:getResourceNode():getChildByName("Panel_MainScene")
    local Panel_Front = panel_MainScene:getChildByName("Panel_Front"):getChildByName("Image_45")

    local bTouchEnabled = false  -- 滑动时触控无效
    local touchMoveBeganX = 0
    local touchMoveEndX = 0   
    local timeIntervel = 0.3 -- 连续点击时间间隔  
    local bTouchIntervel = true -- 点击间隔
    local id =1
    local ProjectNode_MainHero = img_BG:getChildByName("ProjectNode_MainHero")
    local main_character_1 = ProjectNode_MainHero:getChildByName("main_character_1")
    main_character_1:setVisible(false)
--    local monster = SanGuoAnimationLib.createAdvancedAnimation("hero_"..id, "animation_idle");
--    SanGuoAnimationLib.play(monster:getTag());
--    ProjectNode_MainHero:addChild(monster);  
--    monster:setPosition(main_character_1:getPosition())
    -- 主城按钮图标触控
    local touchBtn = img_BG:getChildByName(LANG("Panel_building_%d",1)) -- 记录当前被点击按钮
    local touchSelect = touchBtn:getChildByName(LANG("SelectBg_%d",1))   -- 记录当前被选中imageView
    for i = 1, 11 do
        local img_bg = img_BG:getChildByName(LANG("Panel_building_%d",i))
        local img_select = img_bg:getChildByName(LANG("SelectBg_%d",i))        
        img_bg:setTouchEnabled(true)
        self:addBtnTouchEventListener(img_bg,function(sender,eventType)
            if eventType==0 then   -- touchBegan
                img_bg:setScale(1.1)
                img_select:show()
                touchBtn = img_bg
                touchSelect = img_select
                bTouchEnabled = true       
            elseif eventType==2 then   -- touchEnd
                img_bg:setScale(1.0)
                img_select:hide()
                if bTouchEnabled and bTouchIntervel then
                    bTouchIntervel = false
                    Timer:runOnce(function(dt, data, timerId)
                        if bTouchIntervel ~= nil then
                            bTouchIntervel = true
                        end
                    end,timeIntervel)            
                    self:btnListener(i) -- 按钮触发
                end
            elseif eventType==3 then  -- touchCancel
                img_bg:setScale(1.0)
                img_select:hide()
            end
        end)
    end
    local scale = img_BG:getScale()  -- 美术将背景图片扩充到了1.3
    local bgWid = img_BG:getContentSize().width * scale  
    local bgX  = img_BG:getPositionX()
    local bgOffSetX = (bgWid - CC_DESIGN_RESOLUTION.width) * 0.5 
    local scopeMinX = bgX - bgOffSetX
    local scopeMaxX = bgX + bgOffSetX
       
 -- 添加背景滑动触控层
    local m_isMoving = false
    local m_dragV = 0
    local m_dragDt = 0.0

    local function onTouchBegan(touch, event)
        touchMoveBeganX =  touch:getLocation().x   
        m_dragDt = 0.0
        m_isMoving = true
        return true
    end  

    local function onTouchMoved(touch, event)
        local x = img_BG:getPositionX()
        local x2 = img_MidBG:getPositionX()   
        local x3 = Panel_Front:getPositionX()    
        local d = touch:getDelta()      
        local moveX = x+d.x  
        local moveX2 = x2 + d.x* 0.5 
        local moveX3 = x3 + d.x* 1.1  
        if(moveX >= scopeMinX and moveX <= scopeMaxX) then
            img_BG:setPositionX(moveX)
            img_MidBG:setPositionX(moveX2)
            Panel_Front:setPositionX(moveX3)
        end  
        if(math.abs(touchMoveBeganX -  touch:getLocation().x) >= 40) then
            if touchBtn then
                touchBtn:setScale(1.0)  
                touchSelect:hide()
            end
            bTouchEnabled = false
        end        
    end

    local function onTouchEnded(touch, event)
        touchMoveEndX = touch:getLocation().x     
        if(m_isMoving) then
            local curPos = img_BG:getPositionX()
            if(curPos >= scopeMinX and curPos <= scopeMaxX) then
                if (math.abs(touchMoveEndX - touchMoveBeganX) < 40) then
                    return
                end
                if (m_dragDt > 0.4)  then
                    return
                end
                if (m_dragDt > 0.0) then
                    m_dragV = (touchMoveEndX - touchMoveBeganX) / m_dragDt
                end
            else
                m_dragV = 0
                m_isMoving = false
            end
        end 
        m_isMoving = false  
   end
      
    local layer = display.newLayer()
    self:getResourceNode():getChildByName("Panel_MainScene"):addChild(layer) 
    self:regTouch(layer, onTouchBegan, onTouchEnded, onTouchMoved)
    
    -- 添加帧事件刷新
    local function update(dt)
        if (m_isMoving) then
            m_dragDt = m_dragDt + dt
        else
            if (m_dragV ~= 0) then
                local curPos = img_BG:getPositionX()
                local curPos2 = img_MidBG:getPositionX()
                local curPos3 = Panel_Front:getPositionX()
                if(curPos + m_dragV * dt >= scopeMinX and curPos + m_dragV * dt <= scopeMaxX) then
                    img_BG:setPositionX(curPos + m_dragV * dt)
                    img_MidBG:setPositionX(curPos2 + m_dragV * dt * 0.5)
                    Panel_Front:setPositionX(curPos3 + m_dragV * dt * 1.1)
                else
                    local curPosFix = scopeMaxX                   
                    if(math.abs(curPos - scopeMinX) < math.abs(curPos - scopeMaxX)) then
                        curPosFix = scopeMinX
                    end                  
                    img_BG:setPositionX(curPosFix)
                    m_dragV = 0
                    m_isMoving = false
                    return
                end
                if (m_dragV > 0) then
                    if (m_dragV < 100) then
                        m_dragV = m_dragV - 10
                    elseif (m_dragV < 200) then
                        m_dragV = m_dragV - 20
                    else
                        m_dragV = m_dragV - 40
                    end
                    if (m_dragV < 0) then
                        m_dragV = 0
                    end
                elseif (m_dragV < 0) then
                    if (m_dragV > -100) then
                        m_dragV = m_dragV + 10
                    elseif (m_dragV > -200) then
                        m_dragV = m_dragV + 20
                    else
                        m_dragV = m_dragV + 40
                    end
                    if (m_dragV > 0) then
                        m_dragV = 0
                    end
                end
            end
        end
    end
    
   layer:scheduleUpdateWithPriorityLua(update, 0)	
end

function MainCityScene:btnListener(index)
    self:recordBGPosX() 
	if index == 1 then
      cclog("PVE")
      self:pveCallback()  
	elseif index == 2 then
      cclog("阵容")
      SWITSCENE("battle/BattleTeamScene")
	elseif index == 3 then
      cclog("点将台")
      SWITSCENE("gacha/GachaScene")
	elseif index == 4 then
      cclog("地图")  
        SWITSCENE("world/WorldScene")
	elseif index == 5 then
      cclog("过关斩将")   
      PopMsg.getInstance():flashShow(LANG("由于程序员放假回家生孩子，这个功能还没做完……"))   
	elseif index == 6 then
      cclog("远征")
      PopMsg.getInstance():flashShow(LANG("由于程序员放假回家生孩子，这个功能还没做完……"))          
    elseif index == 7 then
      cclog("押镖")
      PopMsg.getInstance():flashShow(LANG("由于程序员放假回家生孩子，这个功能还没做完……"))    
	elseif index == 8 then
	  cclog("竞技场")
      ctlHighladderGetListRequest:sendRequest(handler(self,self.cutArenaPanel));   
    elseif index == 9 then
      cclog("福利关卡")
      PopMsg.getInstance():flashShow(LANG("由于程序员放假回家生孩子，这个功能还没做完……"))
    elseif index == 10 then
      cclog("战盟")
      PopMsg.getInstance():flashShow(LANG("由于程序员放假回家生孩子，这个功能还没做完……"))
	elseif index == 11 then
      cclog("冲出重围")
      PopMsg.getInstance():flashShow(LANG("由于程序员放假回家生孩子，这个功能还没做完……"))
    end 
end

function MainCityScene:cutArenaPanel(success, data, code, msg)
    if(success) then
        SWITSCENE("arena/ArenaScene",data)
    else
        release_print("ffffffffffffff")
    end
end

function MainCityScene:pveCallback()
    model_user.selectAreaId = 0
    model_user.selectStageType = pbCommon.StageType.NORMAL
    helper_pve:initStage(function(sucess,data)
        if sucess then
            SWITSCENE("battle/ChapterScene")
        end
    end)
end

function MainCityScene:regTouch(node, onTouchBegan, onTouchEnded, onTouchMoved)
    local evListen = cc.EventListenerTouchOneByOne:create()
    if onTouchEnded ~= nil then
        evListen:registerScriptHandler(onTouchBegan, cc.Handler.EVENT_TOUCH_BEGAN)
    end
    if onTouchEnded ~= nil then
        evListen:registerScriptHandler(onTouchEnded, cc.Handler.EVENT_TOUCH_ENDED)
    end
    if onTouchMoved ~= nil then
        evListen:registerScriptHandler(onTouchMoved, cc.Handler.EVENT_TOUCH_MOVED)
    end
    local evDisp = node:getEventDispatcher()
    evDisp:addEventListenerWithSceneGraphPriority(evListen, node)
end

function MainCityScene:onExit()
    self:onExitHandler()
end

function MainCityScene:addRightSlideBar(rightBtn, isShow)  
    local panel_Root = rightBtn:getChildByName("Panel_RightButton")
    local btn_Triangle =  panel_Root:getChildByName("RightBtn_Plus_43") -- 三角按钮显示
    local btn_Triangle2 =  panel_Root:getChildByName("RightBtn_Plus_42") -- 三角按钮隐藏    
    local panel_Bg = panel_Root:getChildByName("Panel_1"):getChildByName("Panel_RightBtn_36") -- 下拉条
    panel_Root:getChildByName("Panel_1"):setTouchEnabled(false)
    local bgSize = panel_Bg:getContentSize()
    panel_Bg:setTouchEnabled(true)
    local showBgY = panel_Bg:getPositionY()
    local hideBgY = showBgY + bgSize.height
      
    local bShow = model_user.MAINCITYSCENE_SLIDERBAR
    if bShow == true then  -- 显示状态
        panel_Bg:show()
        panel_Bg:setPositionY(showBgY)
        btn_Triangle:show()
        btn_Triangle2:hide()
    else                   -- 隐藏状态
        panel_Bg:hide() 
        panel_Bg:setPositionY(hideBgY)  
        btn_Triangle:hide()
        btn_Triangle2:show()
    end
 
    local antTime = 0.3 -- 动画播放时间
    -- 隐藏
    self:addBtnTouchEventListener(btn_Triangle,function(sender,eventType)
        if eventType==2 then
            local move = cc.MoveTo:create(antTime, cc.p(panel_Bg:getPositionX(), hideBgY))
            local delay =  cc.DelayTime:create(1.9)
            local func = cc.CallFunc:create(function() panel_Bg:hide() end)
            local seq = cc.Sequence:create(move, delay, func)
            panel_Bg:stopAllActions()
            panel_Bg:setTouchEnabled(true)
            panel_Bg:runAction(seq)
            btn_Triangle:hide()
            btn_Triangle2:show()
            model_user.MAINCITYSCENE_SLIDERBAR = false         
            self:updateSliderBarRedPoint(rightBtn)
        end
    end)
    -- 显示
    self:addBtnTouchEventListener(btn_Triangle2,function(sender,eventType)
        if eventType==2 then        
            local move2 = cc.MoveTo:create(antTime, cc.p(panel_Bg:getPositionX(), showBgY))
            panel_Bg:show() 
            local seq2 = cc.Sequence:create(move2)
            panel_Bg:stopAllActions()
            panel_Bg:setTouchEnabled(true)
            panel_Bg:runAction(seq2)
            btn_Triangle:show()
            btn_Triangle2:hide()
            model_user.MAINCITYSCENE_SLIDERBAR = true
            self:updateSliderBarRedPoint(rightBtn)
        end
    end)
    
    -- 按钮回调
    local j = 0
    for i = 0,12,2  do
       local k = i + 30
       j = j+1
    local btn = panel_Bg:getChildByName(LANG("RightBtn_0%d_%d",j,k))
        if i == 0 then --武将
            self:addBtnTouchEventListener(btn,function(sender,eventType)
             if eventType==2 then 
                self:recordBGPosX()
                SWITSCENE("hero/HeroListScene")
             end
          end)
        elseif i == 2 then --背包
            self:addBtnTouchEventListener(btn,function(sender,eventType)
              if eventType==2 then 
                 self:recordBGPosX()
                 SWITSCENE("bag/BagScene")
              end
           end)
        elseif i == 4 then --珍宝
            self:addBtnTouchEventListener(btn,function(sender,eventType)
              if eventType==2 then 
                 self:recordBGPosX()
                 SWITSCENE("treasure/TreasureListPanel")
              end
           end)
        elseif i == 6 then --商城
            self:addBtnTouchEventListener(btn,function(sender,eventType)
              if eventType==2 then 
                    ctlShopListRequest:sendRequest(function(sucess,data, code, msg)
                        self:recordBGPosX()
                        if sucess then
                            SWITSCENE("arena/StoreScene",data)
                        end

                    end, pbCommon.ShopListType.ITEM);

                end
            end)
        end  
    end

    local RightBtn_05_36 = panel_Bg:getChildByName("RightBtn_05_36")
     self:addBtnTouchEventListener(RightBtn_05_36,function(sender,eventType)
              if eventType==2 then
            SWITSCENE("hero/AdvancedListScene", {from = "MainCityScene"})
              end
     end)
end

-- 侧边栏红点状态显示
function MainCityScene:updateSliderBarRedPoint(rightBtn)
    local panel_Root = rightBtn:getChildByName("Panel_RightButton")
    local img_Root = panel_Root:getChildByName("Panel_1"):getChildByName("Panel_RightBtn_36")
    local rigthImg = panel_Root:getChildByName("Tip_RightBtn_Plus") -- 右侧三角按钮红点
    local generalImg = img_Root:getChildByName("Tip_RightBtn_01_75")   -- 武将红点
    local bagImg = img_Root:getChildByName("Tip_RightBtn_02_73")   -- 背包红点
    local treasureImg = img_Root:getChildByName("Tip_RightBtn_03_71")   -- 珍宝红点
    local shopImg = img_Root:getChildByName("Tip_RightBtn_04_69")  --商城红点

    local bRightBBarRed = false -- 右侧边栏红点
    local bGeneralRed = false -- 武将
    local bBagRed = false -- 背包
    local bTreasureRed = false --  珍宝
    local bShopRed = false -- 商城
  
-- 武将红点
    monster_Notification:delAll()
    monster_Notification:detectAll()
    if(monster_Notification:hasSpecialNotification(NOTIFICATION_TYPE.MONSTER_STAR_UP)) then
        bGeneralRed = true
        cclog("MONSTER_STAR_UP")
    elseif(monster_Notification:hasSpecialNotification(NOTIFICATION_TYPE.MONSTER_QUALITY_UP)) then
        bGeneralRed = true
        cclog("MONSTER_QUALITY_UP")
    elseif(monster_Notification:hasSpecialNotification(NOTIFICATION_TYPE.MONSTER_MERGE)) then
        bGeneralRed = true
        cclog("MONSTER_MERGE")
    elseif(monster_Notification:hasSpecialNotification(NOTIFICATION_TYPE.EQUIPMENT_QUALITY_UP)) then
        bGeneralRed = true
        cclog("EQUIPMENT_QUALITY_UP")
    end
    
   if bGeneralRed then
     bRightBBarRed = true
   end
   
   
    if bRightBBarRed then
        rigthImg:show()
    else
        rigthImg:hide()
    end
       
    if bGeneralRed then
        generalImg:show()
    else
        generalImg:hide()
    end
       
    -- 弹出状态 强制隐藏三角红点
    if model_user.MAINCITYSCENE_SLIDERBAR then
        rigthImg:hide()
    end
     
end

return MainCityScene