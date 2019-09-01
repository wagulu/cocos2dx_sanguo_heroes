local ChooseHeroInBattleScene = class("ChooseHeroInBattleScene", cc.load("mvc").ViewBase)
ChooseHeroInBattleScene.NEXTSCENE = "battle/ChapterScene"
ChooseHeroInBattleScene.STOPFRAME = 55
ChooseHeroInBattleScene.RESOURCE_FILENAME = "Scene/ArrayPopup.csb"

local model_user = _REQUIRE("model/model_user")
local model_monster = _REQUIRE("model/static/model_monster")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage")
local model_static_library = _REQUIRE("src/model/static/model_static_library.lua")
local helper_team = _REQUIRE("model/helper/helper_team")
local helper_relationship = _REQUIRE("model/helper/helper_relationship")

_REQUIRE("pbCommon.lua")

function ChooseHeroInBattleScene:onCreate()
    self:initUi()
end


function ChooseHeroInBattleScene:initUi()
    local Scene = self:getResourceNode():getChildByName("Scene")
    local ArrayPopup = self:getResourceNode():getChildByName("ArrayPopup")
    local Panel_Center_Base = ArrayPopup:getChildByName("Panel_Center_Base")
    self.Panel_Bg = ArrayPopup:getChildByName("Panel_Bg")
    self.Panel_Center = Panel_Center_Base:getChildByName("Panel_Center")
    self.Panel_Top = Panel_Center_Base:getChildByName("Panel_Top")
    self.Panel_Bottom = Panel_Center_Base:getChildByName("Panel_Bottom")
    self.isShowLight = false
    ArrayPopup:setTouchEnabled(true)
    --SCREEN_TOP(self.Panel_Top)
    SCREEN_SCALE_BG(self.Panel_Bg)
    --SCREEN_BOTTOM(self.Panel_Bottom)
    self.isMoveAction = false
    self.monsterItems = {}
    self.monsterItems = clone(model_user.battleTeam)


    self.sortItems = {}
    self.itemViews = {}
    self.battleItemViews = {}
    self.deletCache = {}

    local level = model_user.level
    self.total = 1
    if level >= 15 then
        self.total = 3
    elseif level >= 7 then
        self.total = 2
    end

    local Button_Go = self.Panel_Center:getChildByName("Button_Go")
    self:addBtnTouchEventListener(Button_Go, function (sender, eventType)
        local isFull = true
        if eventType==2 then
            for i = 1,self.total do
                if self.deletCache[i] then
                    isFull = false
                end
            end

            if isFull == false then
                PopMsg.getInstance():flashShow(LANG("将军，出战位置不可为空哦"))
                return
            end

            self:saveData()
        end
    end)

    local Button_Back = ArrayPopup:getChildByName("Panel_Top_Base"):getChildByName("Panel_Top"):getChildByName("Button_Back")
    self:addBtnTouchEventListener(Button_Back, function (sender, eventType)
        if eventType==2 then

            local isFull = true
            for i = 1,self.total do
                if self.deletCache[i] then
                    isFull = false
                end
            end


            if isFull == false then 
                --PopMsg.getInstance():flashShow(LANG("将军，出战位置不可为空哦"))
                self:showDlg("common/CommonPopScene",{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text=LANG("将军，出战位置不可以为空哦！退出时无法\n保存您做出的更改！"),fun=function(data)
                    if data.cmd=="confirm" then
                        self:changeScene()
                    elseif data.cmd =="cancel" then 

                    end
                end})
                return
            end

            if self:isChangTeam() then  --队伍改变，是否保存并退出？
                self:showDlg("common/CommonPopScene",{confirmStr=LANG("保存"),cancelStr=LANG("不保存"),text=LANG("队伍发生改变，是否保存并退出？"),fun=function(data)
                    if data.cmd=="confirm" then
                        self:saveData()
                    elseif data.cmd =="cancel" then 
                        self:changeScene()
                    end
                end})
            return
            else
                self:changeScene()
            end
        end
    end)

    --可用的monsterItems排序成新的集合
    self:sortMonsterItems(self.monsterItems)
    for key,value in pairs(self.sortItems) do
        cclog("key = "..key)
        cclog("value.id = "..value.monsterId)
        cclog("value.pos = "..value.pos)
        cclog("value.isUse = "..value.isUse)
        cclog("-----------------")
    end
    --根据Item的数量view的顺序的集合
    self:initItemView()
    
    self:addAnimNode()

    self:updateView()



