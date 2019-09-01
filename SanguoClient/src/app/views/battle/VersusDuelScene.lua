_REQUIRE("pb.pbCommon")

local VersusDuelScene = class("VersusDuelScene", cc.load("mvc").ViewBase)
VersusDuelScene.NEXTSCENE = "battle/BattleScene"
VersusDuelScene.PRESCENE = "battle/SpotPopupScene"
--_REQUIRE "src/packages/packet/Packet_Regester"

VersusDuelScene.RESOURCE_FILENAME = "Scene/VersusScene_Duel.csb"
VersusDuelScene.STOPFRAME = 45
local sceneData=_REQUIRE "battle/ChapterSceneData"
local model_game = _REQUIRE("model/model_game.lua")
local model_monster = _REQUIRE "model/static/model_monster.lua"
local model_monsterStorage = _REQUIRE "model/model_monsterStorage.lua"
local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
local helper_battle = _REQUIRE("model/helper/helper_battle.lua")
local model_user = _REQUIRE("model.model_user")
local model_formations = _REQUIRE("model/static/model_formations.lua")
local helper_monster_data = _REQUIRE("model/helper/helper_monster_data.lua")
local model_areaStorage = _REQUIRE("model/model_areaStorage.lua")
--local sceneLogic=_REQUIRE "battle.WinPopupSceneLogic"

function VersusDuelScene:onCreate()
    self:initUI()
end

