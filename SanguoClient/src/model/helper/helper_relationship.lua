
local model_static_library = _REQUIRE("model/static/model_static_library.lua")
local model_user = _REQUIRE("model/model_user")
local model_materialStorage = _REQUIRE("model/model_materialStorage.lua");
local model_monsterStorage = _REQUIRE("model/model_monsterStorage.lua");

local helper = {};


function helper:getRelationship(monster)
    local monsterId = monster.id;
    
    local relationship = static_model_relationship:getNode(monsterId);
    return relationship;
end

function helper:getRelationshipSynthesis(relationshipId)
    return static_model_relationshipSynthesis:getSynthesis(relationshipId);
end

function helper:getRelationshipLevelGain(gainId)
    return static_model_relationshipLevelGain:getSynthesis(gainId);
end

--interface

function helper:getRelationshipLevel(monster, membersId)
    membersId = membersId or {};

    local effects = {};

    local relationship = self:getRelationship(monster);
    if relationship == nil then
        return effects;
    end
    
    for key, value in pairs(relationship) do
        local relationshipId = value.id;
        local relationshipSynthesis = self:getRelationshipSynthesis(relationshipId);
        

        local _actived = false;
        local monsterQuality = 1;
        if(self:isRelationshipSynthesisActived(relationshipSynthesis, membersId)) then
            _actived = true;
            monsterQuality = self:calcCurrentRelationshipQuality(monster, relationshipSynthesis);
        end    

        local _effect = static_model_relationshipLevel:getActiveRelationshipLevel(relationshipId, monsterQuality);
        if(_effect) then
            effects[#(effects) + 1] =
            {
                effect = _effect,
                actived = _actived,
            };
        end
    end

    return effects;
end


function helper:calcCurrentRelationshipQuality(monster, relationshipSynthesis)
    local monsterQuality = monster.quality;

    for key, value in pairs(relationshipSynthesis) do
        local requireMonsterId = value.monsterId;
        if(model_monsterStorage:isMonsterActive(requireMonsterId)) then
            local member = model_monsterStorage:getMonster(requireMonsterId);
            monsterQuality = math.min(monsterQuality, member.quality);
        else
            monsterQuality = 1;
        end
    end
    
    return monsterQuality;
end

function helper:getActiveRelationshipLevelGains(monster, membersId)
    membersId = membersId or {};

    local effects = {};
    
    local levels = self:getRelationshipLevel(monster, membersId);
    for key, value in pairs(levels) do
        if(value.actived) then
            local gains = self:getRelationshipLevelGain(value.effect.id);
            for var=1, #(gains) do
                effects[#(effects) + 1] = gains[var];
            end
        end
    end
    
    return effects;
end

function helper:isRelationshipSynthesisActived(synthesis, membersId)
    local function isThereMonsterInTeam(monsterId, membersId)
        for key, value in pairs(membersId) do
            if(value == monsterId) then
                return true;
            end
        end
        return false;
    end

    for key, value in pairs(synthesis) do
        local requireMonsterId = value.monsterId;
        if not(isThereMonsterInTeam(requireMonsterId, membersId)) then
            return false;
        end
    end

    return true;
end

return helper;
