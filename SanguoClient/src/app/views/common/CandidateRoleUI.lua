local CandidateRoleUI = class("CandidateRoleUI", cc.Node)

--[[--
- ctor: 构造函数
@param: 
params.layer:  触控层(美术UI在cocostudio添加)
params.nodeList: 武将精灵容器
params.btnNode: 左右选中按钮
params.callBack: 选择武将回调
]]

function CandidateRoleUI:ctor(params)
    self:initConfig()
    self:initUI(params.layer, params.nodeList, params.btnNode, params.callBack)
end

function CandidateRoleUI:initConfig()
    -- 椭圆配置
    self.EllipseConfig = {}
    -- layer已经旋转角度 弧度
    self._angle = 0
    -- item项集合,_children顺序会变化，新建数组保存顺序
    self._items = {}
    -- 单位角度 弧度
    self._unitAngle = 0
    -- 是否可以滑动 播放动画是不可以滑动
    self._isScroll = true
    -- 2人角度偏移  弧度 2人 0.5 其他 0
    self._twoRoleOffSetAngle = 0.5
    -- 2人中心点位置偏移 -90
    self._twoRoleOffSetCenPos = -90 * (CC_SCREEN_SIZE.width / CC_DESIGN_RESOLUTION.width)
    -- 图片相对于原图缩放比例
    self._RoleImgZoom = 1.0
    -- 人物选择整体偏移量
    self.offSetY = 0 
    -- 缩放比例
    self._ScaleFactor = 0.67
    -- 透明度变化
    self._OpacityFactor = 0.7
    -- 动画运行时间
    self.animationDuration = 0.3
    -- 左右按钮点击标志位  左 true 有 false
    self._isBtnTouchLeft = true
    -- 每帧移动的角度和
    self._btnFrameAngle = 0
    -- 选择回调函数
    self._callBack = {}
    -- 左右选择按钮
    self._btnNode = {}
    -- 图片索引值 1000 用于图片渐变
    self.imgTag = 1000
end

function CandidateRoleUI:initUI(layer, nodeList, btnNode, callBack)
    self._callBack = callBack
    layer:ignoreAnchorPointForPosition(false)-- 开启锚点有效
    layer:show()
    self._btnNode = btnNode
    -- 图片缩放比例
    self._ScaleFactor = self._ScaleFactor * self._RoleImgZoom
    local LayerSize = layer:getContentSize()  
    local LayerPos = cc.p(LayerSize.width*0.5,LayerSize.height*0.5)  
    local itemCnt = #nodeList  -- 传入sprite个数
    -- 元素个数大于1个 添加触控层
    if itemCnt > 1 then
        self:addTouchListner(layer)
    end
    -- 元素个数为2个 添加角度偏移
    if itemCnt == 2 then
        self:reset()
        self:setAngle(self._twoRoleOffSetAngle)
    else
        self._twoRoleOffSetAngle = 0
        self:reset()
        self._twoRoleOffSetCenPos = 0
    end
    -- 如果只有1个人物 隐藏左右按钮
    if itemCnt == 1 then
        if btnNode[1] then
            btnNode[1]:hide()
        end
        if btnNode[2] then
            btnNode[2]:hide()
        end
    end  
    -- 设置椭圆路径信息    
    self.EllipseConfig.ellipseA = LayerSize.width * 0.25
    --椭圆b的长度
    self.EllipseConfig.ellipseB = self.EllipseConfig.ellipseA * 0.125
    --椭圆的中心坐标
    self.EllipseConfig.cenPos = cc.p(LayerPos.x + self._twoRoleOffSetCenPos, LayerPos.y) 
    -- 添加item
    local itemCnt = #nodeList
    self:setUnitAngle(2 * math.pi / itemCnt)
    for i = 1,#nodeList do
        local item = nodeList[i]     
        self:addChild(item)
        table.insert(self._items,item)
    end
    -- 初始化位置
    self:updatePosition()
end

