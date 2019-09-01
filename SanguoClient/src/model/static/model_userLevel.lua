
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_userLevel", model_static)

function model:getExp(id)
    local node = self:getNode(id);
    if(node) then
        return node.exp;
    end
    return 99999999;
end


local function init()
    model:load("config/static/UserLevel.Json");
end

init();

return model;
