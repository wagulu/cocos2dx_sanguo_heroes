
_REQUIRE("cocos/cocos2d/json.lua")

--        "id": 1,
--        "monsterId": 1,
--        "baseAtkHitRate": "0.95",
--        "baseAbilityHitRate": "1",
--        "baseAtkDodgeRate": "0.05",
--        "baseAbilityDodgeRate": "0",
--        "baseAtkCSR": "0.1",
--        "baseAbilityCSR": "0",
--        "baseAtkAntiCSR": "0",
--        "baseAbilityAntiCSR": "0",
--        "basePhysicalCSDR": "0",
--        "baseSpellCSDR": "0",
--        "basePhysicalResilience": "0",
--        "baseSpellResilience": "0",
--        "baseArPR": "0",
--        "baseSRPR": "0",
--        "basePhysicalDR": "0",
--        "baseSpellDR": "0",
--        "baseAtkVampRate": "0",
--        "baseAbilityVampRate": "0",
--        "holyDamage": "0",
--        "DR": "0",
--        "abilityCD": "0",
--        "arrowSpeed": "0",
--        "ERHPBase": "0",
--        "ERKillBase": "0",
--        "ERLoseBase": "0",
--        "spellCostReduction": "0",
--        "ver": "1.0.0"

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterExpand", model_static)

function model:setNode(node)
    self.map[node.monsterId] = node;
end

local function init()
    model:load("config/static/MonsterExpand.Json");
end

init();

return model;
