
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterSkillUpType", model_static)


function model:setNode(node)
--    self.map[node.level] = node;
    self.map[node.id] = node;
end

function model:getNode(id)
    return self.map[id];
end

function model:getNodeWithLevel(level)
    return self.getNode(level);
end
--技能升级需要金币数量
function model:getCoin(id,level)
    local node 
    for i = 1,table.getn(self.map) do
        if self.map[i].typeId == id and self.map[i].level == level then
            node = self:getNode(self.map[i].id);
            if(node) then
                return node.coin;
            end  
        end
    end
   
    return "";
end

local function init()
    model:load("config/static/MonsterSkillUpType.Json");
end

init();

return model;
