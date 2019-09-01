local BattleHeroListScene = class("HeroListScene", cc.load("mvc").ViewBase)
BattleHeroListScene.NEXTSCENE = "battle/ChapterScene"

BattleHeroListScene.RESOURCE_FILENAME = "Scene/SelectHeroPop.csb"

local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
local helper_team = _REQUIRE("model/helper/helper_team")
local helper_relationship = _REQUIRE("model/helper/helper_relationship")
local model_monster = _REQUIRE("model/static/model_monster")

_REQUIRE("pbCommon.lua")

function BattleHeroListScene:onCreate()
    self:initUi()
end

function BattleHeroListScene:initUi()
    self.monsterItem = self.args[1]
    self.battleMonsters = self.args[2]
    self.callback = self.args[3]
    self.beforMonsters = clone(self.battleMonsters)
    

    local Panel_SelectHeroPop = self:getResourceNode():getChildByName("Panel_SelectHeroPop")
    self.Panel_Center = Panel_SelectHeroPop:getChildByName("Panel_C"):getChildByName("Panel_Center")
    self.Panel_Top = Panel_SelectHeroPop:getChildByName("Panel_Top")
    local Panel_Bg = Panel_SelectHeroPop:getChildByName("Panel_Bg")
    Panel_SelectHeroPop:setTouchEnabled(true)
    SCREEN_TOP(self.Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    
    local Button_Back = self.Panel_Top:getChildByName("Button_Back")
    
    self:addBtnTouchEventListener(Button_Back, function (sender, eventType)
        if eventType==2 then
            self:removeDlgNoAction()
        end
    end)
    
    
    self:updateItem()
end

function BattleHeroListScene:updateItem()
    local scrollView = self.Panel_Center:getChildByName("HeroScrollView")
    local node,item
    node = self:createNode("Node/Node_TeamItem.csb")
    item = node:getChildByName("Node_TeamItem")
    local contentSize = item:getContentSize()

    local heroList = model_monsterStorage:getAllActiveMonsters()

    self.space = 3
    local num = #(heroList)
    local sSize = cc.size(scrollView:getInnerContainerSize().width, 
        math.max(scrollView:getContentSize().height, (contentSize.height + self.space) * math.ceil(num/2)));
    scrollView:setInnerContainerSize(sSize)
    --添加item
    for i=1,num do
        local newItem = item:clone()
        local index = i - 1
        scrollView:addChild(newItem)
        local x = (sSize.width/2) / 2.0 + sSize.width/2 * (index%2) - 10;
        local y = sSize.height - (contentSize.height + self.space) * math.floor(index / 2) - contentSize.height/2
        newItem:setPosition(cc.p(x, y))


        local monster = heroList[i]
        local icon_iv = newItem:getChildByName("Panel_Bg"):getChildByName("Image_Head")
        local job_iv = newItem:getChildByName("Panel_Bg"):getChildByName("Image_Icon_Job")
        job_iv:loadTexture("Job/Icon_job"..model_monster:getType(monster.id)..".png")
        local name_tv = newItem:getChildByName("Text_Name")
        newItem:getChildByName("Panel_Bg"):getChildByName("Image_Head_Off"):setVisible(false)
        icon_iv:loadTexture(model_static_library:getAsset(monster.id, pbCommon.ObjectType.Monster))
        newItem:getChildByName("Text_Level_9"):setString(LANG("LV%s", monster.level))
        name_tv:setString(LANG("%s", model_monster:getName(monster.id)))
        name_tv:setColor(model_static_library:getColor(monster.id, pbCommon.ObjectType.Monster))
        newItem:getChildByName("Panel_Bg"):getChildByName("Image_Head_Bg"):loadTexture(model_monsterStorage:getFrameByQuality(monster.quality))
        local Panel_Star = newItem:getChildByName("Panel_Star")
        --星级显示
        for k = 1,5 do
            Panel_Star:getChildByName("Image_Star_"..k):setVisible(k<= monster.star)
        end

        --头像点击弹出武将详细信息，无“撤下”和“替换”
--        self:addBtnTouchEventListener(icon_iv, function (sender, eventType)
--            if eventType==2 then
--                --打开武将详细信息
--                self:showDlg("battle/HeroDetailPop", monster, self.monsters, self.callback)
--            end
--        end)
--        icon_iv:setTouchEnabled(true)
--        icon_iv:addTouchEventListener(function (sender, eventType)
--            if eventType==2 then
--                --打开武将详细信息
--                self:showDlg("hero/HeroInfoScene", monster, "mine") 
--            end
--        end)
        
        local Button = newItem:getChildByName("Button_6")
        if helper_team:isMonsterInTeam(monster.id,self.battleMonsters) then
            Button:setTitleText(LANG("替换"))
            self:addBtnTouchEventListener(Button, function (sender, eventType)
                if eventType==2 then
                    --弹出提示框
                    self:showDlg("common/CommonPopScene",{confirmStr=LANG("确定"),cancelStr=LANG("取消"),text=LANG("该武将已在阵中，是否更换武将？"),fun=function(data)
                        if data.cmd=="confirm" then
                            self.callback(self.monsterItem, monster.id, self.battleMonsters)
                            --获得激活配缘描述
                            local des = self:getDesForRelationship2(monster.id)
                            --弹出激活的配缘
                            PopMsg.getInstance():flashShow(des)
                            
                            self:removeDlgNoAction()
                        elseif data.cmd =="cancel" then 

                        end
                    end})
                end
            end)
        else
            Button:setTitleText(LANG("上阵"))
            self:addBtnTouchEventListener(Button, function (sender, eventType)
                if eventType==2 then
                    self.callback(self.monsterItem, monster.id, self.battleMonsters)
                    --获得激活配缘描述
                    local des = self:getDesForRelationship2(monster.id)
                    --弹出激活的配缘
                    PopMsg.getInstance():flashShow(des)

                    self:removeDlgNoAction()
                end
            end)
        end

        newItem:getChildByName("Text1"):setString(LANG("可激活缘分%s", self:getRelatExceptCurrent2(monster)))
        --newItem:getChildByName("Text1"):setVisible(false)
    end
end

function BattleHeroListScene:getRelatExceptCurrent(monster)
    --monsterId的集合
    local members = {}
    for key, value in pairs(self.battleMonsters) do
        if(value.monsterId > 0 and value.monsterId ~= self.monsterItem.monsterId) then
            table.insert(members,value.monsterId)
            cclog("monsterId = "..value.monsterId)
        end
    end
	table.insert(members,monster.id)
	
	local count = 0
    local relationship = helper_relationship:getRelationshipLevel(monster, members)
    for key, value in pairs(relationship) do
        if value.actived then
            count = count + 1
            
        end
    end
    
    return count
end

function BattleHeroListScene:getRelatExceptCurrent2(monster)
    local afterMonsters = clone(self.beforMonsters)
    helper_team:setInTeam(monster.id, self.monsterItem.pos, afterMonsters)
    
    local befor_relationship = self:getRelationArr(self.beforMonsters)
    local after_relationship = self:getRelationArr(afterMonsters)
    
    local count = #(after_relationship) - #(befor_relationship)
    if count < 0 then
        count = 0
    end
    
    return count
end

function BattleHeroListScene:getDesForRelationship(monsterId)
    local members = {} 
    
    --monsterId的集合
    for key, value in pairs(self.battleMonsters) do
        if(value.monsterId > 0) then
            table.insert(members,value.monsterId)
            cclog("monsterId = "..value.monsterId)
        end
    end

    local monster = model_monsterStorage:getMonster(monsterId)
    local relationship = helper_relationship:getRelationshipLevel(monster, members)
    local des = ""
    for key, value in pairs(relationship) do
        if value.actived then
            local effect = value["effect"]
            des = des..LANG("%s", effect["description_zhCN"]).."\n"
        end 
    end
    
    return des;
end

function BattleHeroListScene:getDesForRelationship2(monsterId)
    --所有缘分的集合
    local befor_relationship = self:getRelationArr(self.beforMonsters)
    local after_relationship = self:getRelationArr(self.battleMonsters)
    local all_relationship = {}
    
--    for i=1,#(befor_relationship) do
--        local isDiffrent = true
--        for j=1,#(after_relationship) do
--            if befor_relationship[i].effect.id == after_relationship[j].effect.id then
--                isDiffrent = false
--            end
--        end
--        
--        if isDiffrent then
--            table.insert(all_relationship,befor_relationship[i])
--        end
--    end
    
    for i=1,#(after_relationship) do
        local isDiffrent = true
        for j=1,#(befor_relationship) do
            if after_relationship[i].effect.id == befor_relationship[j].effect.id then
                isDiffrent = false
            end
        end

        if isDiffrent then
            table.insert(all_relationship,after_relationship[i])
        end
    end
    
    local des = ""
    for key, value in pairs(all_relationship) do
        if value.actived then
            local effect = value["effect"]
            des = des..LANG("%s", value.name..effect["description_zhCN"]).."\n"
        end 
    end

    return des;
end

--所有缘分的集合
function BattleHeroListScene:getRelationArr(monsterItems)
    local all_relationship = {}
    
    local members = {} 

    --monsterId的集合
    for key, value in pairs(monsterItems) do
        if(value.monsterId > 0) then
            table.insert(members,value.monsterId)
            cclog("monsterId = "..value.monsterId)
        end
    end
    
    
    for key,value in pairs(members) do
        local monster = model_monsterStorage:getMonster(value)
        local relationship = helper_relationship:getRelationshipLevel(monster, members)
        for k, v in pairs(relationship) do
            if v.actived then
               v.name = model_monster:getName(monster.id)
               table.insert(all_relationship, v)
            end 
        end
    end
    
    return all_relationship
end

return BattleHeroListScene