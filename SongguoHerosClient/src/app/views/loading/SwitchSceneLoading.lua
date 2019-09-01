
local SwitchSceneLoading = class("SwitchSceneLoading", cc.load("mvc").ViewBase)
--_REQUIRE "src/packages/packet/Packet_Regester"
--_REQUIRE "cocos.cocos2d.json"
--HomeScene.RESOURCE_FILENAME = "MainScene.csb"


function SwitchSceneLoading:onCreate()
--    local layout = ccui.Layout:create()
--    local size =display.size
--    layout:setContentSize(size)
--    --layout:setSize(size)
--    layout:setBackGroundColorType(1);
--    layout:setBackGroundColor(cc.c3b(0,0,0))
--    layout:setBackGroundColorOpacity(160);
--    layout:setTouchEnabled(true)
--    self:addChild(layout)

    local loading_bg = display.newSprite("res/ui/Assets/login/loading_01.jpg")
    self:addChild(loading_bg)
    loading_bg:setPosition(display.cx,display.cy)
    SCREEN_SCALE_BG(loading_bg)
--    local loading_bg = display.newSprite("res/loading/loading_bg.png")
--    loading_bg:setScale(2)
--    loading_bg:setPosition(display.cx,display.cy)
--
--    self:addChild(loading_bg)
--
--    local loading_sprite = display.newSprite("res/loading/loading.png")
--    loading_sprite:setScale(2)
--    loading_sprite:setPosition(display.cx - 220,display.cy)
--    --display.align(loading_sprite , display.CENTER , display.cx - 220 , display.cy)
--
--    local action = cc.RepeatForever:create( cc.RotateBy:create(0.5 , 180) )
--    loading_sprite:runAction(action)
--    self:addChild(loading_sprite)
--
--    local text = ccui.Text:create()
--    text:setString(LANG("正在加载，请稍等.."))
--    text:setFontSize(50)
--    text:setPosition(display.cx + 35,display.cy)
--    --display.align(text , display.CENTER , display.cx + 35 , display.cy)
--    text:setColor( cc.c3b( 0xff , 0xff , 0xff ) )
--    self:addChild(text)
--    self:setVisible(true)
    Timer:runOnce(function()
        local scenName = self.args[1]
        table.remove(self.args,1)
        GLOBAL_MY_APP:enterScene(scenName,unpack(self.args, 1, table.maxn(self.args)))
    end,.01)
    
end

function SwitchSceneLoading:show()
    self:setVisible(true)
end

function SwitchSceneLoading:hide()
    self:setVisible(false)
end

return SwitchSceneLoading
