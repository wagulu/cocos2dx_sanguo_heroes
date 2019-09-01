_REQUIRE("SanGuoLib")

local BattleScene = class("BattleScene", cc.load("mvc").ViewBase)

local model_game = _REQUIRE("model/model_game.lua")
local helper_battle_data = _REQUIRE("model.helper.helper_battle_data")

function BattleScene:onCreate()

    SanGuoAnimationLib.freeAllAdvancedAnimation();
    SanGuoAnimationLib.freeAllAdvancedAnimationAsset();

    local fight = helper_battle_data:prepareJSON()
    local autoQuickBattle = model_game:getAutoQuickBattle();
    local battleLayer = SanGuoLib.createBattle(fight, autoQuickBattle);
    self:addChild(battleLayer)
--    
--    if model_game:getAutoQuickBattle() then
--        battleLayer:setVisible(false)
--    end
--    Timer:runOnce(function()
--        if model_game:getAutoQuickBattle() then
--            self:showDlg("arena/RapidBattleWaiting")
--        end
--    end,0.01)
    
end

function BattleScene:btnBackCallback(sender, eventType)
    if eventType == 2 then
        SWITSCENE("MainScene")
        --        local test = my.MyClass:create()
        --        release_print("lua bind: " .. test:foo(99))
    end
end

function BattleScene:onExit()
    release_print("dddddd")
    --self:removeTexture()
end

return BattleScene
