local SplashScene = class("SplashScene", cc.load("mvc").ViewBase)
SplashScene.RESOURCE_FILENAME = "Scene/LoginScene.csb"

function SplashScene:onCreate()
    local panel_login = self:getResourceNode():getChildByName("panel_login")
    local Panel_Center = panel_login:getChildByName("Panel_Center")    
    local Panel_Function = Panel_Center:getChildByName("Panel_Function") 
    local Panel_Bottom = panel_login:getChildByName("Panel_Bottom") 
    local Text_loading = Panel_Center:getChildByName("Text_loading") 
    local Image_1 = Panel_Center:getChildByName("Image_1")
    local Text_Progress = Panel_Bottom:getChildByName("Text_Progress")

    Panel_Function:hide()
    Image_1:hide()
    Panel_Bottom:hide()
    Text_loading:show()
    
    local Panel_Bg = panel_login:getChildByName("Panel_Bg")
    SCREEN_SCALE_BG(Panel_Bg)    
    
    initMain()        
end

return SplashScene
