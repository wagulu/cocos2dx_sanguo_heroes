
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_monster", model_static)

function model:getName(id)
    local node = self:getNode(id);
    if(node) then
        return node.name_zhCN;
    end
    return "";
end

function model:getDesc(id)
    local node = self:getNode(id);
    if(node) then
        return node.description_zhCN;
    end
    return "";
end

function model:getStar(id)
    local node = self:getNode(id);
    if(node) then
        return node.initialStar;
    end
    return 1;
end

function model:getAI(id)
    local node = self:getNode(id);
    if(node) then
        return node.AI;
    end
    return "";
end

function model:getAsset(id)
    local node = self:getNode(id);
    if(node) then
        return node.asset;
    end
    return "";
end

function model:getQuality(id)
    local node = self:getNode(id);
    if(node) then
        return node.initialQuality;
    end
    return 1;
end

function model:getQualityLevelTypeId(id)
    local node = self:getNode(id);
    if(node) then
        return node.qualityLevelType;
    end
    return 99999;
end

-- interface

function model:getIcon(id)
    local node = self:getNode(id);
    if(node) then
        return self:getIconByAsset(node.asset)
    end
    return "";
end





function model:getIconByAsset(asset)
    return "Monster/Monster_"..asset..".png"
end

function model:getType(id)
    local node = self:getNode(id);
    if(node) then
        return node.type;
    end
    return "";
end

function model:getFrame(id)
    return self:getFrameByQuality(self:getQuality(id))
end

function model:getFrameByQuality(quality)
    return "frame/frame_"..pbCommon.ColorType[quality]..".png"

end

function model:getHalf(id)
    local node = self:getNode(id);
    if(node) then
        return "Monster_half/Monster_half_"..node.asset..".png"
    end
    return "";
end


function model:getBreakthroughItemId(id)
    return 30;
end

function model:getBreakthroughExclusiveItemId(id)
    local node = self:getNode(id);
    if(node) then
        return node.exclusiveItemId;
    end
    return 0;
end

function model:getMonsterDestinyItemId()
    return 31;
end

local function init()
    model:load("config/static/Monster.Json");
end

init();

return model;
