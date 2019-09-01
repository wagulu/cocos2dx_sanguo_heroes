
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
local model = class("model_relationshipItem", model_static)

function model:setNode(node)
    local monsterId = node.monsterId
    if(self.monsterMap[monsterId] == nil) then
        self.monsterMap[monsterId] = {};
    end
    local monster = self.monsterMap[monsterId]
    monster[#(monster) + 1] = node;
    
    local treasureId = node.itemId
    if(self.treasureMap[treasureId] == nil) then
        self.treasureMap[treasureId] = {};
    end
    local treasure = self.treasureMap[treasureId]
    treasure[#(treasure) + 1] = node;
end

function model:getMonsterRelationship(monsterId)
    return self.monsterMap[monsterId]
end

function model:getTreasureRelationship(treasureId)
    return self.treasureMap[treasureId]
end

function model:prepair()
    self.monsterMap = {}
    self.treasureMap = {}
end

local function init()
    model:prepair()
    model:load("config/static/RelationshipItem.Json")
end

init();

return model;