end

function ChooseHeroInBattleScene:showAction()
    local view1 = self.battleItemViews[2]
    local view = clone(view1)
    local index = view:getTag()
    local node1 = view:getChildByName("ProjectNode"..index)
    local node = clone(node1)
    --    node:stopAllActions();
    --    node:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
    --local location = node:convertToNodeSpace(cc.p(341.52, 108.86))
    local viewTarget = self.battleItemViews[1]
    local targetNode = viewTarget:getChildByName("ProjectNode"..viewTarget:getTag())
    local position2 = cc.p(targetNode:getPositionX(), targetNode:getPositionY())
    local targetPosition = viewTarget:convertToWorldSpace(position2)
    local location = view:convertToNodeSpace(targetPosition)

    --    view:stopAllActions();
    --    view:runAction(cc.Sequence:create(cc.MoveTo:create(1, cc.p(341.52, 108.86)), NULL) )
    node:stopAllActions();
    local origin = cc.p(node:getPositionX(),node:getPositionY())
    node:runAction(cc.Sequence:create(cc.MoveTo:create(0.5, location), cc.Place:create(origin), NULL) )
end

function ChooseHeroInBattleScene:showMoveAction(beginView, endView)
    local z = beginView:getLocalZOrder()
    local t = endView:getLocalZOrder()
    beginView:setLocalZOrder(1000)
    
    local beginPosition = cc.p(beginView:getPositionX(), beginView:getPositionY())
    local endPosition = cc.p(endView:getPositionX(), endView:getPositionY())
    beginView:stopAllActions()
    local callFun = cc.CallFunc:create(function()  self:updateView() self.isMoveAction = false beginView:setLocalZOrder(0) end)
    beginView:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1), cc.MoveTo:create(0.1, endPosition), cc.Place:create(beginPosition), callFun, NULL) )
end

function ChooseHeroInBattleScene:changeScene()
    if self.args[1] == 1 then
        SWITSCENE("battle/ChapterScene")
    elseif self.args[1] == 2 then
        local callback = self.args[2]
        callback()
        self:removeDlgNoAction()
    else
        SWITSCENE("home/HomeScene")
    end
end

function ChooseHeroInBattleScene:isChangTeam() 
    local res = false
    for i,item in pairs(self.monsterItems) do
        for key,value in pairs(model_user.battleTeam) do
            if item.pos == value.pos and item.monsterId ~= value.monsterId then
                res = true
            end
        end
    end

    return res
end

function ChooseHeroInBattleScene:saveData()
    ctlPlayerChangeBattleTeamRequest:sendRequest(function(success, data, code, msg)
        if(success) then
            PopMsg.getInstance():flashShow(LANG("保存成功"))
            self:changeScene()
        else
        --PopMsg.getInstance():flashShow(LANG("将军，出战位置不可为空哦"))
        end
    end, self.monsterItems);
end

