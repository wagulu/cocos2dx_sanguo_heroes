--快速战斗剩余将领界面
local RapidBattleWaiting = class("RapidBattleWaiting", cc.load("mvc").ViewBase)
RapidBattleWaiting.NEXTSCENE = "battle/ChapterScene"
RapidBattleWaiting.STOPFRAME = 15
RapidBattleWaiting.RESOURCE_FILENAME = "Scene/RapidBattleWaiting.csb"
function RapidBattleWaiting:onCreate()
    self:initUi()
end

function RapidBattleWaiting:initUi()
    local RapidBattleWaiting = self:getResourceNode():getChildByName("RapidBattleWaiting")
    local Panel_Center = RapidBattleWaiting:getChildByName("Panel_Center")
    local Panel_Bg = RapidBattleWaiting:getChildByName("Panel_B"):getChildByName("Panel_Bg")
    local Popup = Panel_Center:getChildByName("Popup")
    local Text_1 = Popup:getChildByName("Text_1")
    local Text_2 = Popup:getChildByName("Text_2")
    local LoadingBar = Popup:getChildByName("LoadingBar")
    
    SCREEN_SCALE_BG(Panel_Bg)
    
    Text_1:setString(LANG("正在快速战斗..."))
    Text_2:setVisible(false)
    LoadingBar:setVisible(false)
end

return RapidBattleWaiting
