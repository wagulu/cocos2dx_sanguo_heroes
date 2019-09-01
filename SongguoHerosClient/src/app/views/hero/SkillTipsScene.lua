
local SkillTipsScene = class("SkillTipsScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
local model_monsterSkill = _REQUIRE("model.static.model_monsterSkill")

SkillTipsScene.RESOURCE_FILENAME = "Scene/SkillTips.csb"


function SkillTipsScene:onCreate()
    self.data = self.args[1]
    local Panel_SkillTips = self:getResourceNode():getChildByName("Panel_SkillTips")
    Panel_SkillTips:setTouchEnabled(true)
    local Panel_Bg = Panel_SkillTips:getChildByName("Panel_Bg")
    local Panel_Center = Panel_SkillTips:getChildByName("Panel_C"):getChildByName("Panel_Center")
    local Text_DQSX = Panel_Center:getChildByName("Text_DQSX")
    local Text_Info_1 = Panel_Center:getChildByName("Text_Info_1")
    local Text_XJSX = Panel_Center:getChildByName("Text_XJSX")
    local Text_Info_2 = Panel_Center:getChildByName("Text_Info_2")
    local Text_JNMC = Panel_Center:getChildByName("Text_JNMC")
    local Text_Level_Num = Panel_Center:getChildByName("Text_Level_Num")
--    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    Panel_SkillTips:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end
    end
    )
    
    local Image_Skill1_Icon = Panel_Center:getChildByName("Image_Skill1_Icon")
    Image_Skill1_Icon:loadTexture("icon_skill/icon_skill_"..self.data.id..".png")
    
    Text_JNMC:setString(LANG(model_monsterSkill:getName(self.data.id)))
    Text_Level_Num:setString("LV"..self.data.level)
    
    Text_DQSX:setString(LANG("技能描述"))
    Text_Info_1:setString(LANG(model_monsterSkill:getDesc(self.data.id)))
    Text_XJSX:setVisible(false)
    Text_Info_2:setVisible(false)
end



function SkillTipsScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")

    end
end
return SkillTipsScene