function VersusDuelScene:initUI()
    local data = self.args[1]
    local Panel_VersusScene = self:getResourceNode():getChildByName("Panel_VersusScene")
    Panel_VersusScene:setTouchEnabled(true)
    local Panel_Bg_Base = Panel_VersusScene:getChildByName("Panel_Bg_Base")
    local Panel_Role1_Left = Panel_VersusScene:getChildByName("Panel_Role1_Left")
    self.role1Left = Panel_Role1_Left
    local Panel_Role1_Right = Panel_VersusScene:getChildByName("Panel_Role1_Right")
    local Panel_Bottom_Base = Panel_VersusScene:getChildByName("Panel_Bottom_Base")
    local Panel_Top_Base = Panel_VersusScene:getChildByName("Panel_Top_Base")
    local Panel_Top = Panel_Top_Base:getChildByName("Panel_Top")
    SCREEN_TOP(Panel_Top_Base)
    SCREEN_BOTTOM(Panel_Bottom_Base)
    SCREEN_SCALE_BG(Panel_Bg_Base)
    SCREEN_CENTER(Panel_VersusScene)
    --  关闭按钮
    local Button_Close = Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            GLOBAL_MY_APP:removeDlgs()
            SWITSCENE("battle/ChapterScene")
        end
    end)
    local battleNpc  -- npc阵容
    local battleTeam -- 己方武将阵容
    battleNpc = sceneData:getNpc()
    battleTeam = sceneData:getBattleTeam()
    self.battleTeam = battleTeam
    
    
    local Text_Title = Panel_Top:getChildByName("Text_Title")
    local stageModel = model_areaStorage:getStage(model_user.selectStageId,model_user.selectStageType)
    Text_Title:setString(stageModel.name_zhCN)
    
    local Text_Role_NumL = Panel_Role1_Left:getChildByName("Text_Role_Num")
    local Text_Role_NumR = Panel_Role1_Right:getChildByName("Text_Role_Num")
    Text_Role_NumL:setString(#battleTeam .. "/"..#battleTeam)
    Text_Role_NumR:setString(#battleNpc .. "/"..#battleNpc)
    self.roleItems = {}
    for i=1,3 do
        local role_infoL_1 = Panel_Role1_Left:getChildByName("role_infoL_"..i)
        role_infoL_1:setVisible(i<=#battleTeam)
        self.roleItems[#self.roleItems + 1] = role_infoL_1
        role_infoL_1:setTag(i)
        local Outline_Yellow_1 = Panel_Role1_Left:getChildByName("Outline_Yellow_"..i)
        local Panel_1 = Outline_Yellow_1:getChildByName("Panel_1")
        role_infoL_1:setTouchEnabled(false)
        Panel_1:setTouchEnabled(false)
      
        local Icon_lock_2 = Panel_Role1_Left:getChildByName("Icon_lock_"..i)  
        local Lock_Bg_2 = Panel_Role1_Left:getChildByName("Lock_Bg_"..i) 
        if Icon_lock_2 then
            Icon_lock_2:setVisible(i > #battleTeam)
        end
        
        if Lock_Bg_2 then
            Lock_Bg_2:setVisible(i<=#battleTeam)
        end
        
        local Panel_H1 = role_infoL_1:getChildByName("Panel_H"..i)
        Panel_H1:setTouchEnabled(false)
        local Hero1 = Panel_H1:getChildByName("Hero"..i)
        Hero1:setTouchEnabled(false)

        local Hero1_info_1 = Panel_H1:getChildByName("Hero1_info_"..i)
        Hero1_info_1:setTouchEnabled(false)
        local Info_1 = Hero1_info_1:getChildByName("Info_"..i)
        Info_1:setTouchEnabled(false)
        if i <= #battleTeam then
            local teamInfo = battleTeam[i]
            local monsterData = model_monsterStorage:getMonster(teamInfo.id)  
            self:initInfo(role_infoL_1,teamInfo,i)
        end
        
    end
    
    for i=1,3 do
        local index = i + 3
        local role_infoL_1 = Panel_Role1_Right:getChildByName("role_infoL_"..index)
        role_infoL_1:setVisible(i<=#battleNpc)
        if i <= #battleNpc then
            self:initInfo(role_infoL_1,battleNpc[i],index,true)
        end

    end
    
    
    Panel_Role1_Left:setTouchEnabled(true)
    Panel_Role1_Left:addTouchEventListener(function(sender,eventType)
        if eventType ==0 then
            self:onTouchBegan(sender)
        elseif eventType ==1 then
            self:onTouchMoved(sender)
            --cclog("moved")
        elseif eventType ==2 then
            self:onTouchEnded(sender)
            --cclog("ended")
        elseif eventType ==3 then
            self:onTouchEnded(sender)   
        end
    end)

    local Panel_Bottom = Panel_Bottom_Base:getChildByName("Panel_Bottom")
    local Panel_Jice_Base1 = Panel_Bottom:getChildByName("Panel_Jice_Base1")
    local ProjectNode_4 = Panel_Jice_Base1:getChildByName("ProjectNode_4")
    local function initTeam()
        local team = model_user.battleTeam
        for i=1,3 do
            local teamData = team[i]
            local ProjectNode=ProjectNode_4:getChildByName("ProjectNode_"..i)
            if teamData.isUse >0 then
                local monster = model_monsterStorage:getMonster(teamData.monsterId)
                self:initPanel_RisingStar_Head(ProjectNode,monster)
            else
                self:initPanel_RisingStar_Head(ProjectNode)
            end
        end
    end

    initTeam()
    local Button_BZ=ProjectNode_4:getChildByName("Button_BZ")
    self:addBtnTouchEventListener(Button_BZ,function(sender,eventType)
        if eventType ==  2 then
            self:showDlg("battle/ChooseHeroInBattleScene",2,function()
                initTeam()
            end)
        end
    end)
    local Button_Fire = Panel_Jice_Base1:getChildByName("Button_Fire") 
    Button_Fire:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            local function onBattleExecBattle(success, data, code, msg)
                if(success) then
                    self:sceneBack(function()
                        GLOBAL_MY_APP:removeDlgs()
                        --SWITSCENE(VersusScene.NEXTSCENE)
                        SWITSCENE("battle/BattleScene")
                     end,80)
                    
                end
            end

            model_user.battleSelectMonsterIdList = {}
            model_user.battleSelectNpcIdList = {}

            for i=1,#battleTeam do
                local tag = self.roleItems[i]:getTag()
                model_user.battleSelectMonsterIdList[tag] = battleTeam[i].id
            end

            for key, value in pairs(model_user.battleNpcTeam) do
                table.insert(model_user.battleSelectNpcIdList, value.id)
            end

            local enemyIds
            enemyIds = {}
            for i = 1, #(model_user.battleSelectNpcIdList) do
                for key, value in pairs(model_user.battleNpcTeam) do
                    if value.id == model_user.battleSelectNpcIdList[i] then
                        table.insert(enemyIds, value.NPCId)
                    end
                end
            end

            ctlBattleExecBattleRequest:sendRequest(onBattleExecBattle,
                model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
                model_user.battleProgress, model_user.battleSelectMonsterIdList, enemyIds);
            model_user.oldExp = model_user.exp
            
            
        end
    end)
end

function VersusDuelScene:initInfo(Panel_Info,data,i,isNpc)
    local Panel_H1 = Panel_Info:getChildByName("Panel_H"..i)
    local hero1 = Panel_H1:getChildByName("Hero"..i)
    local Hero1_info_1 = Panel_H1:getChildByName("Hero1_info_"..i)
   
    local hero = hero1:getChildByName("hero_"..i)
    hero:setOpacity(0)      
    local img = ccui.ImageView:create(model_monster:getHalf(data.id))
    img:setTag(1000)
    img:setPosition(cc.p(hero:getPosition()))
    hero1:addChild(img)
    
    local Info_2 = Hero1_info_1:getChildByName("Info_"..i)
    local Text_Name1 = Info_2:getChildByName("Text_Name"..i)
    local Text_Level1 = Info_2:getChildByName("Text_Level"..i)
    local Text_Hp1 = Info_2:getChildByName("Text_Hp"..i)
    local Text_Mp1 = Info_2:getChildByName("Text_Mp"..i)
    local Button_up_1 = Panel_Info:getChildByName("Button_up_"..i)
    local Button_down_1 = Panel_Info:getChildByName("Button_down_"..i)
    local Text_Power1 = Info_2:getChildByName("Text_Power"..i)
    Text_Level1:setString(LANG("Lv%s",data.level))
    Text_Name1:setString(model_monster:getName(data.id))
    if isNpc ==false then
        local color, sprits = model_monsterStorage:getColor(data.id)
        Text_Name1:setColor(color)
    else
        local color, sprits = model_monsterStorage:getEquipColor(data.quality)
        Text_Name1:setColor(color)
    end
    Text_Hp1:setString(LANG("生命:%s",data.currentHP))
    Text_Mp1:setString(LANG("法力:%s",data.currentMP))
--    
    Button_up_1:setVisible(true)
    Button_down_1:setVisible(false)
    self:addBtnTouchEventListener(Button_up_1,function(sender,eventType)
        if eventType ==2 then
            self:moveHandler(true,Panel_Info,i)
        end
    end)
    
    self:addBtnTouchEventListener(Button_down_1,function(sender,eventType)
        if eventType ==2 then
            self:moveHandler(false,Panel_Info,i)
        end
    end)
end

function VersusDuelScene:moveHandler(isUp,Panel_Info,i)
    local Panel_H1 = Panel_Info:getChildByName("Panel_H"..i)
    local hero1 = Panel_H1:getChildByName("Hero"..i)
    local Hero1_info_1 = Panel_H1:getChildByName("Hero1_info_"..i)
    local Button_up_1 = Panel_Info:getChildByName("Button_up_"..i)
    local Button_down_1 = Panel_Info:getChildByName("Button_down_"..i)
    local startHeroP = cc.p(hero1:getPosition())
    local startInfoP = cc.p(Hero1_info_1:getPosition())
    local subY 
    if isUp then
        subY = Panel_H1:getContentSize().height
    else
        subY = -Panel_H1:getContentSize().height
    end
    hero1:runAction(cc.MoveTo:create(.2,cc.p(startHeroP.x,startHeroP.y + subY)))
    Hero1_info_1:runAction(cc.Sequence:create(
        cc.MoveTo:create(.2,cc.p(startInfoP.x,startInfoP.y + subY)),
        cc.CallFunc:create(function()
            Button_up_1:setVisible(isUp == false)
            Button_down_1:setVisible(isUp == true)
        end)))
end

function VersusDuelScene:onTouchBegan(sender)
    local startP = sender:getTouchBeganPosition()
    local isHit,index = self:beginHitTest(startP)
    if isHit then
        cclog("beigin:"..index)
        self.hitIndex = index
        return true
    end
    return false
end

function VersusDuelScene:onTouchMoved(sender)
    if self.hitIndex ==nil then
    	return
    end
    self.roleItems[self.hitIndex]:setPosition(sender:getTouchMovePosition())
    self.roleItems[self.hitIndex]:setLocalZOrder(1)
    
    local moveP = sender:getTouchMovePosition()
    local isHit,index = self:moveHitTest(moveP) -- 当前第几个位置
    self.popMsg = false
    if isHit then
        if index > #self.battleTeam   then
            self.popMsg = true
        	return
        end
        self.moveIndx = index
        cclog("self.roleItems[self.hitIndex]:getTag():"..self.roleItems[self.hitIndex]:getTag())
        cclog("self.moveIndx:"..self.moveIndx)
        if self.moveIndx ~= self.roleItems[self.hitIndex]:getTag() then
            local item =self:moveHitItem()--判断移动位置是否有东西
            if item then
                self.change = true --是否交换 
                local Duel_HeroBg_1 = self.role1Left:getChildByName("Duel_HeroBg_".. self.roleItems[self.hitIndex]:getTag())
                item:setPosition(Duel_HeroBg_1:getPosition())
                item:setTag(self.roleItems[self.hitIndex]:getTag())
                self.roleItems[self.hitIndex]:setTag(self.moveIndx)
            end
        end
        
    end
end

function VersusDuelScene:onTouchEnded(sender)
    if self.moveIndx == nil then
    	return
    end
    if self.popMsg == true then
    	PopMsg.getInstance():flashShow(LANG("此位置为空不能移动"))
    end
    local Duel_HeroBg_1 = self.role1Left:getChildByName("Duel_HeroBg_"..self.moveIndx)
    self.roleItems[self.hitIndex]:setPosition(Duel_HeroBg_1:getPosition())
    self.roleItems[self.hitIndex]:setLocalZOrder(0)
    self.roleItems[self.hitIndex]:setTag(self.moveIndx)
    self.hitIndex = nil
    self.moveIndx = nil
end

function VersusDuelScene:moveHitItem()--移动过程中是否碰到新的武将通过moveIndex获取
    for i=1,3 do
        local item = self.roleItems[i]
        if item:getTag() == self.moveIndx then
        	return item
        end
    end
end

function VersusDuelScene:beginHitTest(point)
    local p = self.role1Left:convertToNodeSpace(point)
    for i=1,3 do
        local item = self.roleItems[i]
        local size = item:getContentSize()
        local rect = cc.rect(item:getPositionX() - size.width/2,item:getPositionY()-size.height/2, size.width, size.height)
        if cc.rectContainsPoint(rect,p ) then
        	return true,i
        end
    end
end

function VersusDuelScene:moveHitTest(point)
    local p = self.role1Left:convertToNodeSpace(point)
    for i=1,3 do
        local item = self.roleItems[i]
        local size = item:getContentSize()
        local Duel_HeroBg_1 = self.role1Left:getChildByName("Duel_HeroBg_"..i)
        local rect = cc.rect(Duel_HeroBg_1:getPositionX() - size.width/2,Duel_HeroBg_1:getPositionY()-size.height/2, size.width, size.height)
        if cc.rectContainsPoint(rect,p ) then
            return true,i
        end
    end
end


return VersusDuelScene
