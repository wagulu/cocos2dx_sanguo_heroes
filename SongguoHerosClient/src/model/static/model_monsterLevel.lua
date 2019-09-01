
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterLevel", model_static)

function model:getExp(id)
    local node = self:getNode(id);
    if(node) then
        return node.exp;
    end
    return 0;
end
--根据总经验，得到武将达到的等级和此等级时的经验值
function model:getTotleToLevel(totleExp)
    local expAdded = 0
    local preExpAdded = 0;
    for i = 1,table.getn(self.map) do
        expAdded = expAdded + self.map[i].exp
        if expAdded >= totleExp then
        	return {i,(totleExp-preExpAdded)}
        end
        preExpAdded = expAdded
    end
end

function model:getLevelToTotleExp(level)
    local expAdded = 0
	for i = 1,level do
        expAdded = expAdded + self.map[i].exp
	end
    return expAdded
end

local function init()
    model:load("config/static/MonsterLevel.Json");
end

init();

return model;
