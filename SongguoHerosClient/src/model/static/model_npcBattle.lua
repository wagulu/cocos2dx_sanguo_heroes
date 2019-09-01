
--        "id": 1,
--        "monsterId": 1,
--        "itemType": "TreasureItem",
--        "itemId": 1,
--        "flag": 1,
--        "ver": "",
--        "name_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206",
--        "description_zhCN": "1\u53f7\u73cd\u5b9d\u7f18\u5206"


--monsterId



_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_npcBattle", model_static)

function model:setNode(node)
    if(self.npcMap[node.battleId] == nil) then
        self.npcMap[node.battleId] = {};
    end
    local npcs = self.npcMap[node.battleId]
    npcs[#(npcs) + 1] = node;
end

function model:isBos(battleId,npcId)
    local npcs = self.npcMap[battleId]
    if npcs then
    	for i = 1,#npcs do
            if npcs[i].NPCId == npcId then
                return npcs[i].isBoss
    	   end
    	end
    end
end

function model:prepair()
    self.npcMap = {}
end

local function init()
    model:prepair()
    model:load("config/static/NPCBattle.Json")
end

init();

return model;
