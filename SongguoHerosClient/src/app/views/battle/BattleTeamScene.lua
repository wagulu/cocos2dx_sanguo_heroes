
local BattleTeamScene = class("BattleTeamScene", cc.load("mvc").ViewBase)
BattleTeamScene.NEXTSCENE = "battle/ChapterScene"
BattleTeamScene.STOPFRAME = 50
BattleTeamScene.RESOURCE_FILENAME = "Scene/TeamScene.csb"

local model_user = _REQUIRE("model/model_user")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage")
local model_monster = _REQUIRE("model/static/model_monster")
local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
local helper_team = _REQUIRE("model/helper/helper_team")
local helper_relationship = _REQUIRE("model/helper/helper_relationship")

_REQUIRE("pbCommon.lua")

function BattleTeamScene:onCreate()
    self:initUi()
end

function BattleTeamScene:initUi()
    local TeamScene = self:getResourceNode():getChildByName("Team")
    self.Panel_Center = TeamScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
    self.Panel_Top = TeamScene:getChildByName("Panel_T"):getChildByName("Panel_Top")
    self.Panel_Bottom = TeamScene:getChildByName("Panel_B"):getChildByName("Panel_Bottom")
    local Panel_Bg = TeamScene:getChildByName("Panel_Bg")
    SCREEN_TOP(TeamScene:getChildByName("Panel_T"))
    SCREEN_SCALE_BG(Panel_Bg)
    SCREEN_BOTTOM(TeamScene:getChildByName("Panel_B"))
    SCREEN_CENTER(TeamScene)
    local Node_Top=self.Panel_Top:getChildByName("ProjectNode_5")
    self:initTopBar(Node_Top)
    for i=1,3 do
        self.Panel_Center:getChildByName("Button_Tap"..i):setVisible(false)
    end
    local Button_20 = self.Panel_Top:getChildByName("Button_20")
    local function changeScene()
        if self.args[1] == 1 then
            SWITSCENE("battle/ChapterScene")
        else
            SWITSCENE("home/MainCityScene")
        end
    end
    self:addBtnTouchEventListener(Button_20, function (sender, eventType)
        if eventType==2 then
            local isFull = self:isFullInBattle()
            if isFull == false then
                --PopMsg.getInstance():flashShow(LANG("将军，出战位置不可为空哦"))
                self:showDlg("common/CommonPopScene",{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text=LANG("将军，出战位置不可以为空哦！退出时无法\n保存您做出的更改！"),fun=function(data)
                    if data.cmd=="confirm" then
                        changeScene()

                    elseif data.cmd =="cancel" then 
                        
                    end
                end})
                return
            end
            if self:isChangTeam() then
                self:showDlg("common/CommonPopScene",{confirmStr=LANG("保存"),cancelStr=LANG("不保存"),text=LANG("队伍发生改变，是否保存并退出？"),fun=function(data)
                    if data.cmd=="confirm" then
                        self:saveData()
                        changeScene()
                        
                    elseif data.cmd =="cancel" then 
                        changeScene()
                    end
                end})
                return
            end
            changeScene()
        end
    end)
    --TODO
    self.ImageViews = {}
    for i = 1, 3 do
        local ImageView = ccui.ImageView:create()
        local view = self.Panel_Center:getChildByName("Panel_Role_"..i)
        local size = view:getContentSize()
        local oldImageView = view:getChildByName("Image_Role_"..i)
        ImageView:setPosition(194,165)
        
        ImageView:setScale(0.9)
        ImageView:setName("Image_Role_"..i)
        view:getChildByName("Text_Name"..i):setLocalZOrder(2)
        view:getChildByName("Text_Quality_Num"..i):setLocalZOrder(2)
        
        view:removeChild(oldImageView)
        view:addChild(ImageView, 1)
        self.ImageViews[i] = ImageView
        ImageView:setLocalZOrder(1)
    end

    cclog("len = "..#(model_user.battleTeam))
    for key, value in pairs(model_user.battleTeam) do
        if(value.isUse > 0) then
            cclog("isUse---position = "..value.pos)
            cclog("monsterId = "..value.monsterId)
        else
            cclog("position = "..value.pos)
        end
    end
 
    local level = model_user.level
    self.total = 1
    if level >= 7 then
        self.total = 2
    end
    if level >= 15 then
        self.total = 3
    end
    
   self.relationships = {}
   self.oldRelationships = {}       --变化前的缘分数量
   self.itemsView = {}
   self.relationshipCount = 0
   self.oldRelationshipCount = 0
   --TODO
   self:addAnimNode()
   for i = 1,7 do
        local node
        if i <= 3 then
            node = self.Panel_Center:getChildByName("Panel_Item"..i)
            table.insert(self.itemsView,node)
        else 
            node = self.Panel_Bottom:getChildByName("Panel_Item"..i)
            table.insert(self.itemsView,node)
        end
   end
   
    self.monsters = {}
    self.monsters = clone(model_user.battleTeam)
--    for i=1,7 do
--        local node = {}
--        node.pos = i
--        node.monsterId = 0
--        node.isUse = 0
--        if i <=3 then
--            node.isUse = i
--        end
--        self.monsters[i] = node
--    end
--    self.monsters[1].monsterId = 2
    
    self.oldMonsters = {}       --变化前的阵容
    self:updateItems()
    --TODO
    self:showFirstInAction()
   
end

function BattleTeamScene:addAnimNode()
    self.heroIconAction = {}
    self.heroIconBackAction3 = {}
    self.heroIconBackAction4 = {}
    self.heroIconBackAction5 = {}
    self.heroAddAction = {}
    self.heroHalfActoin1 = {}
    self.heroHalfActoin2 = {}
    
    for i = 1,7 do
        local Node_Team_item_Anim, action = self:createNode("Node/Node_Team_Item_Anim.csb")
        local Node_Team_A3, actionA3 = self:createNode("Node/Node_Team_A3.csb")
        local Node_Team_A4, actionA4 = self:createNode("Node/Node_Team_A4.csb")
        local Node_Team_A5, actionA5 = self:createNode("Node/Node_Team_A5.csb")
        
        local view
        if i <= 3 then
            view = self.Panel_Center:getChildByName("Panel_Item"..i)
            Node_Team_item_Anim:setScale(0.9)
        else 
            view = self.Panel_Bottom:getChildByName("Panel_Item"..i)
            Node_Team_item_Anim:setScale(1)
        end
        
        local oldNode = view:getChildByName("ProjectNode"..i)
        
        --武将上阵背景动画
        --A3
        actionA3:gotoFrameAndPause(61)
        self.heroIconBackAction3[i] = actionA3
        view:addChild(Node_Team_A3)
        Node_Team_A3:setPosition(oldNode:getPosition())
        Node_Team_A3:getChildByName("Node_Team_Item"):setTouchEnabled(false)
        --actionA3:gotoFrameAndPause(61)
        
        --A4
        actionA4:gotoFrameAndPause(70)
        self.heroIconBackAction4[i] = actionA4
        view:addChild(Node_Team_A4)
        Node_Team_A4:setPosition(oldNode:getPosition())
        Node_Team_A4:getChildByName("Panel_Role_3"):setTouchEnabled(false)
        --actionA4:gotoFrameAndPause(70)
        
        --A5
        actionA5:gotoFrameAndPause(71)
        self.heroIconBackAction5[i] = actionA5
        view:addChild(Node_Team_A5)
        Node_Team_A5:setPosition(oldNode:getPosition())
        Node_Team_A5:getChildByName("Panel_Role_3"):setTouchEnabled(false)
        --actionA5:gotoFrameAndPause(71)
        
        
--        --武将缘分角标动画
        action:gotoFrameAndPause(0)
        Node_Team_item_Anim:setPosition(oldNode:getPosition())
        Node_Team_item_Anim:setName("ProjectNode"..i)
        view:removeChild(oldNode)
        view:addChild(Node_Team_item_Anim)
        self.heroIconAction[i] = action
        
        --"+"号动画
        local Node_Team_Item_AddAnim, actionAdd = self:createNode("Node/Node_Team_Item_AddAnim.csb")
        actionAdd:gotoFrameAndPause(0)
        local Node_Team_Item = Node_Team_item_Anim:getChildByName("Node_Team_Item")
        local Image_Add = Node_Team_Item:getChildByName("Image_Add")
        Node_Team_Item_AddAnim:setPosition(Image_Add:getPosition())
        Node_Team_Item:addChild(Node_Team_Item_AddAnim)
        Node_Team_Item_AddAnim:setName("Image_Add")
        Node_Team_Item:removeChild(Image_Add)
        self.heroAddAction[i] = actionAdd
        
        
        
    end
    
    --缘分总数增加动画
    local Node_Team_Up, actionUp = self:createNode("Node/Node_Team_Up.csb")
    actionUp:gotoFrameAndPause(125)
    self.actionUp = actionUp
    local Text_1 = self.Panel_Center:getChildByName("Text_1")
    self.Panel_Center:addChild(Node_Team_Up)
    Node_Team_Up:setPosition(Text_1:getPosition())
    
    
    --缘分总数减少动画
    local Node_Team_Down, actionDown = self:createNode("Node/Node_Team_Down.csb")
    actionDown:gotoFrameAndPause(125)
    self.actionDown = actionDown
    self.Panel_Center:addChild(Node_Team_Down)
    Node_Team_Down:setPosition(Text_1:getPosition())
    
    
    --武将半身像后绿光和闪烁动画
    for i=1,3 do
        local Node_Team_A1, actionA1 = self:createNode("Node/Node_Team_A1.csb")
        local view = self.Panel_Center:getChildByName("Panel_Role_"..i)
        local Image_R_Light = view:getChildByName("Image_R_Light"..i)
        local Image_Role = view:getChildByName("Image_Role_"..i)
        view:removeChild(Image_R_Light)
        --绿光
        actionA1:gotoFrameAndPause(0)
        view:addChild(Node_Team_A1)
        Node_Team_A1:setPosition(Image_R_Light:getPosition())
        Node_Team_A1:setName("Image_R_Light"..i)
        Node_Team_A1:setLocalZOrder(0)
        self.heroHalfActoin1[i] = actionA1
        --闪烁
        local Node_Team_A2, actionA2 = self:createNode("Node/Node_Team_A2.csb")
        actionA2:gotoFrameAndPause(71)
        view:addChild(Node_Team_A2)
        Node_Team_A2:setPosition(Image_Role:getPosition())
        Node_Team_A2:setLocalZOrder(0)
        self.heroHalfActoin2[i] = actionA2
    end
end

function BattleTeamScene:saveData()
    ctlPlayerChangeBattleTeamRequest:sendRequest(function(success, data, code, msg)
        if(success) then
            PopMsg.getInstance():flashShow(LANG("保存成功"))
            SWITSCENE("home/MainCityScene")
        else
        --PopMsg.getInstance():flashShow(LANG("将军，出战位置不可为空哦"))
        end
    end, self.monsters);
end

function BattleTeamScene:isChangTeam() 
    local res = false
    for i,item in pairs(self.monsters) do
        for key,value in pairs(model_user.battleTeam) do
            if item.pos == value.pos and item.monsterId ~= value.monsterId then
                res = true
            end
        end
    end
    
    return res
end

function BattleTeamScene:updateItems()
    
    for i = 1,3 do
        local view = self.Panel_Center:getChildByName("Panel_Role_"..i)
        view:setVisible(false)
        self.Panel_Center:getChildByName("Image_Light"..i):setVisible(true)
        if i <= self.total then
            for key,value in pairs(self.monsters) do
                if i == value.pos and value.monsterId > 0 then
                    local monster = model_monsterStorage:getMonster(value.monsterId)
                    local color,splits = model_monsterStorage:getColor(monster.id)
                    view:setVisible(true)
                    self.Panel_Center:getChildByName("Image_Light"..i):setVisible(false)
                    self.ImageViews[i]:loadTexture(model_monster:getHalf(value.monsterId))
                    view:getChildByName("Text_Name"..i):setString(LANG("%s", model_monster:getName(value.monsterId)))
                    view:getChildByName("Text_Name"..i):setColor(color)
                    view:getChildByName("Text_Quality_Num"..i):setString(LANG(""))
                    view:getChildByName("Text_Quality_Num"..i):setColor(color)
                    if table.getn(splits) == 2 then
                        view:getChildByName("Text_Quality_Num"..i):setString(LANG("+"..splits[2]))
                    end
                end
            end
        end 
    end
    
    
    for key,value in pairs(self.monsters) do
         cclog("updateItems   pos = "..value.pos)
         cclog("updateItems   isUse = "..value.isUse)
    end
    
    self:totalRelationShip()
    self.Panel_Center:getChildByName("Text_1"):setString(LANG("上阵武将激活配缘数量：%s",self.relationshipCount))
	for i = 1,7 do
        local monsterItem = self.monsters[i]
        local view = self.itemsView[monsterItem.pos]
        
        self:updateItemView(monsterItem, view)
	end
	
end

function BattleTeamScene:updateItemView(monsterItem, view)
	--用户等级限制
	local pos = monsterItem.pos
	local monsterId = monsterItem.monsterId
	local isLock = false
	local level = model_user.level
	local limitDes
	if pos == 2 and level < 7 then
	   isLock = true 
	   limitDes = LANG("7级解锁")
	elseif pos == 3 and level < 15 then
        isLock = true 
        limitDes = LANG("15级解锁")
	elseif pos == 4 and level < 15 then
        isLock = true 
        limitDes = LANG("15级解锁")
	elseif pos == 5 and level < 25 then
        isLock = true 
        limitDes = LANG("25级解锁")
	elseif pos == 6 and level < 35 then
        isLock = true 
        limitDes = LANG("35级解锁")
	elseif pos == 7 and level < 45 then
        isLock = true 
        limitDes = LANG("45级解锁")
	end
	
    local ProjectNode = view:getChildByName("ProjectNode"..pos) 
    local Node_Team = ProjectNode:getChildByName("Node_Team_Item")
--    local Button = view:getChildByName("Button_"..pos)
--    if Button then
--        Button:setEnabled(false);
--    end
    view:setTouchEnabled(true);
    view:getChildByName("Text"..pos):setString("")
    Node_Team:getChildByName("Image_Head_bg"):loadTexture(model_monsterStorage:getFrameByQuality(1))    --将边框设置成1级的
    
    if isLock then 
        self:setMosterVisible(view, "lock", pos)
        view:setTouchEnabled(false)
        Node_Team:getChildByName("Text_1"):setString(limitDes)
        return;
    end
    
    
    view:addTouchEventListener(function (sender, eventType)
        if eventType ==0 then
            sender:stopAllActions();
            sender:runAction(cc.Sequence:create(cc.ScaleTo:create(0.15,1.1),cc.ScaleTo:create(0.15, 1),NULL) )
        elseif eventType==2 then
            --local monster = self.monsters[pos]
            if(monsterItem.monsterId > 0) then 
                --打开武将详细信息
                local monster = model_monsterStorage:getMonster(monsterItem.monsterId)
                
                self:showDlg("battle/HeroInfoPopScene", monsterItem.monsterId
                    , {monsterItem = monsterItem, monsterItems = self.monsters, from = "BattleTeamScene",
                        callback = function(_monsterItem, monster_id, _monsters)
                            self:changTeam(_monsterItem,monster_id, _monsters)
                        end})
            else 
                --打开选取武将列表
                self:showDlg("battle/BattleHeroListScene", monsterItem, self.monsters, function(_monsterItem, monster_id, _monsters)
                    self:changTeam(_monsterItem,monster_id, _monsters)
                end)
            end
        end
    end)
    
    if monsterId == 0 then
        self:setMosterVisible(view, "add", pos)
        return;
    end
    
    self:setMosterVisible(view, "monster", pos)
	
    local monster = model_monsterStorage:getMonster(monsterId)
    local Panel_Star = Node_Team:getChildByName("Panel_Star")
    --星级显示
    for k = 1,5 do
        Panel_Star:getChildByName("Image_Star_"..k):setVisible(k<= monster.star)
        Panel_Star:getChildByName("Image_Star_"..k):setTouchEnabled(false)
    end
	--武将信息
    Node_Team:getChildByName("Image_Hero"):loadTexture(model_static_library:getAsset(monsterId, pbCommon.ObjectType.Monster))
    Node_Team:getChildByName("Image__Job"):loadTexture("Job/Icon_job"..model_monster:getType(monster.id)..".png")
    Node_Team:getChildByName("Image_Head_bg"):loadTexture(model_monsterStorage:getFrameByQuality(monster.quality))
    cclog("pos = "..pos)
    cclog("monsterId = "..monsterId)
    cclog("path = "..model_static_library:getAsset(monsterId, pbCommon.ObjectType.Monster))
    cclog("-----------------")
	view:getChildByName("Text"..pos):setString(LANG("LV%s %s", monster.level, model_monster:getName(monster.id)))
    --view:getChildByName("Text"..pos.."_2"):setString(LANG("%s", self.relationships[pos]))
    Node_Team:getChildByName("Text_Link"):setString(LANG("%s", #(self.relationships[pos])))
	
end 

function BattleTeamScene:isFullInBattle()
    local res = true
    for i = 1,self.total do
        for key,value in pairs(self.monsters) do
            if value.pos == i and value.monsterId == 0 then
                res = false
            end
        end
    end
    
    return res
end

-- "2": {
--        "id": 2,
--        "relationshipId": 1,
--        "level": 2,
--        "monsterQuality": 2,
--        "ver": "",
--        "description_zhCN": "\uff0c\u751f\u547d+30\uff082\u7ea7\u589e\u52a010\uff09"
--    },
function BattleTeamScene:totalRelationShip() 
    local members = {}
    self.relationshipCount = 0

    --monsterId的集合
    for key, value in pairs(self.monsters) do
        if(value.monsterId > 0) then
            table.insert(members,value.monsterId)
            cclog("monsterId = "..value.monsterId)
        end
    end
    
    for index, monsterItem in pairs(self.monsters) do
        local node = {}
        if(monsterItem.monsterId > 0) then
            local monster = model_monsterStorage:getMonster(monsterItem.monsterId)
            local relationship = helper_relationship:getRelationshipLevel(monster, members)
            for key, value in pairs(relationship) do
                if value.actived then
                    self.relationshipCount = self.relationshipCount + 1
                    table.insert(node,value)
                    
                end
            end
        end
        self.relationships[monsterItem.pos] = node
    end
    local i = 1
end

function BattleTeamScene:changTeam(monster, monsterId, monsters)
    --缓存更新前数据，用于比对缘分变化
    self.oldMonsters = clone(self.monsters)
    self.oldRelationships = clone(self.relationships)
    self.oldRelationshipCount = self.relationshipCount
    helper_team:setInTeam(monsterId, monster.pos, self.monsters)
    --更新UI
    self:updateItems()
    self:showChangeAction()
end

function BattleTeamScene:showChangeAction()
    if #(self.oldMonsters) == 0 then
        return
    end
    
    --TODO
    for i = 1,7 do
        local monsterItem = self.monsters[i]
        local oldMonsterItem = self.oldMonsters[i]
        local view = self.itemsView[monsterItem.pos]
        local isDiffRelation = self:isDifRelationship(self.relationships[monsterItem.pos],self.oldRelationships[monsterItem.pos])
        if monsterItem.monsterId ~= oldMonsterItem.monsterId or #(self.relationships[i]) ~= #(self.oldRelationships[i]) or isDiffRelation then
            if monsterItem.monsterId > 0 then
                self:showAction(monsterItem.pos)
            end
        end
        local isLock = self:isLockForPos(monsterItem.pos)
        
        self.heroAddAction[monsterItem.pos]:gotoFrameAndPause(0)
        if monsterItem.monsterId == 0 and isLock == false then
            self.heroAddAction[monsterItem.pos]:gotoFrameAndPlay(0, 65,true)
        end
    end 
    
    for i = 1,self.total do
        local monsterItem = self.monsters[i]
        local oldMonsterItem = self.oldMonsters[i]
        local ImageView = self.ImageViews[i]
        self.heroHalfActoin1[i]:gotoFrameAndPause(0)
        if monsterItem.monsterId > 0 then
            self.heroHalfActoin1[i]:gotoFrameAndPlay(0, 145,true)
        end
        
        if monsterItem.monsterId > 0 and monsterItem.monsterId ~= oldMonsterItem.monsterId then
            self.heroHalfActoin2[i]:gotoFrameAndPlay(0, 71,false)
            self:showEaseOutAction(ImageView)
        end
        
        
    end
    
    if self.oldRelationshipCount < self.relationshipCount then
        self.actionUp:gotoFrameAndPlay(0, 125,false)
    end
    
    if self.oldRelationshipCount > self.relationshipCount then
        self.actionDown:gotoFrameAndPlay(0, 125,false)
    end
end

function BattleTeamScene:showFirstInAction()
    for i = 1,7 do
        local monsterItem = self.monsters[i]
        local isLock = self:isLockForPos(monsterItem.pos)
        --"+"号动画
        self.heroAddAction[monsterItem.pos]:gotoFrameAndPause(0)
        if monsterItem.monsterId == 0 and isLock == false then
            self.heroAddAction[monsterItem.pos]:gotoFrameAndPlay(0, 45,true)
        end
    end
    --半身像动画
--    for i=1,3 do
--        local ImageView = self.ImageViews[i]
--        self:showEaseOutAction(ImageView)
--        
--    end
    for i = 1,self.total do
        local monsterItem = self.monsters[i]
        local ImageView = self.ImageViews[i]
        self.heroHalfActoin1[i]:gotoFrameAndPause(0)
        if monsterItem.monsterId > 0 then
            self.heroHalfActoin1[i]:gotoFrameAndPlay(0, 145,true)
        end
    end
    
end

function BattleTeamScene:showEaseOutAction(ImageView)
    --渐变动画
    local sequence = cc.Sequence:create(cc.ScaleTo:create(0,0.5), cc.ScaleTo:create(0.3,0.9), nil)
    local spawn = cc.Spawn:create(sequence, cc.FadeIn:create(0.3))
    local EaseOut = cc.EaseOut:create(spawn, 0.3)

    ImageView:setOpacity(0)
    ImageView:runAction(EaseOut)
end

function BattleTeamScene:showAction(pos, view)
    self.heroIconAction[pos]:gotoFrameAndPlay(0, 45,false)
    self.heroIconBackAction3[pos]:gotoFrameAndPlay(0, 61,false)
    self.heroIconBackAction4[pos]:gotoFrameAndPlay(0, 70,false)
    self.heroIconBackAction5[pos]:gotoFrameAndPlay(0, 71,false)
end

function BattleTeamScene:isDifRelationship(befor_relationships, after_relationships)
    local isDiffrent = false
    for i=1,#(after_relationships) do
        isDiffrent = true
        for j=1,#(befor_relationships) do
            if after_relationships[i].effect.id == befor_relationships[j].effect.id then
                isDiffrent = false
            end
        end
        
        if isDiffrent then
            break
        end
    
    end
    
    return isDiffrent
end

function BattleTeamScene:isLockForPos(pos)
    local isLock = false
    local level = model_user.level
    if pos == 2 and level < 7 then
       isLock = true 
    elseif pos == 3 and level < 15 then
        isLock = true 
    elseif pos == 4 and level < 15 then
        isLock = true 
    elseif pos == 5 and level < 25 then
        isLock = true 
    elseif pos == 6 and level < 35 then
        isLock = true 
    elseif pos == 7 and level < 45 then
        isLock = true 
    end
    
    return isLock
end

function BattleTeamScene:setMosterVisible(view, type, pos)
    local ProjectNode = view:getChildByName("ProjectNode"..pos) 
    local Node_Team = ProjectNode:getChildByName("Node_Team_Item")
    view:getChildByName("Text"..pos):setVisible(false)
    Node_Team:getChildByName("Panel_Star"):setVisible(false)     --星级
    Node_Team:getChildByName("Image_Hero"):setVisible(false)      --头像
    Node_Team:getChildByName("Image__Job"):setVisible(false)    --品阶
    Node_Team:getChildByName("Text_1"):setVisible(false)          --解锁文字         
    Node_Team:getChildByName("Image_Add"):setVisible(false)       --添加"+"号
    Node_Team:getChildByName("Image_Chain"):setVisible(false)     --解锁图片
    Node_Team:getChildByName("Image_IconBg"):setVisible(false)    --缘分    
    Node_Team:getChildByName("Text_Link"):setVisible(false)
    --TODO
    Node_Team:getChildByName("Image_B"):setVisible(false)
    Node_Team:getChildByName("Image_R"):setVisible(false)
    
    --view:getChildByName("Text"..pos.."_2"):setVisible(false)        --缘分
    
    if type == "lock" then
        Node_Team:getChildByName("Image_Chain"):setVisible(true)
        Node_Team:getChildByName("Text_1"):setVisible(true)
    elseif type == "monster" then
        Node_Team:getChildByName("Panel_Star"):setVisible(true)
        Node_Team:getChildByName("Image_Hero"):setVisible(true)
        Node_Team:getChildByName("Image__Job"):setVisible(true)
        Node_Team:getChildByName("Image_IconBg"):setVisible(true)
        Node_Team:getChildByName("Text_Link"):setVisible(true)
        view:getChildByName("Text"..pos):setVisible(true)
        --TODO
        Node_Team:getChildByName("Image_B"):setVisible(true)
        Node_Team:getChildByName("Image_R"):setVisible(true)
    elseif type == "add" then    
        Node_Team:getChildByName("Image_Add"):setVisible(true)
    end
    --缘分
    --view:getChildByName("Tex")
end

return BattleTeamScene