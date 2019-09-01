
_REQUIRE("cocos/cocos2d/json.lua")

local model_static = _REQUIRE("model/static/model_static.lua")
local model = class("model_item", model_static)

function model:getName(id)
    local node = self:getNode(id);
    if(node) then
        return node.name_zhCN;
    end
    return "";
end

function model:getIncrement(id)
    local node = self:getNode(id);
    if(node) then
        return node.increment;
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
        return node.quality;
    end
    return 1;
end

function model:getType(id)
    local node = self:getNode(id);
    if(node) then
        return node.type;
    end
    return "";
end

function model:getSellPrice(id)
    local node = self:getNode(id);
    if(node) then
        return node.sellPrice;
    end
    return 0;
end

--type

function model:isTypeCoinItem(item)
    return item.type == UseableItemType.COIN_ITEM;
end

function model:isTypeCashItem(item)
    return item.type == UseableItemType.CASH_ITEM;
end

function model:isTypeExpItem(item)
    return item.type == UseableItemType.EXP_ITEM;
end

--获取具体type为某一类的数组
function model:getSameTypeTable(str)
    local nodeTable = {}
	for i = 1,table.getn(self.map) do
        if self.map[i].type == "ExpItem" then
            nodeTable[table.getn(nodeTable)+1] = self.map[i]
		end
	end
	return nodeTable;
end

function model:getNodeItem(id)
    local node = self:getNode(id);
    if(node) then
        return node;
    end
    return "";
end

local function init()
    model:load("config/static/Item.Json");
end

init();

return model;
