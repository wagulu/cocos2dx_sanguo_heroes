local WorldScene = class("WorldScene", cc.load("mvc").ViewBase)

local monster_Notification = _REQUIRE("model/helper/helper_monsterNotification.lua")
local model_user = _REQUIRE("model.model_user")

_REQUIRE "cocos.cocos2d.json"
WorldScene.RESOURCE_FILENAME = "Scene/WorldScene.csb"

--local time1 =   model_user:getCurrentServerTimestamp()
--local tab1 = os.date("*t", time1)
--if(tab1.hour >= 6 and tab1.hour < 18) then    -- 白天
--    WorldScene.RESOURCE_FILENAME = "Scene/MainScene.csb"
--else  -- 夜晚
--    WorldScene.RESOURCE_FILENAME = "Scene/MainScene_N.csb"
--end

--cclog("getCurrentServerTimestamp "..time1.. " hour "..tab1.hour)

function WorldScene:onCreate()
    local WorldScene = self:getResourceNode():getChildByName("WorldScene")

    self.Panel_Bg = WorldScene:getChildByName("Panel_Bg")
    self.Panel_Bg:setTouchEnabled(true)
    local Panel_C = WorldScene:getChildByName("Panel_C")
    local Panel_R = WorldScene:getChildByName("Panel_R")  
    local Panel_B = WorldScene:getChildByName("Panel_B")    
    local Panel_B = WorldScene:getChildByName("Panel_B")   
    local Panel_T = WorldScene:getChildByName("Panel_T")
    local Panel_Top = Panel_T:getChildByName("Panel_Top")
    local Button_Back = Panel_Top:getChildByName("Button_Back")
    self:addBtnTouchEventListener(Button_Back,function(sender,eventType)
        if eventType==2 then
            self:sceneBack(function()
                SWITSCENE("home/MainCityScene")
            end)
        end

    end) 
    
--    SCREEN_SCALE_BG(Panel_Bg)
--    SCREEN_TOP(panel_Top)
--    SCREEN_BOTTOM(panel_Bottom)   
--    SCREEN_BOTTOM(Panel_Front_BG)
    self:addScrollBgListener()
end

-- 记录页面跳转后 背景移动位置
function WorldScene:recordBGPosX()
 
end

-- 添加背景滑动层监听
function WorldScene:addScrollBgListener()

    local bTouchEnabled = false  -- 滑动时触控无效
    local touchMoveBeganX = 0
    local touchMoveBeganY = 0
    local touchMoveEndX = 0   
    local touchMoveEndY = 0 
    local timeIntervel = 0.3 -- 连续点击时间间隔  
    local bTouchIntervel = true -- 点击间隔
    local id =1
    local img_BG = self.Panel_Bg:getChildByName("Image_Bg")
    local scale = img_BG:getScale()  -- 美术将背景图片扩充到了1.3
    local bgWid = img_BG:getContentSize().width * scale  
    local bgHid = img_BG:getContentSize().height * scale 
    local bgX  = img_BG:getPositionX()
    local bgY = img_BG:getPositionY()
    local bgOffSetX = (bgWid - CC_DESIGN_RESOLUTION.width) * 0.5 
    local bgOffSetY = (bgHid - CC_DESIGN_RESOLUTION.height) * 0.5 
    local scopeMinX = bgX - bgOffSetX
    local scopeMinY = bgY - bgOffSetY
    local scopeMaxX = bgX + bgOffSetX
    local scopeMaxY = bgY + bgOffSetY
    -- 添加背景滑动触控层
    local m_isMoving = false
    local m_dragX = 0
    local m_dragY = 0
    local m_dragDt = 0.0

    local function onTouchBegan(touch, event)
        touchMoveBeganX =  touch:getLocation().x   
        touchMoveBeganY =  touch:getLocation().y   
        m_dragDt = 0.0
        m_isMoving = true
        return true
    end  

    local function onTouchMoved(touch, event)
        local x = img_BG:getPositionX()
        local y = img_BG:getPositionY()
        local d = touch:getDelta()      
        local moveX = x+d.x  
        local moveY = y+d.y  
        if((moveX >= scopeMinX and moveX <= scopeMaxX)) then
            img_BG:setPositionX(moveX)
        end  
        cclog("y:"..d.y)
        if((moveY >= scopeMinY and moveY <= scopeMaxY)) then
            img_BG:setPositionY(moveY)
        end
        if(math.abs(touchMoveBeganX -  touch:getLocation().x) >= 40 or math.abs(touchMoveBeganY -  touch:getLocation().y) >= 40) then
