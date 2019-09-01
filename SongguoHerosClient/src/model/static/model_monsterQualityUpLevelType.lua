

_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterQualityUpLevelType", model_static)

function model:setNode(node)
    local typeId = node.typeId;
    local quality = node.quality;
    
    if(self.map[typeId] == nil) then
        self.map[typeId] = {};
    end
    self.map[typeId][quality] = node;
end


function model:getRequireLevel(typeId, quality)
    local group = self.map[typeId];
    return group[quality].level;
end

function model:getNeedCoin(typeId,quality)
    local group = self.map[typeId];
    return group[quality+1].coin;
end


local function init()
    model:load("config/static/MonsterQualityUpLevelType.Json");
end

init();

return model;
