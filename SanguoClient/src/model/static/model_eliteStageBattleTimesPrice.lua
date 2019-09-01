
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_eliteStageBattleTimesPrice", model_static)

function model:getCash(id)
    local node = self:getNode(id);
    if(node) then
        return node.cash;
    end
    return 10000;
end


local function init()
    model:load("config/static/EliteStageBattleTimesPrice.Json");
end

init();

return model;
