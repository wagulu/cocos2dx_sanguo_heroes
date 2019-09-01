--排名奖励界面
local ArenaAwardPopup = class("ArenaAwardPopup", cc.load("mvc").ViewBase)
local model_equipment = _REQUIRE("model/static/model_equipment.lua")
ArenaAwardPopup.NEXTSCENE = "battle/ChapterScene"

ArenaAwardPopup.RESOURCE_FILENAME = "Scene/ArenaAwardPopup.csb"
function ArenaAwardPopup:onCreate()
    self.data = self.args[1]
    self:initUi()
end

function ArenaAwardPopup:initUi()
    local arenaAwardPopup = self:getResourceNode():getChildByName("ArenaAwardPopup")
    local Panel_Center = arenaAwardPopup:getChildByName("Panel_C"):getChildByName("Panel_Center")
--    local Panel_Top = arenaAwardPopup:getChildByName("Panel_Top")
    local Panel_Bg = arenaAwardPopup:getChildByName("Panel_Bg")
--    SCREEN_TOP(Panel_Top)
    SCREEN_SCALE_BG(Panel_Bg)
    arenaAwardPopup:setTouchEnabled(true)
    --关闭按钮
--    local  Button_Close=Panel_Center:getChildByName("Button_Close")
    arenaAwardPopup:addTouchEventListener(function(sender,eventType)
        if eventType==2 then
            self:removeDlgNoAction()
        end
    end
    )
 
    local ScrollView_1 = Panel_Center:getChildByName("ScrollView_1")
    local Panel_1 = ScrollView_1:getChildByName("Panel_1")
    local num = table.getn(self.data.rewards)
    local contentSize =cc.size(409,116)
    self.space = 0
    local sSize=cc.size(ScrollView_1:getInnerContainerSize().width,math.max(ScrollView_1:getContentSize().height,(contentSize.height+self.space)*num));
    ScrollView_1:setInnerContainerSize(sSize)
    self.items = {}
    for i = 1,num do
        local item = Panel_1:clone()
        local index = i-1
        item:setTag(index);
        ScrollView_1:addChild(item)
        local x = 10--(sSize.width/2)
        local y = sSize.height-(contentSize.height+self.space)*math.floor(index)--contentSize.height/2
        item:setPosition(cc.p(x,y))
        self.items[i] = item

    --数据显示
        local Text_Title_1 = item:getChildByName("Text_Title_1")
        local Text_Title_3 = item:getChildByName("Text_Title_3")
        local Text_Title_4 = item:getChildByName("Text_Title_4")
        local info = self.data.rewards[i]
        if (info.to - info.from) == 0 then
            Text_Title_1:setString(LANG("第%s名",info.to))
        else
            if info.to == 0 then
                Text_Title_1:setString(LANG("第%s名",info.from))
            else
                Text_Title_1:setString(LANG("第%s ~ %s名",info.from,info.to))
            end
        end
        for j = 1,table.getn(info.prizes) do
        	if info.prizes[j].type ==  "HighladderPrestige" then--声望
                Text_Title_3:setString(info.prizes[j].content)
            elseif info.prizes[j].type == "Cash" then--元宝
                Text_Title_4:setString(info.prizes[j].content)
            end
        end
    end
--    Panel_1:setVisible(false)
    ScrollView_1:removeChild(Panel_1)
end

return ArenaAwardPopup
