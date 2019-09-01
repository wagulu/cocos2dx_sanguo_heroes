local AdvancedListScene = class("HeroListScene", cc.load("mvc").ViewBase)
AdvancedListScene.NEXTSCENE = "battle/ChapterScene"

AdvancedListScene.RESOURCE_FILENAME = "Scene/SelectHeroPop.csb"

local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_static_library = _REQUIRE "src/model/static/model_static_library.lua"
local helper_team = _REQUIRE("model/helper/helper_team")
local helper_relationship = _REQUIRE("model/helper/helper_relationship")
local model_monster = _REQUIRE("model/static/model_monster")

_REQUIRE("pbCommon.lua")

function AdvancedListScene:onCreate()
    self:initUi()
end

function AdvancedListScene:initUi()
    self.data = self.args[1]
    self.from = self.data.from
    self.callback = self.data.callback      --从HeroInfo界面进入用于刷新界面


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
            if self.from and self.from == "MainCityScene" then
                SWITSCENE("home/MainCityScene")
            else 
                self:removeDlgNoAction()
            end
        end
    end)


    self:updateItem()
end

function AdvancedListScene:updateItem()
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
        local Panel_Bg = newItem:getChildByName("Panel_Bg")
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

        Panel_Bg:setTouchEnabled(true)
        Panel_Bg:addTouchEventListener(function (sender, eventType)
            if eventType==2 then
                if self.from and self.from == "MainCityScene" then
                    --SWITSCENE("hero/HeroUpgradeScene", monster, nil, "MainCityScene")
                    SWITSCENE("hero/DestinyScene", {monster = monster, callback = nil, from = "MainCityScene"})
                    SWITSCENE("hero/BreakthroughScene", {monster = monster, callback = nil, from = "MainCityScene"})
                else 
                    if self.callback then
                        self:callback(monster)
                    end
                    self:removeDlgNoAction()
                end
            end
        end)

        local Button = newItem:getChildByName("Button_6")
        Button:setVisible(false)
        
        newItem:getChildByName("Text1"):setVisible(false)
    end
end

return AdvancedListScene