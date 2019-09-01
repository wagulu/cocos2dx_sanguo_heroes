local PlayerInfoScene = class("PlayerInfoScene", cc.load("mvc").ViewBase)

_REQUIRE "cocos.cocos2d.json"
PlayerInfoScene.RESOURCE_FILENAME = "Scene/PlayerInfoScene1.csb"

function PlayerInfoScene:onCreate()
    local panel_MainScene = self:getResourceNode():getChildByName("PlayerInfo1");
    local panel_Top = panel_MainScene:getChildByName("Panel_Top")
    local panel_center = panel_MainScene:getChildByName("Panel_Center")
    local Panel_R = panel_center:getChildByName("Panel_R")
     
    SCREEN_TOP(panel_Top)
    
    local btn_closed = panel_Top:getChildByName("Button_Close")
    
    -- 关闭页面
    self:addBtnTouchEventListener(btn_closed,function(sender,eventType)
            if eventType == 2 then
                self:removeDlgNoAction()
            end
        end)
     
     
    -- 返回登陆
    local Button_7 = Panel_R:getChildByName("Button_7") 
    self:addBtnTouchEventListener(Button_7,function(sender,eventType)
        if eventType==2 then
            self:showDlg("common/CommonPopScene",{confirmStr=LANG("是"),cancelStr=LANG("否"),text=LANG("是否要返回登录页?"),
                fun=function(data)
                    if data.cmd=="confirm" then -- 返回登陆页面请求
                        SWITSCENE("login/LoginScene")
                    elseif data.cmd =="cancel" then 

                    end
                end})
           end 
    end)
    
    -- 系统设置
    local Button_8 = Panel_R:getChildByName("Button_8")    
    self:addBtnTouchEventListener(Button_8,function(sender,eventType)
        if eventType==2 then
            
        end
     end) 

end

return PlayerInfoScene