--            if touchBtn then
--                touchBtn:setScale(1.0)  
--                touchSelect:hide()
--            end
            bTouchEnabled = false
        end        
    end

    local function onTouchEnded(touch, event)
        touchMoveEndX = touch:getLocation().x     
        touchMoveEndY = touch:getLocation().y 
        if(m_isMoving) then
            local curPosX = img_BG:getPositionX()
            local curPosY = img_BG:getPositionY()
            if((curPosX >= scopeMinX and curPosX <= scopeMaxX)) then
--                if (math.abs(touchMoveEndX - touchMoveBeganX) < 40) then
--                    return
--                end
--                if (m_dragDt > 0.4)  then
--                    return
--                end
                if (m_dragDt > 0.0 and m_dragDt < 0.4 and math.abs(touchMoveEndX - touchMoveBeganX) > 40) then
                    m_dragX = (touchMoveEndX - touchMoveBeganX) / m_dragDt
                end
            else
                m_dragX = 0
                m_isMoving = false
            end
            if((curPosY >= scopeMinY and curPosY <= scopeMaxY) ) then
--                if (math.abs(touchMoveEndY - touchMoveBeganY) < 40) then
--                    return
--                end
--                if (m_dragDt > 0.4)  then
--                    return
--                end
                if (m_dragDt > 0.0 and m_dragDt < 0.4 and math.abs(touchMoveEndY - touchMoveBeganY) > 40) then
                    m_dragY = (touchMoveEndY - touchMoveBeganY) / m_dragDt
                end
            else
                m_dragY = 0
                m_isMoving = false
            end
        end 
        m_isMoving = false  
    end

    local layer = display.newLayer()
    self:getResourceNode():getChildByName("WorldScene"):addChild(layer) 
    self:regTouch(layer, onTouchBegan, onTouchEnded, onTouchMoved)

    -- 添加帧事件刷新
    local function update(dt)
        if (m_isMoving) then
            m_dragDt = m_dragDt + dt
        else
            if (m_dragX ~= 0) then
                local curPosX = img_BG:getPositionX()
                local curPosY = img_BG:getPositionY()
                if((curPosX + m_dragX * dt >= scopeMinX and curPosX + m_dragX * dt <= scopeMaxX)) then
                    img_BG:setPositionX(curPosX + m_dragX * dt)
                else
                    local curPosFix = scopeMaxX 
                    local curPosFiy = scopeMaxY                    
                    if(math.abs(curPosX - scopeMinX) < math.abs(curPosX - scopeMaxX)) then
                        curPosFix = scopeMinX
                    end                  
                    img_BG:setPositionX(curPosFix)
                    m_dragX = 0
                    m_isMoving = false
                    return
                end
                if (m_dragX > 0) then
                    if (m_dragX < 100) then
                        m_dragX = m_dragX - 10
                    elseif (m_dragX < 200) then
                        m_dragX = m_dragX - 20
                    else
                        m_dragX = m_dragX - 40
                    end
                    if (m_dragX < 0) then
                        m_dragX = 0
                    end
                elseif (m_dragX < 0) then
                    if (m_dragX > -100) then
                        m_dragX = m_dragX + 10
                    elseif (m_dragX > -200) then
                        m_dragX = m_dragX + 20
                    else
                        m_dragX = m_dragX + 40
                    end
                    if (m_dragX > 0) then
                        m_dragX = 0
                    end
                end
            end
            
            if (m_dragY ~= 0) then
                local curPosY = img_BG:getPositionY()
                if((curPosY + m_dragY * dt >= scopeMinY and curPosY + m_dragY * dt <= scopeMaxY)) then
                    img_BG:setPositionY(curPosY + m_dragY * dt)
                    
                else
                    local curPosFiy = scopeMaxY                    
                    if(math.abs(curPosY - scopeMinY) < math.abs(curPosY - scopeMaxY)) then
                        curPosFiy = scopeMinY
                    end                   
                    img_BG:setPositionY(curPosFiy)
                    m_dragY = 0
                    m_isMoving = false
                    return
                end

                if (m_dragY > 0) then
                    if (m_dragY < 100) then
                        m_dragY = m_dragY - 10
                    elseif (m_dragY < 200) then
                        m_dragY = m_dragY - 20
                    else
                        m_dragY = m_dragY - 40
                    end
                    if (m_dragY < 0) then
                        m_dragY = 0
                    end
                elseif (m_dragY < 0) then
                    if (m_dragY > -100) then
                        m_dragY = m_dragY + 10
                    elseif (m_dragY > -200) then
                        m_dragY = m_dragY + 20
                    else
                        m_dragY = m_dragY + 40
                    end
                    if (m_dragY > 0) then
                        m_dragY = 0
                    end
                end
            end
        end
    end

    layer:scheduleUpdateWithPriorityLua(update, 0)   
end


function WorldScene:regTouch(node, onTouchBegan, onTouchEnded, onTouchMoved)
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

function WorldScene:onExit()
    self:onExitHandler()
end


return WorldScene