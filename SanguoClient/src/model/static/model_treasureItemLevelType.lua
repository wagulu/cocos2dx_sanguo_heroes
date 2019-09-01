
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_treasureItemLevelType", model_static)


function model:setNode(node)
    if(self.map[node.type] == nil) then
        self.map[node.type] = {};
    end

    self.map[node.type][node.level] = node;
end

function model:getDefine(type, level)
    local node = self:getNode(type);
    return node[level];
end

function model:getExp(type, level)
    local node = self:getNode(type);
    if node[level] then
        return node[level].exp;
    end
    
end

-- interface


local function init()
    model:load("config/static/TreasureItemLevelType.Json");
end

init();

return model;
