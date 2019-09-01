_REQUIRE("pb.pbCommon")

local VersusScene = class("VersusScene", cc.load("mvc").ViewBase)
VersusScene.NEXTSCENE = "battle/BattleScene"
VersusScene.PRESCENE = "battle/SpotPopupScene"
--_REQUIRE "src/packages/packet/Packet_Regester"

VersusScene.RESOURCE_FILENAME = "Scene/VersusScene.csb"
VersusScene.STOPFRAME = 45
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

function VersusScene:onCreate()
    self:initUI()
end

function VersusScene:initUI()
    local data = self.args[1]
    local index = self.args[2]
    self.firstAround = false  --快速战斗特效从头开始播放
    self.endAround = false  --快速战斗特效最后部分播放
    model_user.isPvp = (index==1)
    model_user.preMonsterLevel = model_user.level
    model_user.preEnergy = model_user.energy
    model_user.preEnergyMax = model_user.energyMax
    model_user.preVitality = model_user.vitality
    model_user.preVitalityMax = model_user.vitalityMax
    model_user.preBattleTeam = sceneData:getBattleTeam()
    local Panel_VersusScene = self:getResourceNode():getChildByName("Panel_VersusScene")
    Panel_VersusScene:setTouchEnabled(true)
--    local Panel_B = Panel_VersusScene:getChildByName("Panel_B")
--    local Panel_T = Panel_VersusScene:getChildByName("Panel_T")
--    local Panel_Top_Base = Panel_T:getChildByName("Panel_Top_Base")
    local Panel_Top_Base = Panel_VersusScene:getChildByName("Panel_Top_Base")
    local Panel_Top = Panel_Top_Base:getChildByName("Panel_Top")
--    local Panel_Bottom_Base = Panel_B:getChildByName("Panel_Bottom_Base")
    local Panel_Bottom_Base = Panel_VersusScene:getChildByName("Panel_Bottom_Base")
    local Panel_Bottom = Panel_Bottom_Base:getChildByName("Panel_Bottom")
    local Panel_Bg_Base = Panel_VersusScene:getChildByName("Panel_Bg_Base")
    self.Panel_Weapon = Panel_VersusScene:getChildByName("Panel_Weapon")
    self.Panel_Weapon:setVisible(false)
    Panel_Bg_Base:setTouchEnabled(true)