function ChooseHeroInBattleScene:updateView()

    local current = 0
    current = self:getNumOfInBattle(self.total)
    self.Panel_Center:getChildByName("Text_1"):setString(LANG("上阵队伍:(%s/%s)",current, self.total))

    for key,monsterItem in pairs(self.sortItems) do
        local view = self.itemViews[key]
        local monster = model_monsterStorage:getMonster(monsterItem.monsterId)
        local index = view:getTag()
        --需要改UI
        local ProjectNode
        ProjectNode = view:getChildByName("ProjectNode"..index)

        local Node_Team = ProjectNode:getChildByName("Node_Team_Item")

        --Node_Team:getChildByName("Image_Chain"):setVisible(false)
        Node_Team:getChildByName("ProjectNode_1"):setVisible(false)
        Node_Team:getChildByName("Text_1"):setVisible(false)

        --星级显示
        local Panel_Star = Node_Team:getChildByName("Panel_Star")
        Panel_Star:setTouchEnabled(false)
        for k = 1,5 do
            Panel_Star:getChildByName("Image_Star_"..k):setVisible(k<= monster.star)
            
        end
        --武将信息
        Node_Team:getChildByName("Image_Hero"):loadTexture(model_static_library:getAsset(monster.id, pbCommon.ObjectType.Monster))
        Node_Team:getChildByName("Image__Job"):loadTexture("Job/Icon_job"..model_monster:getType(monster.id)..".png")
        Node_Team:getChildByName("Image_Head_bg"):loadTexture(model_monsterStorage:getFrameByQuality(monster.quality))
        view:getChildByName("Text"..index):setString(LANG("LV%s %s", monster.level, model_monster:getName(monster.id)))
        Node_Team:getChildByName("Text_Link"):setString(LANG("%s",self:getRelationShipCount(monsterItem)))

        Node_Team:getChildByName("Image_Add"):setVisible(false)

        --        local Button
        --        Button = view:getChildByName("Button"..index)

        local isInCache = self:isInCache(monsterItem)
        local isNotInBattle = isInCache or monsterItem.pos > 3
        local isNoMonsterPos = self:getNoMonsterPosInBattle()
        --TODO 未出战UI
        if isNotInBattle then
            view:getChildByName("Image_Text"..index):setVisible(false)
            view:getChildByName("Image_Off"..index):setVisible(false)
        else    --已出战UI
            view:getChildByName("Image_Text"..index):setVisible(true)
            view:getChildByName("Image_Off"..index):setVisible(true)
        end
        view:setTouchEnabled(true);
        view:addTouchEventListener(function (sender, eventType)
            if eventType ==0 then
            --                sender:stopAllActions();
            --                sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
            elseif eventType==2 then
                if self.isMoveAction then       --正在播放动画
                    return
                end
                self.isMoveAction = true
                --TODO 未出战
                if isNotInBattle then
                    if self:isFullInBattle() then
                        --动画
                        self:showLightBatteItem()
                        self.isMoveAction = false
                    else
                        if isInCache then
                            --self:removeFromTable(self.deletCache, monsterItem)

                            --table.remove(self.deletCache, monsterItem.pos)
                            self.deletCache[monsterItem.pos] = nil
                            self:showMoveAction(view, self.battleItemViews[monsterItem.pos])
                        elseif isNoMonsterPos ~= -1 then                --monsterId=0的出战位置
                            self:swapWithBattleMonster(monsterItem, isNoMonsterPos, key)
                            self:showMoveAction(view, self.battleItemViews[isNoMonsterPos])
                        else
                            local pos = self:swapWithCache(monsterItem, key)
                            if pos then
                                local endView = self.battleItemViews[pos]
                                self:showMoveAction(view, endView)
                            end

                        end

                        --self:updateView()
                    end
                else          --已经出战
                    if self.deletCache[monsterItem.pos] == nil then --添加武将
                        self:deleteInBattle(monsterItem.pos)
                        local beginView = self.battleItemViews[monsterItem.pos]
                        self:showMoveAction(beginView, view)
                        --self:updateView()
                end
                end

            end
        end)
    end

    local i = 1
    for key,view in pairs(self.battleItemViews) do
        self:updateBattleItem(view, key)
    end
end

function ChooseHeroInBattleScene:printArr(arr)
    for key,value in ipairs(arr) do
        cclog("key = "..key)
        cclog("value = "..value.monsterId)
    end
end

