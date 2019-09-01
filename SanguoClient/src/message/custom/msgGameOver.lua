local msgGameOver = {};
msgGameOver.MESSAGE_NAME = "msgGameOver";

local helper_battle_result = _REQUIRE("model.helper.helper_battle_result")

function msgGameOver:onResponse(eventType, table)
    --local str = "Custom event 1 received, "..event.data.." times"
    release_print("msgGameOver:begin");
    release_print(table);
    release_print("msgGameOver:end");
    
    helper_battle_result:updateBattleResult(table)    
end

return msgGameOver;