-- 通过index 设置位置
function CandidateRoleUI:setItemPosByIndex(index)
     if (#self._items) > 1 and (index <= #self._items) then      
        self:setAngle(self._unitAngle * (index - 1)*(-1) + self._twoRoleOffSetAngle) 
     end
    self:updatePosition()
end

-- 废弃接口
function CandidateRoleUI:removeCandidateRoleUI(layer)
    layer:unscheduleUpdate()
    for i= 1, #(self._items) do
        self._items[i]:stopAllActions()
    end
    layer:removeAllChildren()
end

--更新位置
function CandidateRoleUI:updatePosition()
    local disX = self.EllipseConfig.ellipseA
    local disY =  self.EllipseConfig.ellipseB
    local cenPos = self.EllipseConfig.cenPos
    for i = 1, #(self._items) do
        local x = cenPos.x + disX*math.sin((i-1)* self._unitAngle + self:getAngle())
        local y = cenPos.y - disY*math.cos((i-1)* self._unitAngle + self:getAngle())      
        self._items[i]:setPosition(cc.p(x, y))
        self._items[i]:setLocalZOrder(math.round(-y))
        local img = self._items[i]:getChildByTag(self.imgTag)
        if(img) then
            img:setOpacity(math.round(self._OpacityFactor * 255 + (1 - self._OpacityFactor) * 255 * math.cos((i-1)* self._unitAngle + self:getAngle())))
        end   
        local info =  self._items[i]:getChildByName("info")
        if(info) then
            info:setOpacity(math.round(self._OpacityFactor * 255 + (1 - self._OpacityFactor) * 255 * math.cos((i-1)* self._unitAngle + self:getAngle())))
        end   
            
        self._items[i]:setScale(self._ScaleFactor + (self._RoleImgZoom - self._ScaleFactor)* math.cos((i-1)* self._unitAngle + self:getAngle()))
    end
end

--按钮点击逐帧更新位置
function CandidateRoleUI:updatePositionByFrame(layer,dt)
    local _speedFrameAngle = 0.15
    if math.abs(self._btnFrameAngle)  > (self._unitAngle* 0.5) then
        _speedFrameAngle = 0.09
    end
    local isFinish = false
    if self._isBtnTouchLeft then
        self._btnFrameAngle = self._btnFrameAngle - _speedFrameAngle
        if self._btnFrameAngle <= -(self._unitAngle) then
            isFinish = true
            self._btnFrameAngle = -(self._unitAngle)    
        end
    else
        self._btnFrameAngle = self._btnFrameAngle + _speedFrameAngle
        if self._btnFrameAngle >=  self._unitAngle then
            isFinish = true
            self._btnFrameAngle =  self._unitAngle  
        end
    end

    local disX = self.EllipseConfig.ellipseA
    local disY =  self.EllipseConfig.ellipseB
    local cenPos = self.EllipseConfig.cenPos
    for i = 1, #(self._items) do
        local x = cenPos.x + disX*math.sin((i-1)* self._unitAngle + self._btnFrameAngle + self:getAngle())
        local y = cenPos.y - disY*math.cos((i-1)* self._unitAngle + self._btnFrameAngle + self:getAngle())
        self._items[i]:setPosition(cc.p(x, y))
        self._items[i]:setLocalZOrder(math.round(-y))
        
        local img = self._items[i]:getChildByTag(self.imgTag)
        if(img) then
            img:setOpacity(math.round(self._OpacityFactor * 255 + (1 - self._OpacityFactor) * 255 * math.cos((i-1)* self._unitAngle + self._btnFrameAngle + self:getAngle())))
        end   
        local info =  self._items[i]:getChildByName("info")
        if(info) then
            info:setOpacity(math.round(self._OpacityFactor * 255 + (1 - self._OpacityFactor) * 255 * math.cos((i-1)* self._unitAngle + self._btnFrameAngle + self:getAngle())))  
        end       
        self._items[i]:setScale(self._ScaleFactor + (self._RoleImgZoom - self._ScaleFactor)* math.cos((i-1)* self._unitAngle + self._btnFrameAngle + self:getAngle()))
    end
    -- 运动结束
    if isFinish then
        local angle = self:getAngle() + self._btnFrameAngle
        while (angle<0)
        do
            angle = angle + math.pi * 2
        end
        while (angle>math.pi * 2)
        do
            angle = angle - math.pi * 2
        end
        self:setAngle(angle)
        self._btnFrameAngle = 0
        self:btnEndTouch(layer)
    end
end

--更新位置，有动画
function CandidateRoleUI:updatePositionWithAnimation(layer)
    for i= 1, #(self._items) do
         self._items[i]:stopAllActions() 
    end
    self._isScroll = false   
    -- 动画播放完成时
    local func = function() 
        self._callBack( self._items[self:getSelectIndex()],self:getSelectIndex())
        self._isScroll = true
    end
    local delay = cc.DelayTime:create(self.animationDuration)
    local callFunc = cc.CallFunc:create(func)
    local seq = cc.Sequence:create(delay,callFunc)
    layer:runAction(seq)

    local disX = self.EllipseConfig.ellipseA
    local disY = self.EllipseConfig.ellipseB
    local cenPos = self.EllipseConfig.cenPos

    for i= 1, #(self._items) do 
        local x = cenPos.x + disX * math.sin((i-1)* self._unitAngle + self:getAngle())
        local y = cenPos.y - disY * math.cos((i-1)* self._unitAngle + self:getAngle())       
        local moveTo = cc.MoveTo:create(self.animationDuration, cc.p(x, y))
        self._items[i]:runAction(moveTo) 
        local fadeTo = cc.FadeTo:create(self.animationDuration, math.round(self._OpacityFactor * 255 + (1 - self._OpacityFactor) * 255 * math.cos((i-1)* self._unitAngle + self:getAngle())))    
        local img = self._items[i]:getChildByTag(self.imgTag)
        if(img) then
            img:runAction(fadeTo)  
        end   
        local fadeTo2 = cc.FadeTo:create(self.animationDuration, math.round(self._OpacityFactor * 255 + (1 - self._OpacityFactor) * 255 * math.cos((i-1)* self._unitAngle + self:getAngle())))    
        local info =  self._items[i]:getChildByName("info")
        if(info) then
            info:runAction(fadeTo2) 
        end   
        local scaleTo = cc.ScaleTo:create(self.animationDuration, self._ScaleFactor + (self._RoleImgZoom - self._ScaleFactor)* math.cos((i-1)* self._unitAngle + self:getAngle()))
        self._items[i]:runAction(scaleTo)
        self._items[i]:setLocalZOrder(math.round(-y))
    end
end

--forward为移动方向  当超过1/3，进1;true 为正向  false 负 
function CandidateRoleUI:rectify(forward)
    local angle = self:getAngle()
    while (angle<0)
    do
        angle = angle + math.pi * 2
    end
    while (angle>math.pi * 2)
    do
        angle = angle - math.pi * 2
    end        
    if forward  then
        angle = (math.floor(((angle + self:getUnitAngle() / 3*2) / self:getUnitAngle()))*self:getUnitAngle())
    else
        angle = (math.floor(((angle + self:getUnitAngle() / 3 ) / self:getUnitAngle()))*self:getUnitAngle()) 
    end
    self:setAngle(angle + self._twoRoleOffSetAngle)
end

--重置  操作有旋转角度设为0
function CandidateRoleUI:reset()
    self._angle = 0
end

--设置layer角度(弧度)
function CandidateRoleUI:setAngle(angle)
     self._angle = angle
end

function CandidateRoleUI:getAngle()
    return  self._angle
end

--设置单位角度(弧度)
function CandidateRoleUI:setUnitAngle(angle)
     self._unitAngle = angle
end

function CandidateRoleUI:getUnitAngle()
    return  self._unitAngle
end

--滑动距离转换角度,转换策略为  移动半个Menu.width等于_unitAngle
function CandidateRoleUI:disToAngle(dis)
    local disX = self.EllipseConfig.ellipseA
    local width = disX / 2;
    return dis / width * self:getUnitAngle() / 5
end

--返回被选中的index
function CandidateRoleUI:getSelectIndex()
    if (#(self._items) <= 0) then
        return -1
    end
    --  找出Y坐标最小的为选中项
    local minPosY = 10000
    local selIdx = 1
    for i = 1, #(self._items) do
        local y =  self._items[i]:getPositionY()
        if(y < minPosY) then
            minPosY = y
            selIdx = i
        end
    end
    return selIdx
end

--添加触控层
function CandidateRoleUI:addTouchListner(layer)
    local function onTouchBegan(sender)
        -- 先停止所有可能存在的动作
        for i= 1, #(self._items) do
            self._items[i]:stopAllActions()
        end
        self.getStartLocation = sender:getTouchBeganPosition().x
        self.touchMovePosition = self.getStartLocation      
        self.touchBeganAngle =  self:getAngle()    
         if(self._isScroll) then
            return true
         else
        end
        return false
    end

    local movePosX = 0 -- 记录临界值滑动位置
    local bRight = true  -- true 右 false 左

    local function onTouchMoved(sender)
        local des = sender:getTouchMovePosition().x - self.touchMovePosition
        local angle = self:disToAngle(des)  
        if(math.abs( self.touchBeganAngle - self:getAngle()) <= 1.1* self:getUnitAngle()) then --拖动条件限定1.1倍角度
            self:setAngle(self:getAngle() + angle)
            self:updatePosition()
            self.touchMovePosition = sender:getTouchMovePosition().x   
            movePosX =  sender:getTouchMovePosition().x               
            if des > 0 then
                bRight = true
            else
                bRight = false
            end                        
        else
            if (sender:getTouchMovePosition().x <=  movePosX) and (bRight == true) then -- 一直向右滑动
                local angle = self:disToAngle(des)
                self:setAngle(self:getAngle() + angle)
                self:updatePosition()
            end
            
            if (sender:getTouchMovePosition().x >=  movePosX) and (bRight == false) then -- 一直向左滑动
                local angle = self:disToAngle(des)
                self:setAngle(self:getAngle() + angle)
                self:updatePosition()
            end           
        end
    end

    local function onTouchEnded(sender)
        local xDelta = sender:getTouchEndPosition().x - self.getStartLocation
        self:rectify(xDelta > 0)
        self:updatePositionWithAnimation(layer)
    end
    self:regTouch(layer, onTouchBegan, onTouchEnded, onTouchMoved)

    -- 添加左右选择按钮
    -- 添加layer update  用于位置刷新
    local function update(delta)                                            
        self:updatePositionByFrame(layer, delta)    
    end

    local btnTouchBegan = function ()
        layer:unscheduleUpdate()
        layer:scheduleUpdateWithPriorityLua(update, 0)
    end

    local btnLeftCB = function ()
        if self._isScroll then
            self._isScroll = false
            self._isBtnTouchLeft = true
            btnTouchBegan()
        end
    end

    local btnrightCB = function ()
        if self._isScroll then
            self._isScroll = false
            self._isBtnTouchLeft = false
            btnTouchBegan()
        end
    end
    -- 添加左右按钮回调
    self:addSelectBtn(btnLeftCB, btnrightCB)

    -- layer 删除时清空数据                             
    local function onNodeEvent(tag)
        if tag == "exit" then
           -- self:removeCandidateRoleUI()
        end
    end
    layer:registerScriptHandler(onNodeEvent)
end

-- 外部调用左右按钮回调
function CandidateRoleUI:addSelectBtn(btnLeftCB, btnrightCB)
    if self._btnNode[1] then
        self._btnNode[1]:setTouchEnabled(true)
        self._btnNode[1]:show()
        self._btnNode[1]:addTouchEventListener(function(sender,eventType)
            if eventType == 2 then
                btnLeftCB()
            end
        end)
    end
    if self._btnNode[2] then
        self._btnNode[2]:setTouchEnabled(true)
        self._btnNode[2]:show()
        self._btnNode[2]:addTouchEventListener(function(sender,eventType)
            if eventType == 2 then
                btnrightCB()
            end
        end)
    end
end  

-- 控件内部添加左右选中按钮 用于测试
function CandidateRoleUI:addSelectBtnBySelf(layer, btnLeftCB, btnrightCB)
    local lBtnPos = cc.p(155,display.cy)
    local rBtnPos = cc.p(display.right-155,display.cy)
    local lBtn = cc.MenuItemImage:create("ArrowBtn_1.png", "ArrowBtn_2.png")
        :onClicked(btnLeftCB)
    cc.Menu:create(lBtn)
        :move(cc.p(lBtnPos))
        :addTo(layer)
    lBtn:setScale(-1)
    local rBtn = cc.MenuItemImage:create("ArrowBtn_1.png", "ArrowBtn_2.png")
        :onClicked(btnrightCB)
    cc.Menu:create(rBtn)
        :move(cc.p(rBtnPos))
        :addTo(layer)
end

function CandidateRoleUI:btnEndTouch(layer)
    layer:unscheduleUpdate()
    self._callBack(self._items[self:getSelectIndex()], self:getSelectIndex())
    self._isScroll = true
end

function CandidateRoleUI:regTouch(node, onTouchBegan, onTouchEnded, onTouchMoved)
      node:addTouchEventListener(function(sender,eventType)
        if eventType ==0 then
            onTouchBegan(sender)
        elseif eventType ==1 then
            onTouchMoved(sender)
        elseif eventType ==2 then
            onTouchEnded(sender)
        elseif eventType ==3 then
            onTouchEnded(sender)       	
        end
      end)
end

return CandidateRoleUI