function ChooseHeroInBattleScene:updateBattleItem(view, pos)
    local index = view:getTag()
    local monsterItem = self:hasInBattle(pos)

    local isLock = false
    local level = model_user.level
    local limitDes
    if pos == 2 and level < 7 then
        isLock = true 
        limitDes = LANG("7级解锁")
    elseif pos == 3 and level < 15 then
        isLock = true 
        limitDes = LANG("15级解锁")
    end

    --    local Button = view:getChildByName("Button"..index)
    --    Button:setEnabled(true);
    view:getChildByName("frameLight_"..pos):setOpacity(0)
    view:setTouchEnabled(true);
    view:getChildByName("Text"..index):setString("")

    local ProjectNode = view:getChildByName("ProjectNode"..index)
    local Node_Team = ProjectNode:getChildByName("Node_Team_Item")
    Node_Team:getChildByName("Image_Head_bg"):loadTexture(model_monsterStorage:getFrameByQuality(1))    --将边框设置成1级的

    if isLock then 
        self:setMosterVisible(view, "lock", index)
        view:setTouchEnabled(false)
        Node_Team:getChildByName("Text_1"):setString(limitDes)
        return;
    end

    view:addTouchEventListener(function (sender, eventType)
        if eventType ==0 then
        --                sender:stopAllActions();
        --                sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
        elseif eventType==2 then
            if self.isMoveAction then       --正在播放动画
                return
            end
            self.isMoveAction = true
            if self.deletCache[pos] == nil then --添加武将
                self:deleteInBattle(pos)
                local endView = self:getItemViewByPos(pos)
                self:showMoveAction(view, endView) 
                --self:updateView()
            end
        end
    end)

    if self.deletCache[pos] ~= nil then
        self:setMosterVisible(view, "add", index)
        view:setTouchEnabled(false)
        return;
    end
    if monsterItem == nil then      --没有上阵武将
        self:setMosterVisible(view, "add", index)
        view:setTouchEnabled(false)
        return;
    end

    local monster = model_monsterStorage:getMonster(monsterItem.monsterId)


    self:setMosterVisible(view, "monster", index)




    local ProjectNode = view:getChildByName("ProjectNode"..index)
    local Node_Team = ProjectNode:getChildByName("Node_Team_Item")

    --Node_Team:getChildByName("Image_Chain"):setVisible(false)
    Node_Team:getChildByName("ProjectNode_1"):setVisible(false)
    Node_Team:getChildByName("Text_1"):setVisible(false)

    --星级显示
    local Panel_Star = Node_Team:getChildByName("Panel_Star")
    Panel_Star:setTouchEnabled(false)
    for k = 1,5 do
        Panel_Star:getChildByName("Image_Star_"..k):setVisible(k<= monster.star)
    end
    --武将信息
    Node_Team:getChildByName("Image_Hero"):loadTexture(model_static_library:getAsset(monster.id, pbCommon.ObjectType.Monster))
    Node_Team:getChildByName("Image__Job"):loadTexture("Job/Icon_job"..model_monster:getType(monster.id)..".png")
    view:getChildByName("Text"..index):setString(LANG("LV%s %s", monster.level, model_monster:getName(monster.id)))
    Node_Team:getChildByName("Text_Link"):setString(LANG("%s",self:getRelationShipCount(monsterItem)))
    Node_Team:getChildByName("Image_Head_bg"):loadTexture(model_monsterStorage:getFrameByQuality(monster.quality))

end

function ChooseHeroInBattleScene:setMosterVisible(view, type, pos)
    local ProjectNode = view:getChildByName("ProjectNode"..pos) 
    local Node_Team = ProjectNode:getChildByName("Node_Team_Item")
    Node_Team:getChildByName("Panel_Star"):setVisible(false)     --星级
    Node_Team:getChildByName("Image_Hero"):setVisible(false)      --头像
    Node_Team:getChildByName("Image__Job"):setVisible(false)    --品阶
    Node_Team:getChildByName("Text_1"):setVisible(false)          --解锁文字         
    Node_Team:getChildByName("Image_Add"):setVisible(false)       --添加"+"号
    --Node_Team:getChildByName("Image_Chain"):setVisible(false)     --解锁图片
    Node_Team:getChildByName("ProjectNode_1"):setVisible(false)
    Node_Team:getChildByName("Image_IconBg"):setVisible(false)    --缘分    
    Node_Team:getChildByName("Text_Link"):setVisible(false)

    if type == "lock" then
        --Node_Team:getChildByName("Image_Chain"):setVisible(true)
        Node_Team:getChildByName("ProjectNode_1"):setVisible(true)
        Node_Team:getChildByName("Text_1"):setVisible(true)
    elseif type == "monster" then
        Node_Team:getChildByName("Panel_Star"):setVisible(true)
        Node_Team:getChildByName("Image_Hero"):setVisible(true)
        Node_Team:getChildByName("Image__Job"):setVisible(true)
        Node_Team:getChildByName("Image_IconBg"):setVisible(true)    --缘分    
        Node_Team:getChildByName("Text_Link"):setVisible(true)
    elseif type == "add" then    
        Node_Team:getChildByName("Image_Add"):setVisible(true)
    end

