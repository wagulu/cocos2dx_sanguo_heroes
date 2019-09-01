

_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterQuality", model_static)

--        "id": 1,
--        "gain": "0",
--        "ver": "1.0.0"

function model:getGain(qualityId)
    local node = self:getNode(qualityId);
    if(node) then
        return node.gain;
    end
    return 0;
end

local function init()
    model:load("config/static/MonsterQuality.Json")
end

init()

return model
