
local HomeScene = class("HomeScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
_REQUIRE "cocos.cocos2d.json"
HomeScene.RESOURCE_FILENAME = "MainScene.csb"


function HomeScene:onCreate()

--    release_print(_REQUIRE("battle/ChapterSceneData").type);
--    
--    -- pb sample
--    _REQUIRE("app.IntegrationTest");
--    
----    release_print("========= create");
----    
----    
----    local str=json.decode("{'a':1}")
----    cclog("------------------------------------")
----    cclog(str)
--    
--   
    local panel_MainScene = self:getResourceNode():getChildByName("Panel_MainScene");
    local panel_Top = panel_MainScene:getChildByName("Panel_Top")
    local panel_Bottom = panel_MainScene:getChildByName("Panel_Bottom")
    local panel_Picture_Bg = panel_MainScene:getChildByName("Panel_Picture_Bg")
    SCREEN_SCALE_BG(panel_Picture_Bg)
    SCREEN_TOP(panel_Top)
    SCREEN_BOTTOM(panel_Bottom)
    local panel_Center = panel_MainScene:getChildByName("Panel_Center");
    local button_Start = panel_Center:getChildByName("Button_Start")
    self:addBtnTouchEventListener(button_Start,handler(self,self.btnBackCallback))
--    local Node_HUD_Top=panel_Top:getChildByName("Node_HUD_Top")
--    self:initTopBar(Node_HUD_Top)
    local button_Start0 = panel_Center:getChildByName("Button_Start_0")
    button_Start0:setTitleText(LANG("竞技场"))
    self:addBtnTouchEventListener(button_Start0,function(sender,eventType)
        if eventType==2 then
--            SWITSCENE("favor/FavorabilityScene")
            ctlHighladderGetListRequest:sendRequest(handler(self,self.cutArenaPanel));
        end
        
    end) 
    
    local Button_Start_0_0 = panel_Center:getChildByName("Button_Start_0_0")
    self:addBtnTouchEventListener(Button_Start_0_0,function(sender,eventType)
        if eventType==2 then--"arena/StoreScene"
            SWITSCENE("hero/HeroListScene.lua")
        end
    end) 
    
    
    local Button_treasureList = panel_Center:getChildByName("Button_treasureList")
    self:addBtnTouchEventListener(Button_treasureList,function(sender,eventType)
        if eventType==2 then
            SWITSCENE("treasure/TreasureListPanel")
        end
        
    end)
    
    local Button_shop = panel_Center:getChildByName("Button_shop")
    local Button_formation = panel_Center:getChildByName("Button_formation")
    local Button_Gacha = panel_Center:getChildByName("Button_temp1")
    local Button_chooseHero = panel_Center:getChildByName("Button_temp2")
    local Button_Bag = panel_Center:getChildByName("Button_temp3")
    local Button_prop = panel_Center:getChildByName("Button_temp4")
    Button_Gacha:setTitleText("Gacha")
    
    self:addBtnTouchEventListener(Button_Gacha,function(sender,eventType)
        if eventType==2 then
            SWITSCENE("gacha/GachaScene")
        end
    end)
    
    self:addBtnTouchEventListener(Button_shop,function(sender,eventType)
        if eventType==2 then
            SWITSCENE("arena/StoreScene")
        end
    end)
    
    self:addBtnTouchEventListener(Button_formation,function(sender,eventType)
        if eventType==2 then
            SWITSCENE("battle/BattleTeamScene")
        end
    end)
    
    Button_chooseHero:setTitleText(LANG("上阵"))
    self:addBtnTouchEventListener(Button_chooseHero,function(sender,eventType)
        if eventType==2 then
            SWITSCENE("battle/ChooseHeroInBattleScene")
        end
    end)
    
    Button_Bag:setTitleText(LANG("背包"))
    self:addBtnTouchEventListener(Button_Bag,function(sender,eventType)
        if eventType==2 then
              SWITSCENE("bag/BagScene")
        end
    end)
    
    Button_prop:setTitleText(LANG("道具信息"))
    self:addBtnTouchEventListener(Button_prop,function(sender,eventType)
        if eventType==2 then 
            self:showDlg("common/PropsPopScene") 
        end
    end)
    
    -- 主城
    
    local Button_mainCity = Button_prop:clone()
    Button_mainCity:setTitleText(LANG("主城"))
    panel_MainScene:addChild(Button_mainCity)
    Button_mainCity:setPosition(cc.p(50,500))
    
    self:addBtnTouchEventListener(Button_mainCity,function(sender,eventType)
        if eventType==2 then 
            SWITSCENE("home/MainCityScene")   
        end
    end)
    
    
   
end

function HomeScene:cutArenaPanel(success, data, code, msg)
    if(success) then
        SWITSCENE("arena/ArenaScene",data)
    else
        release_print("ffffffffffffff")
    end
end

--function MainScene:onRequest()
--    release_print("dddddddddddddddd")
--end
--
--
--function onHttpResponse()
--    release_print("get http response")
--end


function HomeScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        model_user.selectAreaId = 0
        helper_pve:initStage(function(sucess,data)
            if sucess then
                --SWITSCENE("battle/DefeatScene")
            	SWITSCENE("battle/ChapterScene")
            end
        end)
    end
end
return HomeScene
