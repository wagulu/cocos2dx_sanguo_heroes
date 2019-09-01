local HeroDetailPop = class("HeroDetailPop", cc.load("mvc").ViewBase)
HeroDetailPop.NEXTSCENE = "battle/ChapterScene"

HeroDetailPop.RESOURCE_FILENAME = "Scene/HeroDetailsPanel.csb"
function HeroDetailPop:onCreate()
    self:initUi()
end

function HeroDetailPop:initUi()
    local Panel_TreasureDetails = self:getResourceNode():getChildByName("Panel_TreasureDetails")
    self.Panel_Center = Panel_TreasureDetails:getChildByName("Panel_Center")
    self.Panel_Top = Panel_TreasureDetails:getChildByName("Panel_Top")
    self.Panel_Bottom = Panel_TreasureDetails:getChildByName("Panel_Bottom")
    
    local data = self.args[2]
    local monsterItem = data.monsterItem
    local battleMonsters = data.monsterItems
    local callback = data.callback
    local from = data.from
    
    local Button_replace = self.Panel_Center:getChildByName("ScrollView_1"):getChildByName("Button_2")
    local Button_removed = self.Panel_Center:getChildByName("ScrollView_1"):getChildByName("Button_3")
    local Button_Close = self.Panel_Top:getChildByName("Button_Close")
    
    self:addBtnTouchEventListener(Button_Close, function (sender, eventType)
        if eventType==2 then
            self:removeDlgNoAction()
        end
    end)
    
    Button_replace:setTitleText(LANG("替换"))
    self:addBtnTouchEventListener(Button_replace, function (sender, eventType)
        if eventType==2 then
            self:showDlg("battle/HeroListScene", monsterItem, battleMonsters, callback)
            self:removeDlgNoAction()
        end
    end)
    
    Button_removed:setTitleText(LANG("撤下"))
    self:addBtnTouchEventListener(Button_removed, function (sender, eventType)
        if eventType==2 then
            callback(monsterItem, 0)
            self:removeDlgNoAction()
        end
    end)
    
    Button_replace:setVisible(false)
    Button_replace:setEnabled(false)
    Button_removed:setVisible(false)
    Button_removed:setEnabled(false)
    
    if from ~= nil and from == "BattleTeamScene" then
        Button_replace:setVisible(true)
        Button_replace:setEnabled(true)
        Button_removed:setVisible(true)
        Button_removed:setEnabled(true)
    end
    
end


return HeroDetailPop