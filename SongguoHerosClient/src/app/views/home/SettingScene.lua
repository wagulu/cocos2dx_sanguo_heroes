local SettingScene = class("SettingScene", cc.load("mvc").ViewBase)

_REQUIRE "cocos.cocos2d.json"
SettingScene.RESOURCE_FILENAME = "Scene/PauseMenuPanel.csb"


function SettingScene:onCreate() 
    local panel_MainScene = self:getResourceNode():getChildByName("Panel_PauseMenu")
    panel_MainScene:setTouchEnabled(true)
  
    --  点击空白关闭
    panel_MainScene:setTouchEnabled(true)
    self:addBtnTouchEventListener(panel_MainScene,function(sender,eventType)
        if eventType == 2 then
            self:removeDlgNoAction()
        end
    end)
    
    self.centerNode_ = panel_MainScene:getChildByName("Panel_C"):getChildByName("Panel_Center")
         
    -- 音效开
    local Button_Voice = self.centerNode_:getChildByName("Button_Voice") 
    self:addBtnTouchEventListener(Button_Voice,function(sender,eventType)
        if eventType == 2 then
            self:switchEffect(false)
            SanGuoSoundLib.disableSoundEffect() 
        end
    end)
    -- 音效关
    local Button_Mute = self.centerNode_:getChildByName("Button_Mute")
    self:addBtnTouchEventListener(Button_Mute,function(sender,eventType)
        if eventType == 2 then
            self:switchEffect(true)
            SanGuoSoundLib.enableSoundEffect()
        end
    end)
 
    -- 音乐开
    local Button_Music_1 = self.centerNode_:getChildByName("Button_Music_1")
    self:addBtnTouchEventListener(Button_Music_1,function(sender,eventType)
        if eventType == 2 then
            self:switchMusic(false)
            SanGuoSoundLib.disableMusic()
        end
    end)
    
    -- 音乐关
    local Button_Music_2 = self.centerNode_:getChildByName("Button_Music_2")
    self:addBtnTouchEventListener(Button_Music_2,function(sender,eventType)
        if eventType == 2 then
            self:switchMusic(true)
            SanGuoSoundLib.enableMusic()
        end
    end)
    
    -- todo 默认音乐音效 开启 
    Button_Mute:hide()
    Button_Music_2:hide()
    
    -- 继续战斗
    self.centerNode_:getChildByName("Button_Continue"):hide()   
    -- 退出战斗
    self.centerNode_:getChildByName("Button_Menu"):hide()
  
end

function SettingScene:switchEffect(bOn)
    -- 音效开
    local Button_Voice = self.centerNode_:getChildByName("Button_Voice") 
    -- 音效关
    local Button_Mute = self.centerNode_:getChildByName("Button_Mute")
    if bOn then
     	Button_Voice:show()
     	Button_Mute:hide()
    else
        Button_Voice:hide()
        Button_Mute:show()
    end
end

function SettingScene:switchMusic(bOn)
    -- 音乐开
    local Button_Music_1 = self.centerNode_:getChildByName("Button_Music_1")
    -- 音乐关
    local Button_Music_2 = self.centerNode_:getChildByName("Button_Music_2")    
    if bOn then
        Button_Music_1:show()
        Button_Music_2:hide()
    else
        Button_Music_1:hide()
        Button_Music_2:show()
    end
end

return SettingScene