end

function ChooseHeroInBattleScene:addAnimNode()
    self.lightActions = {}
    
    for i=1,3 do
        local Node_Team_Item_YellowLight, actionYellow = self:createNode("Node/Node_Team_Item_YellowLight.csb")
        local view = self.battleItemViews[i]
        local index = view:getTag()
        local ProjectNode = view:getChildByName("ProjectNode"..index)
        view:addChild(Node_Team_Item_YellowLight)
        Node_Team_Item_YellowLight:setPosition(ProjectNode:getPosition())
        Node_Team_Item_YellowLight:setLocalZOrder(-1)
        self.lightActions[i] = actionYellow
        actionYellow:gotoFrameAndPlay(0,false)
    end
end

function ChooseHeroInBattleScene:showLightBatteItem()
--    if self.isShowLight then
--        return
--    end
--
--    self.isShowLight = true
    
    for i = 1, self.total do
--        local view = self.battleItemViews[i]
--        view:getChildByName("frameLight_"..i):setOpacity(255)
        self.lightActions[i]:gotoFrameAndPlay(0, 20, false)
    end


--    local timerId = Timer:start(function()
--        for i = 1, self.total do
--            local view = self.battleItemViews[i]
--            view:getChildByName("frameLight_"..i):setOpacity(0)
--        end
--        self.isShowLight = false
--        self.isMoveAction = false
--    end, 1, 1, nil)
end

function ChooseHeroInBattleScene:getNumOfInBattle(total)

    for i = 1, self.total do
        if self.deletCache[i] then              --在删除出阵的缓存里
            total = total - 1
        end
        for j = 1, #(self.monsterItems) do      --第一次解锁，出战位置monsterId = 0
            if self.monsterItems[j].pos == i and self.monsterItems[j].monsterId == 0 then
                total = total - 1
        end
        end
    end

    return total
end

function ChooseHeroInBattleScene:getItemViewByPos(pos)
    local view
    for i=1,#(self.sortItems) do
        if self.sortItems[i].pos == pos then
            view = self.itemViews[i]
            break
        end
    end
    return view
end

function ChooseHeroInBattleScene:deleteInBattle(pos)
    for key,value in pairs(self.sortItems) do
        if value.pos == pos then
            self.deletCache[pos] = value
        end
    end
end
--与删除的缓存集合里面的monsterItem进行交换 
function ChooseHeroInBattleScene:swapWithCache(monsterItem, sortItemsIndex)
    local index

    for i = 1, self.total do
        if self.deletCache[i] then
            --交换monsterId
            index = i
            local battleMonsterId = self.deletCache[i].monsterId
            local unBattleMonsterInd = monsterItem.monsterId
            self.deletCache[i].monsterId = unBattleMonsterInd
            monsterItem.monsterId = battleMonsterId
            break
        end
    end
    --将sortItems里面的monsterItem也换个位置，防止显示的时候调换位置
    for i=1,#(self.sortItems) do    --在deleteCache中的Item在sortItems中的位置
        if index and self.sortItems[i] == self.deletCache[index] then
            local delItem = self.sortItems[i]
            self.sortItems[i] = self.sortItems[sortItemsIndex]
            self.sortItems[sortItemsIndex] = delItem
    end
    end

    if index then
        self.deletCache[index] = nil
    end
    return index
end
--TODO
--pos 出战monsterId = 0的位置
function ChooseHeroInBattleScene:swapWithBattleMonster(monsterItem, pos, sortItemsIndex)

    --self.monsterItems 里面的位置交换
    helper_team:setInTeam(monsterItem.monsterId, pos, self.monsterItems)
    --将self.sortItems 里面的值改变成上阵的Item
    for i=1,#(self.monsterItems) do
        if self.monsterItems[i].pos == pos then
            self.sortItems[sortItemsIndex] = self.monsterItems[i]
            break
        end
    end
