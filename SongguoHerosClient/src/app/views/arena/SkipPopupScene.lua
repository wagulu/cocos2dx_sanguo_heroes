--跳过战斗确认界面
local SkipPopupScene = class("SkipPopupScene", cc.load("mvc").ViewBase)
SkipPopupScene.NEXTSCENE = "battle/ChapterScene"
--_REQUIRE "src/packages/packet/Packet_Regester"
SkipPopupScene.STOPFRAME = 6
SkipPopupScene.RESOURCE_FILENAME = "Scene/SkipPopupScene.csb"
local model_user = _REQUIRE("model.model_user")
function SkipPopupScene:onCreate()
    self.fun = self.args[1]
    self:initUi()
end

function SkipPopupScene:initUi()
--    self:getActionNode():gotoFrameAndPlay(0,false)
    self:getActionNode():gotoFrameAndPause(45)
    local SkipPopup = self:getResourceNode():getChildByName("SkipPopup")
    local Popup = SkipPopup:getChildByName("Panel_Center"):getChildByName("Popup")
    local Text_1 = Popup:getChildByName("Text_1")
    local Text_2 = Popup:getChildByName("Text_2")
    local Panel_Bg = SkipPopup:getChildByName("Panel_Bg")
--    SCREEN_TOP(Popup)
    SCREEN_SCALE_BG(Panel_Bg)
    
    --选择框
    self.isCutCheckBox = true--false
    local CheckBox_1 = Popup:getChildByName("CheckBox_1")
    CheckBox_1:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
--            self.isCutCheckBox = true
            release_print("ppppppf",sender:isSelected())
            local boo = sender:isSelected() == false
            self.isCutCheckBox = boo
            model_user.isAllQuickBattle = boo
        end
    end
    )
    --取消按钮
    local  Button_cancel=Popup:getChildByName("Button_cancel")
    Button_cancel:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:getActionNode():gotoFrameAndPlay(45,false)
            self:removeDlgNoAction()
        end
    end
    )
    
    --跳过按钮
    local  Button_skip=Popup:getChildByName("Button_skip")
    Button_skip:addTouchEventListener(function(sender,eventType)
        if eventType==2 then  
--            SWITSCENE("arena/RapidBattleWaiting") 
            if self.isCutCheckBox then
            	model_user.isAllQuickBattle = true
            end
            self.fun({cmd="skipBattle"}) 
            self:removeDlgNoAction()
               
--            if exp then--并且向服务器发请求，跳过战斗弹出战斗结果界面，并且保存是否选择了本次不再提示的选项（全局记录）
----                self:removeDlgNoAction()
--                self:getActionNode():gotoFrameAndPlay(0,false)
--                 self:showDlg("battle/VersusScene",data)
--                --
--            else--判断下，如果vip等级不够，那么弹出确认充值弹窗界面
----                SWITSCENE("arena/SkipPopupScene")
--            end 
        end
    end
    )
end

return SkipPopupScene