--    SCREEN_TOP(Panel_T)
--    SCREEN_BOTTOM(Panel_B)
    SCREEN_TOP(Panel_Top_Base)
    SCREEN_BOTTOM(Panel_Bottom_Base)
    SCREEN_SCALE_BG(Panel_Bg_Base)
    SCREEN_CENTER(Panel_VersusScene)
    local Panel_Jice_Base1 = Panel_Bottom:getChildByName("Panel_Jice_Base1")
    local Panel_Jice_Base2 = Panel_Bottom:getChildByName("Panel_Jice_Base2")
    local Panel_Role1_Left = Panel_VersusScene:getChildByName("Panel_Role1_Left")
    local Panel_Role1_Right = Panel_VersusScene:getChildByName("Panel_Role1_Right")
    
    self.Panel_VersusScene_ = Panel_VersusScene
    self.Panel_Role1_Left_ = Panel_Role1_Left
    self.Panel_Role1_Right_ = Panel_Role1_Right 
    
    --标题
    Panel_Top:getChildByName("Text_Title"):setVisible(model_user.isPvp == false)
    Panel_Top:getChildByName("Title_Bg"):setVisible(model_user.isPvp == false)
    if model_user.isPvp == false then
        local stageModel = model_areaStorage:getStage(model_user.selectStageId,model_user.selectStageType)
        Panel_Top:getChildByName("Text_Title"):setString(stageModel.name_zhCN) 
    end
    
    local battleNpc  -- npc阵容
    local battleTeam -- 己方武将阵容
    if model_user.isPvp then
        battleNpc = model_user.battleNpcTeam
    else
        battleNpc = sceneData:getNpc()
    end
    battleTeam = sceneData:getBattleTeam()
   
    -- 左右按钮    
    self.Button_Left_1_ = Panel_VersusScene:getChildByName("Node_Left_1"):getChildByName("Panel_1"):getChildByName("Button_1")  
    self.Button_Right_1_ = Panel_VersusScene:getChildByName("Node_Left_2"):getChildByName("Panel_1"):getChildByName("Button_1")  
    self.Button_Left_2_ = Panel_VersusScene:getChildByName("Node_Right_1"):getChildByName("Panel_1"):getChildByName("Button_1")  
    self.Button_Right_2_ = Panel_VersusScene:getChildByName("Node_Right_2"):getChildByName("Panel_1"):getChildByName("Button_1") 

    Panel_Jice_Base2:setVisible(index==1) -- 计策
    Panel_Jice_Base1:setVisible(index~=1)
    
    -- PVE 开战
    local Button_Fire = Panel_Jice_Base1:getChildByName("Button_Fire") 
    local ProjectNode_4 = Panel_Jice_Base1:getChildByName("ProjectNode_4") 
    local ProjectNode_4_0 = Panel_Jice_Base2:getChildByName("ProjectNode_4_0") 
    
    local function initTeam(projectNode)
        local team = model_user.battleTeam
        for i=1,3 do
            local teamData = team[i]
            local ProjectNode=projectNode:getChildByName("ProjectNode_"..i)
            if teamData.isUse >0 then
                local monster = model_monsterStorage:getMonster(teamData.monsterId)
                self:initPanel_RisingStar_Head(ProjectNode,monster)
            else
                self:initPanel_RisingStar_Head(ProjectNode)
            end
        end
    end
    
    initTeam(ProjectNode_4)
    initTeam(ProjectNode_4_0)
    local Button_BZ=ProjectNode_4:getChildByName("Button_BZ")
    self:addBtnTouchEventListener(Button_BZ,function(sender,eventType)
        if eventType ==  2 then
            self:showDlg("battle/ChooseHeroInBattleScene",2,function()
                initTeam(ProjectNode_4)
            end)
        end
    end)
    
    local Button_BZ_Pvp=ProjectNode_4_0:getChildByName("Button_BZ")
    self:addBtnTouchEventListener(Button_BZ_Pvp,function(sender,eventType)
        if eventType ==  2 then
            self:showDlg("battle/ChooseHeroInBattleScene",2,function()
                initTeam(ProjectNode_4_0)
            end)
        end
    end)
    
    Button_Fire:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            local function onBattleExecBattle(success, data, code, msg)
                if(success) then
                    GLOBAL_MY_APP:removeDlgs()
                    --SWITSCENE(VersusScene.NEXTSCENE)
                    SWITSCENE("battle/BattleScene")
                end
            end
            if self:getSelectMonsterInfo().currentHP<=0 then
                PopMsg.getInstance():flashShow(LANG("英雄已经死亡不能出阵"))
                return
            end
            model_user.selectHeroPageIndex = self.pageIndexL
            model_user.battleSelectMonsterId = self:getSelectMonsterInfo().id
            model_user.battleSelectNpcId = self:getSelectNpcId()
        
            self.CandidateRoleUIR_:setItemPosByIndex(model_user.selectMonsterPageIndex)
 
            model_user.battleSelectMonsterIdList = {}
            model_user.battleSelectNpcIdList = {}
            if model_user.selectStageBattleMode == pbCommon.BattleMode.NORMAL then
                table.insert(model_user.battleSelectMonsterIdList, model_user.battleSelectMonsterId)
                table.insert(model_user.battleSelectNpcIdList, model_user.battleSelectNpcId)
            elseif model_user.selectStageBattleMode == pbCommon.BattleMode.DUEL then
                for key, value in pairs(model_user.battleMyTeam) do
                    table.insert(model_user.battleSelectMonsterIdList, value.id)
                end
                for key, value in pairs(model_user.battleNpcTeam) do
                    table.insert(model_user.battleSelectNpcIdList, value.id)
                end
            end
            
            local enemyIds
            if model_user.isPvp then
                enemyIds = model_user.battleSelectNpcIdList
            else
                enemyIds = {}
                for i = 1, #(model_user.battleSelectNpcIdList) do
                    for key, value in pairs(model_user.battleNpcTeam) do
                        if value.id == model_user.battleSelectNpcIdList[i] then
                            table.insert(enemyIds, value.NPCId)
                        end
                    end
                end
            end

            ctlBattleExecBattleRequest:sendRequest(onBattleExecBattle,
                model_user.selectIslandId, model_user.selectAreaId, model_user.selectStageId, model_user.selectStageType,
                model_user.battleProgress, model_user.battleSelectMonsterIdList, enemyIds);
            model_user.oldExp = model_user.exp
        end
    end)
    
    -- 阵型
    local Panel_Info = Panel_Role1_Left:getChildByName("Panel_Info")
    local Button_Deploy = Panel_Info:getChildByName("Button_Deploy")
    Button_Deploy:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            self:initFormation()
            self:getActionNode():gotoFrameAndPlay(165,248,false)
        end
    end)

    -- 保存阵型
    local Panel_popup = Panel_VersusScene:getChildByName("Panel_popup")
    local Button_1 = Panel_popup:getChildByName("Button_1")
    Button_1:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            ctlBattleSetBattleConfigRequest:sendRequest(function(sucess,data)
                if sucess then
                    helper_battle:updateConfirmFormation(self:getSelectMonsterInfo().id,self.position,self.selectFormationId)
                    self:initInfo(self.Panel_InfoL,battleTeam[self.pageIndexL])
                    self:getActionNode():gotoFrameAndPlay(248,300,false)
                end
            end,
            self:getSelectMonsterInfo().id, self.selectFormationId, self.position);

        end
    end)

    -- 阵型返回
    local Button_8 = Panel_Top:getChildByName("Button_8")
    Button_8:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            self:getActionNode():gotoFrameAndPlay(248,300,false)
        end
    end)

    
    
    --  关闭按钮
    local Button_Close = Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            if index==1 then
                ctlHighladderGetListRequest:sendRequest(handler(self,self.cutArenaPanel));
            else
                cclog("model_user.battleProgress:"..model_user.battleProgress)
                if model_user.battleProgress >1 then
                    self:showDlg("common/CommonPopScene",{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text=LANG("是否放弃战斗？"),fun=function(data)
                        if data.cmd=="confirm" then
                            GLOBAL_MY_APP:removeDlgs()
                            SWITSCENE("battle/ChapterScene")
                            --self:removeDlgNoAction()
                        elseif data.cmd =="cancel" then 

                        end
                    end})
                else
                    GLOBAL_MY_APP:removeDlgs()
                    SWITSCENE("battle/ChapterScene")
                    --self:removeDlgNoAction()
                    
                    
                end
                --self:showDlg("battle/SpotPopupScene",model_user.selectStageId,{})
            end
            Timer:kill(self.timerId2)
        end
    end)

    -- 屏蔽左侧pageView
    local pageViewLeft = Panel_Role1_Left:getChildByName("PageView_1")
    pageViewLeft:hide()
    pageViewLeft:setTouchEnabled(false)
    
    -- 左侧人物panel 用于clone
    local Panel_roleL = Panel_Role1_Left:getChildByName("role_infoL")
    Panel_roleL:setVisible(false)
  
    -- 左侧武将详情panel
    self.Panel_InfoL = Panel_Role1_Left:getChildByName("Panel_Info") 
    
    -- 更新武将显示名字颜色(根据阵亡和正常状态)
    local function changeHeroColor(item,itemInfo)
        local heroCustom = item:getChildByTag(1000)
        local Image_4 = item:getChildByName("Image_4")
        if itemInfo.currentHP <= 0 then
            if heroCustom then
                heroCustom:setColor(cc.c3b(150,150,150))
            end
            Image_4:setVisible(true)
        else
            if heroCustom then
                heroCustom:setColor(cc.c3b(255,255,255))
            end
            Image_4:setVisible(false)
        end
    end

    self.pageIndexL=1  --  左边选择索引    
    -- 左侧可用武将Label 
    local Text_Role_NumL = Panel_Role1_Left:getChildByName("Text_Role_Num") 
    Text_Role_NumL:ignoreContentAdaptWithSize(true)
    Text_Role_NumL:setString(self:getLeftMonster().."/"..#battleTeam)
     
    -- 左侧人物panel List
    local sprListL = {}
    for i=1,#battleTeam do
        local item = Panel_roleL:clone()       
        item:setAnchorPoint(cc.p(0.5,0.5))
        item:setTouchEnabled(false)
        item:setVisible(true)
        local itemInfo = battleTeam[i]
        local info = item:getChildByName("info")
        local hero = item:getChildByName("hero") 
        hero:setOpacity(0) 
        info:setOpacity(0)      
        local img = ccui.ImageView:create(model_monster:getHalf(itemInfo.id))
        img:setTag(1000)
        img:setPosition(cc.p(hero:getPosition()))
        img:setLocalZOrder(hero:getLocalZOrder()-1)
        item:addChild(img)
        item:setCascadeOpacityEnabled(true)
        info:setCascadeOpacityEnabled(true)
        changeHeroColor(item,itemInfo)
        self:initNameInfo(info,itemInfo)
        local monsterData = model_monsterStorage:getMonster(itemInfo.id)      
        table.insert(sprListL,item)
        
        --设置描边
        self:setLabelEffect(info:getChildByName("Text_heroname"))
        self:setLabelEffect(info:getChildByName("Text_level"))
        self:setLabelEffect(info:getChildByName("Sprite_Quality_Num"))
    end
    
    self:initInfo(self.Panel_InfoL,battleTeam[self.pageIndexL])
    self:initFormation()
 
    --  左侧人物选择回调
    local SelectRolecallCB = function(item, index)
        self.pageIndexL = index
        Text_Role_NumL:setString(self:getLeftMonster().."/"..#battleTeam)
        self:initInfo(self.Panel_InfoL,battleTeam[self.pageIndexL])
        changeHeroColor(item,battleTeam[self.pageIndexL])
        self:initFormation()
    end 
    
    local rolelayerL =  self.Panel_Role1_Left_:getChildByName("Panel_1")
    local btnNodeL = {}
    table.insert(btnNodeL, self.Button_Left_1_)
    table.insert(btnNodeL, self.Button_Right_1_)     
    local CandidateRoleUI = _REQUIRE ("app/views/common/CandidateRoleUI.lua")   
    local classRole = class("classRole1",CandidateRoleUI)
    local roleUI = classRole:create({layer = rolelayerL,nodeList= sprListL, btnNode = btnNodeL, callBack = SelectRolecallCB})  
    rolelayerL:addChild(roleUI) 
    self.CandidateRoleUIL_ = roleUI

    -- 屏蔽右侧pageView
    local pageViewRight = Panel_Role1_Right:getChildByName("PageView_2")
    pageViewRight:setVisible(false)
    
    -- 右侧人物panel 用于clone
    local role_infoR = Panel_Role1_Right:getChildByName("role_infoR")
    role_infoR:setVisible(false)
    -- 右侧武将详情panel
    local Panel_InfoR = Panel_Role1_Right:getChildByName("Panel_Info") 
    self.pageIndexR=1
    local Text_Role_NumR = Panel_Role1_Right:getChildByName("Text_Role_Num") 
    Text_Role_NumR:ignoreContentAdaptWithSize(true)
    Text_Role_NumR:setString(self.getRightNpc().."/"..#battleNpc)
    -- 人物panel List
    local sprListR = {}
    for i=1,#battleNpc do 
        local item = role_infoR:clone()
        item:setVisible(true)
        local itemInfo = battleNpc[i]
        item:setAnchorPoint(cc.p(0.5,0.5))
        item:setTouchEnabled(false)
        local info = item:getChildByName("info")
        local hero = item:getChildByName("hero") 
        item:setCascadeOpacityEnabled(true)
        info:setCascadeOpacityEnabled(true)
        hero:setOpacity(0) 
        info:setOpacity(0)      
        local img = ccui.ImageView:create(model_monster:getHalf(itemInfo.id))
        img:setTag(1000)
        img:setPosition(cc.p(hero:getPosition()))
        img:setLocalZOrder(hero:getLocalZOrder()-1)
        item:addChild(img)
        changeHeroColor(item,itemInfo)
        self:initNameInfo(info,itemInfo,true)
        local monsterData = model_monsterStorage:getMonster(itemInfo.id)
        table.insert(sprListR,item)    
     
        --设置描边
        self:setLabelEffect(info:getChildByName("Text_heroname"))
        self:setLabelEffect(info:getChildByName("Text_level"))
        self:setLabelEffect(info:getChildByName("Sprite_Quality_Num"))
    end
    self:initInfo(Panel_InfoR,battleNpc[self.pageIndexR])

    -- 右侧人物选择回调
    local SelectRolecallCBR = function(item, index)
        self.pageIndexR = index
        Text_Role_NumR:setString(self.getRightNpc().."/"..#battleNpc)
        self:initInfo(Panel_InfoR,battleNpc[self.pageIndexR])
        changeHeroColor(item,battleNpc[self.pageIndexR]) 
    end 
        
    local rolelayerR =  self.Panel_Role1_Right_:getChildByName("Panel_2")
    local btnNodeR = {}
    table.insert(btnNodeR, self.Button_Left_2_)
    table.insert(btnNodeR, self.Button_Right_2_)     
    local CandidateRoleUI2 = _REQUIRE ("app/views/common/CandidateRoleUI.lua")   
    local classRole2 = class("classRole2",CandidateRoleUI2)
    local roleUI2 = classRole2:create({layer = rolelayerR,nodeList= sprListR, btnNode = btnNodeR, callBack = SelectRolecallCBR})  
    rolelayerR:addChild(roleUI2) 
    self.CandidateRoleUIR_ = roleUI2
    
    -- 记录左右选择武将索引
    if model_user.selectHeroPageIndex~=nil then
        self.pageIndexL = model_user.selectHeroPageIndex   
    end

    if model_user.selectMonsterPageIndex~=nil then
        self.pageIndexR = model_user.selectMonsterPageIndex
    end
    
    --快速战斗按钮   
    local Button_Quick_Fire = Panel_Jice_Base2:getChildByName("Button_Fire_2_4_0_16")
    Button_Quick_Fire:setVisible(model_user.isPvp)
    Button_Quick_Fire:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            if not self.nodeAction then
                local node,nodeAction=self:createNode("Node/Node_Weapon.csb")
                self.nodeAction = nodeAction
                self.nodeAction:gotoFrameAndPause(0)
                self:getResourceNode():getChildByName("Panel_VersusScene"):addChild(node)
                node:setPosition(cc.p(545.82,222.91))
            end
            if model_user.isAllQuickBattle then
--                self:gotoBattle()
                self.firstAround = true
                self.nodeAction:gotoFrameAndPlay(0,55,false)
                self:getActionNode():gotoFrameAndPlay(80,140,false)
                ctlHighladderQuickBattleRequest:sendRequest(handler(self,self.onHighladderQuickBattle),model_user.currentBattleRank,model_user.battleProgress);
            else
                self:showDlg("arena/SkipPopupScene",function(data)
                    if data.cmd =="skipBattle" then --跳过战斗回调
                        model_game:setAutoQuickBattle(true);
                        model_user.oldExp = model_user.exp
--                        self:gotoBattle()   
                        --self:showDlg("arena/RapidBattleWaiting") 
                        self.firstAround = true
                        self.nodeAction:gotoFrameAndPlay(0,55,false)
                        self:getActionNode():gotoFrameAndPlay(80,140,false)
                        ctlHighladderQuickBattleRequest:sendRequest(handler(self,self.onHighladderQuickBattle),model_user.currentBattleRank,model_user.battleProgress);
                    end
                end)
            end
            
        end
    end)
    
    --开战按钮 PVP 
    local Button_Start_Fire = Panel_Jice_Base2:getChildByName("Button_Fire_2_4_14")
    Button_Start_Fire:addTouchEventListener(function(sender,eventType)
        if  eventType == 2 then
            model_user.oldExp = model_user.exp
            self:gotoBattle()
            
        end
    end)    
end

function VersusScene:update()
    self:updateHandler()
    if self.firstAround == true and self.nodeAction:isPlaying() == false then
        self.firstAround = false
        self.nodeAction:gotoFrameAndPlay(25,55,true)
        self:getActionNode():gotoFrameAndPlay(105,140,true)
    elseif self.endAround == true and self.nodeAction:isPlaying() == false then
        self.endAround = false
        self.nodeAction:gotoFrameAndPause(0)
        if self.quickBattleData.battleResult == true then --战斗结果为赢
                SWITSCENE("battle/WinPopupScene",self.quickBattleData);
            else --战斗结果为输
                SWITSCENE("battle/DefeatScene",self.quickBattleData);
            end
    end
end

function VersusScene:onHighladderQuickBattle(success, data, code, msg)
    self.quickBattleData = data
    if(success) then
        Timer:kill(self.timerId)
        self.endAround = true
        self.nodeAction:gotoFrameAndPlay(55,71,false)
        self:getActionNode():gotoFrameAndPlay(140,160,false)
    else
    end
end

function VersusScene:setLabelEffect(UIText)
	if UIText then
	   UIText:enableShadow(display.COLOR_BLACK,cc.size(4,-4))
       UIText:enableOutline(display.COLOR_BLACK,3)
	end
end

function VersusScene:gotoBattle()
    local function onPvpBattleExecBattle(success, data, code, msg)
        if(success) then
            SWITSCENE(VersusScene.NEXTSCENE)          
        end
    end
    if self:getSelectMonsterInfo().currentHP<=0 then
        PopMsg.getInstance():flashShow(LANG("英雄已经死亡不能出阵"))
        return
    end
    model_user.selectHeroPageIndex = self.pageIndexL
    model_user.selectMonsterPageIndex = self.pageIndexR 
    model_user.battleSelectMonsterId = self:getSelectMonsterInfo().id
    --model_user.battleSelectNpcId = self:getSelectMonsterId()
    --model_user.battleProgress=1
    local rank = model_user.currentBattleRank
    model_user.battleSelectNpcId = self:getSelectPvpNpcId()  
    self.CandidateRoleUIR_:setItemPosByIndex(model_user.selectMonsterPageIndex)
    
    model_user.battleSelectMonsterIdList = {}
    model_user.battleSelectNpcIdList = {}
    table.insert(model_user.battleSelectMonsterIdList, model_user.battleSelectMonsterId)
    table.insert(model_user.battleSelectNpcIdList, model_user.battleSelectNpcId)
    
    ctlHighladderExecBattleRequest:sendRequest(onPvpBattleExecBattle,
        rank, model_user.battleProgress, model_user.battleSelectMonsterId, model_user.battleSelectNpcId);
end

function VersusScene:getSelectNpcId()
    local battleNpc = sceneData:getNpc()
    local npcIndex = math.random(1,#battleNpc)
    if battleNpc[npcIndex].currentHP<=0 then
        return self:getSelectNpcId()
    else
        model_user.selectMonsterPageIndex = npcIndex 
        return battleNpc[npcIndex].id
    end
    
end

function VersusScene:getSelectPvpNpcId()
    local battleNpc = model_user.battleNpcTeam
    --self:removeDeadMonster(battleNpc,self.pageViewRight)
    for i = 1,#battleNpc do
        if battleNpc[i].currentHP > 0 then
            model_user.selectMonsterPageIndex = i 
            return battleNpc[i].id
        end
    end
end

function VersusScene:removeDeadHero(monsters,pageView)
--    if model_user.selectHeroPageIndex~=nil then
--        model_user.selectHeroPageIndex =nil
--    	return
--    end
--    for i=#monsters,1,-1 do
--        if monsters[i].HP<=0 then
--            table.remove(monsters,i)
--            pageView:removePageAtIndex(i-1)
--            if i<self.pageIndexL then
--                self.pageIndexL=self.pageIndexL-1
--            end
--            
--        end
--    end
end

function VersusScene:getLeftMonster()
    local battleTeam = sceneData:getBattleTeam()
    local num = 0
    for i = 1,#battleTeam do
        if battleTeam[i].currentHP > 0 then
            num = num + 1
        end
    end
    return num
end

function VersusScene:getRightNpc()
    local battleNpc
    if model_user.isPvp then
        battleNpc = model_user.battleNpcTeam
    else
        battleNpc = sceneData:getNpc()
    end
    local num = 0
    for i = 1,#battleNpc do
        if battleNpc[i].currentHP > 0 then
            num = num + 1
        end
    end
    return num
end

function VersusScene:getSelectMonsterInfo()
    local index = self.pageIndexL
    local battleTeam = sceneData:getBattleTeam()
    local monsterInfo = battleTeam[index]
    return monsterInfo
end

function VersusScene:initFormation()
    --local monsterId = self:getSelectMonsterInfo().id    
    local monsterInfo = self:getSelectMonsterInfo()
    local Panel_VersusScene = self:getResourceNode():getChildByName("Panel_VersusScene")
    local Panel_popup = Panel_VersusScene:getChildByName("Panel_popup")
    local img_select1 = Panel_popup:getChildByName("img_select1")
    self.position = monsterInfo.position
    
    local img_position = Panel_popup:getChildByName("Image_"..self.position)
    img_select1:setPosition(img_position:getPosition())
    
    local arr={"f1","b1"}
    for i=1,2 do
        local img_position = Panel_popup:getChildByName("Image_"..arr[i])
        img_position:setTag(i)
        img_position:setTouchEnabled(true)
        img_position:addTouchEventListener(function(sender,eventType)
            if  eventType == 2 then
                self.position = arr[sender:getTag()]
                local img_position = Panel_popup:getChildByName("Image_"..self.position)
                img_select1:setPosition(img_position:getPosition())
            end
        end)
    end
     
    local formations = monsterInfo.formations
    local img_select2 = Panel_popup:getChildByName("img_select2")
    
    local function initPosition()
        local img_formation = Panel_popup:getChildByName("Image_ZX_"..self.selectFormationId)
        img_select2:setPosition(img_formation:getPosition())
        local formationId = formations[self.selectFormationId]
        local txtName = Panel_popup:getChildByName("Text_3")
        local txtDesc = Panel_popup:getChildByName("Text_4")
        local Text_5 = Panel_popup:getChildByName("Text_5")
        txtName:setString(model_formations:getName(formationId))
        txtDesc:setString(model_formations:getDesc(formationId))
        Text_5:setVisible(false)
    end
    
    for i=1,#formations do
        if formations[i].isUse==true then
            self.selectFormationId = i
        end
        local img = Panel_popup:getChildByName("Image_ZX_"..i)
        local fImg = img:getChildByName("Image_"..i)
        fImg:setVisible(formations[i].formationId ~= 0)
        fImg:loadTexture(model_formations:getIcon(formations[i]["formationId"]))
        img:setTag(i) 
        img:setTouchEnabled(formations[i].formationId ~= 0)
        img:addTouchEventListener(function(sender,eventType)
            if  eventType == 2 then
                self.selectFormationId = sender:getTag()
                initPosition()
--                local img_formation = Panel_popup:getChildByName("Image_ZX_"..self.selectFormationId)
--                img_select2:setPosition(img_formation:getPosition())
            end
        end)
    end
      initPosition()
--    local img_formation = Panel_popup:getChildByName("Image_ZX_"..self.selectFormationId)
--    img_select2:setPosition(img_formation:getPosition())
end

function VersusScene:initNameInfo(info,data,isNpc)
    local Text_heroname = info:getChildByName("Text_heroname")
    Text_heroname:setString(model_monster:getName(data.id))   
    local Text_level = info:getChildByName("Text_level")
    local Sprite_Quality_Num = info:getChildByName("Sprite_Quality_Num")
    Text_level:setString(LANG("Lv%s",data.level))
    Text_level:setColor(cc.c3b(226,222,159))
    local index = 0
    if isNpc ==false then
        local color, sprits = model_monsterStorage:getColor(data.id)
        Text_heroname:setColor(color)
        if #sprits > 1 then
            index = sprits[2]
        end
    else
        local color, sprits = model_monsterStorage:getEquipColor(data.quality)
        Text_heroname:setColor(color)
        if #sprits > 1 then
            index = sprits[2]
        end
    end
    if tonumber(index) > 0 then
        Sprite_Quality_Num:setString("+" .. index)
    else
        Sprite_Quality_Num:setString("")
    end
    for i=1,5 do
        local star = info:getChildByName("Star_"..i)
        star:setVisible(i<=data.star)
    end
end

function VersusScene:initInfo(Panel_Info,data)
    local Panel_Info_Nums = Panel_Info:getChildByName("Panel_Info_Nums")  
    local LoadingBar_Xue = Panel_Info_Nums:getChildByName("LoadingBar_Xue")
    local Text_Xue_Num = LoadingBar_Xue:getChildByName("Text_Xue_Num")
    Text_Xue_Num:setString(data.currentHP.."/"..data.HP)
    LoadingBar_Xue:setPercent(data.currentHP/data.HP*100)  
    local LoadingBar_Fa = Panel_Info_Nums:getChildByName("LoadingBar_Fa")
    local Text_Fa_Num = LoadingBar_Fa:getChildByName("Text_Fa_Num")
    Text_Fa_Num:setString(data.currentMP.."/"..data.MP)
    LoadingBar_Fa:setPercent(data.currentMP/data.MP*100)   
    local Image_1 = Panel_Info:getChildByName("Image_1")
    local Icon_zhen = Panel_Info:getChildByName("Icon_zhen")
    Icon_zhen:loadTexture("Formation/Formation_"..data.position..".png")
    for i = 1, #data.formations do
        if data.formations[i].isUse == true then
            Image_1:loadTexture(model_formations:getIcon(data.formations[i]["formationId"]))
            break;
        end
    end
    
    local LoadingBar_Bing = Panel_Info_Nums:getChildByName("LoadingBar_Bing")
    local Text_Bing_Num = LoadingBar_Bing:getChildByName("Text_Bing_Num")
    Text_Bing_Num:setString(data.maxSoldier.."/"..data.maxSoldier)
    LoadingBar_Bing:setPercent(data.maxSoldier/data.maxSoldier*100)
    
    local Text_Wu_Num = Panel_Info_Nums:getChildByName("Text_Wu_Num")
    local Text_Zhi_Num = Panel_Info_Nums:getChildByName("Text_Zhi_Num")
    local Text_Tong_Num = Panel_Info_Nums:getChildByName("Text_Tong_Num")
    
    local Strength = helper_monster_data:calculateStrength(data.id)
    local Intelligence = helper_monster_data:calculateIntelligence(data.id)
    local Leadership = helper_monster_data:calculateLeadership(data.id)
    Text_Wu_Num:setString(math.floor(Strength))
    Text_Zhi_Num:setString(math.floor(Intelligence))
    Text_Tong_Num:setString(math.floor(Leadership))

    local helper_monster_data = _REQUIRE("model/helper/helper_monster_data.lua")
    local base = helper_monster_data:getBaseMonsterProp(data.id)
    local  prop = helper_monster_data:getFinalMonsterProp(data.id)  
    
    local Image_69 = Panel_Info_Nums:getChildByName("Image_69")  
    Image_69:loadTexture("army/icon_"..data.soldier.icon..".png")
end

function VersusScene:onEnter()
-- 进入场景 选人界面切换到当前选中项
    if model_user.selectHeroPageIndex~=nil then
        self.CandidateRoleUIL_:setItemPosByIndex(model_user.selectHeroPageIndex)   
    end
    
    if model_user.selectMonsterPageIndex~=nil then
        self.CandidateRoleUIR_:setItemPosByIndex(model_user.selectMonsterPageIndex)
    end
    self:playStart()
end

function VersusScene:cutArenaPanel(success, data, code, msg)
    if(success) then
        SWITSCENE("arena/ArenaScene",data)
    else
        release_print("ffffffffffffff")
    end
end

function VersusScene:removeDeadMonster(monsters,pageView)
--    if model_user.selectMonsterPageIndex~=nil then
--        cclog("remvoedeadMonster")
--        model_user.selectMonsterPageIndex =nil
--        return
--    end
--
--    for i=#monsters,1,-1 do
--        if monsters[i].HP<=0 then
--            table.remove(monsters,i)
--            pageView:removePageAtIndex(i-1)
--            if i<self.pageIndexR then
--                self.pageIndexR=self.pageIndexR-1
--            end
--        end
--    end
end

function VersusScene:removeDeadHero(monsters,pageView)
--    if model_user.selectHeroPageIndex~=nil then
--        model_user.selectHeroPageIndex =nil
--      return
--    end
--    for i=#monsters,1,-1 do
--        if monsters[i].HP<=0 then
--            table.remove(monsters,i)
--            pageView:removePageAtIndex(i-1)
--            if i<self.pageIndexL then
--                self.pageIndexL=self.pageIndexL-1
--            end
--            
--        end
--    end
end

return VersusScene
