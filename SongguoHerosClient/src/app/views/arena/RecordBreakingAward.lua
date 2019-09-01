--额外奖励界面
local RecordBreakingAward = class("RecordBreakingAward", cc.load("mvc").ViewBase)
local model_equipment = _REQUIRE("model/static/model_equipment.lua")
RecordBreakingAward.NEXTSCENE = "battle/ChapterScene"

RecordBreakingAward.RESOURCE_FILENAME = "Scene/RecordBreakingAward.csb"
function RecordBreakingAward:onCreate()
    self.data = self.args[1]
    self:initUi()
end

function RecordBreakingAward:initUi()
    local RecordBreakingAward = self:getResourceNode():getChildByName("RecordBreakingAward")
    local Panel_Center = RecordBreakingAward:getChildByName("Panel_C"):getChildByName("Panel_Center")
--    local Panel_Top = RecordBreakingAward:getChildByName("Panel_Top")
    local Panel_Bg = RecordBreakingAward:getChildByName("Panel_Bg")
--    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    --关闭按钮
    local  Button_Close=Panel_Center:getChildByName("Button_Close")
    Button_Close:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:removeDlgNoAction()
        end
    end
    )
    
    --确定按钮
    local  Button_1=Panel_Center:getChildByName("Button_1")
    Button_1:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:removeDlgNoAction()
        end
    end
    )
 
    local Text_HP_Text3 = Panel_Center:getChildByName("Text_HP_Text3")
    local Text_1 = Panel_Center:getChildByName("Text_1")
    local Image_1 = Panel_Center:getChildByName("Image_1")
    Text_HP_Text3:setString(LANG("恭喜您达到了新的历史排名:%s，众将恭贺新禧，为您送上贺礼:",self.data.rank))
    
    local prize = self.data.highRankPrizes
    for i = 1,table.getn(prize) do
        if prize[i].type == "Cash" then
            Text_1:setString(LANG(prize[i].content))
        end
    end
end

return RecordBreakingAward
