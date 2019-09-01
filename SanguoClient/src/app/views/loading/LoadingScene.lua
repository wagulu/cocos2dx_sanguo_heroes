
local LoadingScene = class("LoadingScene", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
--_REQUIRE "cocos.cocos2d.json"
--HomeScene.RESOURCE_FILENAME = "MainScene.csb"
LoadingScene.RESOURCE_FILENAME = "Scene/LoadingScene.csb"

function LoadingScene:onCreate()
    local panel_login = self:getResourceNode():getChildByName("panel_login")
    local Panel_Center = panel_login:getChildByName("Panel_Center")
    local ProjectNode_1 = Panel_Center:getChildByName("ProjectNode_1")
    local loading_sprite = ProjectNode_1:getChildByName("loading_1")
    local action = cc.RepeatForever:create( cc.RotateBy:create(2 , 180) )
    loading_sprite:runAction(action)
    panel_login:setTouchEnabled(true)
    local Panel_Bg = panel_login:getChildByName("Panel_Bg")
    SCREEN_SCALE_BG(Panel_Bg)
    self:setVisible(false)
    
end

function LoadingScene:show()
    self:setVisible(true)
end

function LoadingScene:hide()
    self:setVisible(false)
end

return LoadingScene
