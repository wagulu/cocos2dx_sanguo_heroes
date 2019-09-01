
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_equipment", model_static)

function model:getName(id)
    local node = self:getNode(id);
    if(node) then
        return node.name_zhCN;
    end
    return "";
end

function model:getLevelUpCoinType(id)
    local node = self:getNode(id);
    if(node) then
        return node.levelUpCoinType;
    end
    return "";
end

function model:getQualityUpMaterialType(id)
    local node = self:getNode(id);
    if(node) then
        return node.qualityUpMaterialType;
    end
    return "";
end

function model:getQualityBaseAttributeType(id)
    local node = self:getNode(id);
    if(node) then
        return node.qualityBaseAttributeType;
    end
    return "";	
end

function model:getIcon(id,quality)
    local node = self:getNode(id);
    if(node) then
        local name = pbCommon.ColorType[quality]
        local splits = string.split(name,"_")
--        return "equipment/equipment_"..node.asset.."_"..splits[1]..".png"
        return "Equipment/Equipment_"..id.."_"..splits[1]..".png"
    end
    return "";
end

function model:getFrameByQuality(quality)
    cclog("asset:".."frame/frame_"..pbCommon.ColorType[quality]..".png")
    return "frame/frame_"..pbCommon.ColorType[quality]..".png"
end

local function init()
    model:load("config/static/Equipment.Json");
end

init();

return model;
