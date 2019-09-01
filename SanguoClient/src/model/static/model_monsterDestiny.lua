
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monsterDestiny", model_static)

local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")
local model_monster = _REQUIRE("model/static/model_monster.lua");
local model_itemStorage = _REQUIRE("model/model_itemStorage.lua")
_REQUIRE("pbCommon.lua")

function model:getItemCount(id)         --相应等级下每次洗髓所加经验和所需洗髓丹(数量一样)
    local node = self:getNode(id);
    if(node) then
        return node.itemNum;
    end
    
    return 0;
end

function model:getExp(id)
    local node = self:getNode(id);
    if(node) then
        return node.exp;
    end
    
    return 0;
end

function model:getAttack(id)
    local node = self:getNode(id);
    if(node) then
        return node.Attack;
    end
    
    return 0;
end

function model:getPower(id)
    local node = self:getNode(id);
    if(node) then
        return node.Power;
    end
    
    return 0;
end

function model:getArmor(id)
    local node = self:getNode(id);
    if(node) then
        return node.Armor;
    end
    
    return 0;
end

function model:getResistance(id)
    local node = self:getNode(id);
    if(node) then
        return node.Resistance;
    end
    
    return 0;
end


-- interface
function model:getDestinyItemCount()
    local destinyItemId = model_monster:getMonsterDestinyItemId();
    return model_itemStorage:getCount(destinyItemId, pbCommon.ObjectType.BreakthroughItem)
end

function model:getMonsterLevelUpRate5(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel;
    local exp = monster.destinyExp;
    
    local needExp = self:getExp(level);
    
    local rate = exp / needExp;
    return rate;
    
end

--物理攻击
function model:getMonsterEffectAttack(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel;

    return self:getAttack(level);
end

function model:getNextMonsterEffectAttack(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel + 1;

    return self:getAttack(level);
end

--法术攻击
function model:getMonsterEffectPower(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel;

    return self:getPower(level);
end

function model:getNextMonsterEffectPower(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel + 1;

    return self:getPower(level);
end

--物理防御
function model:getMonsterEffectArmor(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel;

    return self:getArmor(level);
end

function model:getNextMonsterEffectArmor(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel + 1;

    return self:getArmor(level);
end

--法术防御
function model:getMonsterEffectResistance(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel;

    return self:getResistance(level);
end

function model:getNextMonsterEffectResistance(monsterId)
    local monster = model_monsterStorage:getMonster(monsterId);
    local level = monster.destinyLevel + 1;

    return self:getResistance(level);
end


local function init()
    model:load("config/static/MonsterDestiny.Json");
end

init();

return model;
