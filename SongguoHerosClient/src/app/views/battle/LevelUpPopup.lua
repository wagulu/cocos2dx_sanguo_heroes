
local LevelUpPopup = class("LevelUpPopup", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
LevelUpPopup.RESOURCE_FILENAME = "Scene/LevelUpPopup.csb"


function LevelUpPopup:onCreate()
    local LevelUpPopup = self:getResourceNode():getChildByName("LevelUpPopup")
    LevelUpPopup:setTouchEnabled(true)
    local Panel_Bg = LevelUpPopup:getChildByName("Panel_Bg")
    local Panel_Center = LevelUpPopup:getChildByName("Panel_C"):getChildByName("Panel_Center")
    SCREEN_SCALE_BG(Panel_Bg)
    
--    local Button_Close = Panel_Top:getChildByName("Button_Close")
--    Button_Close:setTouchEnabled(true)
    LevelUpPopup:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end
    end
    )
    
    Panel_Center:getChildByName("Text_Shuxing_2"):setString(model_user.preMonsterLevel)
    Panel_Center:getChildByName("Text_Shuxing_3"):setString(model_user.level)
    Panel_Center:getChildByName("Text_Shuxing_5"):setString(model_user.preEnergy)
    Panel_Center:getChildByName("Text_Shuxing_6"):setString(model_user.energy)
    Panel_Center:getChildByName("Text_Shuxing_8"):setString(model_user.preEnergyMax)
    Panel_Center:getChildByName("Text_Shuxing_9"):setString(model_user.energyMax)
    Panel_Center:getChildByName("Text_Shuxing_11"):setString(model_user.preVitality)
    Panel_Center:getChildByName("Text_Shuxing_12"):setString(model_user.vitality)
    Panel_Center:getChildByName("Text_Shuxing_14"):setString(model_user.preVitalityMax)
    Panel_Center:getChildByName("Text_Shuxing_15"):setString(model_user.vitalityMax)
    Panel_Center:getChildByName("Text_Shuxing_17"):setString(model_user.preMonsterLevel)
    Panel_Center:getChildByName("Text_Shuxing_18"):setString(model_user.level)
    

--    local Text_HP_Text3 = Panel_Center:getChildByName("Text_HP_Text3")
--    --最后还有一行字，策划说等他配表
--    Text_HP_Text3:setString(LANG(" 等级:%s->%s\n 当前体力:%s->%s\n 体力上限:%s\n 当前精力:%s\n 精力上限:%s\n 武将等级上限:%s\n",
--        model_user.preMonsterLevel,model_user.level,model_user.preEnergy,model_user.energy,model_user.energyMax,
--        model_user.vitality,model_user.vitalityMax,model_user.level))
    
end

return LevelUpPopup
