
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_skillUpTimesPrice", model_static)

function model:getName(id)
    local node = self:getNode(id);
    if(node) then
        return node.name_zhCN;
    end
    return "";
end

function model:getCash(id)
    local node = self:getNode(id);
    if(node) then
        return node.cash;
    end
    return "";
end

local function init()
    model:load("config/static/SkillUpTimesPrice.Json");
end

init();

return model;
