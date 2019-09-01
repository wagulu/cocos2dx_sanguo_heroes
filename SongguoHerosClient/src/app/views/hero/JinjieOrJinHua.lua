
local JinjieOrJinHua = class("JinjieOrJinHua", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
local helper_monster_data = _REQUIRE("model/helper/helper_monster_data.lua")
local model_monster = _REQUIRE("model/static/model_monster.lua")
local model_monsterSkill = _REQUIRE("model.static.model_monsterSkill")
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
JinjieOrJinHua.RESOURCE_FILENAME = "Scene/RisingStarPop.csb"


function JinjieOrJinHua:onCreate()
    self.data = self.args[2]
    self.preData = self.args[3]
    self.starArr = self.args[4]
    local Panel_RisingStarPop = self:getResourceNode():getChildByName("Panel_RisingStarPop")
    Panel_RisingStarPop:setTouchEnabled(true)
    local Panel_Bg = Panel_RisingStarPop:getChildByName("Panel_Bg")
    local Panel_PopupBg_Center = Panel_RisingStarPop:getChildByName("Panel_P"):getChildByName("Panel_PopupBg_Center")
    local Panel_Top = Panel_RisingStarPop:getChildByName("Panel_Top")
    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    local Button_Close = Panel_Top:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end

    end
    )

    local Button_Confirm = Panel_PopupBg_Center:getChildByName("Button_Confirm")
    Button_Confirm:addTouchEventListener(function(sender,eventType)
        if eventType ==2 then
            self:removeDlgNoAction()
        end

    end
    )


    local Text_Title = Panel_PopupBg_Center:getChildByName("Text_Title")
    local node_RisingStar_Head_1 = Panel_PopupBg_Center:getChildByName("Node_RisingStar_Head_1"):getChildByName("Panel_RisingStar_Head")
    local Image_Hero = node_RisingStar_Head_1:getChildByName("Image_Hero")
    local Image_Head_bg = node_RisingStar_Head_1:getChildByName("Image_Head_bg")
    local Image__Job = node_RisingStar_Head_1:getChildByName("Image__Job")
    local node_RisingStar_Head_2 = Panel_PopupBg_Center:getChildByName("Node_RisingStar_Head_2"):getChildByName("Panel_RisingStar_Head")
    local Image_Hero2 = node_RisingStar_Head_2:getChildByName("Image_Hero")
    local Image_Head_bg2 = node_RisingStar_Head_2:getChildByName("Image_Head_bg")
    local Image__Job2 = node_RisingStar_Head_2:getChildByName("Image__Job")
    if self.args[1] ==1 then
        Text_Title:setString("角色进阶")
        local advancePanel = Panel_PopupBg_Center:getChildByName("Panel_Advance")
        local Image_Skill1_Icon = advancePanel:getChildByName("Image_Skill1_Icon")
        local Text3 = advancePanel:getChildByName("Text3")
        local Text4 = advancePanel:getChildByName("Text4")
        advancePanel:setVisible(true)
        Panel_PopupBg_Center:getChildByName("Panel_Upgrade"):setVisible(false)
        advancePanel:getChildByName("Text1"):setString(LANG("战斗力：%s",math.floor(helper_monster_data:fightCapacity(self.data.id))))
        advancePanel:getChildByName("Text2"):setString(LANG(math.floor(helper_monster_data:fightCapacityNextQuality(self.data.id))))
        --是否新开了新技能
        local isHaveNewSkill = false
        for i = 1,4 do
            if self.data.skills[i].isUnlock ~= self.preData.skills[i].isUnlock then
        		isHaveNewSkill = true
                Text3:setString(LANG("新技能:%s",model_monsterSkill:getName(self.data.skills[i].id)))
                Text4:setString(LANG(model_monsterSkill:getDesc(self.data.skills[i].id))) -- 技能描述
--                Image_Skill1_Icon:loadTexture()--技能图标
        	end
        end
        Image_Skill1_Icon:setVisible(isHaveNewSkill)
        Text3:setVisible(isHaveNewSkill)
        Text4:setVisible(isHaveNewSkill)
        
    elseif self.args[1]==2 then
        Text_Title:setString("角色升星")
        local upgradePanel = Panel_PopupBg_Center:getChildByName("Panel_Upgrade")
        Panel_PopupBg_Center:getChildByName("Panel_Advance"):setVisible(false)
        upgradePanel:setVisible(true)
--        local arr ={LANG("武力：%s",math.floor(helper_monster_data:calculateStrength(self.data.id))),math.floor(helper_monster_data:calculateStrengthNextStar(self.data.id)),
--            LANG("智力：%s",math.floor(helper_monster_data:calculateIntelligence(self.data.id))),math.floor(helper_monster_data:calculateIntelligenceNextStar(self.data.id)),
--            LANG("统率力：%s",math.floor(helper_monster_data:calculateLeadership(self.data.id))),math.floor(helper_monster_data:calculateLeadershipNextStar(self.data.id)),}
        for i = 1,6 do
            upgradePanel:getChildByName("Text_Shuxing_"..i):setString(self.starArr[i])
        end
        
    end
    
    --头像及颜色框
    Image_Hero:loadTexture(model_monster:getIcon(self.data.id))
    Image_Head_bg:loadTexture(model_monsterStorage:getFrameByQuality(self.preData["quality"]))
    Image_Hero2:loadTexture(model_monster:getIcon(self.data.id))
    Image_Head_bg2:loadTexture(model_monsterStorage:getFrameByQuality(self.data["quality"]))

    --星级显示
    for j = 1,5 do
        node_RisingStar_Head_1:getChildByName("Panel_Star"):getChildByName("Image_Star_"..j):setVisible(j<=self.preData.star)
        node_RisingStar_Head_2:getChildByName("Panel_Star"):getChildByName("Image_Star_"..j):setVisible(j<=self.data.star)
    end
    
    --职业
    Image__Job:loadTexture("Job/Icon_job"..model_monster:getType(self.data["id"])..".png")
    Image__Job2:loadTexture("Job/Icon_job"..model_monster:getType(self.data["id"])..".png")
end



function JinjieOrJinHua:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("battle/ChapterScene")

    end
end
return JinjieOrJinHua