end

function ChooseHeroInBattleScene:isFullInBattle()
    local res = true

    for i = 1,self.total do
        if self.deletCache[i] then
            res = false
        end
        for j = 1,#(self.monsterItems) do   --不能在sortItems里面找，里面没有monsterId=0的条目
            if self.monsterItems[j].pos == i and self.monsterItems[j].monsterId == 0 then --该上证位置在删除cache里面没有，并且也没有上阵武将
                res = false
        end
        end
    end

    return res
end

function ChooseHeroInBattleScene:isInCache(monsterItem)
    local flag = false
    for i = 1,self.total do
        if self.deletCache[i] then
            if monsterItem.pos == self.deletCache[i] .pos then
                flag = true
            end
        end
    end

    return flag
end

function ChooseHeroInBattleScene:hasInBattle(pos)
    for key,value in pairs(self.sortItems) do
        if value.pos == pos then
            return value
        end
    end
    return nil
end
--TODO
--在出战位置是否有monsterId 为0的位置
function ChooseHeroInBattleScene:getNoMonsterPosInBattle()
    local pos = -1
    for i=1,self.total do
        for j=1,#(self.monsterItems) do
            if self.monsterItems[j].pos == i and self.monsterItems[j].monsterId == 0 then
                pos = i
                break
            end
        end
    end

    return pos

end

function ChooseHeroInBattleScene:initItemView()
    local len = #self.sortItems
    if len == 1 then
        self:chooseItemView({2})
    elseif len == 2 then
        self:chooseItemView({2, 3})
    elseif len == 3 then
        self:chooseItemView({2, 3, 6})
    elseif len == 4 then
        self:chooseItemView({2, 3, 6, 7})
    elseif len == 5 then
        self:chooseItemView({2, 3, 5, 6, 7})
    elseif len == 6 then
        self:chooseItemView({1, 2, 3, 4, 5, 7})
    elseif len == 7 then
        self:chooseItemView({1, 2, 3, 4, 5, 6, 7})
    end

    for i = 8,10 do
        local view = self.Panel_Center:getChildByName("Panel_Item"..i)
        view:setTag(i)
        table.insert(self.battleItemViews, view)
    end

end

function ChooseHeroInBattleScene:chooseItemView(include)
    local num = 7
    for i=1,num do
        local view = self.Panel_Center:getChildByName("Panel_Item"..i)
        view:setVisible(false)
    end

    for key,value in pairs(include) do 
        local view = self.Panel_Center:getChildByName("Panel_Item"..value)
        view:setTag(value)
        view:setVisible(true)
        table.insert(self.itemViews, view)
    end

end

function ChooseHeroInBattleScene:sortMonsterItems(monsterItems)
    --将monsterId非0的条目拿出
    for key,value in pairs(monsterItems) do
        if value.monsterId > 0 then
            table.insert(self.sortItems,value)
        end
    end
    --按照等级，品质，ID排序
    table.sort(self.sortItems, function(a, b)
        local monsterA = model_monsterStorage:getMonster(a.monsterId)
        local monsterB = model_monsterStorage:getMonster(b.monsterId)

        if monsterA.level ~= monsterB.level then
            return monsterA.level > monsterB.level
        end

        if monsterA.quality ~= monsterB.quality then
            return monsterA.quality > monsterB.quality
        end

        if monsterA.id ~= monsterB.id then
            return monsterA.id > monsterB.id
        end
    end)
end

function ChooseHeroInBattleScene:getRelationShipCount(monsterItem) 
    local members = {}

    --monsterId的集合
    for key, value in pairs(self.monsterItems) do
        if(value.monsterId > 0) then
            table.insert(members,value.monsterId)
            cclog("monsterId = "..value.monsterId)
        end
    end


    local monster = model_monsterStorage:getMonster(monsterItem.monsterId)
    local relationship = helper_relationship:getRelationshipLevel(monster, members)
    local count = 0
    for key, value in pairs(relationship) do
        if value.actived then
            count = count + 1
        end
    end

    return count
end

return ChooseHeroInBattleScene