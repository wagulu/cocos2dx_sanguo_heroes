
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_gacha", model_static)

function model:getName(id)
    local node = self:getNode(id);
    if(node) then
        return node.name_zhCN;
    end
    return "";
end

function model:getDesc(id)
    local node = self:getNode(id);
    if(node) then
        return node.description_zhCN;
    end
    return "";
end

 
function model:getTotalFreeTimes(id)
    local node = self:getNode(id);
    if(node) then
        return node.freeTimes;
    end
    return 0;
end

function model:getRequirePoint(id)
    local node = self:getNode(id);
    if(node) then
        return node.requiredPoint;
    end
    return 0;
end


local function init()
    model:load("config/static/Gacha.Json");
end

init();

return model;
