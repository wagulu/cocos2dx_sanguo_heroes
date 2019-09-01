
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_equipmentStrengthenMasterConfig", model_static)

local static_model_equipmentStrengthenMaster = _REQUIRE("model/static/model_equipmentStrengthenMaster.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua")

function model:getMinLevel(id)
    return self.map[id].minLevel;
end



-- used to calculate monster strength level
function model:getStrengthLevel(monsterId)
    local equipmentLevel = 0;
    local monster = model_monsterStorage:getMonster(monsterId);
    for key, value in pairs(monster.equipment) do
        if(equipmentLevel == 0) then
            equipmentLevel = value.level;
        else
            equipmentLevel = math.min(equipmentLevel, value.level);
        end
    end


    local level = 0;
    for key, value in pairs(self.map) do
        if(value.minLevel <= equipmentLevel) then
            level = math.max(level, value.id);
        end
    end

    return level;
end

-- used to calculate monster strength effect
function model:getStrengthEffectGroup(level)
--    local level = self:getStrengthLevel(monsterId);
    if(level > 0) then
        local group = static_model_equipmentStrengthenMaster:getGroup(level);
        return group;
    end
    
    return {};
end
--获取所能强化的最高等级
function model:getHighestLevel()
    local level = 0;
    for key, value in pairs(self.map) do
        if(value.minLevel >= level) then
            level = value.minLevel;
        end
    end

    return level;
end

function model:prepair()
end

local function init()
    model:prepair();
    model:load("config/static/EquipmentStrengthenMasterConfig.Json");
end

init();

return model;
